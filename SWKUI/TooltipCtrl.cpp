#include "pch.h"
#include "ctrl.h"
#include "TooltipCtrl.h"

namespace swktool {
	TooltipCtrl::TooltipCtrl(std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, Window* pParent, UINT CtrlID) {
		INITCOMMONCONTROLSEX icex;

		icex.dwSize = sizeof(icex);
		icex.dwICC = ICC_BAR_CLASSES;

		InitCommonControlsEx(&icex);

		hInst = pParent->GetInstance();
		hDlg = pParent->GetHwnd();

		ID = CtrlID;
		hwndCtrl = ::CreateWindowEx(0,
			TOOLTIPS_CLASS, (LPCWSTR)Caption.c_str(),
			WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP, 
			CW_USEDEFAULT, CW_USEDEFAULT,
			CW_USEDEFAULT, CW_USEDEFAULT,
			hDlg, (HMENU)(UINT_PTR)ID, hInst, NULL);

	}
	TooltipCtrl::TooltipCtrl(UINT CtrlID, Window* pParent) : Ctrl(CtrlID, pParent) {
		INITCOMMONCONTROLSEX icex;

		icex.dwSize = sizeof(icex);
		icex.dwICC = ICC_BAR_CLASSES;

		InitCommonControlsEx(&icex);

		hInst = pParent->GetInstance();
		hDlg = pParent->GetHwnd();

		hwndCtrl = CreateWindowEx(NULL, TOOLTIPS_CLASS, NULL,
			WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,
			CW_USEDEFAULT, CW_USEDEFAULT,
			CW_USEDEFAULT, CW_USEDEFAULT,
			hDlg, NULL, hInst,
			NULL);

		SetWindowPos(hwndCtrl, HWND_TOPMOST, 0, 0, 0, 0,
			SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	}

	TooltipCtrl::TooltipCtrl(UINT CtrlID, DialogWindow* pParent) : Ctrl(CtrlID, pParent) {
		INITCOMMONCONTROLSEX icex;

		icex.dwSize = sizeof(icex);
		icex.dwICC = ICC_BAR_CLASSES;

		InitCommonControlsEx(&icex);

		hInst = pParent->GetInstance();
		hDlg = pParent->GetHwnd();

		hwndCtrl = CreateWindowEx(NULL, TOOLTIPS_CLASS, NULL,
			WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,
			CW_USEDEFAULT, CW_USEDEFAULT,
			CW_USEDEFAULT, CW_USEDEFAULT,
			hDlg, NULL, hInst,
			NULL);

		SetWindowPos(hwndCtrl, HWND_TOPMOST, 0, 0, 0, 0,
			SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
	}
}

