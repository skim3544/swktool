#include "pch.h"
#include "AWindow.h"
#include "ListViewCtrl.h"

namespace swktool {
	ListViewCtrl::ListViewCtrl(std::wstring Caption, DWORD Style, int x, int y, int Width, int Height, AWindow* pParent, UINT CtrlID)
	{
		INITCOMMONCONTROLSEX icex;

		icex.dwSize = sizeof(icex);
		icex.dwICC = ICC_LISTVIEW_CLASSES;

		InitCommonControlsEx(&icex);

		hInst = pParent->GetInstanceHandle();
		hDlg = pParent->GetDlgHWND();
		ID = CtrlID;
		hwndCtrl = ::CreateWindowEx(0,
			WC_LISTVIEW, (LPCWSTR)Caption.c_str(),
			Style, x, y, Width, Height,
			hDlg, (HMENU)(UINT_PTR)ID, hInst, NULL);

	}


	ListViewCtrl::ListViewCtrl(UINT CtrlID, AWindow* pParent) : Ctrl(CtrlID, pParent) {

	}


}