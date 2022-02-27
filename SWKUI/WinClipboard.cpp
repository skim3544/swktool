#include "pch.h"
#include "WinMemory.h"
#include "WinClipboard.h"

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#else
#define DBG_NEW new
#endif


namespace swktool {


	void WinClipboard::SetDta(WinMemory& Mem, UINT DataFormat)
	{
		HGLOBAL hMemory = Mem.HandOverHandle();
		::SetClipboardData(DataFormat, hMemory);
	}

	WinMemory WinClipboard::GetDataMemory(UINT DataFormat) {
		HGLOBAL hMemory = GetClipboardData(DataFormat);
		WinMemory Mem(hMemory);
		return Mem;
	}


}
