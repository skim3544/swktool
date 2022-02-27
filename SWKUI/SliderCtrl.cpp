#include "pch.h"
#include "AWindow.h"
#include "SliderCtrl.h"

namespace swktool {
	TrackbarCtrl::TrackbarCtrl(std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, swktool::AWindow* pParent, UINT CtrlID) {
		hInst = pParent->GetInstanceHandle();
		hDlg = pParent->GetDlgHWND();
		ID = CtrlID;
		hwndCtrl = ::CreateWindowEx(
			0, TRACKBAR_CLASS, (LPCWSTR)Caption.c_str(),
			Style, x, y, Height, Width,
			hDlg, (HMENU)(UINT_PTR)ID, hInst, NULL);
	}

	TrackbarCtrl::TrackbarCtrl(UINT CtrlID, swktool::AWindow* pParent) : Ctrl(CtrlID, pParent) {
	}

}