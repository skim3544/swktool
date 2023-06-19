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
		public DialogMsgHandler
	{
	protected:
		LPCTSTR			DialogTemplateName = nullptr;
		UINT			m_ResourceID;

		std::wstring    cwCaption;

	public:
		DialogWindow(UINT ID, Window* pParent);
		DialogWindow(LPCTSTR pDialogTemplateName, Window* pParent);

		virtual ~DialogWindow() {
			;
		}

		void SetCaption(LPCWSTR pCaptioNText) {
			cwCaption = pCaptioNText;
		}

		virtual INT_PTR  OnSetFont(WPARAM wParam, LPARAM lParam) {
			return (INT_PTR)TRUE;  
		}

		virtual INT_PTR OnInitDialog(HWND hwndFocusedCtrl, LPARAM lParam);
		virtual INT_PTR OnCommand(WPARAM wParam, LPARAM lParam);			
		virtual INT_PTR OnClose();


		virtual INT_PTR ShowDialog();
		virtual INT_PTR HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) override;
	};

}
