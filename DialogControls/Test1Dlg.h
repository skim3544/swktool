#pragma once
#include "resource.h"

#include "../SWKUI/DialogWindow.h"
#include "../SWKUI/ControlBinder.h"
#include "../SWKUI/StaticTextCtrl.h"
#include "../SWKUI/ButtonCtrl.h"
#include "../SWKUI/EditCtrl.h"

class swktool::Window;
class swktool::ButtonCtrl;

class Test1Dlg : public swktool::DialogWindow {
	swktool::ControlBinder binder_;

	swktool::StaticTextCtrl* pStaticText_;
	swktool::EditCtrl* pEditCtrl_;
	swktool::ButtonCtrl* button_;
	swktool::CheckboxCtrl* checkbox1_;
	swktool::RadioCtrl* Radio1;
	swktool::RadioCtrl* Radio2;

public:
	Test1Dlg(UINT ID, swktool::Window* pParent) :
		DialogWindow(ID, pParent) {

	}

	virtual INT_PTR OnInitDialog(HWND hwndFocusedCtrl, LPARAM lParam) override {
		swktool::DialogWindow::OnInitDialog(hwndFocusedCtrl, lParam);

		pStaticText_ = binder_.Bind<swktool::StaticTextCtrl>(IDC_STATIC1, this);
		pStaticText_->SetText(TEXT("Please enter your name:"));
		
		pEditCtrl_   = binder_.Bind<swktool::EditCtrl>(IDC_EDIT1, this);
		pEditCtrl_->SetText(TEXT("Name"));

		button_ = binder_.Bind<swktool::ButtonCtrl>(IDOK, this);
		button_->SetText(TEXT("Test OK"));
		
		checkbox1_ = binder_.Bind<swktool::CheckboxCtrl>(IDC_CHECK1, this);
		checkbox1_->SetCheck(swktool::CheckboxCtrl::CheckState::Checked);

		Radio1 = binder_.Bind<swktool::RadioCtrl>(IDC_RADIO1, this);
		Radio2 = binder_.Bind<swktool::RadioCtrl>(IDC_RADIO2, this);

		Radio1->Set();

		return TRUE;
	}
};