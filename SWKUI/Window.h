#ifndef ___BASE_WINDOW_H___
#define ___BASE_WINDOW_H___

#include "targetver.h"
#include "MsgHandler.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>
#include <windowsx.h>
#include <memory>
#include "Drawing.h"
#include "msg_filter.h"


namespace swktool 
{


	LRESULT CALLBACK SWKWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	class Window :
		  public WindowHandlerBase
		, public IWindowsRegister		
	{		
	public:
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
			// Get the default reguster structure
			auto wc = WindowsDefaultRegister::CreateDefault(ClassName());

			// call PreRegister to allow client code to customize the setting
			PreRegisterWindow(wc);

			// register this window
			auto atom = RegisterClassEx(&wc);
			if (0 == atom) 
			{
				auto lastError = GetLastError();
			}

			auto handle = CreateWindowEx(
				dwExStyle, ClassName(), lpWindowName, dwStyle, x, y,
				nWidth, nHeight, hWndParent, hMenu, GetModuleHandle(NULL),
				this // passing this pointer here is what makes msg_routing work
			);

			if (nullptr == handle)
			{
				auto lastError = GetLastError();
				wchar_t buf[256]; 
				FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, lastError, 0, buf, 256, nullptr); 
				MessageBox(nullptr, buf, L"CreateWindowEx failed", MB_OK);
			}

			SetHwnd(handle);

