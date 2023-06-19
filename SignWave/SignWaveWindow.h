#pragma once

#include "../SWKUI/Window.h"


class MainWindow : public swktool::Window {
public:
	PCWSTR  ClassName() const {
		return L"Signwave Window";
	}

	virtual void PreRegisterWindow(WNDCLASSEX& wc) override {
		wc.style = CS_HREDRAW | CS_VREDRAW;
		wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	}

	void virtual OnSize(UINT state, int nWidth, int nHeight) override;
	virtual void OnPaint() override;

private:
	int	cxClient_;
	int cyClient_;
};


