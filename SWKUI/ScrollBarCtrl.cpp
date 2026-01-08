#include "pch.h"
#include "ScrollBarCtrl.h"

namespace swktool {

	ScrollBarCtrl::ScrollBarCtrl(DWORD Style, Window* pParent) 
	{
		hInst = pParent->GetInstance();
		hParent = pParent->GetHwnd();

		Style = Style | WS_OVERLAPPEDWINDOW | WS_HSCROLL | WS_VSCROLL;

		hwndCtrl = ::CreateWindowEx(0,
//				TEXT("SCROLLBAR"), 
			(LPCWSTR)pParent->ClassName(),
			L"Text",
				Style, 
			CW_USEDEFAULT,         // default horizontal position 
			CW_USEDEFAULT,         // default vertical position 
			CW_USEDEFAULT,         // default width 
			CW_USEDEFAULT,         // default height 
				NULL, 
				(HMENU)(UINT_PTR)NULL, 
			hInst, this);
		//const int sbHeight = SM_CYVSCROLL;

		//RECT rect;
		//GetClientRect(hDlg, &rect);

		//hwndCtrl = ::CreateWindowEx(0,
		//	TEXT("SCROLLBAR"), (LPCWSTR)NULL,
		//	Style, 
		//	rect.left,              // horizontal position 
		//	rect.bottom - sbHeight, // vertical position 
		//	rect.right,             // width of the scroll bar 
		//	sbHeight,
		//	hDlg, 
		//	(HMENU)(UINT_PTR)NULL, hInst, NULL);

		UpdateWindow(hwndCtrl);
	}

	ScrollBarCtrl::ScrollBarCtrl(UINT CtrlID, Window* pParent) : 
		Ctrl(CtrlID, pParent) 
	{

	}

	//ScrollBarCtrl::ScrollBarCtrl(UINT CtrlID, DialogWindow* pParent) : 
	//	Ctrl(CtrlID, pParent) 
	//{

	//}

	HScrollBarCtrl::HScrollBarCtrl(Window* pParent, int sbHeight) :
		Ctrl( pParent),
		BarHeight_(sbHeight)
	{
		RECT rect;

		// Get the dimensions of the parent window's client area;
		if (GetClientRect(hParent, &rect))
		{
			hwndCtrl = CreateWindowEx(
				0,                      // no extended styles 
				L"SCROLLBAR",           // scroll bar control class 
				(PTSTR)NULL,           // no window text 
				WS_CHILD | WS_VISIBLE   // window styles  
				| SBS_HORZ,         // horizontal scroll bar style 
				rect.left,              // horizontal position 
				rect.bottom - sbHeight, // vertical position 
				rect.right,             // width of the scroll bar 
				BarHeight_,               // height of the scroll bar
				hParent,             // handle to main window 
				(HMENU)NULL,           // no menu 
				hInst,                // instance owning this window 
				(PVOID)NULL            // pointer not needed 
			);
		}			
	}

	void HScrollBarCtrl::OnSize(UINT state, int nWidth, int nHeight) 
	{
		RECT rect;
		if (GetClientRect(hParent, &rect)) {
			MoveWindow(rect.left, rect.bottom - BarHeight_, rect.right, BarHeight_);
		}


		UpdateWindow(hwndCtrl);
	}

	VScrollBarCtrl::VScrollBarCtrl(Window* pParent, int sbWidth) :
		Ctrl(pParent), BarWidth_(sbWidth)
	{
		RECT rect;

		// Get the dimensions of the parent window's client area;
		if (GetClientRect(hParent, &rect))
		{
			hwndCtrl = CreateWindowEx(
				0,                      // no extended styles 
				L"SCROLLBAR",           // scroll bar control class 
				(PTSTR)NULL,           // no window text 
				WS_CHILD | WS_VISIBLE   // window styles  
				| SBS_HORZ,         // horizontal scroll bar style 
				rect.right - BarWidth_,              // horizontal position 
				rect.top, // vertical position 
				BarWidth_,             // width of the scroll bar 
				rect.bottom - rect.top,           // height of the scroll bar
				hParent,             // handle to main window 
				(HMENU)NULL,           // no menu 
				hInst,                // instance owning this window 
				(PVOID)NULL            // pointer not needed 
			);
		}

	}

	void VScrollBarCtrl::OnSize(UINT state, int nWidth, int nHeight) {
		RECT rect;
		if (GetClientRect(hParent, &rect)) {
			MoveWindow(
				rect.right - BarWidth_, 
				rect.top, BarWidth_, 
				rect.bottom - rect.top);
		}


		UpdateWindow(hwndCtrl);

	}
}