#include "pch.h"
#include <Richedit.h>
#include "RichEditCtrl.h"
#include "WindowInterfaces.h"

namespace swktool
{
	RichEditCtrl::RichEditCtrl(std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, IWindow* pParent, UINT CtrlID) : 
		Ctrl(pParent) 
	{
		::LoadLibrary(TEXT("Msftedit.dll"));


		ID = CtrlID;
		hwndCtrl = ::CreateWindowEx(0,
			MSFTEDIT_CLASS, (LPCWSTR)Caption.c_str(),
			Style, x, y, Height, Width,
			hParent, (HMENU)(UINT_PTR)ID, hInst, NULL);

	}


	RichEditCtrl::RichEditCtrl(UINT CtrlID, IWindow* pParent) : Ctrl(CtrlID, pParent) 
	{

	}


}