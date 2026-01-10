#include "pch.h"
#include "Ctrl.h"
#include "UpDownCtrl.h"


namespace swktool {
	UpDownCtrl::UpDownCtrl(std::wstring Caption, DWORD Style, int x, int y, int Width, int Height,  IWindow* pParent, UINT CtrlID) :
		Ctrl(pParent)
	{
		ID = CtrlID;

		INITCOMMONCONTROLSEX icex{};

		icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
		icex.dwICC = ICC_UPDOWN_CLASS;    // Set the Initialization Flag value.
		InitCommonControlsEx(&icex);      // Initialize the Common Controls Library.

		
		hwndCtrl = ::CreateWindowEx(
			0, 
			UPDOWN_CLASS, (LPCWSTR)Caption.c_str(),
			Style, x, y, Height, Width,
			hParent, (HMENU)(UINT_PTR)ID, hInst, NULL);
	}

	UpDownCtrl::UpDownCtrl(UINT CtrlID, swktool::IWindow* pParent) : Ctrl(CtrlID, pParent) {
	}

	//UpDownCtrl::UpDownCtrl(UINT CtrlID, DialogWindow* pParent) : Ctrl(CtrlID, pParent) {
	//}


}

