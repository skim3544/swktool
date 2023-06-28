#ifndef  ___STATUS_BAR_CTRL___
#define  ___STATUS_BAR_CTRL___

#pragma once

#include <Windows.h>
#include <CommCtrl.h>
#include <string>
#include "Ctrl.h"
#include "WinMemory.h"


namespace swktool {

	/// <summary>
	/// StatusBar control
	/// </summary>
	class StatusBarCtrl : public Ctrl {		
	protected:
		RECT   rectClient_;

	public:
		StatusBarCtrl(Window* pParent);
		StatusBarCtrl(DialogWindow* pParent);
		StatusBarCtrl(std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, Window* pParent, UINT CtrlID);		
		~StatusBarCtrl() { ; }

		// Must chain to Parent/Owner window's OnSize message
		// 3 parameters are not used, but match the calling Windows parameters
		virtual void OnSize(UINT state = 0, int nWidth= 0, int nHeight = 0) {
			::SendMessage(hwndCtrl, WM_SIZE, nWidth, nHeight);
			::UpdateWindow(hwndCtrl);
		}

		// parts array is array integer of each section of status bar where each number
		// represent the right edge location
		// 
		// example:
		// int iRightEdge[] = {300, 500, -1};
		// SetParts(iRightEdge, _countof(iRightEdge);
		void SetParts(int* pParts, int count) {
			::SendMessage(hwndCtrl, SB_SETPARTS, count, (LPARAM)pParts);
		}


		void SetStatusText(WPARAM Index, LPCTSTR sText) {
			::SendMessage(hwndCtrl, SB_SETTEXT, Index, (LPARAM)sText);
		}

		std::wstring GetStatusText(int index)
		{
			int StatusStrLen = (int)::SendMessage(hwndCtrl, SB_GETTEXTLENGTH, index, (LPARAM)0);
			int alloc_size = StatusStrLen + 1;

			std::wstring Result;
			const std::wstring::size_type PreAllocSize = static_cast<std::wstring::size_type>(alloc_size);
			Result.reserve(PreAllocSize);
			::SendMessage(hwndCtrl, SB_GETTEXT, index, (LPARAM)Result.c_str());

			return Result;
		}

		void SetSimple(BOOL bSimple= TRUE) {
			::SendMessage(hwndCtrl, SB_SIMPLE, bSimple, (LPARAM)0);
		}

		BOOL IsSimple() {
			const BOOL Result = (BOOL)::SendMessage(hwndCtrl, SB_SIMPLE, 0, (LPARAM)0);
			return Result;
		}

		void SetUnicodeFormat(BOOL bUnicode = TRUE) {
			::SendMessage(hwndCtrl, SB_SIMPLE, bUnicode, (LPARAM)0);
		}


		// Diplay only if status is icon or text was trancated
		void SetStatusTipText(WPARAM Index, LPCTSTR sTipText) {
			::SendMessage(hwndCtrl, SB_SETTIPTEXT, Index, (LPARAM)sTipText);
		}

		std::wstring GetStatusTipText(DWORD Index, DWORD nSizeBuffer = 255) {
			std::wstring str;

			int alloc_size = nSizeBuffer + 1;
			const std::wstring::size_type PreAllocSize = static_cast<std::wstring::size_type>(alloc_size);
			str.reserve(PreAllocSize);
			WPARAM Param = (WPARAM)(MAKELONG(Index, nSizeBuffer));

			::SendMessage(hwndCtrl, SB_GETTIPTEXT, Param, (LPARAM)str.c_str());
			return str;
		}

		BOOL GetRect(WPARAM Index, LPRECT pRECT) {
			return (BOOL)::SendMessage(hwndCtrl, SB_GETRECT, Index, (LPARAM)pRECT);
		}

		BOOL SetICON(WPARAM Index, HANDLE IconHandle) {
			return (BOOL)::SendMessage(hwndCtrl, SB_SETICON, Index, (LPARAM)IconHandle);
		}

		HANDLE GetICON(WPARAM Index) {
			return (HANDLE)::SendMessage(hwndCtrl, SB_GETICON, Index, (LPARAM)0);
		}

		// pass array with 3 integer storage
		BOOL GetBorders(int Borders[3]) {
			return (BOOL)::SendMessage(hwndCtrl, SB_GETBORDERS, 0, (LPARAM)(&Borders[0]));
		}
	};

}

#endif

