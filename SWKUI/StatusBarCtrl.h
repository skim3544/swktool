#ifndef  ___STATUS_BAR_CTRL___
#define  ___STATUS_BAR_CTRL___

#pragma once

#include <Windows.h>
#include <CommCtrl.h>
#include <string>
#include <memory>
#include "Ctrl.h"
#include "WinMemory.h"
#include "ProgressBarCtrl.h"

namespace swktool 
{

	/// <summary>
	/// StatusBar control
	/// </summary>
	class StatusBarCtrl : public Ctrl 
	{		
	protected:
		RECT   rectClient_;

		//Child control that can be attached any of the parts
		std::unordered_map<int, std::unique_ptr<ProgressBarCtrl>> progressBars_;

	public:
		StatusBarCtrl(IWindow* pParent);		
		StatusBarCtrl(std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, IWindow* pParent, UINT CtrlID);		
		~StatusBarCtrl() { ; }

		// Must chain to Parent/Owner window's OnSize message
		// 3 parameters are not used, but match the calling Windows parameters
		virtual void OnSize(UINT state = 0, int nWidth= 0, int nHeight = 0) 
		{			
			::SendMessage(hwndCtrl, WM_SIZE, nWidth, nHeight);
			RepositionChildControls();
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


		/// <summary>
		/// When status line is split,
		/// send text to split portion of the Status area.   Index is 1 based
		/// </summary>
		/// <param name="Index"></param>
		/// <param name="sText"></param>
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

		BOOL IsSimple() const 
		{
			const BOOL Result = (BOOL)::SendMessage(hwndCtrl, SB_SIMPLE, 0, (LPARAM)0);
			return Result;
		}

		void SetUnicodeFormat(BOOL bUnicode = TRUE) 
		{
			::SendMessage(hwndCtrl, SB_SIMPLE, bUnicode, (LPARAM)0);
		}


		// Diplay only if status is icon or text was trancated
		void SetStatusTipText(WPARAM Index, LPCTSTR sTipText) 
		{
			::SendMessage(hwndCtrl, SB_SETTIPTEXT, Index, (LPARAM)sTipText);
		}

		std::wstring GetStatusTipText(DWORD Index, DWORD nSizeBuffer = 255) 
		{
			std::wstring str;

			int alloc_size = nSizeBuffer + 1;
			const std::wstring::size_type PreAllocSize = static_cast<std::wstring::size_type>(alloc_size);
			str.reserve(PreAllocSize);
			WPARAM Param = (WPARAM)(MAKELONG(Index, nSizeBuffer));

			::SendMessage(hwndCtrl, SB_GETTIPTEXT, Param, (LPARAM)str.c_str());
			return str;
		}

		BOOL GetRect(WPARAM Index, LPRECT pRECT) 
		{
			return (BOOL)::SendMessage(hwndCtrl, SB_GETRECT, Index, (LPARAM)pRECT);
		}

		BOOL SetICON(WPARAM Index, HANDLE IconHandle) 
		{
			return (BOOL)::SendMessage(hwndCtrl, SB_SETICON, Index, (LPARAM)IconHandle);
		}

		HANDLE GetICON(WPARAM Index) 
		{
			return (HANDLE)::SendMessage(hwndCtrl, SB_GETICON, Index, (LPARAM)0);
		}

		// pass array with 3 integer storage
		BOOL GetBorders(int Borders[3]) 
		{
			return (BOOL)::SendMessage(hwndCtrl, SB_GETBORDERS, 0, (LPARAM)(&Borders[0]));
		}

		void RepositionChildControls()
		{
			for (auto& kv : progressBars_)
			{
				int partIndex = kv.first;
				ProgressBarCtrl* bar = kv.second.get();

				RECT rc{};
				if (GetRect(partIndex, &rc))
				{
					::MoveWindow(
						bar->GetHwnd(),
						rc.left + 2,
						rc.top + 2,
						(rc.right - rc.left) - 4,
						(rc.bottom - rc.top) - 4,
						TRUE
					);
				}
			}
		}

		ProgressBarCtrl* GetProgressBar(int PartIndex) {
			return progressBars_[PartIndex].get();
		}

		ProgressBarCtrl* AddProgressBarToPart(int partIndex, DWORD style = WS_CHILD | WS_VISIBLE)
		{
			RECT rc{};
			if (!GetRect(partIndex, &rc))
				throw std::runtime_error("Invalid status bar part index");

			// Create ProgressBarCtrl object
			auto bar = std::make_unique<ProgressBarCtrl>(
				hwndCtrl,
				hInst,
				L"",
				style,
				rc.left + 2,
				rc.top + 2,
				(rc.right - rc.left) - 4,
				(rc.bottom - rc.top) - 4,
				0               // ID not needed for embedded controls
			);

			HWND hChild = bar->GetHwnd();
			if (!hChild)
				throw std::runtime_error("Failed to create progress bar");

			// Store it			
			progressBars_[partIndex] = std::move(bar);

			return progressBars_[partIndex].get();
		}


	};

}

#endif

