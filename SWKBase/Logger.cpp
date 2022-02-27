#include "pch.h"
#include "Logger.h"
#include "..\SWKUI\WinKernel.h"

namespace swktool {    
    CriticalSection LevelLogger::cs_;
    std::unique_ptr<LoggerStream> LevelLogger::pStream = std::unique_ptr<LoggerStream>(nullptr);
    bool            LevelLogger::bInitialized = false;;
    LogLevelList    LevelLogger::ClassList_;    
    int           LevelLogger::DefaultLogLevel_ = 0;

    LPCTSTR                 LevelLogger::LogFileName_ = TEXT("");


    Logger::Logger() : bInitialized(false), currentLevel_(0) { ; }


    void Logger::Log(int Level, std::ostringstream& msg) {
        CS_Guard lg(cs_);        
        if (Level <= currentLevel_)
            *pStream << msg.str() << std::endl;
    }

    
}