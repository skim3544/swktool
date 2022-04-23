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
        STATUS,
        ERRORS,
        DETAIL1,
        DETAIL2,
        DETAIL3
    };

    // Common Logger Interface
    class ILogger {
    public:
        virtual ~ILogger() { ; }
        virtual void init(LPCTSTR LogFileName) = 0;
        virtual void SetLogLevel(LogLevel Level) = 0;
        virtual void Register(const char* FunctionFullName, LogLevel nLevel) = 0;

        virtual void Log(const char* FunctionFullName, LogLevel Level, LPCSTR msg) = 0;
        virtual void Log(const char* FunctionFullName, LogLevel Level, const std::string& msg) = 0;
        virtual void Log(const char* FunctionFullName, LogLevel Level, std::ostringstream& msg) = 0;

        virtual void Log(LogLevel Level, LPCSTR msg) = 0;
        virtual void Log(LogLevel Level, const std::string& msg) = 0;
        virtual void Log(LogLevel Level, std::ostringstream& msg) = 0;
    };


    class FileStreamBuf : public std::stringbuf
    {
    protected:
        WinFile  File_;
        SYSTEMTIME  LastTime_;
    public:
        FileStreamBuf() : std::stringbuf() { ::GetLocalTime(&LastTime_); }
        FileStreamBuf(LPCTSTR fname) :
            File_(fname, FileAccessMode::SharedWrite, nullptr,
            swktool::CreationDisposition::OpenAlways)
        {
            // append data
            LARGE_INTEGER itg{ 0,0 };
            File_.Seek(itg, FileSeekMethod::END);            
            
            ::GetLocalTime(&LastTime_);            
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
        virtual std::string GetLogHeader() {
            SYSTEMTIME LocalTime = {  };
            ::GetLocalTime(&LocalTime);

            std::ostringstream ossMessage;
            DWORD tid = GetCurrentThreadId();

            // format mm/dd/yyyy hh:mm:ss:msss (tid)."
            ossMessage
                << std::setw(2) << std::setfill('0') << LocalTime.wMonth << "-"
                << std::setw(2) << std::setfill('0') << LocalTime.wDay << "-"
                << LocalTime.wYear << " "
                << std::setw(2) << std::setfill('0') << LocalTime.wHour << ":"
                << std::setw(2) << std::setfill('0') << LocalTime.wMinute << ":"
                << std::setw(2) << std::setfill('0') << LocalTime.wSecond << "."
                << std::setw(3) << std::setfill('0') << LocalTime.wMilliseconds
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



    /// <summary>
    /// dummy logger that does not do anything
    /// </summary>
    class NoLogger : public ILogger
    {
        void init(LPCTSTR LogFileName) { ; }
        void Register(const char* FunctionFullName, LogLevel nLevel) { ; }
        void SetLogLevel(LogLevel Level) { ; }
        void Log(const char* FunctionFullName, LogLevel Level, LPCSTR msg) { ; }
        void Log(const char* FunctionFullName, LogLevel Level, const std::string& msg) { ; }
        void Log(const char* FunctionFullName, LogLevel Level, std::ostringstream& msg) { ; }

        void Log(LogLevel Level, LPCSTR msg) { ; }
        void Log(LogLevel Level, std::string& msg) { ; }
        void Log(LogLevel Level, std::ostringstream& msg) { ; }
    };






    /// <summary>
    ///  Used for Class level logging
    /// </summary>
    class ClassLoggingData
    {
    private:
        std::string ClassName_;
        LogLevel     LogLevel_;

    public:
        ClassLoggingData(LogLevel Level, std::string sClassName) :
            LogLevel_(Level), ClassName_(sClassName) {
        }

        ClassLoggingData(const ClassLoggingData& Data) :
            LogLevel_(Data.LogLevel_), ClassName_(Data.ClassName_) {
        }

        const std::string& GetName() const { 
            return ClassName_;
        }

        LogLevel GetLogLevel() const {
            return LogLevel_;
        }
    };

    

    class CriticalSection;
    class Logger : public ILogger {
        using LogLevelList = std::vector<ClassLoggingData>;
        using LogLevelListItr = LogLevelList::iterator;

    protected:
        LogLevel currentLevel_;
        CriticalSection cs_;
        std::unique_ptr<LoggerStream> pStream;
        bool bInitialized;

        //  list of Class, level pair.
        LogLevelList    ClassList_;

        // default logging level
        LogLevel        DefaultLogLevel_;

    public:
        Logger();
        virtual ~Logger() { ; }


        /// <summary>
        /// Sets the default logging level
        /// </summary>
        /// <param name="nLevel"></param>
        void SetLogLevel(LogLevel nLevel) override {
            currentLevel_ = nLevel;
        }
              

        /// <summary>
        /// Initialize, pass the logging file name
        /// </summary>
        /// <param name="LogFileName"></param>
        virtual void init(LPCTSTR LogFileName) override {
            if (bInitialized == false)
            {
                {
                    std::lock_guard<CriticalSection> lg(cs_);
                    pStream = std::make_unique<LoggerStream>(LogFileName);
                    bInitialized = true;
                }
            }
        }

        /// <summary>
        /// Register the functional logging level
        /// </summary>
        /// <param name="nLevel"></param>
        /// <param name="FunctionFullName"></param>
        void Register(const char* FunctionFullName, LogLevel nLevel) override {
            assert(bInitialized == true);
            std::lock_guard<CriticalSection> lg(cs_);

            auto ClassNamePair = GetClassNamePair(FunctionFullName);
            auto& ClassName = ClassNamePair.first;
            //auto& MethodName = ClassNamePair.second;

            // register only if new class
            if (IsClassRegistered(ClassName) == false)
            {
                ClassLoggingData NewData(nLevel, ClassName);
                ClassList_.push_back(NewData);
            }
        }

        void Log(const char* FunctionFullName, LogLevel Level, LPCSTR msg) override {
            assert(bInitialized == true);
            std::lock_guard<CriticalSection> lg(cs_);

            auto ClassNamePair = GetClassNamePair(FunctionFullName);
            auto& ClassName = ClassNamePair.first;
            LogLevelListItr FindItr = std::find_if(ClassList_.begin(), ClassList_.end(), [ClassName](const auto& Data) { return Data.GetName() == ClassName; });

            if (FindItr != ClassList_.end()) {
                auto& Item = *FindItr;
                auto currentLevel_ = Item.GetLogLevel();

                if ((int)Level <= (int)currentLevel_ || (int)Level <= (int)DefaultLogLevel_) {
                    std::string outputmsg = FunctionFullName + std::string(" - ") + msg;
                    *pStream << outputmsg.c_str() << std::endl;
                }
            }
        }

        void Log(const char* FunctionFullName, LogLevel Level, const std::string& msg) override {            
            Log(FunctionFullName, Level, msg.c_str());
        }

        void Log(const char* FunctionFullName, LogLevel Level, std::ostringstream& msg) override {            
            Log(FunctionFullName, Level, msg.str().c_str());
        }

        void Log(LogLevel Level, LPCSTR msg) override { 
            assert(bInitialized == true);
            std::lock_guard<CriticalSection> lg(cs_);

            if ((int)Level <= (int)currentLevel_ || (int)Level <= (int)DefaultLogLevel_) {
                //std::string outputmsg = msg;
                *pStream << msg << std::endl;
            }
        }

        void Log(LogLevel Level, const std::string& msg) override {  
            Log(Level, msg.c_str());
        }

        void Log(LogLevel Level, std::ostringstream& msg) override { 
            Log(Level, msg.str().c_str());
        }

    private:
        std::pair<std::string, std::string> GetClassNamePair(const char* FullFuncName)
        {
            std::string funcName(FullFuncName);
            std::string MethodName;
            std::string ClassName;

            // if in ClassName::Method format, pull out class name
            auto SepLoc = funcName.find("::");
            if (SepLoc != std::string::npos) {
                ClassName = funcName.substr(0, SepLoc);
                MethodName = funcName.substr(SepLoc + 2);
            }
            else {
                ClassName = FullFuncName;
                MethodName = "";
            }

            return std::make_pair(ClassName, MethodName);
        }

        bool IsClassRegistered(const std::string ClassName) {
            if (std::find_if(ClassList_.begin(), ClassList_.end(), [ClassName](const auto& Data) { return Data.GetName() == ClassName; }) != ClassList_.end())
            {
                return true;
            }
            else
            {
                return false;
            }
        }
    };


    ///// <summary>
    ///// Object for class level logger
    ///// From contstructor of the class, register __function__ with the level.  Depending on level, log message of the class will be logged
    ///// When not interested in logging for the class, set log level to 0
    ///// </summary>
    //class LevelLogger {

    //protected:
    //    // critical section to prevent multiple threads from corrupting the logging process
    //    static CriticalSection cs_;

    //    // output stream
    //    static std::unique_ptr<LoggerStream> pStream;

    //    // set to true if initiaized
    //    static bool            bInitialized;

    //    //  list of Class, level pair.
    //    static LogLevelList    ClassList_;

    //    // default logging level
    //    static LogLevel        DefaultLogLevel_;

    //public:
    //    
    //    static void SetLogLevel(LogLevel nLevel) {
    //        DefaultLogLevel_ = nLevel;
    //    }

    //    static void init(LPCTSTR LogFileName)   {
    //        if (bInitialized == false) {
    //            std::lock_guard<CriticalSection> lg(cs_);
    //            if (bInitialized == false) {
    //                pStream = std::make_unique<LoggerStream>(LogFileName);
    //                bInitialized = true;
    //            }
    //        }
    //    }

    //    static void Register(LogLevel nLevel, const char* FunctionFullName) {
    //        assert(bInitialized == true);
    //        std::lock_guard<CriticalSection> lg(cs_);            

    //        auto ClassNamePair = GetClassNamePair(FunctionFullName);
    //        auto& ClassName = ClassNamePair.first;
    //        //auto& MethodName = ClassNamePair.second;

    //        if (IsClassRegistered(ClassName) == false)
    //        {
    //            ClassLoggingData NewData(nLevel, ClassName);
    //            ClassList_.push_back(NewData);
    //        }
    //    }

    //    static void Log(const char* FunctionFullName, LogLevel Level, LPCSTR msg) {
    //        assert(bInitialized == true);
    //        std::lock_guard<CriticalSection> lg(cs_);

    //        auto ClassNamePair = GetClassNamePair(FunctionFullName);
    //        auto& ClassName = ClassNamePair.first;
    //        LogLevelListItr FindItr = std::find_if(ClassList_.begin(), ClassList_.end(), [ClassName](const auto& Data){ return Data.GetName() == ClassName; });

    //        if (FindItr != ClassList_.end()) {
    //            auto& Item = *FindItr;
    //            auto currentLevel_ = Item.GetLogLevel();

    //            if ((int)Level <= (int)currentLevel_ || (int)Level <= (int)DefaultLogLevel_) {
    //                std::string outputmsg = FunctionFullName + std::string(" - ") + msg;
    //                *pStream << outputmsg.c_str() << std::endl;
    //            }
    //        }                       
    //    }

    //    static void Log(const char* FunctionFullName, LogLevel Level, std::string& msg) {
    //        assert(bInitialized == true);
    //        Log(FunctionFullName, Level, msg.c_str());
    //    }

    //    static void Log(const char* FunctionFullName, LogLevel Level, std::ostringstream& msg) {
    //        assert(bInitialized == true);
    //        Log(FunctionFullName, Level, msg.str().c_str());            
    //    }

    //    //
    //    // Will system log this function,Level?
    //    //   Used when constructing log data takes long CPU time, no point wastimg time
    //    //   when data will not even be logged
    //    //
    //    static bool WillBeLogged(const char* FunctionFullName, LogLevel Level) {
    //        assert(bInitialized == true);
    //        std::lock_guard<CriticalSection> lg(cs_);
    //        bool bLogged = false;
    //        auto ClassNamePair = GetClassNamePair(FunctionFullName);
    //        auto& ClassName = ClassNamePair.first;
    //        LogLevelListItr FindItr = std::find_if(ClassList_.begin(), ClassList_.end(), [ClassName](const auto& Data) { return Data.GetName() == ClassName; });

    //        if (FindItr != ClassList_.end()) {
    //            auto& Item = *FindItr;
    //            auto currentLevel_ = Item.GetLogLevel();

    //            if ((int)Level <= (int)currentLevel_ || (int)Level <= (int)DefaultLogLevel_) {
    //                bLogged = true;
    //            }
    //        }

    //        return bLogged;
    //    }
    //    
    //protected:
    //    static std::pair<std::string, std::string> GetClassNamePair(const char* FullFuncName)
    //    {
    //        std::string funcName(FullFuncName);
    //        std::string MethodName;
    //        std::string ClassName;

    //        // if in ClassName::Method format, pull out class name
    //        auto SepLoc = funcName.find("::");
    //        if (SepLoc != std::string::npos) {
    //            ClassName = funcName.substr(0, SepLoc);
    //            MethodName = funcName.substr(SepLoc+2);
    //        }
    //        else {
    //            ClassName = FullFuncName;
    //            MethodName = "";
    //        }

    //        return std::make_pair(ClassName, MethodName);
    //    }

    //    static bool IsClassRegistered(const std::string ClassName) {
    //        if (std::find_if(ClassList_.begin(), ClassList_.end(), [ClassName](const auto& Data) { return Data.GetName() == ClassName; }) != ClassList_.end())
    //        {
    //            return true;
    //        }
    //        else
    //        {
    //            return false;
    //        }            
    //    }
    //};


    /// <summary>
    ///  This class, logs the "Entering" at construction and logs "Leaving" when going out of scope from destructor
    /// </summary>
    class LogScope {
    public:
        LogScope(const std::string& scope_name, ILogger& Logger, LogLevel level = LogLevel::DETAIL2) :
            scope_name_(scope_name),
            pLogger_(Logger),
            LogLevel_(level) 
        {
            pLogger_.Log(LogLevel_, "Entering - " + scope_name);
        }

        ~LogScope() {
            pLogger_.Log(LogLevel_, "Leaving - " + scope_name_);
        }

    protected:
        ILogger& pLogger_;
        LogLevel LogLevel_;
        std::string scope_name_;
    };

    class HexDump {
        unsigned char* pData_;
        size_t Len_;
    public:
        HexDump(unsigned char* pData, size_t Len) : pData_(pData), Len_(Len) { ; }
        std::string GetData() const {
            std::string HexStr;
            std::string ASCStr;
            GetData(HexStr, ASCStr);

            std::string Total = HexStr + " " + ASCStr;
            return Total;
        }

        void GetData(std::string& Hex, std::string& Ascii, char Sep = ' ') const {
            bool bFirstData = true;
            std::stringstream str;
            std::stringstream Asciistr;
            for (size_t i = 0; i < Len_; i++) {
                if (bFirstData) {
                    str << std::setw(2) << std::hex  << std::uppercase  << (int)pData_[i];
                    Asciistr << pData_[i];
                    bFirstData = false;
                }
                else {
                    str << Sep << std::setw(2) << std::hex << std::uppercase << (int)pData_[i];
                    Asciistr << pData_[i];
                }
            }
            Hex = str.str();
            Ascii = Asciistr.str();
        }
    };
}

