#include "pch.h"
#include <windowsx.h>
#include "AWindow.h"
#include "Window.h"
#include "MDIChildWindow.h"

namespace swktool {
	LRESULT MDIChildWindow::ProcessMessage(WORD msg, WPARAM wParam, LPARAM lParam) {
		LRESULT lResult = 1;

		// by Default we did not handle the message
		//Result = 1;		
		switch (msg) {
			PROC_MSG(WM_COMMAND, OnCommand);
			PROC_MSG(WM_CREATE, OnCreate);
			PROC_MSG(WM_PAINT, OnPaint);
			PROC_MSG(WM_DESTROY, OnDestroy);

			// MDI child window cannot handle this message as it will block WM_MDIACTIVATE message from getting generated
			//PROC_MSG(WM_CHILDACTIVATE, OnChildAcrivate);
			//PROC_MSG(WM_NCCREATE, OnNCCreate);

			PROC_MSG(WM_NCDESTROY, OnNCDestroy);
			PROC_MSG(WM_CLOSE, OnClose);			
			PROC_MSG(WM_TIMER, OnTimer);
			PROC_MSG(WM_QUIT, OnQuit);
			PROC_MSG(WM_SHOWWINDOW, OnShowWindow);
			PROC_MSG(WM_MOVE, OnMove);
			PROC_MSG(WM_SIZE, OnSize);

			PROC_MSG(WM_KEYDOWN, OnKeyDown);
			PROC_MSG(WM_KEYUP, OnKeyUp);

			PROC_MSG(WM_LBUTTONDOWN, OnMouseLButtonDown);
			PROC_MSG(WM_LBUTTONUP, OnMouseLButtonUp);
			PROC_MSG(WM_LBUTTONDBLCLK, OnMouseLButtonDblClick);

			PROC_MSG(WM_RBUTTONDOWN, OnMouseRButtonDown);
			PROC_MSG(WM_RBUTTONUP, OnMouseRButtonUp);
			PROC_MSG(WM_RBUTTONDBLCLK, OnMouseRButtonDblClick);

			PROC_MSG(WM_MBUTTONDOWN, OnMouseMButtonDown);
			PROC_MSG(WM_MBUTTONUP, OnMouseMButtonUp);
			PROC_MSG(WM_MBUTTONDBLCLK, OnMouseMButtonDblClick);

			PROC_MSG(WM_MOUSEWHEEL, OnMouseWheel);
			PROC_MSG(WM_MOUSEMOVE, OnMouseMove);

			PROC_MSG(WM_HSCROLL, OnHScroll);
			PROC_MSG(WM_VSCROLL, OnVScroll);

			PROC_MSG(WM_CUT, OnClipboardCut);
			PROC_MSG(WM_COPY, OnClipboardCopy);
			PROC_MSG(WM_PASTE, OnClipboardPaste);
			PROC_MSG(WM_CLEAR, OnClipboardClear);
			PROC_MSG(WM_UNDO, OnClipboardUndo);


			PROC_MSG(WM_SETFOCUS, OnSetFocus);
			PROC_MSG(WM_KILLFOCUS, OnKillFocus);
			//PROC_MSG(WM_SETCURSOR, OnSetCursor);
			PROC_MSG(WM_SETTINGCHANGE, OnSettingChange);
			PROC_MSG(WM_DRAWITEM, OnDrawItem);

			PROC_MSG(WM_CTLCOLORSCROLLBAR, OnCtrlColorScrollbar);
			PROC_MSG(WM_CTLCOLORMSGBOX, OnCtrlColorMsgBox);
			PROC_MSG(WM_CTLCOLOREDIT, OnCtrlColorEdit);
			PROC_MSG(WM_CTLCOLORLISTBOX, OnCtrlColorListBox);
			PROC_MSG(WM_CTLCOLORBTN, OnCtrlColorBtn);
			PROC_MSG(WM_CTLCOLORDLG, OnCtrlColorDlg);
			PROC_MSG(WM_CTLCOLORSTATIC, OnCtrlColorStatic);
			PROC_MSG(WM_SYSCOLORCHANGE, OnSysColorChange);
			PROC_MSG(WM_INITMENU, OnInitMenu);
			PROC_MSG(WM_INITMENUPOPUP, OnInitMenuPopup);
			PROC_MSG(WM_QUERYENDSESSION, OnQueryEndSession);
			PROC_MSG(WM_ENDSESSION, OnEndSession);

			PROC_MSG(WM_MDIACTIVATE, OnMDIActivate);

		default:
			lResult = ::DefMDIChildProc(this->GetWindowHWND(), msg, wParam, lParam);
		}

		return lResult;
	}
}