#include "pch.h"
#include "AWindow.h"
#include "AnimationCtrl.h"

namespace swktool {
	AnimationCtrl::AnimationCtrl(std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, AWindow* pParent, UINT CtrlID) {
		hInst = pParent->GetInstanceHandle();
		hDlg = pParent->GetDlgHWND();
		ID = CtrlID;
		hwndCtrl = ::CreateWindow(ANIMATE_CLASS, (LPCWSTR)Caption.c_str(),
			Style, x, y, Height, Width,
			hDlg, (HMENU)(UINT_PTR)ID, hInst, NULL);
	}

	AnimationCtrl::AnimationCtrl(UINT CtrlID, AWindow* pParent) : Ctrl(CtrlID, pParent) {
	}

}