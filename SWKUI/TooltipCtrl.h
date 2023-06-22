#pragma once

#include "Ctrl.h"

namespace swktool {	

	class TooltipCtrl : public Ctrl {
	public:
		// Common Style WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP
		TooltipCtrl(std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, Window* pParent, UINT CtrlID);
		TooltipCtrl(UINT CtrlID, Window* pParent);
		TooltipCtrl(UINT CtrlID, DialogWindow* pParent);
		~TooltipCtrl() { ; }
	};
}