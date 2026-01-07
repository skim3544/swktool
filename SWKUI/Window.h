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
		, public WindowsRegister
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
			WindowRegisterClass wc = { 0 };
			wc.cbSize = sizeof(WindowRegisterClass);
			wc.lpfnWndProc = SWKWindowProc;
			wc.hInstance = GetModuleHandle(NULL);
			wc.lpszClassName = ClassName();
			wc.hCursor = LoadCursor(NULL, IDC_ARROW);
			wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
			wc.hIconSm = wc.hIcon;

			PreRegisterWindow(wc);

			auto atom = RegisterClassEx(&wc);
			if (0 == atom) {
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
				wchar_t buf[256]; FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, lastError, 0, buf, 256, nullptr); MessageBox(nullptr, buf, L"CreateWindowEx failed", MB_OK);
			}

			SetHwnd(handle);

			return (handle ? TRUE : FALSE);
		}

		LRESULT OnCreate(CREATESTRUCT* lpCreateStruct) override {
			return 0;
		}
		void OnClose() override { DestroyWindow(GetHwnd()); }
		void OnDestroy() override {}


		//
		// Commands & Notifications
		//
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

		LRESULT OnEraseBkgnd(HDC hdc) override { return 1; }

		//
		// Sizing / Moving
		//
		LRESULT OnSize(UINT type, int cx, int cy) override { return 0; }
		LRESULT OnMove(int x, int y) override { return 0; }

		//
		// Fallback
		//
		LRESULT OnMessage(UINT msg, WPARAM wParam, LPARAM lParam) override
		{
			return DefWindowProc(GetHwnd(), msg, wParam, lParam);
		}
	};
}


#endif //___BASE_WINDOW_H___