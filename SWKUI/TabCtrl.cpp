#include "pch.h"
#include "Ctrl.h"
#include "AWindow.h"
#include "TabCtrl.h"

namespace swktool {

	TabCtrl::TabCtrl(std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, Window* pParent, UINT CtrlID) {
		INITCOMMONCONTROLSEX icex;

		icex.dwSize = sizeof(icex);
		icex.dwICC = ICC_TAB_CLASSES;

		InitCommonControlsEx(&icex);

		hInst = pParent->GetInstance();
		hDlg = pParent->GetWindowHandle();

		ID = CtrlID;
		hwndCtrl = ::CreateWindowEx(0,
			WC_TABCONTROL, (LPCWSTR)Caption.c_str(),
			Style, x, y, Height, Width,
			hDlg, (HMENU)(UINT_PTR)ID, hInst, NULL);

	}
	TabCtrl::TabCtrl(UINT CtrlID, Window* pParent) : Ctrl(CtrlID, pParent) {

	}

}