#include "pch.h"
#include "ComboboxExCtrl.h"

namespace swktool {
	ComboboxExCtrl::ComboboxExCtrl(std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, IWindow* pParent, UINT CtrlID) : Ctrl(pParent) 
	{
		ID = CtrlID;

		INITCOMMONCONTROLSEX icex{};

		icex.dwSize = sizeof(icex);
		icex.dwICC = ICC_USEREX_CLASSES;

		InitCommonControlsEx(&icex);

		hwndCtrl = ::CreateWindowEx(0,
			WC_COMBOBOXEX, (LPCWSTR)Caption.c_str(),
			Style, x, y, Width, Height,
			hParent, (HMENU)(UINT_PTR)ID, hInst, NULL);
	}

	ComboboxExCtrl::ComboboxExCtrl(UINT CtrlID, IWindow* pParent) : Ctrl(CtrlID, pParent) {
	}

	//ComboboxExCtrl::ComboboxExCtrl(UINT CtrlID, DialogWindow* pParent) : Ctrl(CtrlID, pParent) {
	//}
}