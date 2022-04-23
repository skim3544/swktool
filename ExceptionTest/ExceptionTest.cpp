// ExceptionTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "../SWKBase/Exception.h"
#include "../SWKBase/Logger.h"
#include "../SWKBase/IOC.h"

int RetZero() {
    return 0;
}


swktool::IOCContainer& GetDI() {
    static swktool::IOCContainer oIOC;
    return oIOC;
}


int main()
{
    using namespace swktool;

    GetDI().Register<ILogger, Logger>(object_type::Singleton);

    auto* pLogger = GetDI().Resolve<ILogger, Logger>();
    pLogger->init(TEXT("Exception.log"));
    pLogger->Register("Exception", LogLevel::STATUS);

    CCrashHandler::SetLogger(pLogger);
    
    __try {
        int value = 1000;
        int value2 = value / RetZero();
        std::cout << "Hello World!\n";
    }
    __except (swktool::CCrashHandler::seh_filter(GetExceptionCode(), GetExceptionInformation()))
    {
         ExitProcess(1);
    }


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
