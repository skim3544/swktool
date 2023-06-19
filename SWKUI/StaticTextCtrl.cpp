#include "pch.h"
#include "Ctrl.h"
#include "StaticTextCtrl.h"

namespace swktool {
	StaticTextCtrl::StaticTextCtrl(UINT CtrlID, Window* pParent) : Ctrl(CtrlID, pParent)
	{
	}

	StaticTextCtrl::StaticTextCtrl(std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, Window* pParent, UINT CtrlID) {
		hInst = pParent->GetInstance();
		hDlg = pParent->GetWindowHandle();
		hwndCtrl = ::CreateWindow(TEXT("StaticText"), (LPCWSTR)Caption.c_str(),
			Style, x, y, Height, Width,
			hDlg, (HMENU)(UINT_PTR)CtrlID, hInst, NULL);
	}
}

