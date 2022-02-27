#include "pch.h"
#include "Logger.h"
#include "..\SWKUI\WinKernel.h"

namespace swktool {    
    CriticalSection ClassLogger::cs_;
    ClassLogger* ClassLogger::mInst = nullptr;

    Logger::Logger() : bInitialized(false), currentLevel_(0) { ; }


    void Logger::Log(int Level, std::ostringstream& msg) {
        CS_Guard lg(cs_);        
        if (Level <= currentLevel_)
            *pStream << msg.str() << std::endl;
    }

    
}