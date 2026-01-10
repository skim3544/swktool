#include "pch.h"
#include "AnimationCtrl.h"

namespace swktool {
	AnimationCtrl::AnimationCtrl(std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, IWindow* pParent, UINT CtrlID) : Ctrl(pParent)
	{
		ID = CtrlID;
		hwndCtrl = ::CreateWindow(ANIMATE_CLASS, (LPCWSTR)Caption.c_str(),
			Style, x, y, Height, Width,
			hParent, (HMENU)(UINT_PTR)ID, hInst, NULL);
	}

	AnimationCtrl::AnimationCtrl(UINT CtrlID, IWindow* pParent) : Ctrl(CtrlID, pParent) {
	}


}