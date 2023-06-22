#pragma once

#include <CommCtrl.h>
#include "Ctrl.h"


namespace swktool {	

	class ProgressBarCtrl : public Ctrl {
	public:
		ProgressBarCtrl(std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, Window* pParent, UINT CtrlID);
		ProgressBarCtrl(UINT CtrlID, Window* pParent);
		ProgressBarCtrl(UINT CtrlID, DialogWindow* pParent);
		~ProgressBarCtrl() { ; }


		void SetStep(int Step)
		{
			::SendMessage(hwndCtrl, PBM_SETSTEP, (WPARAM)Step, 0);
		}

		void SetRange(int Min, int Max) {

			::SendMessage(hwndCtrl, PBM_SETRANGE, 0, MAKELPARAM(Min, Max));
		}

		void StepIt()
		{
			::SendMessage(hwndCtrl, PBM_SETSTEP, 0, 0);
		}

		void SetPosition(int nPos) {
			::SendMessage(hwndCtrl, PBM_SETPOS, (WPARAM)nPos, 0);
		}

		void DeltaPosition(int nDelta) {
			::SendMessage(hwndCtrl, PBM_DELTAPOS, (WPARAM)nDelta, 0);
		}
	};
}