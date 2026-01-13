#pragma once

#include <Windows.h>
#include "Logger.h"
#include "IOC.h"
#include "AppContext.h"
#include "TraceBuffer.h"
#include "../SWKUI/MsgLoop.h"


namespace swktool
{

	// this IOC require unique Interface per object association
	// we accomplish the limitation by creating unique interface signatures
	class IAppLogger : public virtual ILogger {};
	class ICrashLogger : public virtual ILogger {};

#pragma warning(push) 
#pragma warning(disable : 4250)

	// make sure the two of the same logers are made unique
	// by inheriting separate interface for the logger
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
	};


	/// <summary>
	/// Common default bootstrap
	/// Installs crash handler with logger exception.log
	/// installs service for application logging
	/// 
	/// </summary>
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

	

	class WinAppBootstrap 
	{
		BootStrapHelper helper_;
	public:

	public:
		bool Init(HINSTANCE hInstance,
			HINSTANCE hPrevInstance,
			LPWSTR    lpCmdLine,
			int       nCmdShow)
		{
			hInstance_ = hInstance;
			hPrevInstance_ = hPrevInstance;
			lpCmdLine_ = lpCmdLine;
			nCmdShow_ = nCmdShow;

			// servie initialixing
			using namespace swktool;
			helper_.Register();
			helper_.ConfigureCrashHandler(TEXT("Exception.log"));
			helper_.ConfigureAppLogger(TEXT("App.log"));

			return true;
		}


		int Run()
		{
			swktool::AppMsgLoop MsgLoop;
			return MsgLoop.Run();
		}

	private:
		HINSTANCE hInstance_ = nullptr;
		HINSTANCE hPrevInstance_ = nullptr;
		LPWSTR    lpCmdLine_ = nullptr;
		int       nCmdShow_ = 0;
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