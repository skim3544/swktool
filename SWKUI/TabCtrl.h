#pragma once

#include "Ctrl.h"

namespace swktool 
{	
	class IWindow;
	class TabCtrl : public Ctrl {
	public:
		// Common Style WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP
		TabCtrl(std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, IWindow* pParent, UINT CtrlID);
		TabCtrl(UINT CtrlID, IWindow* pParent);
		~TabCtrl() { ; }
	};
}