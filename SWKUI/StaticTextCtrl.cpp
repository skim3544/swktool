#include "pch.h"
#include "Ctrl.h"
#include "StaticTextCtrl.h"

namespace swktool {
	StaticTextCtrl::StaticTextCtrl(UINT CtrlID, AWindow* pParent) : Ctrl(CtrlID, pParent)
	{
	}

	StaticTextCtrl::StaticTextCtrl(std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, AWindow* pParent, UINT CtrlID) {
		hInst = pParent->GetInstanceHandle();
		hDlg = pParent->GetWindowHWND();
		hwndCtrl = ::CreateWindow(TEXT("StaticText"), (LPCWSTR)Caption.c_str(),
			Style, x, y, Height, Width,
			hDlg, (HMENU)(UINT_PTR)CtrlID, hInst, NULL);
	}
}