			return (handle ? TRUE : FALSE);
		}

		void OnClose() override { DestroyWindow(GetHwnd()); }
		void OnDestroy() override { 	}


		//
		// Commands & Notifications*.*
		// Function must return TRUE when handled.  FALSE and allow DefWindowProc to handle
		LRESULT OnCommand(WORD id, WORD code, HWND control) override { return 0; }
		LRESULT OnNotify(int idCtrl, NMHDR* hdr) override { return 0; }

		//
		// Focus
		//
		LRESULT OnSetFocus(HWND oldFocus) override { return 0; }
		LRESULT OnKillFocus(HWND newFocus) override { return 0; }

		//
		// Keyboard
		//
		LRESULT OnKeyDown(UINT vk, UINT flags) override { return 0; }
		LRESULT OnKeyUp(UINT vk, UINT flags) override { return 0; }
		LRESULT OnChar(UINT ch, UINT flags) override { return 0; }

		//
		// Mouse
		//
		LRESULT OnMouseMove(UINT keys, int x, int y) override { return 0; }
		LRESULT OnLButtonDown(UINT keys, int x, int y) override { return 0; }
		LRESULT OnLButtonUp(UINT keys, int x, int y) override { return 0; }
		LRESULT OnRButtonDown(UINT keys, int x, int y) override { return 0; }
		LRESULT OnRButtonUp(UINT keys, int x, int y) override { return 0; }

		LRESULT OnPaint() override
		{
			PaintDeviceContext PC(GetHwnd());

			return 0;
		}


		LRESULT OnEraseBkgnd(HDC hdc) override  {
			RECT rc;
			GetClientRect(GetHwnd(), &rc);
			FillRect(hdc, &rc, (HBRUSH)(COLOR_WINDOW + 1));
			return 1; // tell Windows we erased the background
		}

		//
		// Sizing / Moving
		//
		LRESULT OnSize(UINT type, int cx, int cy) override { return 0; }
		LRESULT OnMove(int x, int y) override { return 0; }

		virtual LRESULT OnNcCalcSize(BOOL calcValidRects, NCCALCSIZE_PARAMS* params)
		{
			return DefWindowProc(GetHwnd(), WM_NCCALCSIZE, calcValidRects, (LPARAM)params);
		}
		virtual LRESULT OnNcPaint(HRGN hrgn)
		{
			return DefWindowProc(GetHwnd(), WM_NCPAINT, (WPARAM)hrgn, 0);
		}
		virtual LRESULT OnNcHitTest(POINT pt)
		{
			return DefWindowProc(GetHwnd(), WM_NCHITTEST, 0, MAKELPARAM(pt.x, pt.y));
		}

		//
		// Fallback
		//
		LRESULT OnMessage(UINT msg, WPARAM wParam, LPARAM lParam) override
		{
			return WindowHandlerBase::OnMessage(msg, wParam, lParam);
		}

		void ApplyTheme(const Theme& theme)
		{
			currentTheme_ = theme;

			// Repaint window background
			InvalidateRect(GetHwnd(), nullptr, TRUE);

			// Propagate to all controls
			binder_->PropagateTheme(theme);
		}

		void Show(int nCmdShow) 
		{ 
			::ShowWindow(GetHwnd(), nCmdShow); 
			::UpdateWindow(GetHwnd()); 
		}

		// To register MDI child class
		bool Register() 
		{ 
			WNDCLASSEX wc = {}; 
			PreRegisterWindow(wc); 
			return RegisterClassEx(&wc); 
		}

		bool Subclass(HWND hwnd)
		{
			//DebugBreak();

			if (!hwnd)
				return false;
			
			SetHwnd(hwnd);

			WindowHandlerRegistry::Instance().RegisterHandler(hwnd, this);

			// Store 'this' so SWKUI can route messages
			SetWindowLongPtr(GetHwnd(), GWLP_USERDATA, (LONG_PTR)this);

			// Replace the window procedure with SWKUI's
			originalWndProc_ = (WNDPROC)SetWindowLongPtr(
				GetHwnd(),
				GWLP_WNDPROC,
				(LONG_PTR)&SWKWindowProc
			);

			return true;
		}

		WNDPROC GetOriginalWndProc() const override { return originalWndProc_; }

		protected:

		WNDPROC originalWndProc_ = nullptr;
	};

	class CaptionFadeWindow : public swktool::Window
	{
	public:
		static constexpr int CAPTION_HEIGHT = 32;

		PCWSTR ClassName() const override { return L"CaptionFadeWindow"; }

		//
		// Remove default Windows caption & frame
		//
		LRESULT OnNcCalcSize(BOOL calcValidRects, NCCALCSIZE_PARAMS* params) override
		{
			// Shrink client area upward by caption height
			params->rgrc[0].top += CAPTION_HEIGHT;
			return 0;
		}

		//
		// Draw custom gradient caption bar
		//
		LRESULT OnNcPaint(HRGN hrgn) override
		{
			HDC hdc = GetWindowDC(GetHwnd());

			RECT rc;
			GetWindowRect(GetHwnd(), &rc);
			OffsetRect(&rc, -rc.left, -rc.top);

			RECT caption = { 0, 0, rc.right, CAPTION_HEIGHT };

			TRIVERTEX vert[2] = {};
			vert[0].x = 0;
			vert[0].y = 0;
			vert[0].Red = 0x0000;   // Blue
			vert[0].Green = 0x0000;
			vert[0].Blue = 0xFFFF;

			vert[1].x = rc.right;
			vert[1].y = CAPTION_HEIGHT;
			vert[1].Red = 0xFFFF;   // White
			vert[1].Green = 0xFFFF;
			vert[1].Blue = 0xFFFF;

			GRADIENT_RECT gRect = { 0, 1 };
			GradientFill(hdc, vert, 2, &gRect, 1, GRADIENT_FILL_RECT_H);

			// Draw window title text
			SetBkMode(hdc, TRANSPARENT);
			SetTextColor(hdc, RGB(0, 0, 0));

			HFONT hFont = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
			HFONT old = (HFONT)SelectObject(hdc, hFont);

			RECT textRc = { 10, 7, rc.right, CAPTION_HEIGHT };
			DrawText(hdc, L"Caption Fade Window", -1, &textRc,
				DT_LEFT | DT_VCENTER | DT_SINGLELINE);

			SelectObject(hdc, old);
			ReleaseDC(GetHwnd(), hdc);
			return 0;
		}

		//
		// Make the custom caption draggable
		//
		LRESULT OnNcHitTest(POINT pt) override
		{
			ScreenToClient(GetHwnd(), &pt);
			if (pt.y < CAPTION_HEIGHT)
				return HTCAPTION;
			return HTCLIENT;
		}
	};

}


#endif //___BASE_WINDOW_H___