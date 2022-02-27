#pragma once

#include "..\SWKUI\Window.h"
#include "..\SWKUI\DialogWindow.h"
#include "..\SWKUI\WinKernel.h"
#include "..\SWKUI\Drawing.h"
#include "..\SWKUI\Drawing.h"
#include "..\SWKUI\StatusBarCtrl.h"
#include "..\SWKUI\CommonDialog.h"
#include "..\SWKUI\ControlBinder.h"
#include "resource.h"
#include <memory>
//#include <mutex>

#include "SimpleDlg.h"

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#else
#define DBG_NEW new
#endif


class swktool::Window;


class SimpleWindow : public swktool::Window
{	
	swktool::ControlBinder Binder_;

	swktool::StatusBarCtrl* StatusCtrl_ = nullptr;
public:
	SimpleWindow(LPCWSTR lpClassName, LPCWSTR lpWindowName, HINSTANCE hInstance) :
		swktool::Window(lpClassName, lpWindowName, hInstance)
	{

	}

	~SimpleWindow() { ; }

	virtual void OnPaint() override {
		RECT rect;
		GetClientRect(hwndWindow, &rect);

		swktool::PaintDeviceContext PC(GetWindowHWND());
		PC.DrawText(TEXT("Hello World"), &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	}

	BOOL OnCreate(LPCREATESTRUCT pCreateStruct)  override {
		//swktool::DeviceContext dc = swktool::DeviceContext::GetDC(hWnd);
		std::unique_ptr<swktool::DeviceContext> dc = swktool::DeviceContext::GetDC(hwndWindow);

		DWORD Style = (DWORD)(swktool::EditCtrlStyle::VISIBLE | swktool::EditCtrlStyle::CHLID | swktool::EditCtrlStyle::BORDER);
		swktool::RichEditCtrl* Ctrl = Binder_.Create<swktool::Window, swktool::RichEditCtrl>(this, TEXT("Test1"),
			(DWORD)Style, 10, 0, 100, 25, 101);

		//Ctrl->SetText(_TEXT("Test Text"));
		

		DWORD Style1 = WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP | SBARS_TOOLTIPS;
		StatusCtrl_ = Binder_.Create<swktool::Window, swktool::StatusBarCtrl>(this, TEXT("Test1"),
			(DWORD)Style1, 10, 0, 100, 25, 101);

		int iRightEdge[] = { 300, 600};
		StatusCtrl_->SetParts(iRightEdge, _countof(iRightEdge));
		StatusCtrl_->SetStatusText(1, _TEXT("Processing..."));
		StatusCtrl_->SetStatusTipText(1, _TEXT("TIP $$$"));
		std::wstring Text = StatusCtrl_->GetStatusTipText(1);

		return TRUE;
	}

	BOOL DoAbout() {

		std::shared_ptr <SimpleDlg> pDlg = std::make_shared<SimpleDlg>(MAKEINTRESOURCE(IDD_DIALOG1), this);
		
		pDlg->Create(pDlg);
		pDlg->SetCaption(TEXT("Test"));
		int id = pDlg->ShowDialog();


		//swktool::FileSaveDlg dlg(hInst, hWnd);
		//
		//dlg.SetFile(TEXT("Untitled"));
		//std::vector<swktool::FileFilter> filter{};

		//dlg.SetDefaultExtension(L"txt");

		//filter.emplace_back(swktool::FileFilter(L"All Files" , L"*.*" ));
		//filter.emplace_back(swktool::FileFilter(L"Text Files", L"*.txt"));
		//filter.emplace_back(swktool::FileFilter(L"PDF Files", L"*.pdf"));
		//dlg.SetFilter(filter);
		//dlg.ShowDialog();
		
//		swktool::ChooseColorDlg dlg(hInst, hWnd);
//			dlg.ShowDialog();


		return FALSE;
	}

	BOOL DoExit()
	{
		DestroyWindow(hwndWindow);
		return FALSE;
	}

	virtual void OnClose() { 
		DestroyWindow(hwndWindow);	// destroy window
		PostQuitMessage(0);	// send Quit-message so the window closes
	}

	virtual void OnSize(UINT state, int nWidth, int nHeight) override {
		Width = nWidth;
		Height = nHeight;

		if (StatusCtrl_)
			StatusCtrl_->OnSize(state, nWidth, nHeight);
	}

	virtual LRESULT OnCommand(WPARAM wParam, LPARAM lParam) override {
		int  CmdID = (int)LOWORD(wParam);
		switch (CmdID)
		{			
		ON_COMMAND(IDM_ABOUT, DoAbout);
		ON_COMMAND(IDM_EXIT, DoExit);
		}

		return 0l;
	}

};

