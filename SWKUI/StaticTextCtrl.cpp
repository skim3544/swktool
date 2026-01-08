#include "pch.h"
#include "Ctrl.h"
#include "StaticTextCtrl.h"

namespace swktool {

	StaticTextCtrl::StaticTextCtrl(UINT CtrlID, Window* pParent) : Ctrl(CtrlID, pParent)
	{
	}
	//StaticTextCtrl::StaticTextCtrl(UINT CtrlID, DialogWindow* pParent) : Ctrl(CtrlID, pParent)
	//{
	//}

	StaticTextCtrl::StaticTextCtrl(std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, Window* pParent, UINT CtrlID) {
		hInst = pParent->GetInstance();
		hParent = pParent->GetHwnd();
		hwndCtrl = ::CreateWindow(TEXT("StaticText"), (LPCWSTR)Caption.c_str(),
			Style, x, y, Height, Width,
			hParent, (HMENU)(UINT_PTR)CtrlID, hInst, NULL);
	}

}

