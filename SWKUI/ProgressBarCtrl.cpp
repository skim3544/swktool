#include "pch.h"
#include "ProgressBarCtrl.h"

namespace swktool {
	ProgressBarCtrl::ProgressBarCtrl(std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, Window* pParent, UINT CtrlID) {
		hInst = pParent->GetInstance();
		hDlg = pParent->GetWindowHandle();
		ID = CtrlID;
		hwndCtrl = ::CreateWindow(PROGRESS_CLASS, (LPCWSTR)Caption.c_str(),
			Style, x, y, Height, Width,
			hDlg, (HMENU)(UINT_PTR)ID, hInst, NULL);
	}

	ProgressBarCtrl::ProgressBarCtrl(UINT CtrlID, Window* pParent) : Ctrl(CtrlID, pParent) {
	}

	ProgressBarCtrl::ProgressBarCtrl(UINT CtrlID, DialogWindow* pParent) : Ctrl(CtrlID, pParent) {
	}
}