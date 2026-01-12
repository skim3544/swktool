#include "pch.h"
#include "CrashHandler.h"
#include <signal.h>

#include <DbgHelp.h>
#ifndef MAX_SYM_NAME
    #define MAX_SYM_NAME 256
#endif

#include <new.h>
#include <exception>
#include <tchar.h>
#include <intrin.h>
#include <string>
#include <iomanip>
#include "Logger.h"
#include "TraceBuffer.h"


#pragma intrinsic(_ReturnAddress)
#pragma intrinsic(_AddressOfReturnAddress)

#pragma comment(lib, "Dbghelp.lib")

namespace swktool 
{
    ILogger* CCrashHandler::pLogger = nullptr;
    MINIDUMP_TYPE    CCrashHandler::MemDumpType_ = MiniDumpNormal;
    bool CCrashHandler::DumpMemory_ = true;
    bool CCrashHandler::CallTrace_ = true;

    void CCrashHandler::HandleCrash(unsigned code, EXCEPTION_POINTERS* ep)
    {
        if (!ep) 
        {
            EXCEPTION_POINTERS local{};
            GetExceptionPointers(code, local);
            ep = &local;
        }

        CreateLog(ep);
        if (DumpMemory_)
            CreateMiniDump(ep);
    }


    LONG WINAPI CCrashHandler::SehHandler(PEXCEPTION_POINTERS ep) {        

        HandleCrash(ep->ExceptionRecord->ExceptionCode, ep);
        return EXCEPTION_EXECUTE_HANDLER;
    }

    void __cdecl CCrashHandler::TerminateHandler() {
        HandleCrash(STATUS_FATAL_APP_EXIT, nullptr);
    }

    void __cdecl CCrashHandler::UnexpectedHandler() {
        HandleCrash(STATUS_FATAL_APP_EXIT, nullptr);
    }

    void __cdecl CCrashHandler::PureCallHandler() {
        HandleCrash(STATUS_ILLEGAL_INSTRUCTION, nullptr);
    }

    void __cdecl CCrashHandler::InvalidParameterHandler(const wchar_t* expression,
        const wchar_t* function, const wchar_t* file,
        unsigned int line, uintptr_t pReserved) 
    {
        HandleCrash(STATUS_INVALID_PARAMETER, nullptr);
    }

    int __cdecl CCrashHandler::NewHandler(size_t) {
        HandleCrash(STATUS_NO_MEMORY, nullptr);
        return 0;
    }

    void CCrashHandler::SigabrtHandler(int) {        
        HandleCrash(STATUS_FATAL_APP_EXIT, nullptr);

    }
    void CCrashHandler::SigfpeHandler(int /*code*/, int subcode) {
        HandleCrash(EXCEPTION_FLT_DIVIDE_BY_ZERO, nullptr);
    }
    void CCrashHandler::SigintHandler(int) {
        HandleCrash(STATUS_CONTROL_C_EXIT, nullptr);
    }
    void CCrashHandler::SigillHandler(int) {
        HandleCrash(EXCEPTION_ILLEGAL_INSTRUCTION, nullptr);
    }
    void CCrashHandler::SigsegvHandler(int) {
        HandleCrash(EXCEPTION_ACCESS_VIOLATION, nullptr);
    }
    void CCrashHandler::SigtermHandler(int) {
        HandleCrash(STATUS_FATAL_APP_EXIT, nullptr);
    }
    
