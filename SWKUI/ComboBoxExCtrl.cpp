#include "pch.h"
#include "AWindow.h"
#include "ComboboxExCtrl.h"

namespace swktool {
	ComboboxExCtrl::ComboboxExCtrl(std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, Window* pParent, UINT CtrlID) {
		//hInst = pParent->GetInstanceHandle();
		//hDlg = pParent->GetDlgHWND();
		ID = CtrlID;

		INITCOMMONCONTROLSEX icex;

		icex.dwSize = sizeof(icex);
		icex.dwICC = ICC_USEREX_CLASSES;

		InitCommonControlsEx(&icex);

		hwndCtrl = ::CreateWindowEx(0,
			WC_COMBOBOXEX, (LPCWSTR)Caption.c_str(),
			Style, x, y, Width, Height,
			hDlg, (HMENU)(UINT_PTR)ID, hInst, NULL);
	}

	ComboboxExCtrl::ComboboxExCtrl(UINT CtrlID, Window* pParent) : Ctrl(CtrlID, pParent) {
	}

}