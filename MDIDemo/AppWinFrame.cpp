#include "AppWinFrame.h"
#include "TestMDIFrameWindow.h"
#include "..\SWKBase\Logger.h"


std::shared_ptr<swktool::Window> AppWindowFrame::CreateWin(LPCWSTR WinClassName, LPCWSTR WinTitle, HMENU hMDIMenu, int nCmdShow)
{	
	
	swktool::Logfunction Guard(__FUNCTION__, *pLogger_, swktool::LogLevel::DETAIL1);

	std::shared_ptr<swktool::Window> pWin = std::make_shared<TestMDIFrameWindow>(WinClassName, WinTitle, hMDIMenu, hInstance);

	if (pWin->Create(pWin)) {
		pWin->Show(nCmdShow);
	}

	return pWin;
}
