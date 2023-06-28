#include "pch.h"
#include "framework.h"
#include "Ctrl.h"
#include "Window.h"


#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#else
#define DBG_NEW new
#endif


// When you are using pre-compiled headers, this source file is necessary for compilation to succeed.
#pragma comment(lib, "Comctl32.lib")




namespace swktool {
	Ctrl::Ctrl(Window* pParent) :
		hInst(nullptr), hwndCtrl(nullptr), hDlg(nullptr), ID(-1) {
		hInst = pParent->GetInstance();
		hDlg = pParent->GetWindowHandle();
	}

	Ctrl::Ctrl(DialogWindow* pParent) :
		hInst(nullptr), hwndCtrl(nullptr), hDlg(nullptr), ID(-1) {
		hInst = pParent->GetInstance();
		hDlg = pParent->GetWindowHandle();
	}


	Ctrl::Ctrl(UINT CtrlID, Window* pParent) :
		hInst(nullptr), hwndCtrl(nullptr), hDlg(nullptr), ID(CtrlID) {

		hInst = pParent->GetInstance();
		hDlg = pParent->GetWindowHandle();

		hwndCtrl = GetDlgItem(hDlg, CtrlID);

		GetWindowRect(hwndCtrl, &Rect);
	}

	Ctrl::Ctrl(UINT CtrlID, DialogWindow* pParent) :
		hInst(nullptr), hwndCtrl(nullptr), hDlg(nullptr), ID(CtrlID) {

		hInst = pParent->GetInstance();
		hDlg = pParent->GetWindowHandle();

		hwndCtrl = GetDlgItem(hDlg, CtrlID);

		GetWindowRect(hwndCtrl, &Rect);

	}

	void Ctrl::Enable() {
		::EnableWindow(hwndCtrl, TRUE);
	}

	void Ctrl::Disable() {
		::EnableWindow(hwndCtrl, FALSE);
	}

	void Ctrl::Show() {
		::ShowWindow(hwndCtrl, TRUE);
	}
	void Ctrl::Hide() {
		::ShowWindow(hwndCtrl, FALSE);
	}
}