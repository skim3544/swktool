#include "pch.h"
#include <iosfwd>
#include <memory>

#include "FileStreamBuf.h"
#include "LoggerStream.h"
#include "OpenOptions.h"
#include "Logger.h"

namespace swktool 
{    
    //CriticalSection LevelLogger::cs_;
    //std::unique_ptr<LoggerStream> LevelLogger::pStream = std::unique_ptr<LoggerStream>(nullptr);

    //bool            LevelLogger::bInitialized = false;;
    //LogLevelList    LevelLogger::ClassList_;    
    //LogLevel        LevelLogger::DefaultLogLevel_ = LogLevel::STATUS;


    ////////////////////////////////////////////
    Logger::Logger() : 
        bInitialized(false), 
        currentLevel_(LogLevel::STATUS),
        DefaultLogLevel_(LogLevel::STATUS)
    { 
        ; 
    }    



	/// <summary>
	/// Initialize, pass the logging file name?
	/// </summary>
	/// <param name="LogFileName"></param>
	void Logger::init(LPCTSTR LogFileName)
	{
		if (bInitialized == false)
		{
			{
				std::lock_guard<CriticalSection> lg(cs_);
				pStream_ = std::make_unique<LoggerStream>(LogFileName);
				bInitialized = true;
			}
		}
	}



} //namespace swktool 