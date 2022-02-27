#pragma once

#include <list>
#include "..\SWKUI\Window.h"
#include "..\SWKUI\WinEventHandler.h"
#include "..\SWKUI\MDIFrameWindow.h"
#include "resource.h"
#include "..\SWKBase\Logger.h"

	class TestMDIFrameWindow : public swktool::MDIFrameWindow
	{
	protected:
		//HANDLE    MDIMenu_;

	public:
		TestMDIFrameWindow(LPCWSTR lpClassName, LPCWSTR lpWindowName, HMENU hMDIMenu, HINSTANCE hInstance) :
			swktool::MDIFrameWindow(lpClassName, lpWindowName, hInstance)
		{
			hMenu = hMDIMenu;
			//swktool::LevelLogger::Register((int)swktool::LogLevel::NONE, __FUNCTION__);
			swktool::LevelLogger::Register((int)swktool::LogLevel::DETAIL3, __FUNCTION__);
		}

		virtual BOOL OnCreate(LPCREATESTRUCT lpCreateStruct) override {
			HMENU hSubMenu = GetSubMenu(hMenu, 0);

			clientcreate_.hWindowMenu = hSubMenu;
			clientcreate_.idFirstChild = 50000;

			hwndClient = ::CreateWindow(TEXT("MDICLIENT"), NULL, 
				WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE,
				0, 0, 0, 0, GetWindowHWND(), (HMENU)1, hInst, &clientcreate_);

			std::ostringstream ss;
			ss << "HwndClient=" << std::setbase(16) << hwndClient;
			//swktool::ClassLogger::Inst().Log(__FUNCTION__, (int)swktool::LogLevel::DETAIL2, ss);

			return TRUE;
		}

		HWND GetMDIClientHWND() const {
			return hwndClient;
		}		


		virtual LRESULT OnCommand(WPARAM wParam, LPARAM lParam) override;

		BOOL OnNewHello();
		BOOL OnNewRect();
		BOOL OnFileClose();
		BOOL OnExit() {
			//PostMessage(hwndWindow, WM_CLOSE, 0, 0);
			//DestroyWindow(hwndWindow);
			PostQuitMessage(0);
			return FALSE;
		}

		virtual void OnClose() override;
		//LRESULT ProcessMessage(WORD msg, WPARAM wParam, LPARAM lParam) override;
	};

