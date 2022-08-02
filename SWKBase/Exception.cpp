#include "pch.h"
#include "Exception.h"
#include <signal.h>
#include <DbgHelp.h>
#include <new.h>
#include <exception>
#include <tchar.h>
#include <intrin.h>
#include <string>
#include <iomanip>
#include "Logger.h"


#pragma intrinsic(_ReturnAddress)
#pragma intrinsic(_AddressOfReturnAddress)

#pragma comment(lib, "Dbghelp.lib")

namespace swktool {
    ILogger* CCrashHandler32::pLogger = nullptr;
    MINIDUMP_TYPE    CCrashHandler32::MemDumpType_ = MiniDumpNormal;
    bool CCrashHandler32::DumpMemory_ = true;


    LONG WINAPI CCrashHandler32::SehHandler(PEXCEPTION_POINTERS pExceptionPtrs) {
        if (pLogger != nullptr) pLogger->Log(LogLevel::STATUS, __FUNCTION__);

        EXCEPTION_POINTERS* pExPointers = new EXCEPTION_POINTERS;
        CCrashHandler32::GetExceptionPointers(0, &pExPointers);


        CCrashHandler32::seh_filter(0, pExPointers);

        return 0l;
    }

    void __cdecl CCrashHandler32::TerminateHandler() {
        if (pLogger != nullptr) pLogger->Log(LogLevel::STATUS, __FUNCTION__);
        EXCEPTION_POINTERS* pExPointers = new EXCEPTION_POINTERS;
        CCrashHandler32::GetExceptionPointers(0, &pExPointers);


        CCrashHandler32::seh_filter(0, pExPointers);

    }
    void __cdecl CCrashHandler32::UnexpectedHandler() {
        if (pLogger != nullptr) pLogger->Log(LogLevel::STATUS, __FUNCTION__);

        EXCEPTION_POINTERS* pExPointers = new EXCEPTION_POINTERS;
        CCrashHandler32::GetExceptionPointers(0, &pExPointers);


        CCrashHandler32::seh_filter(0, pExPointers);

    }

    void __cdecl CCrashHandler32::PureCallHandler() {
        if (pLogger != nullptr) pLogger->Log(LogLevel::STATUS, __FUNCTION__);

        EXCEPTION_POINTERS* pExPointers = new EXCEPTION_POINTERS;
        CCrashHandler32::GetExceptionPointers(0, &pExPointers);


        CCrashHandler32::seh_filter(0, pExPointers);

    }

    void __cdecl CCrashHandler32::InvalidParameterHandler(const wchar_t* expression,
        const wchar_t* function, const wchar_t* file,
        unsigned int line, uintptr_t pReserved) 
    {
        if (pLogger != nullptr) pLogger->Log(LogLevel::STATUS, __FUNCTION__);                

        EXCEPTION_POINTERS* pExPointers = new EXCEPTION_POINTERS;
        CCrashHandler32::GetExceptionPointers(0, &pExPointers);


        CCrashHandler32::seh_filter(0, pExPointers);
    }

    int __cdecl CCrashHandler32::NewHandler(size_t) {
        if (pLogger != nullptr) pLogger->Log(LogLevel::STATUS, __FUNCTION__);
        EXCEPTION_POINTERS* pExPointers = new EXCEPTION_POINTERS;
        CCrashHandler32::GetExceptionPointers(0, &pExPointers);


        CCrashHandler32::seh_filter(0, pExPointers);

        return 0;
    }

