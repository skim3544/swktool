#include "pch.h"
#include "SliderCtrl.h"

namespace swktool {
	TrackbarCtrl::TrackbarCtrl(std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, swktool::Window* pParent, UINT CtrlID) {
		hInst = pParent->GetInstance();
		hDlg = pParent->GetWindowHandle();
		ID = CtrlID;
		hwndCtrl = ::CreateWindowEx(
			0, TRACKBAR_CLASS, (LPCWSTR)Caption.c_str(),
			Style, x, y, Height, Width,
			hDlg, (HMENU)(UINT_PTR)ID, hInst, NULL);
	}

	TrackbarCtrl::TrackbarCtrl(UINT CtrlID, swktool::Window* pParent) : Ctrl(CtrlID, pParent) {
	}

	TrackbarCtrl::TrackbarCtrl(UINT CtrlID, DialogWindow* pParent) : Ctrl(CtrlID, pParent) {
	}
}