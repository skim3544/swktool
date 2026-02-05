
#pragma once

#include "Ctrl.h"


namespace swktool 
{	

	class RebarCtrl : public Ctrl {
	public:
		RebarCtrl(std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, IWindow* pParent, UINT CtrlID);
		RebarCtrl(IWindow* pParent);
		~RebarCtrl() { ; }
	};
}