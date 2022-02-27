#pragma once
#include <Windows.h>
#include "WinMemory.h"

namespace swktool {
	class WinMemory;
	class WinClipboard {
	public:
		WinClipboard(HWND hwnd) {
			::OpenClipboard(hwnd);
		}

		~WinClipboard() {
			::CloseClipboard();
		}

		HGLOBAL   GetMemoryHandle(UINT CopyFormat = CF_TEXT) const {
			HGLOBAL hMemory = GetClipboardData(CopyFormat);
			return hMemory;
		}

		void SetData(HGLOBAL hMemory, UINT DataFormat) {
			::SetClipboardData(DataFormat, hMemory);
		}

		

		void SetDta(WinMemory& Mem, UINT DataFormat = CF_TEXT);


		HGLOBAL GetData(UINT DataFormat = CF_TEXT) const {
			HGLOBAL hMemory = GetClipboardData(DataFormat);
			return hMemory;
		}

		WinMemory GetDataMemory(UINT DataFormat = CF_TEXT);

		void Empty()
		{
			::EmptyClipboard();
		}
	};
}