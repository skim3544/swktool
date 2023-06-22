#pragma once

#include "Ctrl.h"


namespace swktool {	

	class ScrollBarCtrl : public Ctrl {
	public:
		ScrollBarCtrl(std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, Window* pParent, UINT CtrlID);
		ScrollBarCtrl(UINT CtrlID, Window* pParent);
		ScrollBarCtrl(UINT CtrlID, DialogWindow* pParent);
		~ScrollBarCtrl() { ; }
	};
}
