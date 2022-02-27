#include "AppWinFrame.h"
#include "TestMDIFrameWindow.h"
#include "..\SWKBase\Logger.h"


std::shared_ptr<swktool::Window> AppWindowFrame::CreateWin(LPCWSTR WinClassName, LPCWSTR WinTitle, HMENU hMDIMenu, int nCmdShow)
{	
	//pLogger_->Log(__FUNCTION__, 0, "Testing");

	std::shared_ptr<swktool::Window> pWin = std::make_shared<TestMDIFrameWindow>(WinClassName, WinTitle, hMDIMenu, hInstance);

	if (pWin->Create(pWin)) {
		pWin->Show(nCmdShow);
	}

	return pWin;
}

//std::shared_ptr<swktool::Window> AppWindowFrame::CreateMFDIWin(LPCWSTR WinClassName, LPCWSTR WinTitle, int nCmdShow) {
//
//}