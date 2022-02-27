#include "pch.h"
#include "AWindow.h"
#include "MonthCalendarCtrl.h"

namespace swktool {
	MonthCalendarCtrl::MonthCalendarCtrl(std::wstring Caption, DWORD Style, int x, int y, int Width, int Height, AWindow* pParent, UINT CtrlID)
	{
		INITCOMMONCONTROLSEX icex;

		icex.dwSize = sizeof(icex);
		icex.dwICC = ICC_DATE_CLASSES;

		InitCommonControlsEx(&icex);

		hInst = pParent->GetInstanceHandle();
		hDlg = pParent->GetDlgHWND();
		ID = CtrlID;
		hwndCtrl = ::CreateWindowEx(0,
			MONTHCAL_CLASS, (LPCWSTR)Caption.c_str(),
			Style, x, y, Width, Height,
			hDlg, (HMENU)(UINT_PTR)ID, hInst, NULL);
	}


	MonthCalendarCtrl::MonthCalendarCtrl(UINT CtrlID, AWindow* pParent) : Ctrl(CtrlID, pParent) {

	}


}