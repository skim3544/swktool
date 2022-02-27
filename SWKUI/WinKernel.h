#pragma once
#include <Windows.h>
#include <atomic>

namespace swktool {
	class CriticalSection {
		CRITICAL_SECTION	cs_;

	public:
		CriticalSection() {
			::InitializeCriticalSection(&cs_);
		}

		~CriticalSection()
		{
			// unlock critical section if necessary
			//unlock();

			::DeleteCriticalSection(&cs_);
		}


		bool trylock()
		{
			return ::TryEnterCriticalSection(&cs_);
		}

		// lock and unlock calls are required by lock_guard<>
		void lock() {
			::EnterCriticalSection(&cs_);
		}

		void unlock() {
			::LeaveCriticalSection(&cs_);
		}

		LPCRITICAL_SECTION operator()(void) {
			return &cs_;
		}

		// object cannot copy construct or assigned
		CriticalSection(const CriticalSection& Other) = delete;
		CriticalSection& operator = (const CriticalSection& Other) = delete;
	};

	class CS_Guard {
		CriticalSection& cs_;

	public:
		CS_Guard(CriticalSection& cs) : cs_(cs){
			cs_.lock();
		}

		~CS_Guard() {
			cs_.unlock();
		}
	};

	
	class ConditionalCriticalSection final {
		CRITICAL_SECTION	cs_;
		bool				bLock_;

	public:
		ConditionalCriticalSection(bool bLock) : bLock_(bLock) {
			::InitializeCriticalSection(&cs_);
		}

		~ConditionalCriticalSection()
		{
			// unlock critical section if necessary
			unlock();
			::DeleteCriticalSection(&cs_);
		}


		bool trylock()
		{
			return ::TryEnterCriticalSection(&cs_);
		}

		// lock and unlock calls are required by lock_guard<>
		void lock() {
			if (bLock_)
				::EnterCriticalSection(&cs_);
		}

		void unlock() {
			if (bLock_)
				::LeaveCriticalSection(&cs_);
		}

		LPCRITICAL_SECTION operator()(void) {
			return &cs_;
		}

		// object cannot copy construct or assigned
		ConditionalCriticalSection(const ConditionalCriticalSection& Other) = delete;
		ConditionalCriticalSection& operator = (const ConditionalCriticalSection& Other) = delete;
	};




	class WinMutex final {
		HANDLE				hMutex;
		DWORD				WaitResult;
		std::atomic<DWORD>  LastError = 0;

	public:
		WinMutex(LPSECURITY_ATTRIBUTES lpMutexAttributes = NULL,
			BOOL bInitialOwner = CREATE_MUTEX_INITIAL_OWNER, LPCWSTR
			lpName = NULL) : WaitResult(0), hMutex(0)
		{
			hMutex = ::CreateMutex(lpMutexAttributes, bInitialOwner, lpName);
			if (hMutex == NULL)
			{
				LastError = ::GetLastError();
			}
		}

		~WinMutex()
		{
			unlock();

			::CloseHandle(hMutex);
			hMutex = NULL;
		}

		DWORD GetErrorCode() const {
			return LastError;
		}


		// lock and unlock calls are required by lock_guard<>
		void lock(DWORD dwMilliSeconds = INFINITE) {
			if (hMutex != NULL) {
				WaitResult = ::WaitForSingleObject(hMutex, dwMilliSeconds);
			}
		}

		void unlock() {
			if (hMutex != NULL) {
				if (WaitResult == WAIT_OBJECT_0) {
					::ReleaseMutex(hMutex);
				}
			}
		}

		// object cannot copy construct or assigned
		//WinMutex(const CriticalSection& Other) = delete;
		//WinMutex& operator = (const WinMutex& Other) = delete;

		// block the heap allocation of the object
		void* operator new(size_t size) = delete;
		void operator delete(void*) = delete;
	};
}