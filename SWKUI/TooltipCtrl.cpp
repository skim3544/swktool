#include "pch.h"
#include "ctrl.h"
#include "AWindow.h"
#include "TooltipCtrl.h"

namespace swktool {

	TooltipCtrl::TooltipCtrl(std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, AWindow* pParent, UINT CtrlID) {
		INITCOMMONCONTROLSEX icex;

		icex.dwSize = sizeof(icex);
		icex.dwICC = ICC_BAR_CLASSES;

		InitCommonControlsEx(&icex);

		hInst = pParent->GetInstanceHandle();
		hDlg = pParent->GetDlgHWND();
		ID = CtrlID;
		hwndCtrl = ::CreateWindowEx(0,
			TOOLTIPS_CLASS, (LPCWSTR)Caption.c_str(),
			Style, x, y, Height, Width,
			hDlg, (HMENU)(UINT_PTR)ID, hInst, NULL);

	}
	TooltipCtrl::TooltipCtrl(UINT CtrlID, AWindow* pParent) : Ctrl(CtrlID, pParent) {

	}
}

