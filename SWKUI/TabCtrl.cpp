#include "pch.h"
#include "Ctrl.h"
#include "TabCtrl.h"

namespace swktool {

	TabCtrl::TabCtrl(std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, IWindow* pParent, UINT CtrlID) : 
		Ctrl(pParent) 
	{
		INITCOMMONCONTROLSEX icex{};

		icex.dwSize = sizeof(icex);
		icex.dwICC = ICC_TAB_CLASSES;

		InitCommonControlsEx(&icex);

		ID = CtrlID;
		hwndCtrl = ::CreateWindowEx(0,
			WC_TABCONTROL, (LPCWSTR)Caption.c_str(),
			Style, x, y, Height, Width,
			hParent, (HMENU)(UINT_PTR)ID, hInst, NULL);

	}
	TabCtrl::TabCtrl(UINT CtrlID, IWindow* pParent) : Ctrl(CtrlID, pParent) 
	{

	}

}