    /// <summary>
    /// VectoredExceptionHandler
    /// Used tocapture heap corruption exception
    /// C/C++ startup code calls HeapSetInformation with HeapEnableTerminationOnCorruption class as parameter,
    /// which prevents our exception handler from capturing the heap corruption exception (0xc0000374)
    /// This exception is captured and handled from Vectored Exception handler
    /// </summary>
    /// <param name="pExceptionPtrs"></param>
    /// <returns></returns>
    LONG CCrashHandler::VectoredExceptionHandler(PEXCEPTION_POINTERS ep)
    {
        const auto code = ep->ExceptionRecord->ExceptionCode;

        if (code == STATUS_HEAP_CORRUPTION)
        {
            if (pLogger)
                pLogger->Log(LogLevel::STATUS, __FUNCTION__);

            static bool handling = false;
            if (handling)
                return EXCEPTION_CONTINUE_SEARCH;

            handling = true;
            HandleCrash(code, ep);
            handling = false;

            return EXCEPTION_EXECUTE_HANDLER;
        }

        return EXCEPTION_CONTINUE_SEARCH;
    }


    void CCrashHandler::SetProcessExceptionHandlers()
    {
        // Install top-level SEH handler
        SetUnhandledExceptionFilter(SehHandler);


        // Chain the vectored Exception Handler
        AddVectoredExceptionHandler(TRUE, VectoredExceptionHandler);

        // Catch pure virtual function calls.
        // Because there is one _purecall_handler for the whole process, 
        // calling this function immediately impacts all threads. The last 
        // caller on any thread sets the handler. 
        // http://msdn.microsoft.com/en-us/library/t296ys27.aspx
        _set_purecall_handler(PureCallHandler);

        // Catch new operator memory allocation exceptions
        //_set_new_handler(NewHandler);

        // Catch invalid parameter exceptions.
        _set_invalid_parameter_handler(InvalidParameterHandler);

        // Set up C++ signal handlers

        _set_abort_behavior(_CALL_REPORTFAULT, _CALL_REPORTFAULT);

        // Catch an abnormal program termination
        signal(SIGABRT, SigabrtHandler);

        // Catch illegal instruction handler
        signal(SIGINT, SigintHandler);

        // Catch a termination request
        signal(SIGTERM, SigtermHandler);
    }

    void CCrashHandler::SetThreadExceptionHandlers()
    {

        // Catch terminate() calls. 
        // In a multithreaded environment, terminate functions are maintained 
        // separately for each thread. Each new thread needs to install its own 
        // terminate function. Thus, each thread is in charge of its own termination handling.
        // http://msdn.microsoft.com/en-us/library/t6fk7h29.aspx
        set_terminate(TerminateHandler);

        // Catch unexpected() calls.
        // In a multithreaded environment, unexpected functions are maintained 
        // separately for each thread. Each new thread needs to install its own 
        // unexpected function. Thus, each thread is in charge of its own unexpected handling.
        // http://msdn.microsoft.com/en-us/library/h46t5b69.aspx  
        set_unexpected(UnexpectedHandler);

        // Catch a floating point error
        typedef void (*sigh)(int);
        signal(SIGFPE, (sigh)SigfpeHandler);

        // Catch an illegal instruction
        signal(SIGILL, SigillHandler);

        // Catch illegal storage access errors
        signal(SIGSEGV, SigsegvHandler);

    }


    void CCrashHandler::GetExceptionPointers(DWORD code, EXCEPTION_POINTERS& out)
    {
        static EXCEPTION_RECORD record{};
        static CONTEXT context{};

        RtlCaptureContext(&context);

        record.ExceptionCode = code;
        record.ExceptionAddress = _ReturnAddress();

        out.ExceptionRecord = &record;
        out.ContextRecord = &context;
    }


