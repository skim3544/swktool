
#include "pch.h"
#include "Logger.h"
#include "AppBootstrap.h"
#include "CrashHandler.h"
#include "TraceBuffer.h"
#include "TraceMacros.h"

namespace swktool
{
	void BootStrapHelper::Register()
	{
		using namespace swktool;
		TRACE_SCOPE();
		TRACE("Registering Services");
		// register loggers
		AppContext::IOC().Register<ICrashLogger, CCrashLogger>(object_type::Singleton);
		AppContext::IOC().Register<IAppLogger, CAppLogger>(object_type::Singleton);

		// Register Crash handler
		AppContext::IOC().Register<ICrashHandler, CCrashHandler>(object_type::Singleton);
	}

	void BootStrapHelper::ConfigureCrashHandler(LPCTSTR LogFileName, bool bWantMemoryDump, bool bWantTrace)
	{
		using namespace swktool;
		TRACE_SCOPE();

		TRACE("Initializing Crash Logger Service");
		// Resolve logger 
		auto crashLogger = AppContext::IOC().ResolveShared<ICrashLogger>(); 
		crashLogger->init(LogFileName); 

		auto crash = AppContext::IOC().ResolveShared<swktool::ICrashHandler>();

		// we want the memory mini dump
		TRACE("Setting up Crash Handler options");
		crash->EnableMemoryDump(bWantMemoryDump);

		// we want the call trace log
		crash->EnableCallTrace(bWantTrace);		
		crash->SetLogger(crashLogger.get());

		TRACE("Installing exception handler hooks");
		crash->Install();
	}

	void BootStrapHelper::ConfigureAppLogger(LPCTSTR LogFileName)
	{
		TRACE_SCOPE();
		TRACE("Initializing Application Logger Service");

		auto crashLogger = AppContext::IOC().ResolveShared<IAppLogger>();
		crashLogger->init(LogFileName);
	}
}