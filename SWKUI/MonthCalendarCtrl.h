#pragma once

#include "Ctrl.h"


namespace swktool {	

	class MonthCalendarCtrl : public Ctrl {
	public:
		MonthCalendarCtrl(std::wstring Caption, DWORD Style, int x, int y, int Width, int Height,  Window* pParent, UINT CtrlID);
		MonthCalendarCtrl(UINT CtrlID, Window* pParent);
		MonthCalendarCtrl(UINT CtrlID, DialogWindow* pParent);
		~MonthCalendarCtrl() { ; }

		RECT GetMinReqRect() const {
			RECT rect;
			MonthCal_GetMinReqRect(hwndCtrl, &rect);
			return rect;
		}

		void SetPosition(int X, int Y, int cx, int cy, UINT Flags) {
			::SetWindowPos(hwndCtrl, NULL, X, Y, cx, cy, Flags);
		}

	};
}