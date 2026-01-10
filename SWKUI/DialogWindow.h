#pragma once

#include <Windows.h>
#include <memory>

//#include "Ctrl.h"
//#include "Window.h"
//#include "WinEventHandler.h"
//#include "StaticTextCtrl.h"
//#include "EditCtrl.h"
//#include "ButtonCtrl.h"
//#include "ComboboxCtrl.h"
//#include "ListboxCtrl.h"
//#include "GroupboxCtrl.h"
//#include "ProgressBarCtrl.h"
//#include "AnimationCtrl.h"

#include "MsgHandler.h"




namespace swktool
{
	class DialogWindow :
		public DialogHandlerBase
	{
	protected:
		LPCTSTR			DialogTemplateName = nullptr;
		UINT			m_ResourceID = 0;
		IWindow* parentWindow_ = nullptr;

		std::wstring    cwCaption;


	public:
		DialogWindow() = default;
		DialogWindow(UINT ID, IWindow* pParent);
		DialogWindow(LPCTSTR pDialogTemplateName, IWindow* pParent);
		virtual ~DialogWindow() = default;

		void SetCaption(LPCWSTR pCaptioNText) {
			cwCaption = pCaptioNText;
		}

		virtual INT_PTR ShowDialog();

		virtual INT_PTR OnDialogMessage(UINT msg, WPARAM wParam, LPARAM lParam);


	protected:
		virtual INT_PTR  OnSetFont(WPARAM wParam, LPARAM lParam) {
			return (INT_PTR)TRUE;
		}

		//virtual INT_PTR OnInitDialog(HWND hwndFocusedCtrl, LPARAM lParam);
		virtual INT_PTR OnInitDialog(HWND hwndFocusedCtrl, LPARAM lParam);

		virtual LRESULT OnCommand(WORD id, WORD code, HWND control) override;
	};

}
