#pragma once

#include <Windows.h>
#include "Logger.h"
#include "IOC.h"
#include "AppContext.h"
#include "TraceBuffer.h"
#include "../SWKUI/MsgLoop.h"


namespace swktool
{
	class IAppLogger : public virtual ILogger {};
	class ICrashLogger : public virtual ILogger {};

#pragma warning(push) 
#pragma warning(disable : 4250)

	class CAppLogger : public Logger, public IAppLogger {

	};

	class CCrashLogger : public Logger, public ICrashLogger {

	};

#pragma warning(pop) 
	/// <summary>
	/// Help functions for processing Bootstrap
	/// </summary>
	class BootStrapHelper 
	{
	public:
		void Register();

		void ConfigureCrashHandler(LPCTSTR LogFileName, bool bWantMemoryDump = true, bool bWantTrace = true);
		void ConfigureAppLogger(LPCTSTR LogFileName);

		~BootStrapHelper() 
		{
			delete pCrashLogger; pCrashLogger = nullptr;
			pAppLogger = nullptr;
		}

		
		ICrashLogger* pCrashLogger = nullptr;
		ILogger* pAppLogger = nullptr;
	};


	class DefaultAppBootstrap
	{
		BootStrapHelper helper_;
	public:
		DefaultAppBootstrap() = default;
		~DefaultAppBootstrap() = default;

		void Init()
		{
			using namespace swktool;			
			helper_.Register();
			helper_.ConfigureCrashHandler(TEXT("Exception.log"));
			helper_.ConfigureAppLogger(TEXT("App.log"));
		}

		int Run() { return 0; };


	private:
		inline static swktool::Logger  logger_;

	};

	class DefaultWinAppBootstrap 
	{
		BootStrapHelper helper_;
	public:
		void Init()
		{
			using namespace swktool;
			helper_.Register();
			helper_.ConfigureCrashHandler(TEXT("Exception.log"));
			helper_.ConfigureAppLogger(TEXT("App.log"));
		}		

		int Run()
		{
			swktool::AppMsgLoop MsgLoop;
			return MsgLoop.Run();
		}
	};

	/// <summary>
	/// Bootstrap for Secure Application
	/// which cannot have memory mini dump (which is disabled by default)
	/// </summary>
	class SecureWinAppBootstrap
	{
		BootStrapHelper helper_;
	public:
		void Init()
		{
			using namespace swktool;			
			helper_.Register();

			// disable memory crash dump on this one
			helper_.ConfigureCrashHandler(TEXT("Exception.log"), false, true);
			helper_.ConfigureAppLogger(TEXT("App.log"));
		}
	};
}