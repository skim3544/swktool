#pragma once

#include "MDIDemo.h"
#include "..\SWKUI\MDIChildWindow.h"
#include "..\SWKUI\WinMenu.h"
#include "..\SWKUI\GDIObject.h"
#include "..\SWKUI\Brush.h"
#include "..\SWKUI\DeviceContext.h"

#include "resource.h"

swktool::WinMenu GetHelloMenu();
swktool::WinMenu GetInitMenu();
swktool::WinMenu GetRectMenu();


typedef struct TagRECTDATA {
	short cxClient;
	short cyClient;
}
RECDATA, * PRECTDATA;


class RectWindow : public swktool::MDIChildWindow
{
protected:
	std::unique_ptr<RECDATA>  pRectData_;

public:
	RectWindow(LPCTSTR className, LPCTSTR Title, HINSTANCE hInst) :
		swktool::MDIChildWindow(className, Title, hInst)
	{
	}


	virtual BOOL OnCreate(LPCREATESTRUCT lpCreateStruct) override {
		swktool::MDIChildWindow::OnCreate(lpCreateStruct);
		pRectData_ = std::make_unique< RECDATA>();

		::SetTimer(GetWindowHWND(), 1, 250, NULL);

		return TRUE;
	}

	virtual void OnSize(UINT state, int nWidth, int nHeight) override {
		Width = nWidth;
		Height = nHeight;


		if (state != SIZE_MINIMIZED) {
			pRectData_->cxClient = nWidth;
			pRectData_->cyClient = nHeight;
		}
	}

	virtual void OnTimer(UINT TimerID) override {
		int xLeft = rand() % pRectData_->cxClient;
		int xRight = rand() % pRectData_->cxClient;
		int yTop = rand() % pRectData_->cyClient;
		int yBottom = rand() % pRectData_->cyClient;
		short nRed = rand() % 255;
		short nGreen = rand() % 255;
		short nBlue = rand() % 255;

		std::unique_ptr<swktool::DeviceContext> dc = swktool::DeviceContext::GetDC(this);
		swktool::Brush brush;
		brush.CreateSolid( RGB(nRed, nGreen, nBlue));

		dc->SelectObject(brush);

		dc->Rectangle(min(xLeft, xRight), min(yTop, yBottom), max(xLeft, xRight), max(yTop, yBottom));
	}

	virtual void OnDestroy() override {
		::KillTimer(GetWindowHWND(), 1);
	}


	virtual void OnMDIActivate(HWND hwndActivating, HWND Deactivating) override
	{
		swktool::WinMenu RectMenu = GetRectMenu();

		HMENU hMenuRectWindow = GetSubMenu((HMENU)RectMenu, 1);
		if (GetWindowHWND() == hwndActivating) {
			::SendMessage(hwndMDIClient_, WM_MDISETMENU, (WPARAM)(HMENU)RectMenu, (LPARAM)hMenuRectWindow);

			RectMenu.Detach();
		}
		else
		{
			swktool::WinMenu InitMenu = GetInitMenu();
			HMENU hMenuInitWindow = GetSubMenu((HMENU)InitMenu, 0);
			::SendMessage(hwndMDIClient_, WM_MDISETMENU, (WPARAM)(HMENU)InitMenu, (LPARAM)hMenuInitWindow);

			InitMenu.Detach();
		}

		::DrawMenuBar(hwndMDIFrame_);
	}

};


