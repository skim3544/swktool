#pragma once


#include "../SWKUI/pch.h"


class SignWaveWindow : public swktool::Window
{
public:
	SignWaveWindow(LPCWSTR lpClassName, LPCWSTR lpWindowName, HINSTANCE hInstance) :
		swktool::Window(lpClassName, lpWindowName, hInstance),
		cxClient_(0),
		cyClient_(0)
	{

	}

	~SignWaveWindow() { ; }

	virtual void OnClose() {		
		PostQuitMessage(0);	// send Quit-message so the window closes
	}


	virtual void OnSize(UINT state, int nWidth, int nHeight) override; 
	virtual void OnPaint() override;

private:
	int	cxClient_;
	int cyClient_;
};

