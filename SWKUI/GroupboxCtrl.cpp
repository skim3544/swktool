#include "pch.h"
#include "Ctrl.h"
#include "GroupboxCtrl.h"

namespace swktool {
	GroupboxCtrl::GroupboxCtrl(std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, Window* pParent, UINT CtrlID) {
		hInst = pParent->GetInstance();
		hParent = pParent->GetHwnd();
		ID = CtrlID;
		hwndCtrl = ::CreateWindow(TEXT("BUTTON"), (LPCWSTR)Caption.c_str(),
			Style, x, y, Height, Width,
			hParent, (HMENU)(UINT_PTR)ID, hInst, NULL);
	}

	GroupboxCtrl::GroupboxCtrl(UINT CtrlID, Window* pParent) : Ctrl(CtrlID, pParent) {
	}

	//GroupboxCtrl::GroupboxCtrl(UINT CtrlID, DialogWindow* pParent) : Ctrl(CtrlID, pParent) {
	//}
}