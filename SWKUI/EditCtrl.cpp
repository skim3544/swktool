#include "pch.h"
#include "Ctrl.h"

namespace swktool {
	EditCtrl::EditCtrl(
		std::wstring Caption, 
		DWORD Style, 
		int x, int y, int Height, int Width, 
		swktool::Window* pParent, UINT CtrlID) {
		hInst = pParent->GetInstance();
		hDlg = pParent->GetWindowHandle();
		ID = CtrlID;
		hwndCtrl = ::CreateWindow(TEXT("EDIT"), (LPCWSTR)Caption.c_str(),
			(DWORD)Style, x, y, Height, Width,
			hDlg, (HMENU)(UINT_PTR)ID, hInst, NULL);
	}


	EditCtrl::EditCtrl(UINT CtrlID, Window* pParent) :
		Ctrl(CtrlID, pParent)
	{
	}
	EditCtrl::EditCtrl(UINT CtrlID, DialogWindow* pParent) :
		Ctrl(CtrlID, pParent)
	{
	}


}
