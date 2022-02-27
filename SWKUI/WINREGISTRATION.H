#pragma once

#include <Windows.h>
#include <memory>
#include "WinEventHandler.h"
#include "..\SWKBase\Logger.h"

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#else
#define DBG_NEW new
#endif


namespace swktool {

	class WindowReg {
		WNDCLASSEXW mCLS = { };

	public:
		WindowReg(const WNDCLASSEXW& O) : mCLS(O)
		{
		}

		WindowReg(const WindowReg& O)
		{
			mCLS = O.mCLS;
		}

		WindowReg() {
			mCLS.cbSize = sizeof(WNDCLASSEX);
			mCLS.style = CS_HREDRAW | CS_VREDRAW;
			//mCLS.lpfnWndProc = WndProc;
			mCLS.cbClsExtra = 0;
			mCLS.cbWndExtra = 0;
			//mCLS.hInstance = hInstance;
			//mCLS.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINAPP));
			mCLS.hCursor = LoadCursor(nullptr, IDC_ARROW);
			mCLS.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
			//mCLS.lpszMenuName = MAKEINTRESOURCEW(IDC_WINAPP);			
			mCLS.lpszClassName = L"WINAPP";
			//mCLS.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));		
		}
		~WindowReg() { ; }

		WNDCLASSEX& GetWndClass() {
			return mCLS;
		}

		LPCWSTR  GetWinClassName() const
		{
			return mCLS.lpszClassName;
		}

		void SetWinClassName(LPCTSTR Name)
		{
			mCLS.lpszClassName = Name;			
		}

		static WNDCLASSEX  CreateWinClsTemplate(HINSTANCE hInstance)
		{
			WNDCLASSEX wcex = { };

			wcex.cbSize = sizeof(WNDCLASSEX);
			wcex.style = CS_HREDRAW | CS_VREDRAW;
			wcex.lpfnWndProc = (WNDPROC)swktool::WinEventHandler::ProcWindowMessage;
			wcex.cbClsExtra = 0;
			wcex.cbWndExtra = 0;
			wcex.hInstance = hInstance;			
			wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);			
			wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);

			// Likely these fields needs customization
			//wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WINAPP);
			//wcex.lpszClassName = szWindowClass;
			//wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_AppIcon));
			//wcex.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SmallIcon));

			return wcex;
		}

		static WNDCLASSEX  CreateMDIFrameTemplate(HINSTANCE hInstance)
		{
			WNDCLASSEX wcex = { };

			wcex.cbSize = sizeof(WNDCLASSEX);
			wcex.style = CS_HREDRAW | CS_VREDRAW;
			wcex.lpfnWndProc = (WNDPROC)swktool::WinEventHandler::ProcMDIFrameMessage;
			wcex.cbClsExtra = 0;
			wcex.cbWndExtra = 0;
			wcex.hInstance = hInstance;
			wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
			wcex.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);

			// Likely these fields needs customization
			//wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WINAPP);
			//wcex.lpszClassName = szWindowClass;
			//wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_AppIcon));
			//wcex.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SmallIcon));

			return wcex;
		}

		static WNDCLASSEXW  CreateMDIChildTemplate(HINSTANCE hInstance)
		{
			WNDCLASSEXW wcex = { };

			wcex.cbSize = sizeof(WNDCLASSEX);
			wcex.style = CS_HREDRAW | CS_VREDRAW;
			wcex.lpfnWndProc = (WNDPROC)swktool::WinEventHandler::ProcMDIChildMessage;
			wcex.cbClsExtra = 0;
			wcex.cbWndExtra = sizeof(HANDLE);
			wcex.hInstance = hInstance;
			wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
			wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
			wcex.lpszMenuName = NULL;
			// Likely these fields needs customization			
			//wcex.lpszClassName = szWindowClass;
			//wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_AppIcon));
			//wcex.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SmallIcon));

			return wcex;
		}
	};



}