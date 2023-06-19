
#pragma once

#include "Ctrl.h"
namespace swktool {

	class AWindow;

	class TreeViewCtrl : public Ctrl {

	public:
		TreeViewCtrl(std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, Window* pParent, UINT CtrlID);
		TreeViewCtrl(UINT CtrlID, Window* pParent);
		~TreeViewCtrl() { ; }
	};
}
