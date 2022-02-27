#include "pch.h"
#include <Richedit.h>
#include "AWindow.h"
#include "RichEditCtrl.h"

namespace swktool {
	RichEditCtrl::RichEditCtrl(std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, AWindow* pParent, UINT CtrlID)
	{
		::LoadLibrary(TEXT("Msftedit.dll"));

		hInst = pParent->GetInstanceHandle();
		hDlg = pParent->GetDlgHWND();
		ID = CtrlID;
		hwndCtrl = ::CreateWindowEx(0,
			MSFTEDIT_CLASS, (LPCWSTR)Caption.c_str(),
			Style, x, y, Height, Width,
			hDlg, (HMENU)(UINT_PTR)ID, hInst, NULL);

	}


	RichEditCtrl::RichEditCtrl(UINT CtrlID, AWindow* pParent) : Ctrl(CtrlID, pParent) {

	}


}