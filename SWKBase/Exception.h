#pragma once
#include <Windows.h>
#include <DbgHelp.h>
#include "Logger.h"
//#include <rtcapi.h>

namespace swktool {

    

    /// <summary>
    ///  Exception crash handler
    ///  based on https://www.codeproject.com/Articles/207464/Exception-Handling-in-Visual-Cplusplus
    /// </summary>
    class CCrashHandler
    {
        static ILogger*  pLogger;    
        static MINIDUMP_TYPE    MemDumpType_;
        static bool             DumpMemory_;

    public:
        // Destructor
        virtual ~CCrashHandler() { ; }

        void Configure(bool bDumpMemoryOnCrash, MINIDUMP_TYPE oType = MiniDumpNormal) {
            DumpMemory_ = bDumpMemoryOnCrash;
            MemDumpType_ = oType;
        }

        // Sets exception handlers that work on per-process basis
        static void SetProcessExceptionHandlers();

        // Installs C++ exception handlers that function on per-thread basis
        static void SetThreadExceptionHandlers();

        static void SetLogger(ILogger* Logger) {
            pLogger = Logger;
        }

        // Collects current process state.
        static void GetExceptionPointers(
            DWORD dwExceptionCode,
            EXCEPTION_POINTERS** pExceptionPointers);

        static void CreateLog(EXCEPTION_POINTERS* pExcPtrs);

        // This method creates minidump of the process
        static void CreateMiniDump(EXCEPTION_POINTERS* pExcPtrs);

        /* Exception handler functions. */

        static LONG WINAPI SehHandler(PEXCEPTION_POINTERS pExceptionPtrs);
        static void __cdecl TerminateHandler();
        static void __cdecl UnexpectedHandler();

        static void __cdecl PureCallHandler();

        static void __cdecl InvalidParameterHandler(const wchar_t* expression,
            const wchar_t* function, const wchar_t* file,
            unsigned int line, uintptr_t pReserved);

        static int __cdecl NewHandler(size_t);

        static void SigabrtHandler(int);
        static void SigfpeHandler(int /*code*/, int subcode);
        static void SigintHandler(int);
        static void SigillHandler(int);
        static void SigsegvHandler(int);
        static void SigtermHandler(int);

        static int seh_filter(unsigned int code, struct _EXCEPTION_POINTERS* ep);
    };
}