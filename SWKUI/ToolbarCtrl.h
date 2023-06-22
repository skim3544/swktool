#pragma once

#include "Ctrl.h"

namespace swktool {	
	class ToolbarCtrl : public Ctrl {
	public:
		// Common Style WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP
		ToolbarCtrl(std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, Window* pParent, UINT CtrlID);
		ToolbarCtrl(UINT CtrlID, Window* pParent);
		ToolbarCtrl(UINT CtrlID, DialogWindow* pParent);
		~ToolbarCtrl() { ; }
	};
}
