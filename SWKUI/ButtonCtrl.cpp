#include "pch.h"
#include "Window.h"
#include "ButtonCtrl.h"



namespace swktool {
	ButtonCtrl::ButtonCtrl(std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, Window* pParent, UINT CtrlID) {
		hInst = pParent->GetInstance();
		hDlg = pParent->GetWindowHandle();
		ID = CtrlID;
		hwndCtrl = ::CreateWindow(TEXT("BUTTON"), (LPCWSTR)Caption.c_str(),
			Style, x, y, Height, Width,
			hDlg, (HMENU)(UINT_PTR)ID, hInst, NULL);
	}

	ButtonCtrl::ButtonCtrl(UINT CtrlID, Window* pParent) :
		Ctrl(CtrlID, pParent)
	{
	}

	ButtonCtrl::ButtonCtrl(UINT CtrlID, DialogWindow* pParent) : 
		Ctrl(CtrlID, pParent) 
	{

	}



	CheckboxCtrl::CheckboxCtrl(std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, Window* pParent, UINT CtrlID) {
		hInst = pParent->GetInstance();
		hDlg = pParent->GetWindowHandle();
		ID = CtrlID;
		hwndCtrl = ::CreateWindow(
			TEXT("CHECKBOX"), 
			(LPCWSTR)Caption.c_str(),
			Style, x, y, Height, Width,
			hDlg, (HMENU)(UINT_PTR)ID, hInst, NULL);
	}

	CheckboxCtrl::CheckboxCtrl(UINT CtrlID, Window* pParent) :
		Ctrl(CtrlID, pParent) {
	}
	CheckboxCtrl::CheckboxCtrl(UINT CtrlID, swktool::DialogWindow* pParent) :
		Ctrl(CtrlID, pParent) {
	}



	RadioCtrl::RadioCtrl(std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, Window* pParent, UINT CtrlID) {
		hInst = pParent->GetInstance();
		hDlg = pParent->GetWindowHandle();
		ID = CtrlID;
		hwndCtrl = ::CreateWindow(TEXT("BUTTON"), (LPCWSTR)Caption.c_str(),
			Style, x, y, Height, Width,
			hDlg, (HMENU)(UINT_PTR)ID, hInst, NULL);
	}

	RadioCtrl::RadioCtrl(UINT CtrlID, Window* pParent) :
		Ctrl(CtrlID, pParent) {		
	}

	RadioCtrl::RadioCtrl(UINT CtrlID, DialogWindow* pParent) :
		Ctrl(CtrlID, pParent) {
	}

}