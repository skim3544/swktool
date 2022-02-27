#include "..\SWKUI\\MDIFrameWindow.h"
#include "TestMDIFrameWindow.h"
#include "..\SWKUI\MDIChildWindow.h"
#include "HelloWindow.h"
#include "RectWindow.h"
#include "..\SWKBase\Logger.h"


BOOL TestMDIFrameWindow::OnNewHello() {

	std::shared_ptr<swktool::MDIChildWindow> pWin =
		std::make_shared<HelloWindow>(TEXT("MDIHelloChild"), TEXT("Hello"), hInst);

	HWND hChild = CreateMDIClient(this, pWin);

	std::ostringstream ss;
	ss << "HwndcHILD=" << std::setbase(16) << hChild;
	swktool::ClassLogger::Inst().Log(__FUNCTION__, (int)swktool::LogLevel::DETAIL2, ss);

	return TRUE;
}

BOOL TestMDIFrameWindow::OnNewRect() {
	std::shared_ptr<swktool::MDIChildWindow> pWin =
		std::make_shared<RectWindow>(TEXT("MDIHelloChild"), TEXT("Hello"), hInst);

	HWND hChild = CreateMDIClient(this, pWin);
	
	std::ostringstream ss;
	ss << "HwndcHILD=" << std::setbase(16) << hChild;
	swktool::ClassLogger::Inst().Log(__FUNCTION__, (int)swktool::LogLevel::DETAIL2, ss);


	return TRUE;
}

BOOL TestMDIFrameWindow::OnFileClose() {
	HWND hwndChild = (HWND)::SendMessage(GetMDIClientHWND(), WM_MDIGETACTIVE, 0, 0);
	if (SendMessage(hwndChild, WM_QUERYENDSESSION, 0, 0)) {
		SendMessage(GetMDIClientHWND(), WM_MDIDESTROY, (WPARAM)hwndChild, 0);		
	}

	std::ostringstream ss;
	ss << "HwndcHILD=" << std::setbase(16) << hwndChild;
	swktool::ClassLogger::Inst().Log(__FUNCTION__, (int)swktool::LogLevel::DETAIL2, ss);

	return TRUE;
}

LRESULT TestMDIFrameWindow::OnCommand(WPARAM wParam, LPARAM lParam) {
	WPARAM  CmdID = (WPARAM)wParam;

	switch (CmdID)
	{
		ON_COMMAND(IDM_FILE_NEWHELLO, OnNewHello);
		ON_COMMAND(IDM_FILE_NEWRECT, OnNewRect);
		ON_COMMAND(IDM_FILE_CLOSE, OnFileClose);
		
		ON_COMMAND(IDM_WINDOW_TILE, OnWindowTile);
		ON_COMMAND(IDM_WINDOW_CASCADE, OnWindowCascade);
		ON_COMMAND(IDM_WINDOW_ARRANGEICONS, OnWindowArrange);
		ON_COMMAND(IDM_WINDOW_CLOSEALL, OnCloseAll);

		ON_COMMAND(IDM_EXIT, OnExit)

		default:
		{
			HWND hwndChild = (HWND)::SendMessage(hwndClient, WM_MDIGETACTIVE, 0, 0);
			if (IsWindow(hwndChild))
				SendMessage(hwndChild, WM_COMMAND, wParam, lParam);
			break;
		}
	}
	return FALSE;
}

void TestMDIFrameWindow::OnClose() {
	::SendMessage(GetWindowHWND(), WM_COMMAND, IDM_WINDOW_CLOSEALL, 0);

	PostQuitMessage(0);
}