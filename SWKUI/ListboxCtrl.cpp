#include "pch.h"
#include "ListboxCtrl.h"

namespace swktool {

	ListboxCtrl::ListboxCtrl(std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, Window* pParent, UINT CtrlID) {
		hInst = pParent->GetInstance();
		hParent = pParent->GetHwnd();
		ID = CtrlID;
		hwndCtrl = ::CreateWindow(TEXT("LISTBOX"), (LPCWSTR)Caption.c_str(),
			Style, x, y, Height, Width,
			hParent, (HMENU)(UINT_PTR)ID, hInst, NULL);
	}

	ListboxCtrl::ListboxCtrl(UINT CtrlID, Window* pParent) : Ctrl(CtrlID, pParent) {
	}

	//ListboxCtrl::ListboxCtrl(UINT CtrlID, DialogWindow* pParent) : Ctrl(CtrlID, pParent) {
	//}


}