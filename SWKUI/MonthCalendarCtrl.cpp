#include "pch.h"
#include "MonthCalendarCtrl.h"

namespace swktool {
	MonthCalendarCtrl::MonthCalendarCtrl(std::wstring Caption, DWORD Style, int x, int y, int Width, int Height, IWindow* pParent, UINT CtrlID) :
		Ctrl(pParent)
	{
		INITCOMMONCONTROLSEX icex{};

		icex.dwSize = sizeof(icex);
		icex.dwICC = ICC_DATE_CLASSES;

		InitCommonControlsEx(&icex);

		ID = CtrlID;
		hwndCtrl = ::CreateWindowEx(0,
			MONTHCAL_CLASS, (LPCWSTR)Caption.c_str(),
			Style, x, y, Width, Height,
			hParent, (HMENU)(UINT_PTR)ID, hInst, NULL);
	}


	MonthCalendarCtrl::MonthCalendarCtrl(UINT CtrlID, IWindow* pParent) : Ctrl(CtrlID, pParent) {

	}

	//MonthCalendarCtrl::MonthCalendarCtrl(UINT CtrlID, DialogWindow* pParent) : Ctrl(CtrlID, pParent) {

	//}
}