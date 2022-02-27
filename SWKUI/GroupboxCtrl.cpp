#include "pch.h"
#include "Ctrl.h"
#include "AWindow.h"
#include "GroupboxCtrl.h"

namespace swktool {
	GroupboxCtrl::GroupboxCtrl(std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, AWindow* pParent, UINT CtrlID) {
		hInst = pParent->GetInstanceHandle();
		hDlg = pParent->GetDlgHWND();
		ID = CtrlID;
		hwndCtrl = ::CreateWindow(TEXT("BUTTON"), (LPCWSTR)Caption.c_str(),
			Style, x, y, Height, Width,
			hDlg, (HMENU)(UINT_PTR)ID, hInst, NULL);
	}

	GroupboxCtrl::GroupboxCtrl(UINT CtrlID, AWindow* pParent) : Ctrl(CtrlID, pParent) {
	}

}