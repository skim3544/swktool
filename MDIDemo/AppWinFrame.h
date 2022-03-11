#pragma once

#include "MDIDemo.h"
#include "../SWKUI/pch.h"
#include "../SWKUI/WinFrame.h"
#include "../SWKBase/Logger.h"

class AppWindowFrame : public swktool::WindowFrame {	

public:
	AppWindowFrame() : WindowFrame()
	{
		pLogger_ = GetDI().Resolve<swktool::ILogger, swktool::Logger>();		
		pLogger_->Register(swktool::LogLevel::DETAIL1, __FUNCTION__);
	}


	AppWindowFrame(HINSTANCE hInst) : 
		swktool::WindowFrame(hInst)		
	{
		pLogger_ = GetDI().Resolve<swktool::ILogger, swktool::Logger>();
		pLogger_->Register(swktool::LogLevel::DETAIL1, __FUNCTION__);
	}
	~AppWindowFrame() { ; }

	std::shared_ptr<swktool::Window> CreateWin(LPCWSTR WinClassName, LPCWSTR WinTitle, HMENU hMDIMenu,  int nCmdShow);
	//std::shared_ptr<swktool::Window> CreateMFDIWin(LPCWSTR WinClassName, LPCWSTR WinTitle, int nCmdShow);

private:
	swktool::ILogger* pLogger_;
};

