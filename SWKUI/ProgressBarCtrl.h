#pragma once

#include <CommCtrl.h>
#include "Ctrl.h"


namespace swktool {	

	class IWindow;
	class ProgressBarCtrl : public Ctrl 
	{
	public:
		ProgressBarCtrl(std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, IWindow* pParent, UINT CtrlID);
		ProgressBarCtrl(UINT CtrlID, IWindow* pParent);
		ProgressBarCtrl(HWND hParent, HINSTANCE hInst, std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, UINT CtrlID = 0);
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