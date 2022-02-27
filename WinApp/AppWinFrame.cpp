#include "..\SWKUI\pch.h"
#include "SimpleWindow.h"
#include "AppWinFrame.h"

using namespace swktool;

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#else
#define DBG_NEW new
#endif


std::shared_ptr<swktool::Window> AppWindowFrame::CreateWin(LPCWSTR WinClassName, LPCWSTR WinTitle, int nCmdShow)
{

	return TCreateWin<SimpleWindow>(WinClassName, WinTitle, nCmdShow);
}


