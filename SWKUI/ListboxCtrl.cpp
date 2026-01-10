#include "pch.h"
#include "WindowInterfaces.h"
#include "ListboxCtrl.h"

namespace swktool {

	ListboxCtrl::ListboxCtrl(std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, IWindow* pParent, UINT CtrlID) {		
		hParent = pParent->GetHwnd();
		hInst = reinterpret_cast<HINSTANCE>(GetWindowLongPtr(hParent, GWLP_HINSTANCE));
		ID = CtrlID;
		hwndCtrl = ::CreateWindow(TEXT("LISTBOX"), (LPCWSTR)Caption.c_str(),
			Style, x, y, Height, Width,
			hParent, (HMENU)(UINT_PTR)ID, hInst, NULL);
	}

	ListboxCtrl::ListboxCtrl(UINT CtrlID, IWindow* pParent) : Ctrl(CtrlID, pParent) {
	}
}