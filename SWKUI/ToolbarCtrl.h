#pragma once

#include "Ctrl.h"

namespace swktool {	
	class ToolbarCtrl : public Ctrl {
	public:
		// Common Style WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP
		ToolbarCtrl(std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, IWindow* pParent, UINT CtrlID);
		ToolbarCtrl(IWindow* pParent) : Ctrl(pParent) { ; }
		ToolbarCtrl(UINT CtrlID, IWindow* pParent);
		~ToolbarCtrl() { ; }
	};
}
