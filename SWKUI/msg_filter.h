#ifndef ___MSG__CRACKER_H___
#define ___MSG__CRACKER_H___

// Windows Message Cracker
//  Based on Windowsx.h message cracker, minus the HWND parameter
//  Modified to pass wParam to OnCommand
//
#define PROC_MSG(message, fn)    \
    case (message): return PROC_##message((wParam), (lParam), (fn))

//#define PROC_DLG_MSG(message, fn)    \
//    case (message): PROCDLG_##message((wParam), (lParam), (fn)); break;

#define PROC_DLG_MSG(hwnd, message, fn) \
	case (message): return (SetDlgMsgResult(hwnd, message, PROCDLG_##message((wParam), (lParam), (fn))))

/*OnCommand(int id, HWND hwndCtl, UINT codeNotify) */
// ID = (int)(LOWORD(wParam));
// hWND = (HWND)(lParam);
// IDNotify = (UINT)HIWORD(wParam)
#define PROC_WM_COMMAND(wParam, lParam, fn) \
    ((fn)((WPARAM)(WPARAM(wParam)), (LPARAM)(lParam)), 0L)
//  ((fn)((WPARAM)(LOWORD(wParam)), (LPARAM)(lParam)), 0L)

/* BOOL Cls_OnCreate(LPCREATESTRUCT lpCreateStruct) */
#define PROC_WM_CREATE(wParam, lParam, fn) \
    ((fn)((LPCREATESTRUCT)(lParam)) ? 0L : (LRESULT)-1L)


/* void Cls_OnDestroy(HWND hwnd) */
#define PROC_WM_DESTROY(wParam, lParam, fn) \
    ((fn)(), 0L)

/* void Cls_OnClose(HWND hwnd) */
#define PROC_WM_CLOSE(wParam, lParam, fn) \
    ((fn)(), 0L)

/* void Cls_OnQuit(HWND hwnd, int exitCode) */
#define PROC_WM_QUIT(wParam, lParam, fn) \
    ((fn)((int)(wParam)), 0L)

#define PROC_WM_PAINT(wParam, lParam, fn) \
    ((fn)(), 0L)

#define PROC_WM_TIMER(wParam, lParam, fn) \
    ((fn)((UINT)(wParam)), 0L)

/* void Cls_OnKey(UINT vk, int cRepeat, UINT flags) */
#define PROC_WM_KEYDOWN(wParam, lParam, fn) \
    ((fn)((UINT)(wParam), (int)(short)LOWORD(lParam), (UINT)HIWORD(lParam)), 0L)

/* void Cls_OnKey(UINT vk, int cRepeat, UINT flags) */
#define PROC_WM_KEYUP(wParam, lParam, fn) \
    ((fn)((UINT)(wParam), (int)(short)LOWORD(lParam), (UINT)HIWORD(lParam)), 0L)

/* void Cls_OnLButtonDown(int x, int y, UINT keyFlags) */
#define PROC_WM_LBUTTONDOWN(wParam, lParam, fn) \
    ((fn)((int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), (UINT)(wParam)), 0L)

/* void Cls_OnLButtonDown(int x, int y, UINT keyFlags) */
#define PROC_WM_LBUTTONDBLCLK(wParam, lParam, fn) \
    ((fn)((int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), (UINT)(wParam)), 0L)

/* void Cls_OnLButtonUp(int x, int y, UINT keyFlags) */
#define PROC_WM_LBUTTONUP(wParam, lParam, fn) \
    ((fn)((int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), (UINT)(wParam)), 0L)

#define PROC_WM_RBUTTONDOWN(wParam, lParam, fn) \
    ((fn)((int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), (UINT)(wParam)), 0L)

/* void Cls_OnRButtonDown(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags) */
#define PROC_WM_RBUTTONDBLCLK(wParam, lParam, fn) \
    ((fn)((int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), (UINT)(wParam)), 0L)

/* void Cls_OnRButtonUp(HWND hwnd, int x, int y, UINT flags) */
#define PROC_WM_RBUTTONUP(wParam, lParam, fn) \
    ((fn)((int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), (UINT)(wParam)), 0L)

/* void Cls_OnMButtonDown(int x, int y, UINT keyFlags) */
#define PROC_WM_MBUTTONDOWN(wParam, lParam, fn) \
    ((fn)((int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), (UINT)(wParam)), 0L)

/* void Cls_OnMButtonDown(BOOL fDoubleClick, int x, int y, UINT keyFlags) */
#define PROC_WM_MBUTTONDBLCLK(wParam, lParam, fn) \
    ((fn)((int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), (UINT)(wParam)), 0L)

/* void Cls_OnMButtonUp(int x, int y, UINT flags) */
#define PROC_WM_MBUTTONUP(wParam, lParam, fn) \
    ((fn)((int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), (UINT)(wParam)), 0L)

/* void Cls_OnMouseWheel(int xPos, int yPos, int zDelta, UINT fwKeys) */
#define PROC_WM_MOUSEWHEEL(wParam, lParam, fn) \
    ((fn)((int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), (int)(short)HIWORD(wParam), (UINT)(short)LOWORD(wParam)), 0L)

/* void Cls_OnMouseMove(int x, int y, UINT keyFlags) */
#define PROC_WM_MOUSEMOVE(wParam, lParam, fn) \
    ((fn)((int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam), (UINT)(wParam)), 0L)

/* void Cls_OnHScroll(HWND hwndCtl, UINT code, int pos) */
#define PROC_WM_HSCROLL(wParam, lParam, fn) \
    ((fn)((HWND)(lParam), (UINT)(LOWORD(wParam)), (int)(short)HIWORD(wParam)), 0L)

/* void Cls_OnVScroll(HWND hwndCtl, UINT code, int pos) */
#define PROC_WM_VSCROLL(wParam, lParam, fn) \
    ((fn)((HWND)(lParam), (UINT)(LOWORD(wParam)),  (int)(short)HIWORD(wParam)), 0L)

#define PROC_WM_CUT(wParam, lParam, fn) \
    ((fn)(), 0L)

/* void Cls_OnCopy() */
#define PROC_WM_COPY(wParam, lParam, fn) \
    ((fn)(), 0L)
/* void Cls_OnPaste() */
#define PROC_WM_PASTE(wParam, lParam, fn) \
    ((fn)(), 0L)

/* void Cls_OnClear() */
#define PROC_WM_CLEAR(wParam, lParam, fn) \
    ((fn)(), 0L)

#define PROC_WM_UNDO(wParam, lParam, fn) \
    ((fn)(), 0L)

/* void Cls_OnSetFocus(HWND hwnd, HWND hwndOldFocus) */
#define PROC_WM_SETFOCUS(wParam, lParam, fn) \
    ((fn)((HWND)(wParam)), 0L)

#define PROC_WM_KILLFOCUS(wParam, lParam, fn) \
    ((fn)((HWND)(wParam)), 0L)

#define PROC_WM_SETCURSOR(wParam, lParam, fn) \
    (LRESULT)(DWORD)(BOOL)(fn)((HWND)(wParam), (UINT)LOWORD(lParam), (UINT)HIWORD(lParam))

/* BOOL Cls_OnNCCreate(HWND hwnd, LPCREATESTRUCT lpCreateStruct) */
#define PROC_WM_NCCREATE(wParam, lParam, fn) \
    (LRESULT)(DWORD)(BOOL)(fn)((LPCREATESTRUCT)(lParam))

/* void Cls_OnNCDestroy(HWND hwnd) */
#define PROC_WM_NCDESTROY(wParam, lParam, fn) \
    ((fn)(), 0L)

/* void Cls_OnShowWindow(BOOL fShow, UINT status) */
#define PROC_WM_SHOWWINDOW(wParam, lParam, fn) \
    ((fn)((BOOL)(wParam), (UINT)(lParam)), 0L)

/* void OnMove(int x, int y) */
#define PROC_WM_MOVE(wParam, lParam, fn) \
    ((fn)((int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam)), 0L)

/* void OnSize(UINT state, int cx, int cy) */
#define PROC_WM_SIZE(wParam, lParam, fn) \
    ((fn)((UINT)(wParam), (int)(short)LOWORD(lParam), (int)(short)HIWORD(lParam)), 0L)

/* BOOL OnInitDialog(HWND hwndFocus, LPARAM lParam) */
#define PROCDLG_WM_INITDIALOG(wParam, lParam, fn) \
    (LRESULT)(DWORD)(UINT)(BOOL)(fn)((HWND)(wParam), lParam)

#define PROCDLG_WM_COMMAND(wParam, lParam, fn) \
    ((fn)((WPARAM)(wParam), (LPARAM)(lParam)), 0L)

#define PROCDLG_WM_CLOSE(wParam, lParam, fn) \
    ((fn)(), 0L)

#define PROC_WM_SETTINGCHANGE(wParam, lParam, fn) \
    ((fn)(), 0L)

/* void Cls_OnDrawItem(HWND hwnd, const DRAWITEMSTRUCT * lpDrawItem) */
#define PROC_WM_DRAWITEM(wParam, lParam, fn) \
    ((fn)((const DRAWITEMSTRUCT *)(lParam)), 0L)

/* HBRUSH OnCtrlColorScrollbar(HWND hwnd, HDC hdc, HWND hwndChild, int type) */
#define PROC_WM_CTLCOLORSCROLLBAR(wParam, lParam, fn) \
    (LRESULT)(UINT_PTR)(HBRUSH)(fn)((HDC)(wParam), (HWND)(lParam), CTLCOLOR_SCROLLBAR)
#define PROC_WM_CTLCOLORMSGBOX(wParam, lParam, fn) \
    (LRESULT)(UINT_PTR)(HBRUSH)(fn)((HDC)(wParam), (HWND)(lParam), CTLCOLOR_MSGBOX)
#define PROC_WM_CTLCOLOREDIT(wParam, lParam, fn) \
    (LRESULT)(UINT_PTR)(HBRUSH)(fn)((HDC)(wParam), (HWND)(lParam), CTLCOLOR_EDIT)
#define PROC_WM_CTLCOLORLISTBOX(wParam, lParam, fn) \
    (LRESULT)(UINT_PTR)(HBRUSH)(fn)((HDC)(wParam), (HWND)(lParam), CTLCOLOR_LISTBOX)
#define PROC_WM_CTLCOLORBTN(wParam, lParam, fn) \
    (LRESULT)(UINT_PTR)(HBRUSH)(fn)((HDC)(wParam), (HWND)(lParam), CTLCOLOR_BTN)
#define PROC_WM_CTLCOLORDLG(wParam, lParam, fn) \
    (LRESULT)(UINT_PTR)(HBRUSH)(fn)((HDC)(wParam), (HWND)(lParam), CTLCOLOR_DLG)
#define PROC_WM_CTLCOLORSTATIC(wParam, lParam, fn) \
    (LRESULT)(UINT_PTR)(HBRUSH)(fn)((HDC)(wParam), (HWND)(lParam), CTLCOLOR_STATIC)

/* void OnSysColorChange() */
#define PROC_WM_SYSCOLORCHANGE(wParam, lParam, fn) \
    ((fn)(), 0L)

// void OnChildAcrivate()
#define PROC_WM_CHILDACTIVATE(wParam, lParam, fn) \
    ((fn)(), 0L)


/* void OnInitMenu(HMENU hMenu) */
#define PROC_WM_INITMENU(wParam, lParam, fn) \
    ((fn)((HMENU)(wParam)), 0L)

/* void OnInitMenuPopup(HWND hwnd, HMENU hMenu, UINT item, BOOL fSystemMenu) */
#define PROC_WM_INITMENUPOPUP(wParam, lParam, fn) \
    ((fn)((HMENU)(wParam), (UINT)LOWORD(lParam), (BOOL)HIWORD(lParam)), 0L)

/* BOOL OnQueryEndSession() */
#define PROC_WM_QUERYENDSESSION(wParam, lParam, fn) \
    MAKELRESULT((BOOL)(fn)(), 0L)
/* void OnEndSession(BOOL fEnding) */
#define PROC_WM_ENDSESSION(wParam, lParam, fn) \
    ((fn)((BOOL)(wParam)), 0L)


//  used inside of OnCommand(UINT wParam, LPARAM lParam) method
//  wParam must be LOWORD filtered to get the command ID
#define ON_COMMAND(message, fn) \
    case (message): \
        return fn() ? TRUE : FALSE; break;

//  OnCommandNOTIFIED(int id, HWND hwndCtl, UINT codeNotify)
//  used inside of OnCommand(UINT wParam, LPARAM lParam) method
//  wParam must be LOWORD filtered to get the command ID
#define ON_COMMAND_NOTIFIED(message, fn) \
   case (message): return fn((int)(LOWORD(wParam)), (HWND)(lParam), (UINT)(HIWORD(wParam))) ? TRUE : FALSE; break;

// void OnMDIActivate(HWND Activating, HWND Deactivating);
#define PROC_WM_MDIACTIVATE(wParam, lParam, fn) \
    ((fn)((HWND)(lParam), (HWND)(wParam)), 0L)

/* BOOL Cls_OnQueryEndSession(HWND hwnd) */
#define PROC_WM_QUERYENDSESSION(wParam, lParam, fn) \
    MAKELRESULT((BOOL)(fn)(), 0L)


/*
/* void Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify) */
//#define HANDLE_WM_COMMAND(hwnd, wParam, lParam, fn) \
//    ((fn)((hwnd), (int)(LOWORD(wParam)), (HWND)(lParam), (UINT)HIWORD(wParam)), 0L)
//*/

#endif

