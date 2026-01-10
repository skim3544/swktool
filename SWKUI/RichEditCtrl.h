#pragma once

#include <Richedit.h>
#include "Ctrl.h"


namespace swktool {	
	class IWindow;
	class RichEditCtrl : public Ctrl {
	public:
		RichEditCtrl(std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, IWindow* pParent, UINT CtrlID);
		RichEditCtrl(UINT CtrlID, IWindow* pParent);
		~RichEditCtrl() { ; }
	};
}
