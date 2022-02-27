#pragma once

#include <Windows.h>
#include <sysinfoapi.h>
#include <ostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <mutex>
#include <vector>
#include <assert.h>

#include "..\SWKUI\WinFile.h"
#include "..\SWKUI\WinKernel.h"


namespace swktool {

    //
    // Common Logging Levels
    //
    enum class LogLevel : int {
        NONE = 0,
        ERRORS,
        DETAIL1,
        DETAIL2,
        DETAIL3
    };


    class FileStreamBuf : public std::stringbuf
    {
    protected:
        WinFile  File_;

        SYSTEMTIME  LastLogTime_;
    public:
        FileStreamBuf() : std::stringbuf() { LastLogTime_ = {}; }
        FileStreamBuf(LPCTSTR fname) :
            File_(fname, FileAccessMode::SharedWrite, nullptr,
            swktool::CreationDisposition::OpenAlways)
        {
            // append data
            LARGE_INTEGER itg{ 0,0 };
            File_.Seek(itg, FileSeekMethod::END);            
            ::GetSystemTime(&LastLogTime_);
        }

        ~FileStreamBuf() { sync(); }

        int sync()
        {
            std::string LogLine;
            // if data in buffer
            if (str().length() > 0) {
                // merge with the time stamp, write
                std::string LogLine = GetLogHeader() + str();
                File_.Write((LPVOID*)LogLine.c_str(), (DWORD)LogLine.length());                
            }

            str(std::string()); // Clear the string buffer
            return 0;
        }

        // stabdard output is MM-DD-YYYY HH:MM:SS:ms (ThreadID). "
        virtual std::string GetLogHeader() const {
            SYSTEMTIME st = {  };
            //::GetSystemTime(&st);
            ::GetLocalTime(&st);
            std::ostringstream ossMessage;
            DWORD tid = GetCurrentThreadId();

            // format mm/dd/yyyy hh:mm:ss:msss (tid)."
            ossMessage
                << std::setw(2) << std::setfill('0') << st.wMonth << "-"
                << std::setw(2) << std::setfill('0') << st.wDay << "-"
                << st.wYear << " "
                << std::setw(2) << std::setfill('0') << st.wHour << ":"
                << std::setw(2) << std::setfill('0') << st.wMinute << ":"
                << std::setw(2) << std::setfill('0') << st.wSecond << "."
                << std::setw(3) << std::setfill('0') << st.wMilliseconds
                << "(" << std::setw(8) << std::setfill('0') << tid << ").";
            std::string LogHeader = ossMessage.str();

            return LogHeader;
        }
    };

    class LoggerStream : public std::ostream {
        FileStreamBuf buffer_;
        std::streambuf* pOldbuf;
        LPCTSTR FileName_;


    public:
        LoggerStream(LPCTSTR FileName) : 
            FileName_(FileName), // preserve file name
            pOldbuf(nullptr), 
            buffer_(FileName), 
            std::ostream(&buffer_) {
            pOldbuf = rdbuf(&buffer_);
        }

        ~LoggerStream() {
            if (pOldbuf != nullptr)
                rdbuf(pOldbuf);
        }
    };

    class ILogger {
    public:
        virtual void init(LPCTSTR LogFileName) = 0;        
        virtual void Register(int nLevel, const char* FunctionFullName) = 0;
        virtual void Log(const char* FunctionFullName, int Level, LPCSTR msg) = 0;
        virtual void Log(const char* FunctionFullName, int Level, std::string& msg) = 0;
        virtual void Log(const char* FunctionFullName, int Level, std::ostringstream& msg) = 0;        
    };


    class CriticalSection;
    class Logger {
    protected:
        int currentLevel_;
        CriticalSection cs_;
        std::unique_ptr<LoggerStream> pStream;
        bool bInitialized;

    public:
        Logger();
        virtual ~Logger() { ; }


        void SetLogLevel(int nLevel) {
            currentLevel_ = nLevel;
        }

        virtual void init() {
            pStream = std::make_unique<LoggerStream>(TEXT("app.log"));
            bInitialized = true;
        }

        virtual void Log(int Level, std::ostringstream& msg);

        virtual void Log(int Level, LPCSTR msg) {
            std::lock_guard<CriticalSection> lg(cs_);
            if (Level <= currentLevel_)
                *pStream << msg << std::endl;
        }

        virtual void Log(int Level, std::string& msg) {
            std::lock_guard<CriticalSection> lg(cs_);
            if (Level <= currentLevel_)
                *pStream << msg.c_str() << std::endl;
        }
    };

    class ClassData
    {
    private:
        std::string ClassName_;
        short       LogLevel_;

    public:
        ClassData(short Level, std::string sClassName) : 
            LogLevel_(Level), ClassName_(sClassName) {
        }

        ClassData(const ClassData& Data) :
            LogLevel_(Data.LogLevel_), ClassName_(Data.ClassName_) {
        }

        const std::string& GetName() const { 
            return ClassName_;
        }

        short GetLogLevel() const {
            return LogLevel_;
        }
    };


