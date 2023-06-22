#include "pch.h"
#include "AnimationCtrl.h"

namespace swktool {
	AnimationCtrl::AnimationCtrl(std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, Window* pParent, UINT CtrlID) {
		hInst = pParent->GetInstance();
		hDlg = pParent->GetWindowHandle();
		ID = CtrlID;
		hwndCtrl = ::CreateWindow(ANIMATE_CLASS, (LPCWSTR)Caption.c_str(),
			Style, x, y, Height, Width,
			hDlg, (HMENU)(UINT_PTR)ID, hInst, NULL);
	}

	AnimationCtrl::AnimationCtrl(UINT CtrlID, Window* pParent) : Ctrl(CtrlID, pParent) {
	}

	AnimationCtrl::AnimationCtrl(UINT CtrlID, DialogWindow* pParent) : Ctrl(CtrlID, pParent) {
	}

}