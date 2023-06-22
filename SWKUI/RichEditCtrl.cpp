#include "pch.h"
#include <Richedit.h>
#include "RichEditCtrl.h"

namespace swktool {
	RichEditCtrl::RichEditCtrl(std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, Window* pParent, UINT CtrlID)
	{
		::LoadLibrary(TEXT("Msftedit.dll"));

		hInst = pParent->GetInstance();
		hDlg = pParent->GetWindowHandle();
		ID = CtrlID;
		hwndCtrl = ::CreateWindowEx(0,
			MSFTEDIT_CLASS, (LPCWSTR)Caption.c_str(),
			Style, x, y, Height, Width,
			hDlg, (HMENU)(UINT_PTR)ID, hInst, NULL);

	}


	RichEditCtrl::RichEditCtrl(UINT CtrlID, Window* pParent) : Ctrl(CtrlID, pParent) {

	}

	RichEditCtrl::RichEditCtrl(UINT CtrlID, DialogWindow* pParent) : Ctrl(CtrlID, pParent) {

	}

}