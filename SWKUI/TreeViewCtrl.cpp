#include "pch.h"
#include "ctrl.h"
#include "AWindow.h"
#include "TreeViewCtrl.h"

namespace swktool {
	TreeViewCtrl::TreeViewCtrl(std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, Window* pParent, UINT CtrlID) {
		hInst = pParent->GetInstance();
		hDlg = pParent->GetWindowHandle();

		ID = CtrlID;
		hwndCtrl = ::CreateWindowEx(0,
			WC_TREEVIEW, (LPCWSTR)Caption.c_str(),
			Style, x, y, Height, Width,
			hDlg, (HMENU)(UINT_PTR)ID, hInst, NULL);
	}
	TreeViewCtrl::TreeViewCtrl(UINT CtrlID, Window* pParent) : Ctrl(CtrlID, pParent) {

	}
	
}