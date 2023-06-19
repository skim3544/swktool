#pragma once

#include "Ctrl.h"


namespace swktool {
	class UpDownCtrl : public Ctrl {
	public:
	public:
		UpDownCtrl(std::wstring Caption, DWORD Style, int x, int y, int Width, int Height,  Window* pParent, UINT CtrlID);
		UpDownCtrl(UINT CtrlID, swktool::Window* pParent);
		~UpDownCtrl() { ; }


		void SetRange(int Min, int Max) {
			SendMessage(hwndCtrl, UDM_SETRANGE, 0, MAKELPARAM(Max, Min));
		}

		void SetRange32(int Min, int Max) {
			SendMessage(hwndCtrl, UDM_SETRANGE, Min, Max);
		}

		void SetPosition(short nPosition)
		{
			SendMessage(hwndCtrl, UDM_SETPOS, 0, nPosition);
		}

		void SetPosition32(int nPosition)
		{
			SendMessage(hwndCtrl, UDM_SETPOS32, 0, nPosition);
		}


		HWND GetBuddyWindowHandle() {
			HWND hBuddyHandle = (HWND)SendMessage(hwndCtrl, UDM_GETBUDDY, 0, 0);
			return hBuddyHandle;
		}

		HWND SetBuddyWindowHandle(HWND hBuddy) {
			HWND hPrevWindow = (HWND)SendMessage(hwndCtrl, UDM_SETBUDDY, (WPARAM)hBuddy, 0);
			return hPrevWindow;
		}

		void SetUnicodeFormat(BOOL bUnicodeFormat)
		{
			SendMessage(hwndCtrl, UDM_SETPOS32, bUnicodeFormat, 0);
		}

		int GetBase() {
			return (int)SendMessage(hwndCtrl, UDM_GETBASE, 0, 0);
		}
	};
}