#include "pch.h"
#include "StatusBarCtrl.h"




namespace swktool {
	StatusBarCtrl::StatusBarCtrl(std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, Window* pParent, UINT CtrlID) :
		Ctrl(pParent)
	{
		INITCOMMONCONTROLSEX iccx{};
		iccx.dwSize = sizeof(INITCOMMONCONTROLSEX);
		iccx.dwICC = ICC_BAR_CLASSES;
		bool bInit = InitCommonControlsEx(&iccx);

		// Create the status bar.
		hwndCtrl = CreateWindowEx(
			0,                       // no extended styles
			STATUSCLASSNAME,         // name of status bar class
			(PCTSTR)NULL,           // no text when first created
			SBARS_SIZEGRIP |         // includes a sizing grip
			WS_CHILD | WS_VISIBLE,   // creates a visible child window
			0, 0, 0, 0,              // ignores size and position
			hDlg,              // handle to parent window
			(HMENU)0,       // child window identifier
			hInst,                   // handle to application instance
			NULL);                   // no window creation data

		::GetClientRect(pParent->GetWindowHandle(), &rectClient_);
	}

	StatusBarCtrl::StatusBarCtrl(Window* pParent) : Ctrl(pParent) {
		INITCOMMONCONTROLSEX iccx{};
		iccx.dwSize = sizeof(INITCOMMONCONTROLSEX);
		iccx.dwICC = ICC_BAR_CLASSES;
		bool bInit = InitCommonControlsEx(&iccx);

		// Create the status bar.
		hwndCtrl = CreateWindowEx(
			0,                       // no extended styles
			STATUSCLASSNAME,         // name of status bar class
			(PCTSTR)NULL,           // no text when first created
			SBARS_SIZEGRIP |         // includes a sizing grip
			WS_CHILD | WS_VISIBLE,   // creates a visible child window
			0, 0, 0, 0,              // ignores size and position
			hDlg,              // handle to parent window
			(HMENU)0,       // child window identifier
			hInst,                   // handle to application instance
			NULL);                   // no window creation data

		::GetClientRect(pParent->GetWindowHandle(), &rectClient_);
	}

	StatusBarCtrl::StatusBarCtrl(DialogWindow* pParent) : Ctrl(pParent) {
		INITCOMMONCONTROLSEX iccx{};
		iccx.dwSize = sizeof(INITCOMMONCONTROLSEX);
		iccx.dwICC = ICC_BAR_CLASSES;
		bool bInit = InitCommonControlsEx(&iccx);

		// Create the status bar.
		hwndCtrl = CreateWindowEx(
			0,                       // no extended styles
			STATUSCLASSNAME,         // name of status bar class
			(PCTSTR)NULL,           // no text when first created
			SBARS_SIZEGRIP |         // includes a sizing grip
			WS_CHILD | WS_VISIBLE,   // creates a visible child window
			0, 0, 0, 0,              // ignores size and position
			hDlg,              // handle to parent window
			(HMENU)0,       // child window identifier
			hInst,                   // handle to application instance
			NULL);                   // no window creation data

		::GetClientRect(pParent->GetWindowHandle(), &rectClient_);
	}


}