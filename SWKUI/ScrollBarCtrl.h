#pragma once

#include "Ctrl.h"

class IWindow;
namespace swktool {	

	class ScrollBarCtrl : public Ctrl {
	public:
		ScrollBarCtrl(DWORD Style, IWindow* pParent);
		ScrollBarCtrl(UINT CtrlID, IWindow* pParent);		
		~ScrollBarCtrl() { ; }
	};


	class HScrollBarCtrl : public Ctrl {
		int BarHeight_;

	public:
		HScrollBarCtrl(IWindow* pParent, int sbHeight = SM_CXHSCROLL);
		virtual void OnSize(UINT state, int nWidth, int nHeight);
	};

	class VScrollBarCtrl : public Ctrl {
		int BarWidth_;

	public:
		VScrollBarCtrl(IWindow* pParent, int sbWidth = SM_CXVSCROLL);
		virtual void OnSize(UINT state, int nWidth, int nHeight);
	};

}
