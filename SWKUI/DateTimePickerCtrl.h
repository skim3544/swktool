#pragma once
#include <Windows.h>
#include <CommCtrl.h>
#include <string>
#include "Ctrl.h"

namespace swktool {
	

	class DateTimePickerCtrl : public Ctrl {

	public:
		// set default style to WS_BORDER | WS_CHILD | WS_VISIBLE | DTS_SHOWNONE
		DateTimePickerCtrl(std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, Window* pParent, UINT CtrlID);
		DateTimePickerCtrl(UINT CtrlID, Window* pParent);
		DateTimePickerCtrl(UINT CtrlID, DialogWindow* pParent);
	};
}