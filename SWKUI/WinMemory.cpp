#include "pch.h"
//#include "WinClipboard.h"
#include "WinMemory.h"
#include <assert.h>

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#else
#define DBG_NEW new
#endif


namespace swktool {	


	void WinMemory::CopyFrom(WinMemory& Mem)
	{
		const size_t FromSize = Mem.AllocateSize;

		// if target memory not large enough, realloc
		if (AllocateSize < FromSize) {
			ReAlloc(FromSize, Alloc_Flags);
		}
		
		// if nothing locked
		if (IsLocked() == false && Mem.IsLocked() == false)
		{
			WinMemLockGuard LoclSource(Mem);
			WinMemLockGuard LockTaret(*this);

			std::memcpy(LockTaret.get<void>(), LoclSource.get<void>(), FromSize);
		}
		else if (IsLocked() == true && Mem.IsLocked() == true)
		{
			std::memcpy(this->get<void>(), Mem.get<void>(), FromSize);
		}
		else if (IsLocked() == true && Mem.IsLocked() == false)
		{
			WinMemLockGuard LockSource(Mem);
			std::memcpy(this->get<void>(), LockSource.get<void>(), FromSize);
		}
		else if (IsLocked() == false && Mem.IsLocked() == true) {
			{
				WinMemLockGuard LockTarget(*this);
				std::memcpy(LockTarget.get<void>(), Mem.get<void>(), FromSize);
			}			
		}
	}


	void WinMemory::CopyFrom(void* pSource, size_t nSize) 
	{
		// if target memory not large enough, realloc
		if (AllocateSize < nSize) {
			ReAlloc(nSize, Alloc_Flags);
		}

		// if memory already locked, then direct copy
		if (IsLocked()) {
			std::memcpy(pLockedAddr, pSource, nSize);
		}
		else 
		{
			// memory not in locked state, lock, copy, then unlock
			WinMemLockGuard Lock(*this);
			std::memcpy(Lock.get<void>(), pSource, nSize);
		}
	}


	void WinMemory::CopyTo(WinMemory& Mem) {
		const size_t FromSize = AllocateSize;

		// if target memory not large enough, realloc
		if (AllocateSize < FromSize) {
			Mem.ReAlloc(FromSize, Alloc_Flags);
		}

		// if nothing locked
		if (IsLocked() == false && Mem.IsLocked() == false)
		{
			WinMemLockGuard LoclSource(*this);
			WinMemLockGuard LockTaret(Mem);

			std::memcpy(LockTaret.get<void>(), LoclSource.get<void>(), FromSize);
		}
		else if (IsLocked() == true && Mem.IsLocked() == true)
		{
			std::memcpy(Mem.get<void>(), this->get<void>(), FromSize);
		}
		else if (IsLocked() == true && Mem.IsLocked() == false)
		{
			WinMemLockGuard LockSource(*this);
			std::memcpy(Mem.get<void>(), LockSource.get<void>(), FromSize);
		}
		else if (IsLocked() == false && Mem.IsLocked() == true) {
			{
				WinMemLockGuard LockTarget(Mem);
				std::memcpy(LockTarget.get<void>(), this->get<void>(), FromSize);
			}
		}
	}

	void WinMemory::CopyTo(void* pTarget, size_t nSize) {
		if (nSize < AllocateSize) {
			assert(false);
		}

		// if memory already locked, then direct copy
		if (IsLocked()) {
			std::memcpy(pTarget, pLockedAddr, nSize);
		}
		else
		{
			WinMemLockGuard SourceLock(*this);
			std::memcpy(pTarget, SourceLock.get<void>(), nSize);
		}
	}
}