#include "pch.h"
#include "Ctrl.h"
#include "SysLinkCtrl.h"

namespace swktool {

    SysLinkCtrl::SysLinkCtrl(std::wstring HtmlLinkText, DWORD Style, int x, int y, int Width, int Height, Window* pParent, UINT CtrlID)
    {
		INITCOMMONCONTROLSEX icex{};

        icex.dwSize = sizeof(icex);
        icex.dwICC = ICC_LINK_CLASS;

        InitCommonControlsEx(&icex);
		hInst = pParent->GetInstance();
		hDlg = pParent->GetWindowHandle();
		ID = CtrlID;

		hwndCtrl = CreateWindowEx(
			0,                       // no extended styles
			WC_LINK,         // name of status bar class
			(PCTSTR)HtmlLinkText.c_str(),           // no text when first created
			Style,   
			x, y, Width, Height,              // ignores size and position
			hDlg,              // handle to parent window
			(HMENU)0,       // child window identifier
			hInst,                   // handle to application instance
			NULL);                   // no window creation data
    }
    SysLinkCtrl::SysLinkCtrl(UINT CtrlID, Window* pParent) : Ctrl(CtrlID, pParent) {

    }
	SysLinkCtrl::SysLinkCtrl(UINT CtrlID, DialogWindow* pParent) : Ctrl(CtrlID, pParent) {

	}

}
