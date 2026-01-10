#include "pch.h"
#include "ProgressBarCtrl.h"

namespace swktool {
	ProgressBarCtrl::ProgressBarCtrl(std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, IWindow* pParent, UINT CtrlID) {		
		hParent = pParent->GetHwnd();
        hInst = reinterpret_cast<HINSTANCE>(GetWindowLongPtr(hParent, GWLP_HINSTANCE));
		ID = CtrlID;
		hwndCtrl = ::CreateWindow(PROGRESS_CLASS, (LPCWSTR)Caption.c_str(),
			Style, x, y, Height, Width,
			hParent, (HMENU)(UINT_PTR)ID, hInst, NULL);
	}

    ProgressBarCtrl::ProgressBarCtrl(HWND hParent, HINSTANCE hInstance,
        std::wstring Caption, DWORD Style,
        int x, int y, int Height, int Width, UINT CtrlID)
    {
        hInst = hInstance;
        hParent = hParent;
        ID = CtrlID;

        hwndCtrl = ::CreateWindowEx(
            0,
            PROGRESS_CLASS,
            Caption.c_str(),
            Style,
            x, y, Height, Width,
            hParent,                     
            (HMENU)(UINT_PTR)ID,
            hInst,
            nullptr
        );
    }

	ProgressBarCtrl::ProgressBarCtrl(UINT CtrlID, IWindow* pParent) : Ctrl(CtrlID, pParent) {
	}

	//ProgressBarCtrl::ProgressBarCtrl(UINT CtrlID, DialogWindow* pParent) : Ctrl(CtrlID, pParent) {
	//}
}