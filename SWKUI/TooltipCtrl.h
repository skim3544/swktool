#pragma once

#include "Ctrl.h"

namespace swktool {
	class AWindow;

	class TooltipCtrl : public Ctrl {
	public:
		// Common Style WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP
		TooltipCtrl(std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, AWindow* pParent, UINT CtrlID);
		TooltipCtrl(UINT CtrlID, AWindow* pParent);
		~TooltipCtrl() { ; }
	};
}