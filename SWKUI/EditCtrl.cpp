#include "pch.h"
#include "Ctrl.h"

namespace swktool {
	EditCtrl::EditCtrl(
		std::wstring Caption, 
		DWORD Style, 
		int x, int y, int Height, int Width, 
		swktool::IWindow* pParent, UINT CtrlID) 
	{		
		hParent = pParent->GetHwnd();
		hInst = (HINSTANCE)GetWindowLongPtr(hParent, GWLP_HINSTANCE);
		ID = CtrlID;
		hwndCtrl = ::CreateWindow(TEXT("EDIT"), (LPCWSTR)Caption.c_str(),
			(DWORD)Style, x, y, Height, Width,
			hParent, (HMENU)(UINT_PTR)ID, hInst, NULL);
	}


	EditCtrl::EditCtrl(UINT CtrlID, IWindow* pParent) :
		Ctrl(CtrlID, pParent)
	{
	}
}