    void CCrashHandler::CreateLog(EXCEPTION_POINTERS* pExcPtrs) {
        if (pLogger == nullptr) return;

        std::ostringstream str;
        std::string Desc = GetExceptionDesc(pExcPtrs->ExceptionRecord->ExceptionCode);
        str << "Exception Thrown: " << Desc.c_str();

        if (pExcPtrs->ExceptionRecord->ExceptionCode == EXCEPTION_ACCESS_VIOLATION ||
            pExcPtrs->ExceptionRecord->ExceptionCode == EXCEPTION_IN_PAGE_ERROR) {
            ULONG_PTR IO = pExcPtrs->ExceptionRecord->ExceptionInformation[0];
            if (IO == 0) 
                str << " Read Error";
            if (IO == 1)
                str << " Write Error";
            else
                str << " Data execution Error";

            ULONG_PTR Address = pExcPtrs->ExceptionRecord->ExceptionInformation[1];
            str << " Address : " << std::setw(8) << std::setfill('0') << std::hex << Address << std::setfill(' ');

            ULONG_PTR NCStatus = pExcPtrs->ExceptionRecord->ExceptionInformation[2];
            str << " Status Code: " << std::setw(8) << std::setfill('0') << std::hex << NCStatus << std::setfill(' ');            
        }

        pLogger->Log(LogLevel::STATUS, str);

        // Walk the stack
        WalkStack(pExcPtrs);

        if (CallTrace_)
        {
            pLogger->Log(LogLevel::STATUS, "\r\n=== TRACE START OF TRACE BUFFER ===\r\n");
            TraceBuffer::DumpGrouped(pLogger);
            pLogger->Log(LogLevel::STATUS, "\r\n=== TRACE END OF TRACE BUFFER ===\r\n");
        }
    }

    void CCrashHandler::CreateMiniDump(EXCEPTION_POINTERS* pExcPtrs)
    {
        // Create the dump file
        HANDLE hFile = CreateFile(
            _T("crashdump.dmp"),
            GENERIC_WRITE,
            0,
            nullptr,
            CREATE_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            nullptr);

        if (hFile == INVALID_HANDLE_VALUE)
            return;

        MINIDUMP_EXCEPTION_INFORMATION mei{};
        mei.ThreadId = GetCurrentThreadId();
        mei.ExceptionPointers = pExcPtrs;
        mei.ClientPointers = FALSE;

        MINIDUMP_CALLBACK_INFORMATION mci{};
        mci.CallbackRoutine = nullptr;
        mci.CallbackParam = nullptr;

        // Direct call — no LoadLibrary, no GetProcAddress
        MiniDumpWriteDump(
            GetCurrentProcess(),
            GetCurrentProcessId(),
            hFile,
            MemDumpType_,
            &mei,
            nullptr,
            &mci);

        CloseHandle(hFile);
    }

   


    // Use the following filter
    // void main() {
    //  __try {
    //  } 
    //  __except(CCrashHandler::seh_filter(GetExceptionCode(), GetExceptionInformation())))
    //  {
    //      ExitProcess(1);
    //  }
    // }

    //DWORDLONG* GetTopOfStack() {
    //    DWORDLONG* pRetAddr;
    //    __asm {
    //        mov rax, 4
    //        mov [pRetAddr], rax
    //    }

    //    return pRetAddr;
    //}

    int CCrashHandler::seh_filter(unsigned int code, struct _EXCEPTION_POINTERS* ep) {
        
        // Create exception log
        CCrashHandler::CreateLog(ep);

        // Create Memory Dump
        if (DumpMemory_) {
            CCrashHandler::CreateMiniDump(ep);
        }

        return EXCEPTION_EXECUTE_HANDLER;
    }

