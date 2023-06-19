#ifndef ___BASE_WINDOW_H___
#define ___BASE_WINDOW_H___

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
#include <windowsx.h>
#include <memory>
#include "Drawing.h"
#include "msg_filter.h"
//#include "AWindow.h"

//#include "MDIFrameWindow.h"

#include "MsgHandler.h"

namespace swktool {
	class Window : 
		 public WindowsMsgHandler
		,public WindowsRegister		
	{		
	public:

		virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

		// called right before register to be able to change the registration information
		virtual void   PreRegisterWindow(WindowRegisterClass& wc) { ; }



		BOOL Create(
			PCWSTR lpWindowName,
			DWORD dwStyle = WS_OVERLAPPEDWINDOW,
			DWORD dwExStyle = 0,
			int x = CW_USEDEFAULT,
			int y = CW_USEDEFAULT,
			int nWidth = CW_USEDEFAULT,
			int nHeight = CW_USEDEFAULT,
			HWND hWndParent = 0,
			HMENU hMenu = 0
		)
		{
			WindowRegisterClass wc = { 0 };
			wc.cbSize = sizeof(WindowRegisterClass);
			wc.lpfnWndProc = WindowProc;
			wc.hInstance = GetModuleHandle(NULL);
			wc.lpszClassName = ClassName();

			PreRegisterWindow(wc);

			RegisterClassEx(&wc);

			m_hwnd = CreateWindowEx(
				dwExStyle, ClassName(), lpWindowName, dwStyle, x, y,
				nWidth, nHeight, hWndParent, hMenu, GetModuleHandle(NULL),
				this // passing this pointer here is what makes msg_routing work
			);

			return (m_hwnd ? TRUE : FALSE);
		}

		virtual BOOL OnCreate(LPCREATESTRUCT lpCreateStruct) {
			return TRUE;
		}

		virtual void OnPaint()
		{
			HWND hWindow = WindowHandle();
			//PaintDeviceContext PC(hWindow);
		}

		virtual void OnClose() { 
			//_asm { int 3 };
			PostQuitMessage(0);
			
		}
		virtual void OnShowWindow(BOOL fShow, UINT status) { ; }
		virtual void OnKeyDown(UINT vk, int cRepeat, UINT flags) { ; }
		virtual void OnKeyUp(UINT vk, int cRepeat, UINT flags) { ; }

		virtual void OnMouseLButtonDown(int x, int y, UINT keyFlags) { ; }
		virtual void OnMouseLButtonUp(int x, int y, UINT keyFlags) { ; }
		virtual void OnMouseLButtonDblClick(int x, int y, UINT keyFlags) { ; }

		virtual void OnMouseRButtonDown(int x, int y, UINT keyFlags) { ; }
		virtual void OnMouseRButtonUp(int x, int y, UINT keyFlags) { ; }
		virtual void OnMouseRButtonDblClick(int x, int y, UINT keyFlags) { ; }

		virtual void OnMouseMButtonDown(int x, int y, UINT keyFlags) { ; }
		virtual void OnMouseMButtonUp(int x, int y, UINT keyFlags) { ; }
		virtual void OnMouseMButtonDblClick(int x, int y, UINT keyFlags) { ; }

		virtual void OnMouseWheel(int xPos, int yPos, int zDelta, UINT fwKeys) { ; }
		virtual void OnMouseMove(int x, int y, UINT keyFlags) { ; }

		virtual void OnVScroll(HWND hwndCtl, UINT code, int pos) { ; }
		virtual void OnHScroll(HWND hwndCtl, UINT code, int pos) { ; }

		virtual void OnTimer(UINT TimerID) { ; }

		virtual void OnMove(int x, int y) { ; }
		virtual void OnSize(UINT state, int nWidth, int nHeight) {
			//	Width = nWidth; 
			//	Height = nHeight;
		}

		virtual void OnDestroy() {
			PostQuitMessage(0);
		}
		virtual void OnNCDestroy() { ; }
		virtual void OnQuit(int exitCode) { ; }

		virtual void OnSetFocus(HWND hWnd) { ; }
		virtual void OnKillFocus(HWND hWnd) { ; }

		virtual void OnClipboardCopy() { ; }
		virtual void OnClipboardCut() { ; }
		virtual void OnClipboardPaste() { ; }
		virtual void OnClipboardClear() { ; }
		virtual void OnClipboardUndo() { ; }
		virtual BOOL OnSetCursor(HWND hWnd, WPARAM wParam, LPARAM lParam) {
			return FALSE;
		}

		virtual LRESULT OnCommand(WPARAM wParam, LPARAM lParam) {
			return 0L; 
		}

		virtual void OnSettingChange() { ; }
		virtual void OnDrawItem(const DRAWITEMSTRUCT* p) { ; }
		virtual HBRUSH OnCtrlColorScrollbar(HDC hdc, HWND hwndChild, int type) { return (HBRUSH)(GetStockBrush(WHITE_BRUSH)); }
		virtual HBRUSH OnCtrlColorMsgBox(HDC hdc, HWND hwndChild, int type) { return (HBRUSH)(GetStockBrush(WHITE_BRUSH)); }
		virtual HBRUSH OnCtrlColorEdit(HDC hdc, HWND hwndChild, int type) { return (HBRUSH)(GetStockBrush(WHITE_BRUSH)); }
		virtual HBRUSH OnCtrlColorListBox(HDC hdc, HWND hwndChild, int type) { return (HBRUSH)(GetStockBrush(WHITE_BRUSH)); }
		virtual HBRUSH OnCtrlColorBtn(HDC hdc, HWND hwndChild, int type) { return (HBRUSH)(GetStockBrush(WHITE_BRUSH)); }
		virtual HBRUSH OnCtrlColorDlg(HDC hdc, HWND hwndChild, int type) { return (HBRUSH)(GetStockBrush(WHITE_BRUSH)); }
		virtual HBRUSH OnCtrlColorStatic(HDC hdc, HWND hwndChild, int type) { return (HBRUSH)(GetStockBrush(WHITE_BRUSH)); }

		virtual void OnSysColorChange() { ; }
		virtual void OnInitMenu(HMENU hMenu) { ; }
		virtual void OnInitMenuPopup(HMENU hMenu, UINT item, BOOL fSystemMenu) { ; }
		virtual BOOL OnQueryEndSession() {
			return TRUE;
		}
		virtual void OnEndSession(BOOL fEnding) { ; }
	};
}


