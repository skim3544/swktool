#include "pch.h"
#include "AWindow.h"
#include "HeaderCtrl.h"

namespace swktool {
	HeaderCtrl::HeaderCtrl(std::wstring Caption, DWORD Style, int x, int y, int Width, int Height,  Window* pParent, UINT CtrlID)
	{
		INITCOMMONCONTROLSEX icex;

		icex.dwSize = sizeof(icex);
		icex.dwICC = ICC_LISTVIEW_CLASSES;

		InitCommonControlsEx(&icex);

		hInst = pParent->GetInstance();
		hDlg = pParent->GetWindowHandle();
		ID = CtrlID;
		hwndCtrl = ::CreateWindowEx(0,
			WC_HEADER, (LPCWSTR)Caption.c_str(),
			Style, x, y, Width, Height,
			hDlg, (HMENU)(UINT_PTR)ID, hInst, NULL);

	}


	HeaderCtrl::HeaderCtrl(UINT CtrlID, Window* pParent) : Ctrl(CtrlID, pParent) {

	}


}