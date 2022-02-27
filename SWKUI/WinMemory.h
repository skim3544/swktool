#pragma once
#include <windows.h>
#include <memory>
#include <assert.h>
#include "WinClipboard.h"

namespace swktool {

	/// <summary>
	/// Windows Memory scope guard
	/// Locks the memory on create, unlocks when goes out of scope
	/// </summary>
	class WinMemory;
	class WinMemLockGuard {
	public:
		WinMemLockGuard(WinMemory& WinMemory);
		~WinMemLockGuard();

		template <typename T>
		T* get() const noexcept;

	private:
		WinMemory& WinMemory_;
	};




	/// <summary>
	/// WinMemoy
	/// Windows Handle based Memory object
	/// Typically used for transfering data through Clipboard
	/// but has other uses
	/// </summary>
	class WinMemory {
	protected:
		HGLOBAL		hMemory = nullptr;
		void* pLockedAddr = nullptr;
		size_t		AllocateSize = 0;
		UINT		Alloc_Flags = 0;
		bool		bOwner = true;

	public:
		// GHND  = (GMEM_MOVEABLE | GMEM_ZEROINIT)
		// GPTR  = (GMEM_FIXED | GMEM_ZEROINIT)
		// For Clipboard data pass (GMEM_MOVEABLE | GMEM_SHARE)
		WinMemory(size_t nSize, DWORD flags = GHND) :
			hMemory(nullptr),
			pLockedAddr(nullptr),
			AllocateSize(nSize),
			Alloc_Flags(flags),
			bOwner(true) 
		{			
			// allocate memory
			Alloc(nSize, flags);
		}

		WinMemory(HGLOBAL hHandle) :
			hMemory(hHandle),
			pLockedAddr(nullptr) {
			
			assert(hMemory != NULL);
			AllocateSize = Size();
			Alloc_Flags = GetAllocFlags();
			;
		}

		WinMemory(WinMemory&& Other) noexcept {
			hMemory = Other.hMemory;
			pLockedAddr = Other.pLockedAddr;
			AllocateSize = Other.AllocateSize;
			Alloc_Flags = Other.Alloc_Flags;
			bOwner = true;
			Other.hMemory = nullptr;
			Other.pLockedAddr = nullptr;
			Other.AllocateSize = 0;
			Other.Alloc_Flags = 0;
		}

		void Alloc(size_t nSize, UINT flags = GHND) {
			hMemory = ::GlobalAlloc(flags, nSize);
		}

		void ReAlloc(size_t nSize, UINT flags = GHND) {
			bool bInitiallyLocked = IsLocked();
			if (bInitiallyLocked) {
				unlock();
			}
			hMemory = ::GlobalReAlloc(hMemory, nSize, flags);
			Alloc_Flags = flags;
			AllocateSize = nSize;
			if (bInitiallyLocked) {
				lock<void*>();
			}
		}

		template <typename T>
		T* lock() {
			T* pPtr = nullptr;
			if (IsLocked() == false) {
				pLockedAddr = ::GlobalLock(hMemory);
				pPtr = static_cast<T*>(pLockedAddr);
			}
			else {
				pPtr = static_cast<T*>(pLockedAddr);
			}
			return pPtr;
		}


		bool IsLocked() const {
			return (pLockedAddr != nullptr);
		}

		template <typename T>
		T* get() const noexcept {
			return static_cast<T*>(pLockedAddr);
		}

		void unlock() {
			if (IsLocked() == true) {
				::GlobalUnlock(hMemory);
				pLockedAddr = nullptr;
			}
		}

		SIZE_T  Size() {
			return ::GlobalSize(hMemory);
		}

		UINT   GetAllocFlags() const {
			return ::GlobalFlags(hMemory);
		}

		void CopyFrom(WinMemory& Mem);
		void CopyTo(WinMemory& Mem);
		void CopyFrom(void* p, size_t nSize);
		void CopyTo(void* p, size_t nSize);
		
		// used for passing over the memory handle to Clipboard	
		HGLOBAL  HandOverHandle() {
			// bad memory handle
			assert(hMemory != nullptr);

			// memory locked, cannot hand over
			assert(pLockedAddr != nullptr);

			HGLOBAL hHandle = hMemory;
			hMemory = nullptr;

			return hHandle;
		}

		virtual ~WinMemory() {
			try {
				if (IsLocked())
				{
					unlock();
				}

				if (hMemory && bOwner) {
					::GlobalFree(hMemory);
					hMemory = nullptr;
				}
			}
			catch (...) {}
		}
	};



	template <class TClipboard>
	class ClipboardMemory : public WinMemory {

	protected:
		using WinMemory::hMemory;
		using WinMemory::bOwner;
		using WinMemory::AllocateSize;
		using WinMemory::Alloc_Flags;

	public:
		ClipboardMemory(const TClipboard& oClipboard, UINT CopyFormat) :
			WinMemory() {
			// Get the memory handle from Clipboard
			hMemory = TClipboard.GetData(CopyFormat);

			// not the owner of this memory
			bOwner = false;

			AllocateSize = WinMemory::Size();
			Alloc_Flags = WinMemory::AllocFlags();
		}
		~ClipboardMemory() { ; }
	};



	inline WinMemLockGuard::WinMemLockGuard(WinMemory& WinMemory) : WinMemory_(WinMemory)
	{
		WinMemory_.lock<void>();
	}

	inline WinMemLockGuard::~WinMemLockGuard() {
		WinMemory_.unlock();
	}

	template <typename T>
	inline T* WinMemLockGuard::get() const noexcept {
		return WinMemory_.get<T>();
	}

}

