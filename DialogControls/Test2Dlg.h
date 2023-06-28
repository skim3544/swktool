#pragma once

class Test2Dlg : public swktool::DialogWindow {
	swktool::ControlBinder binder_;
	std::unique_ptr<swktool::StatusBarCtrl> StatusBar_;

public:
	Test2Dlg(UINT ID, swktool::Window* pParent) :
		DialogWindow(ID, pParent) {

	}

	virtual INT_PTR OnInitDialog(HWND hwndFocusedCtrl, LPARAM lParam) override {
		swktool::DialogWindow::OnInitDialog(hwndFocusedCtrl, lParam);

		StatusBar_ = std::make_unique<swktool::StatusBarCtrl>(this);

		return TRUE;
	}

	//virtual void OnSize()
};
