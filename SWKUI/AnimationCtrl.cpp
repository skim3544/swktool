#include "pch.h"
#include "AnimationCtrl.h"

namespace swktool {
	AnimationCtrl::AnimationCtrl(std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, IWindow* pParent, UINT CtrlID) : Ctrl(pParent)
	{
		ID = CtrlID;
		hwndCtrl = ::CreateWindow(ANIMATE_CLASS, (LPCWSTR)Caption.c_str(),
			Style, x, y, Height, Width,
			hParent, (HMENU)(UINT_PTR)ID, hInst, nullptr);
	}

	AnimationCtrl::AnimationCtrl(HWND hParent, HINSTANCE hInst, std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, UINT CtrlID)
	{
		hInst = hInst;
		this->hParent = hParent;
		ID = CtrlID;
		hwndCtrl = ::CreateWindow(ANIMATE_CLASS, (LPCWSTR)Caption.c_str(),
			Style, x, y, Height, Width,
			hParent, (HMENU)(UINT_PTR)ID, hInst, nullptr);
	}

	AnimationCtrl::AnimationCtrl(UINT CtrlID, IWindow* pParent) : 
		Ctrl(CtrlID, pParent) 
	{
	}


}