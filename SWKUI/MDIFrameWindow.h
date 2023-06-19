//#pragma once
//
//#include <list>
//#include "Window.h"
//#include "WinEventHandler.h"
//
//
//namespace swktool {
//
//	class MDIFrameWindow : public Window
//	{
//		typedef std::list < std::shared_ptr<AWindow>> MDIClientList;
//
//
//	protected:
//		HWND hwndClient;
//		CLIENTCREATESTRUCT clientcreate_;
//		MDIClientList  mdiClientList_;
//
//	public:
//		MDIFrameWindow(LPCWSTR lpClassName, LPCWSTR lpWindowName, HINSTANCE hInstance) :
//			swktool::Window(lpClassName, lpWindowName, hInstance), clientcreate_{}, hwndClient(nullptr)
//		{
//			Style = WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN;
//		}
//
//		HWND Create(std::shared_ptr<Window>& pWin) override
//		{
//			// Downcast the pointer to MDIFrameWindow
//			std::shared_ptr<MDIFrameWindow> pFrameWin =
//				std::dynamic_pointer_cast<MDIFrameWindow> (pWin);
//
//			if (pFrameWin != nullptr) {
//				// Preregister the window
//				WinEventHandler::GetInst()->PreRegisterMDIFramedWindow(pFrameWin);
//
//				// Create window, this will start message pumping into the message Processer
//				hwndWindow = ::CreateWindow(szWindowClass, szTitle, Style,
//					x, y, Width, Height, hWndParent, hMenu, hInst, NULL);
//
//
//				return hwndWindow;
//			}
//			else {
//				return nullptr;
//			}		
//		}
//
//		virtual BOOL OnCreate(LPCREATESTRUCT lpCreateStruct) override {
//			clientcreate_.hWindowMenu = hMenu;
//			clientcreate_.idFirstChild = 50000;
//
//			// On Create, create MDIClient window
//			hwndClient = ::CreateWindow(
//				TEXT("MDICLIENT"), 
//				NULL, 
//				WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE,
//				0, 0, 0, 0, 
//				GetWindowHWND(), 
//				(HMENU)NULL, 
//				hInst, 
//				(void*)&clientcreate_);
//
//			return TRUE;
//		}
//
//		HWND GetMDIClientHWND() const {
//			return hwndClient;
//		}
//
//		virtual void OnSize(UINT state, int nWidth, int nHeight) override {
//			Width = nWidth;
//			Height = nHeight;
//
//			RECT rcClient;
//			::GetClientRect(this->GetWindowHWND(), &rcClient);
//			::MoveWindow(hwndClient, 0, 0, rcClient.right, rcClient.bottom, TRUE);
//		}
//
//
//		BOOL OnExit() {
//
//			return TRUE;
//		}
//
//		BOOL OnWindowTile() {
//			::SendMessage(hwndClient, WM_MDITILE, 0, 0);
//			return TRUE;
//		}
//
//		BOOL OnWindowCascade() {
//			::SendMessage(hwndClient, WM_MDICASCADE, 0, 0);
//			return TRUE;
//		}
//
//		BOOL OnWindowArrange()
//		{
//			::SendMessage(hwndClient, WM_MDIICONARRANGE, 0, 0);
//			return TRUE;
//		}
//
//		BOOL OnCloseAll() {
//			EnumChildWindows(hwndClient, CloseAllEnumProc, 0);
//			return TRUE;
//		}
//
//		static BOOL CALLBACK CloseAllEnumProc(HWND hWnd, LPARAM lParam);
//		LRESULT ProcessMessage(WORD msg, WPARAM wParam, LPARAM lParam) override;
//	};
//
//}
