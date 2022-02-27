#include "pch.h"
#include "AWindow.h"
#include "ListboxCtrl.h"

namespace swktool {

	ListboxCtrl::ListboxCtrl(std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, AWindow* pParent, UINT CtrlID) {
		hInst = pParent->GetInstanceHandle();
		hDlg = pParent->GetDlgHWND();
		ID = CtrlID;
		hwndCtrl = ::CreateWindow(TEXT("LISTBOX"), (LPCWSTR)Caption.c_str(),
			Style, x, y, Height, Width,
			hDlg, (HMENU)(UINT_PTR)ID, hInst, NULL);
	}

	ListboxCtrl::ListboxCtrl(UINT CtrlID, AWindow* pParent) : Ctrl(CtrlID, pParent) {
	}

}