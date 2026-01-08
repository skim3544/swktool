#include "pch.h"
#include "DateTimePickerCtrl.h"


namespace swktool {
	DateTimePickerCtrl::DateTimePickerCtrl(std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, Window* pParent, UINT CtrlID)
	{
		INITCOMMONCONTROLSEX icex{};

		icex.dwSize = sizeof(icex);
		icex.dwICC = ICC_DATE_CLASSES;

		InitCommonControlsEx(&icex);

		hInst = pParent->GetInstance();
		hParent = pParent->GetHwnd();

		ID = CtrlID;

		hwndCtrl = CreateWindowEx(0,
			DATETIMEPICK_CLASS,
			TEXT("DateTime"),
			Style,
			x, y, Height, Width,
			hParent,
			(HMENU)(UINT_PTR)ID,
			hInst,
			NULL);
	}

	DateTimePickerCtrl::DateTimePickerCtrl(UINT CtrlID, Window* pParent) : 
		Ctrl(CtrlID, pParent) {

	}

	//DateTimePickerCtrl::DateTimePickerCtrl(UINT CtrlID, DialogWindow* pParent) : Ctrl(CtrlID, pParent) {

	//}
}