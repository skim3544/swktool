#include "pch.h"
#include "Logger.h"
#include "..\SWKUI\WinKernel.h"

namespace swktool {    
    //CriticalSection LevelLogger::cs_;
    //std::unique_ptr<LoggerStream> LevelLogger::pStream = std::unique_ptr<LoggerStream>(nullptr);

    //bool            LevelLogger::bInitialized = false;;
    //LogLevelList    LevelLogger::ClassList_;    
    //LogLevel        LevelLogger::DefaultLogLevel_ = LogLevel::STATUS;



    Logger::Logger() : 
        bInitialized(false), 
        currentLevel_(LogLevel::STATUS),
        DefaultLogLevel_((LogLevel::STATUS))
    { 
        ; 
    }


    
}