#pragma once

#pragma once

#include "Ctrl.h"


namespace swktool {

	class AWindow;

	class ScrollBarCtrl : public Ctrl {
	public:
		ScrollBarCtrl(std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, AWindow* pParent, UINT CtrlID);
		ScrollBarCtrl(UINT CtrlID, AWindow* pParent);
		~ScrollBarCtrl() { ; }
	};
}
