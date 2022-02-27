#pragma once

#include "Ctrl.h"

namespace swktool {

	class AnimationCtrl : public Ctrl {
	public:
		AnimationCtrl(std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, AWindow* pParent, UINT CtrlID);
		AnimationCtrl(UINT CtrlID, AWindow* pParent);
		~AnimationCtrl() { ; }

		void Open(LPTSTR szName) {
			Animate_Open(hwndCtrl, szName);
			ShowWindow(hwndCtrl, SW_SHOW);
		}

		void OpenEx(HINSTANCE hInst, LPTSTR szName) {
			Animate_OpenEx(hwndCtrl, hInst, szName);
			ShowWindow(hwndCtrl, SW_SHOW);
		}
		
		BOOL Play(int From = 0, int To = -1, int Repeat = -1)
		{
			return Animate_Play(hwndCtrl, From, To, Repeat);
		}

		BOOL Stop() {
			return Animate_Stop(hwndCtrl);
		}

		void Close() {
			Animate_Close(hwndCtrl);
		}

		BOOL IsPlaying() {
			return Animate_IsPlaying(hwndCtrl);
		}

		void Seek(int Frame) {
			Animate_Seek(hwndCtrl,Frame);
		}
	};
}