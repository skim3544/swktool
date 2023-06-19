
#pragma once

#include "Ctrl.h"


namespace swktool {

	class AWindow;

	class RebarCtrl : public Ctrl {
	public:
		RebarCtrl(std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, Window* pParent, UINT CtrlID);
		RebarCtrl(UINT CtrlID, Window* pParent);
		~RebarCtrl() { ; }
	};
}