#pragma once

#include <Richedit.h>
#include "Ctrl.h"


namespace swktool {

	class AWindow;

	class RichEditCtrl : public Ctrl {
	public:
		RichEditCtrl(std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, AWindow* pParent, UINT CtrlID);
		RichEditCtrl(UINT CtrlID, AWindow* pParent);
		~RichEditCtrl() { ; }
	};
}