    typedef std::vector<ClassData> LogLevelList;
    typedef LogLevelList::iterator  LogLevelListItr;

    


    /// <summary>
    /// Object for class level logger
    /// From contstructor of the class, register __function__ with the level.  Depending on level, log message of the class will be logged
    /// When not interested in logging for the class, set log level to 0
    /// </summary>
    class ClassLogger : public ILogger {
    protected:
        static CriticalSection cs_;
        std::unique_ptr<LoggerStream> pStream;
        bool            bInitialized;

        LogLevelList    ClassList_;
        short           DefaultLogLevel_;

        static ClassLogger*     mInst;
        LPCTSTR                 LogFileName_;

    public:
        ClassLogger() :
            bInitialized(false),
            DefaultLogLevel_(0),
            LogFileName_(nullptr)
        {
            LogFileName_ = TEXT("App.log");
        }

        ClassLogger(LPCTSTR fname) :
            bInitialized(false),
            DefaultLogLevel_(0),
            LogFileName_(fname)
        {

        }


        void SetLogLevel(int nLevel) {
            DefaultLogLevel_ = nLevel;
        }


         void init(LPCTSTR LogFileName) override  {
            if (bInitialized == false) {
                std::lock_guard<CriticalSection> lg(cs_);
                if (bInitialized == false) {
                    pStream = std::make_unique<LoggerStream>(LogFileName);
                    bInitialized = true;
                }
            }
        }

        static ClassLogger& Inst() {            
            if (!mInst) {
                std::lock_guard<CriticalSection> lg(cs_);
                if (!mInst) {
                    mInst = new ClassLogger();                    
                }
            }
            return *mInst;
        }

        virtual void Register(int nLevel, const char* FunctionFullName) {
            assert(bInitialized == true);
            std::lock_guard<CriticalSection> lg(cs_);            

            auto ClassNamePair = GetClassName(FunctionFullName);
            auto& ClassName = ClassNamePair.first;
            //auto MethodName = ClassNamePair.second;

            if (IsClassRegistered(ClassName) == false)
            {
                ClassData NewData((short)nLevel, ClassName);
                ClassList_.push_back(NewData);
            }
        }

        virtual void Log(const char* FunctionFullName, int Level, LPCSTR msg) {
            std::lock_guard<CriticalSection> lg(cs_);

            auto ClassNamePair = GetClassName(FunctionFullName);
            auto& ClassName = ClassNamePair.first;
            LogLevelListItr FindItr = std::find_if(ClassList_.begin(), ClassList_.end(), [ClassName](const ClassData& Data){ return Data.GetName() == ClassName; });

            if (FindItr != ClassList_.end()) {
                auto& Item = *FindItr;
                auto currentLevel_ = Item.GetLogLevel();

                if (Level <= currentLevel_ || Level <= DefaultLogLevel_) {
                    std::string outputmsg = FunctionFullName + std::string(" - ") + msg;
                    *pStream << outputmsg.c_str() << std::endl;
                }
            }                       
        }

        virtual void Log(const char* FunctionFullName, int Level, std::string& msg) {
            Log(FunctionFullName, Level, msg.c_str());
        }

        virtual void Log(const char* FunctionFullName, int Level, std::ostringstream& msg) {
            Log(FunctionFullName, Level, msg.str().c_str());            
        }

        //
        // Will system log this function,Level?
        //   Used when constructing log data takes long CPU time, no point wastimg time
        //   when data will not even be logged
        //
        bool WillBeLogged(const char* FunctionFullName, int Level) {
            std::lock_guard<CriticalSection> lg(cs_);
            bool bLogged = false;
            auto ClassNamePair = GetClassName(FunctionFullName);
            auto& ClassName = ClassNamePair.first;
            LogLevelListItr FindItr = std::find_if(ClassList_.begin(), ClassList_.end(), [ClassName](const ClassData& Data) { return Data.GetName() == ClassName; });

            if (FindItr != ClassList_.end()) {
                auto& Item = *FindItr;
                auto currentLevel_ = Item.GetLogLevel();

                if (Level <= currentLevel_ || Level <= DefaultLogLevel_) {
                    bLogged = true;
                }
            }

            return bLogged;
        }
        
    protected:
        std::pair<std::string, std::string> GetClassName(const char* FullFuncName) const
        {
            std::string funcName(FullFuncName);
            std::string MethodName;
            std::string ClassName;

            // if in ClassName::Method format, pull out class name
            auto SepLoc = funcName.find("::");
            if (SepLoc != std::string::npos) {
                ClassName = funcName.substr(0, SepLoc);
                MethodName = funcName.substr(SepLoc+2);
            }
            else {
                ClassName = FullFuncName;
                MethodName = "";
            }

            return std::make_pair(ClassName, MethodName);
        }

        bool IsClassRegistered(const std::string ClassName) const {
            if (std::find_if(ClassList_.begin(), ClassList_.end(), [ClassName](const ClassData& Data) { return Data.GetName() == ClassName; }) != ClassList_.end())
            {
                return true;
            }
            else
            {
                return false;
            }            
        }
    };
}

