#pragma once

#include "../SWKUI/pch.h"
#include "../SWKUI/WinFrame.h"

class AppWindowFrame : public swktool::WindowFrame {

public:
	AppWindowFrame() : WindowFrame()
	{
		;
	}
	AppWindowFrame(HINSTANCE hInst) : swktool::WindowFrame(hInst)		
	{
		;
	}
	~AppWindowFrame() { ; }

	std::shared_ptr<swktool::Window> CreateWin(LPCWSTR WinClassName, LPCWSTR WinTitle, int nCmdShow);	
};


