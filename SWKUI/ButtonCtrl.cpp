#include "pch.h"
#include "Window.h"
#include "ButtonCtrl.h"

#include <uxtheme.h>
#pragma comment(lib, "uxtheme.lib")


namespace swktool 
{
	ButtonCtrl::ButtonCtrl(std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, IWindow* pParent, UINT CtrlID) :
		Ctrl(pParent)
	{
		hParent = pParent->GetHwnd();
		ID = CtrlID;
		hwndCtrl = ::CreateWindow(TEXT("BUTTON"), (LPCWSTR)Caption.c_str(),
			Style, x, y, Height, Width,
			hParent, (HMENU)(UINT_PTR)ID, hInst, NULL);


	}

	ButtonCtrl::ButtonCtrl(UINT CtrlID, IWindow* pParent) :
		Ctrl(CtrlID, pParent)
	{

	}

	void ButtonCtrl::ApplyTheme(const Theme& theme)
	{
		currentTheme_ = theme;

		// Disable visual styles so WM_CTLCOLORBTN works
		SetWindowTheme(GetCtrlHandle(), L"", L"");

		// Force repaint
		InvalidateRect(GetCtrlHandle(), nullptr, TRUE);
	}






	CheckboxCtrl::CheckboxCtrl(std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, IWindow* pParent, UINT CtrlID) :
		Ctrl(pParent)
	{
		ID = CtrlID;
		hwndCtrl = ::CreateWindow(
			TEXT("CHECKBOX"),
			(LPCWSTR)Caption.c_str(),
			Style, x, y, Height, Width,
			hParent, (HMENU)(UINT_PTR)ID, hInst, NULL);
	}

	CheckboxCtrl::CheckboxCtrl(UINT CtrlID, IWindow* pParent) :
		Ctrl(CtrlID, pParent) {
	}







	RadioCtrl::RadioCtrl(std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, IWindow* pParent, UINT CtrlID) :
		Ctrl(pParent)
	{
		hParent = pParent->GetHwnd();
		ID = CtrlID;
		hwndCtrl = ::CreateWindow(TEXT("BUTTON"), (LPCWSTR)Caption.c_str(),
			Style, x, y, Height, Width,
			hParent, (HMENU)(UINT_PTR)ID, hInst, NULL);
	}

	RadioCtrl::RadioCtrl(UINT CtrlID, IWindow* pParent) :
		Ctrl(CtrlID, pParent) {		
	}
}