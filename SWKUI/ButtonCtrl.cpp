#include "pch.h"
#include "AWindow.h"
#include "Window.h"
#include "ButtonCtrl.h"



namespace swktool {
	ButtonCtrl::ButtonCtrl(std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, AWindow* pParent, UINT CtrlID) {
		hInst = pParent->GetInstanceHandle();
		hDlg = pParent->GetWindowHWND();
		ID = CtrlID;
		hwndCtrl = ::CreateWindow(TEXT("BUTTON"), (LPCWSTR)Caption.c_str(),
			Style, x, y, Height, Width,
			hDlg, (HMENU)(UINT_PTR)ID, hInst, NULL);
	}

	ButtonCtrl::ButtonCtrl(UINT CtrlID, AWindow* pParent) :
		Ctrl(CtrlID, pParent)
	{
	}




	CheckboxCtrl::CheckboxCtrl(std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, AWindow* pParent, UINT CtrlID) {
		hInst = pParent->GetInstanceHandle();
		hDlg = pParent->GetWindowHWND();
		ID = CtrlID;
		hwndCtrl = ::CreateWindow(
			TEXT("CHECKBOX"), 
			(LPCWSTR)Caption.c_str(),
			Style, x, y, Height, Width,
			hDlg, (HMENU)(UINT_PTR)ID, hInst, NULL);
	}

	CheckboxCtrl::CheckboxCtrl(UINT CtrlID, AWindow* pParent) :
		Ctrl(CtrlID, pParent) {
	}



	RadioCtrl::RadioCtrl(std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, AWindow* pParent, UINT CtrlID) {
		hInst = pParent->GetInstanceHandle();
		hDlg = pParent->GetWindowHWND();
		ID = CtrlID;
		hwndCtrl = ::CreateWindow(TEXT("BUTTON"), (LPCWSTR)Caption.c_str(),
			Style, x, y, Height, Width,
			hDlg, (HMENU)(UINT_PTR)ID, hInst, NULL);
	}

	RadioCtrl::RadioCtrl(UINT CtrlID, AWindow* pParent) :
		Ctrl(CtrlID, pParent) {		
	}

}