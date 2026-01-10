#pragma once

#include "Ctrl.h"

namespace swktool {	

	class IWindow;

	/// <summary>
	///  Sometimes known as Slider control
	/// </summary>
	class TrackbarCtrl : public Ctrl {
	public:
		TrackbarCtrl(IWindow* pParent) : Ctrl(pParent) {}
		TrackbarCtrl(std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, IWindow* pParent, UINT CtrlID);
		TrackbarCtrl(UINT CtrlID, IWindow* pParent);		
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
		SliderCtrl(swktool::IWindow* pParent) : TrackbarCtrl(pParent) {};
		SliderCtrl(std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, swktool::IWindow* pParent, UINT CtrlID) :
			TrackbarCtrl(Caption, Style, x, y, Height, Width, pParent, CtrlID) {
		}

		SliderCtrl(UINT CtrlID, swktool::IWindow* pParent) : TrackbarCtrl(CtrlID, pParent) {
		}

		~SliderCtrl() { ; }

	};
}