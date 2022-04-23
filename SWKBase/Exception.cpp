#include "pch.h"
#include "Exception.h"
#include <signal.h>
#include <DbgHelp.h>
#include <new.h>
#include <exception>
#include <tchar.h>
#include <intrin.h>
#include "Logger.h"


#pragma intrinsic(_ReturnAddress)
#pragma intrinsic(_AddressOfReturnAddress)


namespace swktool {
    ILogger* CCrashHandler::pLogger = nullptr;
    MINIDUMP_TYPE    CCrashHandler::MemDumpType_ = MiniDumpNormal;
    bool CCrashHandler::DumpMemory_ = true;


    LONG WINAPI CCrashHandler::SehHandler(PEXCEPTION_POINTERS pExceptionPtrs) {

        return 0l;
    }

    void __cdecl CCrashHandler::TerminateHandler() {

    }
    void __cdecl CCrashHandler::UnexpectedHandler() {

    }

    void __cdecl CCrashHandler::PureCallHandler() {

    }

    void __cdecl CCrashHandler::InvalidParameterHandler(const wchar_t* expression,
        const wchar_t* function, const wchar_t* file,
        unsigned int line, uintptr_t pReserved) {

    }

    int __cdecl CCrashHandler::NewHandler(size_t) {
        return 0;
    }

    void CCrashHandler::SigabrtHandler(int) {

    }
    void CCrashHandler::SigfpeHandler(int /*code*/, int subcode) {

    }
    void CCrashHandler::SigintHandler(int) {

    }
    void CCrashHandler::SigillHandler(int) {

    }
    void CCrashHandler::SigsegvHandler(int) {

    }
    void CCrashHandler::SigtermHandler(int) {

    }


    void CCrashHandler::SetProcessExceptionHandlers()
    {
        // Install top-level SEH handler
        SetUnhandledExceptionFilter(SehHandler);

        // Catch pure virtual function calls.
        // Because there is one _purecall_handler for the whole process, 
        // calling this function immediately impacts all threads. The last 
        // caller on any thread sets the handler. 
        // http://msdn.microsoft.com/en-us/library/t296ys27.aspx
        _set_purecall_handler(PureCallHandler);

        // Catch new operator memory allocation exceptions
        _set_new_handler(NewHandler);

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


    void CCrashHandler::GetExceptionPointers(DWORD dwExceptionCode, EXCEPTION_POINTERS** ppExceptionPointers) {
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

    void CCrashHandler::CreateLog(EXCEPTION_POINTERS* pExcPtrs) {        
        if (pLogger == nullptr) return;


        std::ostringstream str;
        str << "Exception Thrown:" << std::endl;

        pLogger->Log(LogLevel::STATUS, str);

    }

    // This method creates minidump of the process
    void CCrashHandler::CreateMiniDump(EXCEPTION_POINTERS* pExcPtrs)
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

    int CCrashHandler::seh_filter(unsigned int code, struct _EXCEPTION_POINTERS* ep) {
        
        // Create exception log
        CCrashHandler::CreateLog(ep);

        // Create Memory Dump
        if (DumpMemory_) {
            CCrashHandler::CreateMiniDump(ep);
        }
//        EXCEPTION_ACCESS_VIOLATION
        return EXCEPTION_EXECUTE_HANDLER;
    }

    std::string GetExceptionDesc(DWORD Code) {
        struct { 
            DWORD Code;
            LPCSTR Deesc;
        } Test[] = {
            {EXCEPTION_ACCESS_VIOLATION, "Access Violation"},
            {EXCEPTION_ARRAY_BOUNDS_EXCEEDED, "Array bounds exceeded Violation"},
            {EXCEPTION_DATATYPE_MISALIGNMENT, "Data Misalignment"},
            {EXCEPTION_FLT_DENORMAL_OPERAND, "Denomal operand"},
            {EXCEPTION_FLT_DIVIDE_BY_ZERO, "Divide by Zero"},
            {EXCEPTION_INT_DIVIDE_BY_ZERO, "Divide by Zero"},
            {EXCEPTION_INVALID_HANDLE, "Invalid Handle"}
        }
    }
}

