#pragma once

#include "Ctrl.h"


namespace swktool {
	
	class AWindow;

	class HeaderCtrl : public Ctrl {
	public:
		HeaderCtrl(std::wstring Caption, DWORD Style, int x, int y, int Width, int Height, Window* pParent, UINT CtrlID);
		HeaderCtrl(UINT CtrlID, Window* pParent);
		~HeaderCtrl() { ; }
	};
}