//namespace swktool {
//
//#define MAX_LOADSTRING 100
//
//	class MDIFrameWindow;
//	class MDIChildWindow;
//
//
//	class Window : public AWindow {
//	protected:
//		WCHAR		szWindowClass[MAX_LOADSTRING];            // the main window class name
//		WCHAR		szTitle[MAX_LOADSTRING];                  // The title bar text
//		DWORD		Style = WS_OVERLAPPEDWINDOW;
//
//		int			x = CW_USEDEFAULT;
//		int			y = CW_USEDEFAULT;
//		int			Width = CW_USEDEFAULT;
//		int			Height = CW_USEDEFAULT;
//
//		HWND		hWndParent = 0;
//		HMENU		hMenu = 0;
//		//HINSTANCE	hInst = 0;
//		//HWND		hWnd = 0;
//		LPVOID		lParam = nullptr;
//		int			nCmdShow = SW_SHOWDEFAULT;
//
//	public:
//		Window(HINSTANCE hInstance, HWND WinHandle)
//		{
//			hInst = hInst;
//			hwndWindow = WinHandle;
//			szWindowClass[0] = 0;
//			szTitle[0] = 0;
//		}
//		Window() : AWindow()
//		{
//			szWindowClass[0] = 0;
//			szTitle[0] = 0;
//		}
//
//		Window(LPCWSTR lpClassName, LPCWSTR lpWindowName, HINSTANCE hInstance)
//		{
//			wcscpy_s(szWindowClass, lpClassName);
//			wcscpy_s(szTitle, lpWindowName);
//			hInst = hInstance;
//		}
//
//		Window(
//			LPCWSTR lpClassName,
//			LPCWSTR lpWindowName,
//			DWORD dwStyle = WS_OVERLAPPEDWINDOW,
//			int nX = CW_USEDEFAULT,
//			int nY = CW_USEDEFAULT,
//			int nWidth = CW_USEDEFAULT,
//			int nHeight = CW_USEDEFAULT,
//			HWND hParentWindowHandle = 0,
//			HMENU MenuHandle = 0,
//			HINSTANCE hInstance = 0,
//			LPVOID lpParam = nullptr) :
//			Style(dwStyle),
//			x(nX),
//			y(nY),
//			Width(nWidth),
//			Height(nHeight),
//			hMenu(MenuHandle),
//			hWndParent(hParentWindowHandle),
//			lParam(lpParam)
//		{
//			hInst = hInstance;
//			wcscpy_s(szWindowClass, lpClassName);
//			wcscpy_s(szTitle, lpWindowName);
//		}
//
//		virtual ~Window() { ; }
//
//		BOOL IsWindowMinimized() const {
//			return ::IsIconic(hwndWindow);
//		}
//
//		BOOL IsWindowMaximized() const {
//			return IsZoomed(hwndWindow);
//		}
//		BOOL IsWindowRestored() const {
//			return ((((DWORD)GetWindowLong(hwndWindow, GWL_STYLE)) & (WS_MINIMIZE | WS_MAXIMIZE)) == 0L);
//		}
//		LPCTSTR GetWindowClassName() const {
//			return szWindowClass;
//		}
//		void SetWindowClassName(LPCWSTR lpClassName)
//		{
//			wcscpy_s(szWindowClass, lpClassName);
//		}
//
//		void SetWindowTitle(LPCWSTR lpWindowName)
//		{
//			wcscpy_s(szTitle, lpWindowName);
//		}
//
//		LPCTSTR GetWindowTitle() const {
//			return szTitle;
//		}
//
//		// Create Window, must assign to shared_ptr as this pointer is delivered to the message pumping architecture		
//		virtual HWND Create(std::shared_ptr<Window>& pWin);
//		HWND CreateMDIClient(MDIFrameWindow* pFrameWin, std::shared_ptr<MDIChildWindow>& pWin);
//		void Show(int nCmdShow = 0) {
//			ShowWindow(hwndWindow, nCmdShow);
//			UpdateWindow(hwndWindow);
//		}
//
//		DWORD GetCreateStyle() const {return Style; }
//		void  SetCreateStyle(DWORD dwStyle) { Style  = dwStyle; }
//
//
//		virtual void SetWindowHandle(HWND hWindow)
//		{
//			hwndWindow = hWindow;
//		}
//
//
//		virtual HINSTANCE GetInstanceHandle() const {
//			return hInst;
//		}
//
//		virtual void OnPaint()
//		{
//			HWND hWindow = GetWindowHWND();
//			PaintDeviceContext PC(hWindow);
//		}
//		
//		virtual BOOL OnCreate(LPCREATESTRUCT lpCreateStruct) {			
//			return TRUE;
//		}
//
//		virtual LRESULT OnNCCreate(LPCREATESTRUCT lpCreateStruct) {
//			return 0l;
//		}
//
//		// Message handler, set Result to 1 if not handled, 0 if handled - same as Petzold
//		virtual LRESULT ProcessMessage(WORD msg, WPARAM wParam, LPARAM lParam);
//
//	protected:
//		virtual void OnClose() { ; }
//		virtual void OnShowWindow(BOOL fShow, UINT status) { ; }
//		virtual void OnKeyDown(UINT vk, int cRepeat, UINT flags) { ; }
//		virtual void OnKeyUp(UINT vk, int cRepeat, UINT flags) { ; }
//
//		virtual void OnMouseLButtonDown(int x, int y, UINT keyFlags) { ; }
//		virtual void OnMouseLButtonUp(int x, int y, UINT keyFlags) { ; }
//		virtual void OnMouseLButtonDblClick(int x, int y, UINT keyFlags) { ; }
//
//		virtual void OnMouseRButtonDown(int x, int y, UINT keyFlags) { ; }
//		virtual void OnMouseRButtonUp(int x, int y, UINT keyFlags) { ; }
//		virtual void OnMouseRButtonDblClick(int x, int y, UINT keyFlags) { ; }
//
//		virtual void OnMouseMButtonDown(int x, int y, UINT keyFlags) { ; }
//		virtual void OnMouseMButtonUp(int x, int y, UINT keyFlags) { ; }
//		virtual void OnMouseMButtonDblClick(int x, int y, UINT keyFlags) { ; }
//
//		virtual void OnMouseWheel(int xPos, int yPos, int zDelta, UINT fwKeys) { ; }
//		virtual void OnMouseMove(int x, int y, UINT keyFlags) { ; }
//
//		virtual void OnVScroll(HWND hwndCtl, UINT code, int pos) { ; }
//		virtual void OnHScroll(HWND hwndCtl, UINT code, int pos) { ; }
//
//		virtual void OnTimer(UINT TimerID) { ; }
//		
//		virtual void OnMove(int x, int y) { ; }
//		virtual void OnSize(UINT state, int nWidth, int nHeight) {
//			Width = nWidth; 
//			Height = nHeight;
//		}
//
//		virtual void OnDestroy() {
//			PostQuitMessage(0);
//		}
//		virtual void OnNCDestroy() { ; }
//		virtual void OnQuit(int exitCode) { ; }
//
//		virtual void OnSetFocus(HWND hWnd) { ; }
//		virtual void OnKillFocus(HWND hWnd) { ; }
//		
//		virtual void OnClipboardCopy() { ; }
//		virtual void OnClipboardCut() { ; }
//		virtual void OnClipboardPaste() { ; }
//		virtual void OnClipboardClear() { ; }
//		virtual void OnClipboardUndo() { ; }
//		virtual BOOL OnSetCursor(HWND hWnd, WPARAM wParam, LPARAM lParam) { 
//			return FALSE; 
//		}
//
//		virtual LRESULT OnCommand(WPARAM wParam, LPARAM lParam) { return 0L; }
//
//		virtual void OnSettingChange() { ; }
//		virtual void OnDrawItem(const DRAWITEMSTRUCT* p) { ;  }
//		virtual HBRUSH OnCtrlColorScrollbar(HDC hdc, HWND hwndChild, int type) { return (HBRUSH)(GetStockBrush(WHITE_BRUSH)); }
//		virtual HBRUSH OnCtrlColorMsgBox(HDC hdc, HWND hwndChild, int type) { return (HBRUSH)(GetStockBrush(WHITE_BRUSH)); }
//		virtual HBRUSH OnCtrlColorEdit(HDC hdc, HWND hwndChild, int type) { return (HBRUSH)(GetStockBrush(WHITE_BRUSH)); }
//		virtual HBRUSH OnCtrlColorListBox(HDC hdc, HWND hwndChild, int type) { return (HBRUSH)(GetStockBrush(WHITE_BRUSH)); }
//		virtual HBRUSH OnCtrlColorBtn(HDC hdc, HWND hwndChild, int type) { return (HBRUSH)(GetStockBrush(WHITE_BRUSH)); }
//		virtual HBRUSH OnCtrlColorDlg(HDC hdc, HWND hwndChild, int type) { return (HBRUSH)(GetStockBrush(WHITE_BRUSH)); }
//		virtual HBRUSH OnCtrlColorStatic(HDC hdc, HWND hwndChild, int type) { return (HBRUSH)(GetStockBrush(WHITE_BRUSH)); }
//
//		virtual void OnSysColorChange() { ; }
//		virtual void OnInitMenu(HMENU hMenu) { ; }
//		virtual void OnInitMenuPopup(HMENU hMenu, UINT item, BOOL fSystemMenu) { ;  }
//		virtual BOOL OnQueryEndSession() { 
//			return TRUE; 
//		}
//		virtual void OnEndSession(BOOL fEnding) { ;}
//};
//
//
//}
//
#endif //___BASE_WINDOW_H___