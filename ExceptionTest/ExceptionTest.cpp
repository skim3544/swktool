// ExceptionTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "../SWKBase/CrashHandler.h"
#include "../SWKBase/Logger.h"
#include "../SWKBase/IOC.h"
#include "../SWKBase/TraceBuffer.h"
#include "../SWKBase/TraceMacros.h"
 
#include "../SWKBase/AppBootstrap.h"

class AppBootStrap : public swktool::DefaultAppBootstrap 
{
public:
    int Run()
    {
        TRACE_SCOPE();

        SuccessfulCall();
        CrashMe();

        return 0;
    }

    void SuccessfulCall()
    {
        TRACE_SCOPE();
    }

    void CrashMe()
    {
        TRACE_SCOPE();

        TRACE("Setting pointer to nullptr");
        int* p = nullptr;

        *p = 123; // Access violation
    }
};




int main()
{
    TRACE_SCOPE();
    using namespace swktool;

    AppBootStrap bs;
    bs.Init();

    return bs.Run();
}
