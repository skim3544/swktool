#include "pch.h"
#include "framework.h"
#include "WinEventHandler.h"
#include "WinFrame.h"

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#else
#define DBG_NEW new
#endif

namespace swktool {
	std::shared_ptr<swktool::Window> WindowFrame::CreateWin(LPCWSTR WinClassName, LPCWSTR WinTitle, int nCmdShow)
	{
		std::shared_ptr<Window> pWin = std::make_shared<Window>(WinClassName, WinTitle, hInstance);
		if (pWin->Create(pWin)) {
			pWin->Show(nCmdShow);
		}

		return pWin;
	}


	std::shared_ptr<swktool::Window> WindowFrame::CreateWin(LPCWSTR WinTitle, int nCmdShow)
	{
		std::shared_ptr<swktool::Window> pWin = nullptr;

		// if only one Window registered, then just pull the information from the first entity of the list
		if (WinRegList.size() == 1) {
			WindowReg RegData = WinRegList[0];
			pWin = std::make_shared<Window>(RegData.GetWinClassName(), WinTitle, hInstance);

			if (pWin->Create(pWin)) {
				pWin->Show(nCmdShow);
			}
		}

		return pWin;
	}


	int WindowFrame::DoMessageLoop()
	{
		MSG msg;

		// Main message loop:
		while (GetMessage(&msg, nullptr, 0, 0))
		{
			if (!swktool::WinEventHandler::IsDialogMessage(&msg)) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		WinEventHandler::Destroy();

		return (int)msg.wParam;
	}

	int WindowFrame::DoMDIMessageLoop(const std::shared_ptr<Window>& MDIFrame)
	{
		MSG msg;

		HWND hwndClient = GetWindow(MDIFrame->GetWindowHWND(), GW_CHILD);

		// Main message loop:
		while (GetMessage(&msg, nullptr, 0, 0))
		{
			bool bProcess = !swktool::WinEventHandler::IsDialogMessage(&msg) &&
				!::TranslateMDISysAccel(hwndClient, &msg);

			if (bProcess) {
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}

		WinEventHandler::Destroy();

		return (int)msg.wParam;
	}	
	int WindowFrame::DoMessageLoop(swktool::WinAccelerator& accelerator)
	{
		MSG msg;

		// Main message loop:
		while (GetMessage(&msg, nullptr, 0, 0))
		{
			if (!swktool::WinEventHandler::IsDialogMessage(&msg)) {
				if (!accelerator.Translate(&msg))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
		}

		WinEventHandler::Destroy();

		return (int)msg.wParam;
	}
}