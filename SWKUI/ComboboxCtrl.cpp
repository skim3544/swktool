#include "pch.h"
#include "ComboboxCtrl.h"
#include "WindowInterfaces.h"

namespace swktool {
	ComboboxCtrl::ComboboxCtrl(std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, IWindow* pParent, UINT CtrlID) {
		
		hParent = pParent->GetHwnd();
		ID = CtrlID;
		hwndCtrl = ::CreateWindow(TEXT("COMBOBOX"), (LPCWSTR)Caption.c_str(),
			Style, x, y, Height, Width,
			hParent, (HMENU)(UINT_PTR)ID, hInst, NULL);
	}

	ComboboxCtrl::ComboboxCtrl(UINT CtrlID, IWindow* pParent) : Ctrl(CtrlID, pParent) 
	{
	}
}