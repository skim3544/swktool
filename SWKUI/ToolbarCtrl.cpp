
#include "pch.h"
#include "ctrl.h"
#include "ToolbarCtrl.h"

namespace swktool {

	ToolbarCtrl::ToolbarCtrl(std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, Window* pParent, UINT CtrlID) {
		INITCOMMONCONTROLSEX icex{};

		icex.dwSize = sizeof(icex);
		icex.dwICC = ICC_BAR_CLASSES;

		InitCommonControlsEx(&icex);

		hInst = pParent->GetInstance();
		hDlg = pParent->GetWindowHandle();

		ID = CtrlID;
		hwndCtrl = ::CreateWindowEx(0,
			TOOLBARCLASSNAME, (LPCWSTR)Caption.c_str(),
			Style, x, y, Height, Width,
			hDlg, (HMENU)(UINT_PTR)ID, hInst, NULL);

	}
	ToolbarCtrl::ToolbarCtrl(UINT CtrlID, Window* pParent) : Ctrl(CtrlID, pParent) {

	}

	ToolbarCtrl::ToolbarCtrl(UINT CtrlID, DialogWindow* pParent) : Ctrl(CtrlID, pParent) {

	}
}


