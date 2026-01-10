#include "pch.h"
#include "Ctrl.h"
#include "StaticTextCtrl.h"

namespace swktool {

	StaticTextCtrl::StaticTextCtrl(UINT CtrlID, IWindow* pParent) : Ctrl(CtrlID, pParent)
	{
	}

	StaticTextCtrl::StaticTextCtrl(std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, IWindow* pParent, UINT CtrlID) {

		hParent = pParent->GetHwnd();
		hInst = (HINSTANCE)GetWindowLongPtr(hParent, GWLP_HINSTANCE);

		hwndCtrl = ::CreateWindow(TEXT("STATIC"), (LPCWSTR)Caption.c_str(),
			Style, x, y, Height, Width,
			hParent, (HMENU)(UINT_PTR)CtrlID, hInst, NULL);
	}

}

