#include "pch.h"
#include "AWindow.h"
#include "ScrollBarCtrl.h"

namespace swktool {

	ScrollBarCtrl::ScrollBarCtrl(std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, AWindow* pParent, UINT CtrlID) {
		hInst = pParent->GetInstanceHandle();
		hDlg = pParent->GetDlgHWND();
		ID = CtrlID;
		hwndCtrl = ::CreateWindowEx(0,
			TEXT("SCROLLBAR"), (LPCWSTR)Caption.c_str(),
			Style, x, y, Height, Width,
			hDlg, (HMENU)(UINT_PTR)ID, hInst, NULL);
	}

	ScrollBarCtrl::ScrollBarCtrl(UINT CtrlID, AWindow* pParent) : Ctrl(CtrlID, pParent) {
	}

}