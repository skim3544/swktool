#pragma once

#include "..\SWKBase\Logger.h"
#include <mutex>

template<typename T> class TSingleton
{
	static T* pData;
	
	static T& Inst() {
		if (pData == nullptr) {
			pData = new T();
		}

		return *pData;
	}
};




class AppLog : public swktool::Logger {
	static AppLog* pLogger_;

	static std::mutex m_Mutex;
public:
	
	static AppLog& Inst() {
		if (pLogger_ == nullptr) {
			std::lock_guard<std::mutex> Guard(m_Mutex);
			if (pLogger_ == nullptr) {
				pLogger_ = new AppLog();
				pLogger_->init();
			}
		}
		return *pLogger_;
	}
	
	~AppLog() {
		delete pLogger_;			
	}

protected:
	virtual void init() override {
		if (bInitialized == false) {
			pStream = std::make_unique<swktool::LoggerStream>(TEXT("Test.log"));
			bInitialized = true;
		}
	}

};


