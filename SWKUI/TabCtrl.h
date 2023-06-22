#pragma once

#include "Ctrl.h"

namespace swktool {	

	class TabCtrl : public Ctrl {
	public:
		// Common Style WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP
		TabCtrl(std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, Window* pParent, UINT CtrlID);
		TabCtrl(UINT CtrlID, Window* pParent);
		TabCtrl(UINT CtrlID, DialogWindow* pParent);
		~TabCtrl() { ; }
	};
}