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

//#include "MDIFrameWindow.h"

#include "MsgHandler.h"

namespace swktool 
{
	LRESULT CALLBACK SWKWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	class Window : 
		public WindowHandlerBase
		,public WindowsRegister 
	{		
	public:		
		// called right before register to be able to change the registration information
		virtual void   PreRegisterWindow(WindowRegisterClass& wc) { ; }
		LRESULT OnMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;

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
			wc.lpfnWndProc = SWKWindowProc;
			wc.hInstance = GetModuleHandle(NULL);
			wc.lpszClassName = ClassName();

			PreRegisterWindow(wc);

			RegisterClassEx(&wc);

			auto handle = CreateWindowEx(
				dwExStyle, ClassName(), lpWindowName, dwStyle, x, y,
				nWidth, nHeight, hWndParent, hMenu, GetModuleHandle(NULL),
				this // passing this pointer here is what makes msg_routing work
			);

			SetHwnd(handle);

			return (handle ? TRUE : FALSE);
		}

		virtual BOOL OnCreate(LPCREATESTRUCT lpCreateStruct) {
			return TRUE;
		}

		virtual void OnPaint()
		{
			//HWND hWindow = WindowHandle();
			PaintDeviceContext PC( GetHwnd() );
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

		virtual void OnSysCommand(UINT nID, LPARAM lParam) { 
			DefWindowProc(GetHwnd(), WM_SYSCOMMAND, (WPARAM)nID, lParam);
		}
	};
}


#endif //___BASE_WINDOW_H___