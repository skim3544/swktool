#pragma once

#include "..\SWKBase\Logger.h"
#include <mutex>



//class AppLog : public swktool::Logger {
//	static AppLog* pLogger_;
//
//	static std::mutex m_Mutex;
//public:
//	
//	static AppLog& Inst() {
//		if (pLogger_ == nullptr) {
//			std::lock_guard<std::mutex> Guard(m_Mutex);
//			if (pLogger_ == nullptr) {
//				pLogger_ = new AppLog();
//				pLogger_->init(TEXT("AppLog.log"));
//			}
//		}
//		return *pLogger_;
//	}
//	
//	~AppLog() {
//		delete pLogger_;			
//	}
//
//protected:
//	virtual void init(LPCTSTR FileName) override {
//		if (bInitialized == false) {
//			pStream = std::make_unique<swktool::LoggerStream>(FileName);
//			bInitialized = true;
//		}
//	}
//
//};
//

