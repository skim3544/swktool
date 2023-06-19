#ifndef ___STATIC_TEXT_CONTROL_H___
#define ___STATIC_TEXT_CONTROL_H___

#include "Ctrl.h"

namespace swktool {
	class AWindow;
	class StaticTextCtrl : public Ctrl {
		const int STRING_BUF_SIZE = 255;

	public:		
		StaticTextCtrl(std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, Window* pParent, UINT CtrlID);
		StaticTextCtrl(UINT CtrlID, Window* pParent);
		virtual ~StaticTextCtrl() { ; }
	};
}

#endif // ___STATIC_TEXT_CONTROL_H___