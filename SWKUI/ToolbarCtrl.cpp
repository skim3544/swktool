
#include "pch.h"
#include "ctrl.h"
#include "ToolbarCtrl.h"

namespace swktool {

	ToolbarCtrl::ToolbarCtrl(std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, IWindow* pParent, UINT CtrlID) : Ctrl(pParent)
	{
		INITCOMMONCONTROLSEX icex{};

		icex.dwSize = sizeof(icex);
		icex.dwICC = ICC_BAR_CLASSES;

		InitCommonControlsEx(&icex);

		ID = CtrlID;
		hwndCtrl = ::CreateWindowEx(0,
			TOOLBARCLASSNAME, (LPCWSTR)Caption.c_str(),
			Style, x, y, Height, Width,
			hParent, (HMENU)(UINT_PTR)ID, hInst, NULL);

	}
	ToolbarCtrl::ToolbarCtrl(UINT CtrlID, IWindow* pParent) : Ctrl(CtrlID, pParent) 
	{

	}

	//ToolbarCtrl::ToolbarCtrl(UINT CtrlID, DialogWindow* pParent) : Ctrl(CtrlID, pParent) {

	//}
}


