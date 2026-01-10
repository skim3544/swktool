#pragma once

#include "Ctrl.h"

namespace swktool 
{	
	class IWindow;
	class TooltipCtrl : public Ctrl 
	{
	public:
		// Common Style WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP
		TooltipCtrl(std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, IWindow* pParent, UINT CtrlID);
		TooltipCtrl(UINT CtrlID, IWindow* pParent);
		~TooltipCtrl() { ; }

		void AddTooltipText(HWND hWnd, LPWSTR str) 
		{
			TOOLINFO ti = { 0 };
			ti.cbSize = sizeof(TOOLINFO);
			ti.uFlags = TTF_SUBCLASS;
			ti.hwnd = hWnd;
			ti.hinst = hInst;
			ti.lpszText = str;

			GetClientRect(hWnd, &ti.rect);

			// Associate the tooltip with the "tool" window.
			SendMessage(hwndCtrl, TTM_ADDTOOL, 0, (LPARAM)(LPTOOLINFO)&ti);
		}
	};
}