#pragma once
#include "resource.h"

#include "../SWKUI/DialogWindow.h"
#include "../SWKUI/ControlBinder.h"
#include "../SWKUI/ButtonCtrl.h"

class swktool::Window;
class swktool::ButtonCtrl;

class Test1Dlg : public swktool::DialogWindow {
	swktool::ControlBinder binder_;
	swktool::ButtonCtrl* button_;
public:
	Test1Dlg(UINT ID, swktool::Window* pParent) :
		DialogWindow(IDD_TEST2, pParent) {

	}

	virtual INT_PTR OnInitDialog(HWND hwndFocusedCtrl, LPARAM lParam) override {
		swktool::DialogWindow::OnInitDialog(hwndFocusedCtrl, lParam);

		//binder_.Bind< 
		button_ = binder_.Bind<swktool::ButtonCtrl>(IDOK, this);
		button_->SetText(L"Test");

		return TRUE;
	}
};