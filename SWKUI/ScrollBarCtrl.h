#pragma once

#include "Ctrl.h"


namespace swktool {	

	class ScrollBarCtrl : public Ctrl {
	public:
		ScrollBarCtrl(DWORD Style, Window* pParent);
		ScrollBarCtrl(UINT CtrlID, Window* pParent);
		ScrollBarCtrl(UINT CtrlID, DialogWindow* pParent);
		~ScrollBarCtrl() { ; }
	};


	class HScrollBarCtrl : public Ctrl {
		int BarHeight_;

	public:
		HScrollBarCtrl(Window* pParent, int sbHeight = SM_CXHSCROLL);
		virtual void OnSize(UINT state, int nWidth, int nHeight);
	};

	class VScrollBarCtrl : public Ctrl {
		int BarWidth_;

	public:
		VScrollBarCtrl(Window* pParent, int sbWidth = SM_CXVSCROLL);
		virtual void OnSize(UINT state, int nWidth, int nHeight);
	};

}
