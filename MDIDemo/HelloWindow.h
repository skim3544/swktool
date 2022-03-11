#pragma once

#include "MDIDemo.h"
#include "..\SWKUI\MDIChildWindow.h"
#include "..\SWKUI\WinMenu.h"
#include "resource.h"

swktool::WinMenu GetHelloMenu();
swktool::WinMenu GetInitMenu();
swktool::WinMenu GetRectMenu();

typedef struct tagHELLODATA {
	UINT iColor;
	COLORREF clrText;
} HELLODATA, * PHELLODATA;

class HelloWindow : public swktool::MDIChildWindow
{
protected:
	std::unique_ptr<HELLODATA>  pHelloData_;

	WPARAM		IDM_LAST_COLOR;
public:
	HelloWindow(LPCTSTR className, LPCTSTR Title, HINSTANCE hInst) :
		swktool::MDIChildWindow(className, Title, hInst), pHelloData_(nullptr)
	{
		pHelloData_ = std::make_unique<HELLODATA>();

		pHelloData_->iColor = IDM_COLOR_BLACK;
		pHelloData_->clrText = RGB(0, 0, 0);
	}


	virtual BOOL OnCreate(LPCREATESTRUCT lpCreateStruct) override {
		swktool::MDIChildWindow::OnCreate(lpCreateStruct);

		return TRUE;
	}

	virtual void OnPaint() override {
		swktool::PaintDeviceContext PDC(this);

		PDC.SetTextColor(pHelloData_->clrText);

		RECT rect;
		::GetClientRect(GetWindowHWND(), &rect);
	
		PDC.DrawTextW(TEXT("Hello World!"), &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	}

	virtual BOOL OnQueryEndSession() override {
		LRESULT result = ::MessageBox(GetWindowHWND(), TEXT("OK to close window?"), TEXT("Hello"), MB_ICONQUESTION | MB_OKCANCEL);
		if (result == IDOK) {
			return TRUE;
		}
		else {
			return FALSE;
		}

	}

	BOOL OnColor()
	{
		static COLORREF clrTextArray[] = {
			RGB(0,0,0),
			RGB(255,0,0),
			RGB(0,255,0),
			RGB(0, 0, 255),
			RGB(255,255,255)
		};

		swktool::WinMenu Menu = swktool::WinMenu::GetMenu(hwndMDIFrame_);
		Menu.CheckMenuItem(pHelloData_->iColor, swktool::CheckedType::Unchecked);
		pHelloData_->iColor = (UINT)IDM_LAST_COLOR;
		Menu.CheckMenuItem(pHelloData_->iColor, swktool::CheckedType::Checked);
		pHelloData_->clrText = clrTextArray[IDM_LAST_COLOR - IDM_COLOR_BLACK];

		::InvalidateRect(GetWindowHWND(), NULL, TRUE);

		return TRUE;
	}

	
	virtual void OnMDIActivate(HWND hwndActivating, HWND Deactivating) override 
	{ 
		swktool::WinMenu HelloMenu = GetHelloMenu();

		HMENU hMenuHelloWindow = GetSubMenu((HMENU)HelloMenu, 2);
		if (GetWindowHWND() == hwndActivating) {
			::SendMessage(hwndMDIClient_, WM_MDISETMENU, (WPARAM)(HMENU)HelloMenu, (LPARAM)hMenuHelloWindow);			
		}

		HelloMenu.CheckMenuItem(pHelloData_->iColor, (hwndActivating == GetWindowHWND()) ? swktool::CheckedType::Checked : swktool::CheckedType::Unchecked);
		if (GetWindowHWND() != hwndActivating) {
			swktool::WinMenu hMenuInit = GetInitMenu();
			HMENU hMenuInitWindow = GetSubMenu((HMENU)hMenuInit, 0);
			::SendMessage(hwndMDIClient_, WM_MDISETMENU, (WPARAM)(HMENU)hMenuInit, (LPARAM)hMenuInitWindow);			
		}

		::DrawMenuBar(hwndMDIFrame_);
	}


	virtual LRESULT OnCommand(WPARAM wParam, LPARAM lParam) override {
		IDM_LAST_COLOR = wParam;
		auto Cmd = LOWORD(wParam);

		switch (Cmd) {
			ON_COMMAND(IDM_COLOR_BLACK, OnColor);
			ON_COMMAND(IDM_COLOR_RED, OnColor);
			ON_COMMAND(IDM_COLOR_GREEN, OnColor);
			ON_COMMAND(IDM_COLOR_BLUE, OnColor);
			ON_COMMAND(IDM_COLOR_WHITE, OnColor);
		}

		return TRUE;
	}
};