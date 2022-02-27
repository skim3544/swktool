#include "pch.h"
#include "framework.h"
#include "AWindow.h"
#include "Ctrl.h"
#include "Window.h"


#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#else
#define DBG_NEW new
#endif


namespace swktool {
	Ctrl::Ctrl(AWindow* pParent) :
		hInst(nullptr), hwndCtrl(nullptr), hDlg(nullptr), ID(-1) {
		hInst = pParent->GetInstanceHandle();
		hDlg = pParent->GetWindowHWND();
	}

	Ctrl::Ctrl(UINT CtrlID, AWindow* pParent) :
		hInst(nullptr), hwndCtrl(nullptr), hDlg(nullptr), ID(CtrlID) {

		hInst = pParent->GetInstanceHandle();

		hDlg = pParent->GetWindowHWND();
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