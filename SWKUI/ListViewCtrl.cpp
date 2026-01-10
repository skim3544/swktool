#include "pch.h"
#include "ListViewCtrl.h"

namespace swktool {
	ListViewCtrl::ListViewCtrl(std::wstring Caption, DWORD Style, int x, int y, int Width, int Height, IWindow* pParent, UINT CtrlID) :
		Ctrl(pParent)
	{
		INITCOMMONCONTROLSEX icex{};

		icex.dwSize = sizeof(icex);
		icex.dwICC = ICC_LISTVIEW_CLASSES;

		InitCommonControlsEx(&icex);

		ID = CtrlID;
		hwndCtrl = ::CreateWindowEx(0,
			WC_LISTVIEW, (LPCWSTR)Caption.c_str(),
			Style, x, y, Width, Height,
			hParent, (HMENU)(UINT_PTR)ID, hInst, NULL);

	}


	ListViewCtrl::ListViewCtrl(UINT CtrlID, IWindow* pParent) : Ctrl(CtrlID, pParent) {

	}


}