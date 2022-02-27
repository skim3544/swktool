#include "pch.h"
#include "AWindow.h"
#include "DateTimePickerCtrl.h"


namespace swktool {
	DateTimePickerCtrl::DateTimePickerCtrl(std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, AWindow* pParent, UINT CtrlID)
	{
		INITCOMMONCONTROLSEX icex;

		icex.dwSize = sizeof(icex);
		icex.dwICC = ICC_DATE_CLASSES;

		InitCommonControlsEx(&icex);

		hInst = pParent->GetInstanceHandle();
		hDlg = pParent->GetDlgHWND();

		ID = CtrlID;

		hwndCtrl = CreateWindowEx(0,
			DATETIMEPICK_CLASS,
			TEXT("DateTime"),
			Style,
			x, y, Height, Width,
			hDlg,
			(HMENU)(UINT_PTR)ID,
			hInst,
			NULL);
	}

	DateTimePickerCtrl::DateTimePickerCtrl(UINT CtrlID, AWindow* pParent) : Ctrl(CtrlID, pParent) {

	}
}