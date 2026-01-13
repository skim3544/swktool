#pragma once
#include <Windows.h>
#include "Logger.h"
#include <DbgHelp.h>
#include <string>
#include "AppBootstrap.h"

namespace swktool 
{    
    class ICrashHandler
    {
    public:
        virtual ~ICrashHandler() = default;

        // Inject logger after resolving both services
        virtual void SetLogger(ICrashLogger* logger) = 0;

        // enable or disable memory dump
        virtual void EnableMemoryDump(bool bEnable) = 0;

        virtual void EnableCallTrace(bool bEnable) = 0;

        // Install SEH/VEH handlers, configure minidump, etc.
        virtual void Install() = 0;
    };


    /// <summary>
    ///  Exception crash handler
    ///  based on https://www.codeproject.com/Articles/207464/Exception-Handling-in-Visual-Cplusplus
    ///  Modernized by AI
    ///  1. Use Install function to hook into current process/thread
    ///  2. when runnign a separate thread, you would need to call SetThreadExceptionHandlers to hook thread 
    ///  
    /// </summary>
    class CCrashHandler : public ICrashHandler
    {
        static ILogger* pLogger;
        static MINIDUMP_TYPE    MemDumpType_;
        static bool             DumpMemory_;
        static bool             CallTrace_;

    public:
        // Destructor
        virtual ~CCrashHandler() = default;

        // Install SEH/VEH handlers, configure minidump, etc.
        void Install() override
        {
            SetProcessExceptionHandlers();
            SetThreadExceptionHandlers();
        }

        // Inject logger after resolving both services
        void SetLogger(ICrashLogger* logger) override
        {
            InternalSetLogger(static_cast<ILogger*>(logger));
        }


        // enable or disable memory dump
        void EnableMemoryDump(bool bEnable) override
        {
            EnableMemoryDumpFile(bEnable);
        }
        void EnableCallTrace(bool bEnable) override
        {
            EnableCallTraceReport(bEnable);
        }


        void Configure(bool bDumpMemoryOnCrash, MINIDUMP_TYPE oType = MiniDumpNormal) {
            DumpMemory_ = bDumpMemoryOnCrash;
            MemDumpType_ = oType;
        }


        static void EnableMemoryDumpFile(bool bDumpMemoryOnCrash) 
        {
            DumpMemory_ = bDumpMemoryOnCrash;
        }

        static void EnableCallTraceReport(bool bCallTraceReport)
        {
            CallTrace_ = bCallTraceReport;
        }

        // Sets exception handlers that work on per-process basis
        static void SetProcessExceptionHandlers();

        // Installs C++ exception handlers that function on per-thread basis
        static void SetThreadExceptionHandlers();

        static void InternalSetLogger(ILogger* Logger) {
            pLogger = Logger;
        }

        static void GetExceptionPointers(DWORD code, EXCEPTION_POINTERS& out);
        //// Collects current process state.
        //static void GetExceptionPointers(
        //    DWORD dwExceptionCode,
        //    EXCEPTION_POINTERS** pExceptionPointers);

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
        static LONG NTAPI VectoredExceptionHandler(PEXCEPTION_POINTERS pExceptionPtrs);

        static void SigabrtHandler(int);
        static void SigfpeHandler(int /*code*/, int subcode);
        static void SigintHandler(int);
        static void SigillHandler(int);
        static void SigsegvHandler(int);
        static void SigtermHandler(int);
        

        static int seh_filter(unsigned int code, struct _EXCEPTION_POINTERS* ep);

        static std::string GetExceptionDesc(DWORD Code);




        typedef LONG(WINAPI* RtlGetVersionPtr)(PRTL_OSVERSIONINFOW);

        std::wstring GetWindowsVersionString()
        {
            HMODULE hMod = ::GetModuleHandleW(L"ntdll.dll");
            if (!hMod) return L"Unknown Windows version";

            auto fn = (RtlGetVersionPtr)::GetProcAddress(hMod, "RtlGetVersion");
            if (!fn) return L"Unknown Windows version";

            RTL_OSVERSIONINFOW rovi = { 0 };
            rovi.dwOSVersionInfoSize = sizeof(rovi);

            if (fn(&rovi) != 0)
                return L"Unknown Windows version";

            wchar_t buffer[256];
            swprintf_s(buffer, L"Windows %lu.%lu (Build %lu), %s",
                rovi.dwMajorVersion,
                rovi.dwMinorVersion,
                rovi.dwBuildNumber,
                rovi.szCSDVersion[0] ? rovi.szCSDVersion : L"");

            return buffer;
        }

    protected:
        static void HandleCrash(unsigned code, EXCEPTION_POINTERS* ep);
        static void WalkStack(struct _EXCEPTION_POINTERS* ep);
    };

    /// <summary>
    /// RAII version of process-handler hook
    /// </summary>
    struct CrashHandlerProcessScope 
    {
        CrashHandlerProcessScope(bool bCreateDunpFile = TRUE) 
        {
            CCrashHandler::EnableMemoryDumpFile(bCreateDunpFile);
            CCrashHandler::SetProcessExceptionHandlers();
        }
    };


    /// <summary>
    /// RAII version of thread-handler hook
    /// Every new thread created should start with this object
    /// </summary>
    struct CrashHandlerScope 
    {
        CrashHandlerScope() {
            CCrashHandler::SetThreadExceptionHandlers();
        }

        // Nothing special to clean up — handlers stay installed
        ~CrashHandlerScope() = default;
    };



}