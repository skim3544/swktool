
#pragma once

#include "Ctrl.h"


namespace swktool {

	class AWindow;

	class RebarCtrl : public Ctrl {
	public:
		RebarCtrl(std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, AWindow* pParent, UINT CtrlID);
		RebarCtrl(UINT CtrlID, AWindow* pParent);
		~RebarCtrl() { ; }
	};
}