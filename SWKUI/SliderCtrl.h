#pragma once

#include "Ctrl.h"

namespace swktool {
	class AWindow;

	/// <summary>
	///  Sometimes known as Slider control
	/// </summary>
	class TrackbarCtrl : public Ctrl {
	public:
		TrackbarCtrl(std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, AWindow* pParent, UINT CtrlID);
		TrackbarCtrl(UINT CtrlID, AWindow* pParent);
		~TrackbarCtrl() { ; }

		void SetRange(int Min, int Max) {
			SendMessage(hwndCtrl, TBM_SETRANGE,
				(WPARAM)TRUE,                   // redraw flag 
				(LPARAM)MAKELONG(Min, Max));  // min. & max. positions
		}

		void SetPageSize(int PageSize)
		{
			SendMessage(hwndCtrl, TBM_SETPAGESIZE,
				0, (LPARAM)PageSize);
		}

		void SetSel(int Min, int Max)
		{
			SendMessage(hwndCtrl, TBM_SETSEL,
				(WPARAM)FALSE,                  // redraw flag 
				(LPARAM)MAKELONG(Min, Max));
		}

		void SetPosition(int nPosition)
		{
			SendMessage(hwndCtrl, TBM_SETPOS,
				(WPARAM)TRUE,                   // redraw flag 
				(LPARAM)nPosition);

		}

		int GetPosition()
		{
			return (int)SendMessage(hwndCtrl, TBM_GETPOS, 0, 0);
		}

	};

	class SliderCtrl : public TrackbarCtrl {
	public:
		SliderCtrl(std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, swktool::AWindow* pParent, UINT CtrlID) :
			TrackbarCtrl(Caption, Style, x, y, Height, Width, pParent, CtrlID) {
		}

		SliderCtrl(UINT CtrlID, swktool::AWindow* pParent) : TrackbarCtrl(CtrlID, pParent) {
		}

		~SliderCtrl() { ; }

	};
}