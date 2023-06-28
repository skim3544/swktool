#include "pch.h"
#include "HeaderCtrl.h"

namespace swktool {
	HeaderCtrl::HeaderCtrl(std::wstring Caption, DWORD Style, int x, int y, int Width, int Height,  Window* pParent, UINT CtrlID)
	{
		INITCOMMONCONTROLSEX icex{};

		icex.dwSize = sizeof(icex);
		icex.dwICC = ICC_LISTVIEW_CLASSES;

		InitCommonControlsEx(&icex);

		hInst = pParent->GetInstance();
		hDlg = pParent->GetWindowHandle();
		ID = CtrlID;
		hwndCtrl = ::CreateWindowEx(0,
			WC_HEADER, (LPCWSTR)Caption.c_str(),
			//Style, 
			WS_CHILD | WS_BORDER | HDS_BUTTONS | HDS_HORZ,
			x, y, Width, Height,
			hDlg, (HMENU)(UINT_PTR)ID, hInst, NULL);
	}

	HeaderCtrl::HeaderCtrl(Window* pParent) : 
		Ctrl(pParent) 
	{
		INITCOMMONCONTROLSEX icex{};

		icex.dwSize = sizeof(icex);
		icex.dwICC = ICC_LISTVIEW_CLASSES;
		InitCommonControlsEx(&icex);

		hInst = pParent->GetInstance();
		hDlg = pParent->GetWindowHandle();
		//ID = CtrlID;
		hwndCtrl = ::CreateWindowEx(0,
			WC_HEADER, NULL,
			WS_CHILD | WS_BORDER | HDS_BUTTONS | HDS_HORZ,
			0,0,0,0,
			(HWND)hDlg, (HMENU)(UINT_PTR)ID, hInst, NULL);

		WINDOWPOS wp{};
		RECT rcParent{};		
		if (GetLayout(wp, rcParent)) {
			// Set the size, position, and visibility of the header control. 
			SetWindowPos(hwndCtrl, 
				wp.hwndInsertAfter, 
				wp.x, wp.y,
				wp.cx, wp.cy, 
				wp.flags | SWP_SHOWWINDOW);
		}
	}


	HeaderCtrl::HeaderCtrl(DialogWindow* pParent) :
		Ctrl(pParent)
	{
		INITCOMMONCONTROLSEX icex{};

		icex.dwSize = sizeof(icex);
		icex.dwICC = ICC_LISTVIEW_CLASSES;

		InitCommonControlsEx(&icex);

		hInst = pParent->GetInstance();
		hDlg = pParent->GetWindowHandle();
		//ID = CtrlID;
		hwndCtrl = ::CreateWindowEx(0,
			WC_HEADER, NULL,
			//Style, 
			//
			//WS_CHILD | WS_BORDER | HDS_BUTTONS | HDS_HORZ,
			WS_CHILD | WS_VISIBLE,
			0, 0, 0, 0,
			(HWND)hDlg, (HMENU)(UINT_PTR)ID, hInst, NULL);

		RECT rcParent{};
		HDLAYOUT hdl{};
		// Retrieve the bounding rectangle of the parent window's 
	// client area, and then request size and position values 
	// from the header control. 
		

		//WINDOWPOS wp{};

		//GetClientRect(hDlg, &rcParent);
		//hdl.prc   = &rcParent;
		//hdl.pwpos = &wp;
		//if (!SendMessage(hwndCtrl, HDM_LAYOUT, 0, (LPARAM)&hdl))
		//	return;
		GetClientRect(hDlg, &rcParent);

		int headerWidth = rcParent.right - rcParent.left;
		int headerHeight = (rcParent.bottom - rcParent.top) / 10;


		// Set the size, position, and visibility of the header control. 
		//SetWindowPos(hwndCtrl, wp.hwndInsertAfter, wp.x, wp.y,
		//	wp.cx, wp.cy, wp.flags | SWP_SHOWWINDOW);
		SetWindowPos(hwndCtrl, NULL, 0, 0, headerWidth, headerHeight,
			SWP_NOMOVE | SWP_NOZORDER);

	}


	HeaderCtrl::HeaderCtrl(UINT CtrlID, Window* pParent) : Ctrl(CtrlID, pParent) {

	}

	HeaderCtrl::HeaderCtrl(UINT CtrlID, DialogWindow* pParent) : Ctrl(CtrlID, pParent) {

	}

	void HeaderCtrl::OnSize(UINT state, int nWidth, int nHeight) 
	{
		WINDOWPOS wp{};
		RECT rcParent{};
		if (GetLayout(wp, rcParent)) {
			int NewWidth = 20;
			// Set the size, position, and visibility of the header control. 
			SetWindowPos(hwndCtrl, wp.hwndInsertAfter,
				wp.x, wp.y,
				wp.cx, NewWidth,
				//nWidth, nHeight/ 10
				wp.flags | SWP_SHOWWINDOW);

			::UpdateWindow(hwndCtrl);
		}
	}

	int HeaderCtrl::DoInsertItem(HWND hwndHeader, int iInsertAfter,
		int nWidth, LPTSTR lpsz)
	{
		HDITEM hdi;
		int index;

		hdi.mask = HDI_TEXT | HDI_FORMAT | HDI_WIDTH;
		hdi.cxy = nWidth;
		hdi.pszText = lpsz;
		hdi.cchTextMax = sizeof(hdi.pszText) / sizeof(hdi.pszText[0]);
		hdi.fmt = HDF_LEFT | HDF_STRING;

		index = SendMessage(hwndHeader, HDM_INSERTITEM,
			(WPARAM)iInsertAfter, (LPARAM)&hdi);

		return index;
	}


}