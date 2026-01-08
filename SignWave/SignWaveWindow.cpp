#include "SignWaveWindow.h"
#include "../SWKUI/DeviceContext.h"

#define NUM 1000
#define TWOPI (2 * 3.14159)

LRESULT MainWindow::OnSize(UINT type, int cx, int cy) 
{
	cxClient_ = cx;
	cyClient_ = cy;

	return 0;
}

LRESULT MainWindow::OnPaint() 
{
	// The original code had missing EndPaint, but we cannot make mistake since we use RAII
	auto hwnd = GetHwnd();

	swktool::PaintDeviceContext PC(hwnd);
	
	// Prevent thrown exception from getting out of OnPaint without cleaning up the PaintContext
	try 
	{
		POINT apt[NUM] = {0};
	
		PC.MoveToEx(0, cyClient_ / 2, NULL);
		PC.LineTo(cxClient_, cyClient_ / 2);
	
		for (int i = 0; i < NUM; i++) {
			apt[i].x = i * cxClient_ / NUM;
			apt[i].y = (int)(cyClient_ / 2 * (1 - sin(TWOPI * i / NUM)));
		}
	
		PC.Polyline(apt, NUM);
	}
	catch (...) 
	{
	
	}

	return 0;
}

