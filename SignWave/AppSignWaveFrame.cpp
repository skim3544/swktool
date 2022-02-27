#include "..\SWKUI\pch.h"
#include "AppSignWaveFrame.h"
#include "SignWaveWindow.h"

using namespace swktool;

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#else
#define DBG_NEW new
#endif


std::shared_ptr<swktool::Window> AppSignWaveFrame::CreateWin(LPCWSTR WinClassName, LPCWSTR WinTitle, int nCmdShow)
{
	return TCreateWin<SignWaveWindow>(WinClassName, WinTitle, nCmdShow);
}


