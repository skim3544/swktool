#include "pch.h"
#include "HeaderCtrl.h"

namespace swktool 
{
	HeaderCtrl::HeaderCtrl(std::wstring Caption, DWORD Style, int x, int y, int Width, int Height,  IWindow* pParent, UINT CtrlID) :
		Ctrl(pParent)
	{
		INITCOMMONCONTROLSEX icex{};

		icex.dwSize = sizeof(icex);
		icex.dwICC = ICC_WIN95_CLASSES;

		InitCommonControlsEx(&icex);

		ID = CtrlID;
		hwndCtrl = ::CreateWindowEx(0,
			WC_HEADER, (LPCWSTR)Caption.c_str(),
			//Style, 
			WS_CHILD | WS_BORDER | HDS_BUTTONS | HDS_HORZ,
			x, y, Width, Height,
			hParent, (HMENU)(UINT_PTR)ID, hInst, NULL);
	}

	HeaderCtrl::HeaderCtrl(IWindow* pParent) : 
		Ctrl(pParent) 
	{
		INITCOMMONCONTROLSEX icex{};

		icex.dwSize = sizeof(icex);
		icex.dwICC = ICC_WIN95_CLASSES;
		InitCommonControlsEx(&icex);

		//ID = CtrlID;
		hwndCtrl = ::CreateWindowEx(0,
			WC_HEADER, nullptr,
			WS_CHILD | WS_VISIBLE | HDS_BUTTONS | HDS_HORZ,
			0,0,0,0,
			(HWND)hParent, (HMENU)(UINT_PTR)ID, hInst, nullptr);

		WINDOWPOS wp{};
		wp.cx = 300;
		wp.cy = 25;
		RECT rcParent{};		
		if (!GetLayout(wp, rcParent)) 
		{
			wp.x = 0;
			wp.y = 0;
			wp.cx = rcParent.right - rcParent.left;
			wp.cy = 20;
			wp.flags = SWP_SHOWWINDOW;
		}

		// Set the size, position, and visibility of the header control. 
		SetWindowPos(hwndCtrl,
			wp.hwndInsertAfter,
			wp.x, wp.y,
			wp.cx, wp.cy,
			wp.flags);
	}



	HeaderCtrl::HeaderCtrl(UINT CtrlID, IWindow* pParent) : Ctrl(CtrlID, pParent) 
	{

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

	LRESULT HeaderCtrl::DoInsertItem(HWND hwndHeader, int iInsertAfter,
		int nWidth, LPCTSTR   lpsz)
	{
		HDITEM hdi{};
		//int index;

		hdi.mask = HDI_TEXT | HDI_FORMAT | HDI_WIDTH;
		hdi.cxy = nWidth;
		hdi.pszText = const_cast<LPTSTR>(lpsz);
		//hdi.cchTextMax = sizeof(hdi.pszText) / sizeof(hdi.pszText[0]);
		hdi.cchTextMax = lstrlen(lpsz);
		hdi.fmt = HDF_LEFT | HDF_STRING;

		auto index = SendMessageW(hwndHeader, HDM_INSERTITEMW,
			(WPARAM)iInsertAfter, (LPARAM)&hdi);

		return index;
	}
}