    std::string CCrashHandler::GetExceptionDesc(DWORD Code) {
        struct { 
            DWORD Code;
            LPCSTR Desc;
        } Desc_Lookup[] = {
            {EXCEPTION_ACCESS_VIOLATION, "Access Violation"},
            {EXCEPTION_ARRAY_BOUNDS_EXCEEDED, "Array bounds exceeded Violation"},
            {EXCEPTION_DATATYPE_MISALIGNMENT, "Data Misalignment"},
            {EXCEPTION_FLT_DENORMAL_OPERAND, "Denomal operand"},
            {EXCEPTION_FLT_DIVIDE_BY_ZERO, "Floating point Divide by Zero"},
            {EXCEPTION_INT_DIVIDE_BY_ZERO, "Divide by Zero"},
            {EXCEPTION_FLT_INVALID_OPERATION, "Floating point Invalid operation"},
            {EXCEPTION_ILLEGAL_INSTRUCTION, "Illegal Instruction"},
            {EXCEPTION_INVALID_HANDLE, "Invalid Handle"},
            {EXCEPTION_PRIV_INSTRUCTION, "Privileged instruction"},
            {EXCEPTION_STACK_OVERFLOW, "Stack Overflow"}
        };

        std::string Desc = "Unknown";
        for (int i = 0; i < _countof(Desc_Lookup); i++) {
            if (Desc_Lookup[i].Code == Code) {
                Desc = std::string(Desc_Lookup[i].Desc);
                break;
            }
        }

        return Desc;
    }
    
       
    void CCrashHandler::WalkStack(EXCEPTION_POINTERS* ep)
    {
        if (!pLogger || !ep || !ep->ContextRecord)
            return;

        std::ostringstream s;
        s << "Call stack:" << std::endl;

        HANDLE process = GetCurrentProcess();
        HANDLE thread = GetCurrentThread();

        // Initialize DbgHelp once per process.
        static bool s_symInitialized = false;
        if (!s_symInitialized)
        {
            if (SymInitialize(process, nullptr, TRUE))
            {
                SymSetOptions(SYMOPT_DEFERRED_LOADS | SYMOPT_UNDNAME);
                s_symInitialized = true;
            }
            else
            {
                s << "  <SymInitialize failed>" << std::endl;
                pLogger->Log(LogLevel::STATUS, s);
                return;
            }
        }

        CONTEXT context = *ep->ContextRecord; // work on a copy, not the original

        STACKFRAME64 frame{};
        DWORD machineType = 0;

#ifdef _WIN64
        machineType = IMAGE_FILE_MACHINE_AMD64;
        frame.AddrPC.Offset = context.Rip;
        frame.AddrPC.Mode = AddrModeFlat;
        frame.AddrFrame.Offset = context.Rbp;
        frame.AddrFrame.Mode = AddrModeFlat;
        frame.AddrStack.Offset = context.Rsp;
        frame.AddrStack.Mode = AddrModeFlat;
#else
        machineType = IMAGE_FILE_MACHINE_I386;
        frame.AddrPC.Offset = context.Eip;
        frame.AddrPC.Mode = AddrModeFlat;
        frame.AddrFrame.Offset = context.Ebp;
        frame.AddrFrame.Mode = AddrModeFlat;
        frame.AddrStack.Offset = context.Esp;
        frame.AddrStack.Mode = AddrModeFlat;
#endif

        // Symbol buffer
        BYTE symbolBuffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR)] = {};
        PSYMBOL_INFO pSymbol = reinterpret_cast<PSYMBOL_INFO>(symbolBuffer);
        pSymbol->SizeOfStruct = sizeof(SYMBOL_INFO);
        pSymbol->MaxNameLen = MAX_SYM_NAME;

        const int maxFrames = 128;
        int frameCount = 0;

        while (frameCount < maxFrames)
        {
            BOOL ok = StackWalk64(
                machineType,
                process,
                thread,
                &frame,
                &context,
                nullptr,
                SymFunctionTableAccess64,
                SymGetModuleBase64,
                nullptr);

            if (!ok || frame.AddrPC.Offset == 0)
                break;

            DWORD64 address = frame.AddrPC.Offset;
            DWORD64 displacement = 0;

            if (SymFromAddr(process, address, &displacement, pSymbol))
            {
                s << "  " << pSymbol->Name << " + 0x"
                    << std::hex << displacement << std::dec
                    << " [0x" << std::hex << address << std::dec << "]"
                    << std::endl;
            }
            else
            {
                s << "  <unknown> [0x"
                    << std::hex << address << std::dec << "]"
                    << std::endl;
            }

            ++frameCount;
        }

        if (frameCount == 0)
            s << "  <no frames captured>" << std::endl;

        pLogger->Log(LogLevel::STATUS, s);
    }

}