    void CCrashHandler32::SigabrtHandler(int) {
        if (pLogger != nullptr) pLogger->Log(LogLevel::STATUS, __FUNCTION__);
        EXCEPTION_POINTERS* pExPointers = new EXCEPTION_POINTERS;
        CCrashHandler32::GetExceptionPointers(0, &pExPointers);


        CCrashHandler32::seh_filter(0, pExPointers);

    }
    void CCrashHandler32::SigfpeHandler(int /*code*/, int subcode) {
        if (pLogger != nullptr) pLogger->Log(LogLevel::STATUS, __FUNCTION__);
        EXCEPTION_POINTERS* pExPointers = new EXCEPTION_POINTERS;
        CCrashHandler32::GetExceptionPointers(0, &pExPointers);


        CCrashHandler32::seh_filter(0, pExPointers);

    }
    void CCrashHandler32::SigintHandler(int) {
        if (pLogger != nullptr) pLogger->Log(LogLevel::STATUS, __FUNCTION__);
        EXCEPTION_POINTERS* pExPointers = new EXCEPTION_POINTERS;
        CCrashHandler32::GetExceptionPointers(0, &pExPointers);


        CCrashHandler32::seh_filter(0, pExPointers);

    }
    void CCrashHandler32::SigillHandler(int) {
        if (pLogger != nullptr) pLogger->Log(LogLevel::STATUS, __FUNCTION__);
        EXCEPTION_POINTERS* pExPointers = new EXCEPTION_POINTERS;
        CCrashHandler32::GetExceptionPointers(0, &pExPointers);


        CCrashHandler32::seh_filter(0, pExPointers);

    }
    void CCrashHandler32::SigsegvHandler(int) {
        if (pLogger != nullptr) pLogger->Log(LogLevel::STATUS, __FUNCTION__);
        EXCEPTION_POINTERS* pExPointers = new EXCEPTION_POINTERS;
        CCrashHandler32::GetExceptionPointers(0, &pExPointers);


        CCrashHandler32::seh_filter(0, pExPointers);

    }
    void CCrashHandler32::SigtermHandler(int) {
        if (pLogger != nullptr) pLogger->Log(LogLevel::STATUS, __FUNCTION__);
        EXCEPTION_POINTERS* pExPointers = new EXCEPTION_POINTERS;
        CCrashHandler32::GetExceptionPointers(0, &pExPointers);


        CCrashHandler32::seh_filter(0, pExPointers);

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
    LONG CCrashHandler32::VectoredExceptionHandler(PEXCEPTION_POINTERS pExceptionPtrs) {

        if (pExceptionPtrs->ExceptionRecord->ExceptionCode == STATUS_HEAP_CORRUPTION) {
            if (pLogger != nullptr) pLogger->Log(LogLevel::STATUS, __FUNCTION__);
            CCrashHandler32::seh_filter(STATUS_HEAP_CORRUPTION, pExceptionPtrs);

            return EXCEPTION_EXECUTE_HANDLER;
        }

        return EXCEPTION_CONTINUE_SEARCH;
    }

    void CCrashHandler32::SetProcessExceptionHandlers()
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

    void CCrashHandler32::SetThreadExceptionHandlers()
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


    void CCrashHandler32::GetExceptionPointers(DWORD dwExceptionCode, EXCEPTION_POINTERS** ppExceptionPointers) {
        // The following code was taken from VC++ 8.0 CRT (invarg.c: line 104)

        EXCEPTION_RECORD ExceptionRecord;
        CONTEXT ContextRecord;
        memset(&ContextRecord, 0, sizeof(CONTEXT));

#ifdef _X86_
        __asm {
            mov dword ptr[ContextRecord.Eax], eax
            mov dword ptr[ContextRecord.Ecx], ecx
            mov dword ptr[ContextRecord.Edx], edx
            mov dword ptr[ContextRecord.Ebx], ebx
            mov dword ptr[ContextRecord.Esi], esi
            mov dword ptr[ContextRecord.Edi], edi
            mov word ptr[ContextRecord.SegSs], ss
            mov word ptr[ContextRecord.SegCs], cs
            mov word ptr[ContextRecord.SegDs], ds
            mov word ptr[ContextRecord.SegEs], es
            mov word ptr[ContextRecord.SegFs], fs
            mov word ptr[ContextRecord.SegGs], gs
            pushfd
            pop[ContextRecord.EFlags]
        }
        ContextRecord.ContextFlags = CONTEXT_CONTROL;
#pragma warning(push)
#pragma warning(disable:4311)
        ContextRecord.Eip = (ULONG)_ReturnAddress();
        ContextRecord.Esp = (ULONG)_AddressOfReturnAddress();
#pragma warning(pop)
        ContextRecord.Ebp = *((ULONG*)_AddressOfReturnAddress() - 1);
#elif defined (_IA64_) || defined (_AMD64_)
        /* Need to fill up the Context in IA64 and AMD64. */
        RtlCaptureContext(&ContextRecord);
#else  /* defined (_IA64_) || defined (_AMD64_) */
        ZeroMemory(&ContextRecord, sizeof(ContextRecord));
#endif  /* defined (_IA64_) || defined (_AMD64_) */
        ZeroMemory(&ExceptionRecord, sizeof(EXCEPTION_RECORD));
        ExceptionRecord.ExceptionCode = dwExceptionCode;
        ExceptionRecord.ExceptionAddress = _ReturnAddress();

        EXCEPTION_RECORD* pExceptionRecord = new EXCEPTION_RECORD;
        memcpy(pExceptionRecord, &ExceptionRecord, sizeof(EXCEPTION_RECORD));
        CONTEXT* pContextRecord = new CONTEXT;
        memcpy(pContextRecord, &ContextRecord, sizeof(CONTEXT));
        *ppExceptionPointers = new EXCEPTION_POINTERS;
        (*ppExceptionPointers)->ExceptionRecord = pExceptionRecord;
        (*ppExceptionPointers)->ContextRecord = pContextRecord;
    }

    void CCrashHandler32::CreateLog(EXCEPTION_POINTERS* pExcPtrs) {
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
    }

    // This method creates minidump of the process
    void CCrashHandler32::CreateMiniDump(EXCEPTION_POINTERS* pExcPtrs)
    {
        HMODULE hDbgHelp = NULL;
        HANDLE hFile = NULL;
        MINIDUMP_EXCEPTION_INFORMATION mei;
        MINIDUMP_CALLBACK_INFORMATION mci;

        // Load dbghelp.dll
        hDbgHelp = LoadLibrary(_T("dbghelp.dll"));
        if (hDbgHelp == NULL)
        {
            // Error - couldn't load dbghelp.dll
            return;
        }

        // Create the minidump file
        hFile = CreateFile(
            _T("crashdump.dmp"),
            GENERIC_WRITE,
            0,
            NULL,
            CREATE_ALWAYS,
            FILE_ATTRIBUTE_NORMAL,
            NULL);

        if (hFile == INVALID_HANDLE_VALUE)
        {
            // Couldn't create file
            return;
        }

        // Write minidump to the file
        mei.ThreadId = GetCurrentThreadId();
        mei.ExceptionPointers = pExcPtrs;
        mei.ClientPointers = FALSE;
        mci.CallbackRoutine = NULL;
        mci.CallbackParam = NULL;

        typedef BOOL(WINAPI* LPMINIDUMPWRITEDUMP)(
            HANDLE hProcess,
            DWORD ProcessId,
            HANDLE hFile,
            MINIDUMP_TYPE DumpType,
            CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
            CONST PMINIDUMP_USER_STREAM_INFORMATION UserEncoderParam,
            CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam);

        LPMINIDUMPWRITEDUMP pfnMiniDumpWriteDump =
            (LPMINIDUMPWRITEDUMP)GetProcAddress(hDbgHelp, "MiniDumpWriteDump");
        if (!pfnMiniDumpWriteDump)
        {
            // Bad MiniDumpWriteDump function
            return;
        }

        HANDLE hProcess = GetCurrentProcess();
        DWORD dwProcessId = GetCurrentProcessId();

        BOOL bWriteDump = pfnMiniDumpWriteDump(
            hProcess,
            dwProcessId,
            hFile,
            MemDumpType_,
            &mei,
            NULL,
            &mci);

        if (!bWriteDump)
        {
            // Error writing dump.
            return;
        }

        // Close file
        CloseHandle(hFile);

        // Unload dbghelp.dll
        FreeLibrary(hDbgHelp);
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

    int CCrashHandler32::seh_filter(unsigned int code, struct _EXCEPTION_POINTERS* ep) {
        
        // Create exception log
        CCrashHandler32::CreateLog(ep);

        // Create Memory Dump
        if (DumpMemory_) {
            CCrashHandler32::CreateMiniDump(ep);
        }

        return EXCEPTION_EXECUTE_HANDLER;
    }

    std::string CCrashHandler32::GetExceptionDesc(DWORD Code) {
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
    
    
    using Address = uint64_t;

    Address* StackTop(CONTEXT& context) {
#if defined(_IA64_) || defined(_AMD64_)
        Address* Ptr = reinterpret_cast<Address*>(context.Rsp);
#else
        Address* Ptr = reinterpret_cast<Address*>(context.Esp);
#endif
        return Ptr;
    }


    void CCrashHandler32::WalkStack(struct _EXCEPTION_POINTERS* ep) {
        if (pLogger == nullptr) return;

        return;

        std::ostringstream s;
        CONTEXT& context = *(ep->ContextRecord);

        char symbol_mem[sizeof(IMAGEHLP_SYMBOL) + 256]{};
        IMAGEHLP_SYMBOL* symbol = (IMAGEHLP_SYMBOL*)symbol_mem;
        
        HANDLE process = GetCurrentProcess();
        SymInitialize(process, NULL, TRUE);

        DWORD64 displacement;
        char name[256]{};
        STACKFRAME64         stack{};
        //copyString(out, max_size, "Crash callstack:\n");
        //memset(&stack, 0, sizeof(STACKFRAME64));
        
        HANDLE thread = GetCurrentThread();
        displacement = 0;
        DWORD machineType;
#ifdef _WIN64
        machineType = IMAGE_FILE_MACHINE_IA64;
        stack.AddrPC.Offset = context.Rip;
        stack.AddrPC.Mode = AddrModeFlat;
        stack.AddrStack.Offset = context.Rsp;
        stack.AddrStack.Mode = AddrModeFlat;
        stack.AddrFrame.Offset = context.Rbp;
        stack.AddrFrame.Mode = AddrModeFlat;
#else
        machineType = IMAGE_FILE_MACHINE_I386;
        stack.AddrPC.Offset = context.Eip;
        stack.AddrPC.Mode = AddrModeFlat;
        stack.AddrStack.Offset = context.Esp;
        stack.AddrStack.Mode = AddrModeFlat;
        stack.AddrFrame.Offset = context.Ebp;
        stack.AddrFrame.Mode = AddrModeFlat;
#endif
        BOOL result = false;
        DWORD ErrorCode = 0;
        DWORD count = 0;
        Address* pCurrentStackPtr = StackTop(context);
        do
        {
            

            //result = StackWalk64(machineType,
            //    process,
            //    thread,
            //    &stack,
            //    &context,
            //    NULL,
            //    SymFunctionTableAccess,
            //    SymGetModuleBase,
            //    NULL);
            count = count + 1;
            symbol->SizeOfStruct = sizeof(IMAGEHLP_SYMBOL64);
            symbol->MaxNameLength = 255;

            //SymGetSymFromAddr64(process, (DWORD64)stack.AddrFrame.Offset, &displacement, symbol);
            //SymGetSymFromAddr64(process, (DWORD64)CurrentStackPtr, &displacement, symbol);
            SymGetSymFromAddr(process, (DWORD)*pCurrentStackPtr, 0, symbol);
            UnDecorateSymbolName(symbol->Name, (PSTR)name, 256, UNDNAME_COMPLETE);
            std::string FuncName = symbol->Name;
            if (FuncName.length() > 0)
                s << symbol->Name << std::endl;

            pCurrentStackPtr = pCurrentStackPtr + 1;            

        } while (count < 4096);
        pLogger->Log(LogLevel::STATUS, s);
    }
}



