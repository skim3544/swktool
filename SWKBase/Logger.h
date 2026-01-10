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
#include <string_view>
#include <iosfwd>
#include <memory>


#include "..\SWKUI\WinKernel.h"
#include "WinFile.h"

#include "LoggerStream.h"



namespace swktool
{
	class CriticalSection;
	
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

		virtual void Log(LPCSTR Msg) = 0;
		virtual void Log(const char* FunctionFullName, LogLevel Level, LPCSTR msg) = 0;
		virtual void Log(const char* FunctionFullName, LogLevel Level, const std::string& msg) = 0;
		virtual void Log(const char* FunctionFullName, LogLevel Level, std::ostringstream& msg) = 0;

		virtual void Log(LogLevel Level, LPCSTR msg) = 0;
		virtual void Log(LogLevel Level, const std::string& msg) = 0;
		virtual void Log(LogLevel Level, std::ostringstream& msg) = 0;
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
		void Log(LPCSTR Msg) { ;  }
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


	class LoggerStream;
	class CriticalSection;
	class Logger : public ILogger 
	{
		using LogLevelList = std::vector<ClassLoggingData>;
		//using LogLevelList = std::unordered_set<std::string>
		using LogLevelListItr = LogLevelList::iterator;

	protected:
		LogLevel currentLevel_ = LogLevel::STATUS;
		CriticalSection cs_;
		std::unique_ptr<LoggerStream> pStream_;
		bool bInitialized = false;

		//  list of Class, level pair.
		LogLevelList    ClassList_;

		// default logging level
		LogLevel        DefaultLogLevel_;

	public:
		Logger();
		virtual ~Logger() = default;


		Logger(const Logger&) = delete;
		Logger& operator=(const Logger&) = delete;
		Logger(Logger&&) = delete;
		Logger& operator=(Logger&&) = delete;


		/// <summary>
		/// Sets the default logging level
		/// </summary>
		/// <param name="nLevel"></param>
		void SetLogLevel(LogLevel nLevel) override 
		{
			currentLevel_ = nLevel;
		}


		/// <summary>
		/// Initialize, pass the logging file name
		/// </summary>
		/// <param name="LogFileName"></param>
		virtual void init(LPCTSTR LogFileName) override;

		/// <summary>
		/// Register the functional logging level
		/// </summary>
		/// <param name="nLevel"></param>
		/// <param name="FunctionFullName"></param>
		void Register(const char* FunctionFullName, LogLevel nLevel) override
		{
			if (!bInitialized) return;

			auto ClassNamePair = GetClassNamePair(FunctionFullName);
			auto& ClassName = ClassNamePair.first;
			//auto& MethodName = ClassNamePair.second;

			std::lock_guard<CriticalSection> lg(cs_);
			// register only if new class
			if (IsClassRegistered(ClassName) == false)
			{
				ClassList_.emplace_back(nLevel, ClassName);
			}
		}

		void Log(LPCSTR Msg) override
		{
			std::lock_guard<CriticalSection> lg(cs_);
			*pStream_ << Msg << std::endl;
		}


		void Log(const char* FunctionFullName, LogLevel Level, LPCSTR msg) override
		{
			if (!bInitialized)
				return;

			// Parse outside lock
			auto [ClassName, MethodName] = GetClassNamePair(FunctionFullName);

			bool shouldLog = false;

			{
				// Only protect access to ClassList_ and DefaultLogLevel_
				std::lock_guard<CriticalSection> lg(cs_);

				auto it = std::find_if(
					ClassList_.begin(), ClassList_.end(),
					[&](const auto& Data) { return Data.GetName() == ClassName; }
				);

				if (it != ClassList_.end()) {
					auto classLevel = it->GetLogLevel();
					if ((int)Level <= (int)classLevel || (int)Level <= (int)DefaultLogLevel_) {
						shouldLog = true;
					}
				}
			}

			if (!shouldLog)
				return;

			// Build message outside lock
			std::string outputmsg;
			outputmsg.reserve(strlen(FunctionFullName) + strlen(msg) + 4);
			outputmsg.append(FunctionFullName).append(" - ").append(msg);

			{
				std::lock_guard<CriticalSection> lg(cs_);
				*pStream_ << outputmsg << std::endl;
			}
		}

		void Log(const char* FunctionFullName, LogLevel Level, const std::string& msg) override {
			Log(FunctionFullName, Level, msg.c_str());
		}

		void Log(const char* FunctionFullName, LogLevel Level, std::ostringstream& msg) override {
			Log(FunctionFullName, Level, msg.str().c_str());
		}

		void Log(LogLevel Level, LPCSTR msg) override
		{
			if (!bInitialized)
				return;

			bool shouldLog = false;

			{
				// Only check levels under lock
				std::lock_guard<CriticalSection> lg(cs_);
				if ((int)Level <= (int)currentLevel_ || (int)Level <= (int)DefaultLogLevel_) {
					shouldLog = true;
				}
			}

			if (!shouldLog)
				return;

			// Build message outside lock
			std::string outputmsg;
			outputmsg.reserve(strlen(msg) + 2);
			outputmsg.append(msg);

			{
				// Only the actual write is locked
				std::lock_guard<CriticalSection> lg(cs_);
				*pStream_ << outputmsg << std::endl;
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
			std::string_view func(FullFuncName);

			size_t pos = func.find("::");
			if (pos != std::string_view::npos) {
				return {
					std::string(func.substr(0, pos)),          // ClassName
					std::string(func.substr(pos + 2))          // MethodName
				};
			}

			// No class separator
			return { FullFuncName, "" };
		}


		bool IsClassRegistered(const std::string& ClassName)
		{
			if (std::find_if(ClassList_.begin(), ClassList_.end(), [&ClassName](const auto& Data) { return Data.GetName() == ClassName; }) != ClassList_.end())
			{
				return true;
			}
			else
			{
				return false;
			}
		}
	};


	/// <summary>
	///  This class, logs the "Entering" at construction and logs "Leaving" when going out of scope from destructor
	/// </summary>
	class LogScope 
	{
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
	}; //class LogScope



	/// <summary>
	/// Creates dump of hex of the binary data
	/// </summary>
	class HexDump 
	{
		const unsigned char* pData_;
		size_t Len_;

	public:
		HexDump(const unsigned char* pData, size_t Len)
			: pData_(pData), Len_(Len) {
		}

		std::string GetData() const 
		{
			std::string out;
			out.reserve(Len_ * 4); // rough estimate

			for (size_t i = 0; i < Len_; i += 16) 
			{
				// Offset
				char offset[16];
				sprintf_s(offset, "%08zx  ", i);
				out.append(offset);

				// Hex bytes
				for (size_t j = 0; j < 16; ++j) {
					if (i + j < Len_) {
						char bytebuf[4];
						sprintf_s(bytebuf, "%02X ", pData_[i + j]);
						out.append(bytebuf);
					}
					else {
						out.append("   ");
					}
				}

				out.append(" ");

				// ASCII
				for (size_t j = 0; j < 16; ++j) {
					if (i + j < Len_) {
						unsigned char c = pData_[i + j];
						out.push_back((c >= 32 && c <= 126) ? c : '.');
					}
					else {
						out.push_back(' ');
					}
				}

				out.push_back('\n');
			}

			return out;
		}
	}; //class HexDump 
};
