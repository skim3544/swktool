// ExceptionTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "../SWKBase/Exception.h"
#include "../SWKBase/Logger.h"
#include "../SWKBase/IOC.h"
#include "../SWKBase/TraceBuffer.h"
#include "../SWKBase/TraceMacros.h"
 
swktool::IOCContainer& GetIOC() 
{
    static swktool::IOCContainer oIOC;
    return oIOC;
}

void foo1()
{
    TRACE_SCOPE();
    
    TRACE("Setting pointer to nullptr");
    int* p = nullptr;
    
    *p = 123; // Access violation
}

void SuccessfulCall()
{
    TRACE_SCOPE();    
}


void foo()
{
    try {
        char buffer[10]{};
        sprintf_s(buffer, _countof(buffer), "012345678901234567890");
        using namespace swktool;
        //Logger* pLogger = GetDI().Resolve<ILogger, Logger>();
        Logger* pLogger = nullptr;
        pLogger->Log(LogLevel::STATUS, "test");


    }
    catch (...) {
        ExitProcess(2);
    }

}

int main()
{
    TRACE_SCOPE();
    using namespace swktool;

    CrashHandlerProcessScope scope_hook;
    CrashHandlerScope thread_hook;

    GetIOC().Register<ILogger, Logger>(object_type::Singleton);

    auto* pLogger = GetIOC().ResolveRaw<ILogger>();
    pLogger->init(TEXT("Exception.log"));
    pLogger->Register("Exception", LogLevel::STATUS);
    CCrashHandler::SetLogger(pLogger);
    
    pLogger->Log(swktool::LogLevel::STATUS, "STARTING");
    SuccessfulCall();

    foo1();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
