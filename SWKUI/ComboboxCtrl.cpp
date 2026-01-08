#include "pch.h"
#include "ComboboxCtrl.h"

namespace swktool {
	ComboboxCtrl::ComboboxCtrl(std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, Window* pParent, UINT CtrlID) {
		hInst = pParent->GetInstance();
		hParent = pParent->GetHwnd();
		ID = CtrlID;
		hwndCtrl = ::CreateWindow(TEXT("COMBOBOX"), (LPCWSTR)Caption.c_str(),
			Style, x, y, Height, Width,
			hParent, (HMENU)(UINT_PTR)ID, hInst, NULL);
	}

	ComboboxCtrl::ComboboxCtrl(UINT CtrlID, Window* pParent) : Ctrl(CtrlID, pParent) {
	}
	//ComboboxCtrl::ComboboxCtrl(UINT CtrlID, DialogWindow* pParent) : Ctrl(CtrlID, pParent) {
	//}


}