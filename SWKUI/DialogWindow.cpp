#include "pch.h"
#include "framework.h"
#include "WinEventHandler.h"
#include "Window.h"
#include "DialogWindow.h"
#include "Ctrl.h"
#include "ListboxCtrl.h"

#include "StaticTextCtrl.h"
#include <CommCtrl.h>

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#else
#define DBG_NEW new
#endif


namespace swktool {

	BOOL DialogWindow::Create(std::shared_ptr<DialogWindow> pDlg) {
		WinEventHandler::GetInst()->PreRegisterDlgWindow(pDlg);

		return TRUE;
	}


	/*
	HWND DialogWindow::CreateDlgIndirect(LPCTSTR lpszTemplateName, HINSTANCE hInst) {
		HWND hDialog = NULL;
		LPCDLGTEMPLATE lpDialogTemplate = NULL;
		HGLOBAL hDialogTemplate = nullptr;
		HRSRC hResource = ::FindResource(hInst, lpszTemplateName, RT_DIALOG);
		if (hResource != nullptr) {
			hDialogTemplate = LoadResource(hInst, hResource);
		}

		// If loading the dialog template from the resource file was successful
		if (hDialogTemplate != nullptr) {
			lpDialogTemplate = (LPCDLGTEMPLATE)LockResource(hDialogTemplate);

			std::shared_ptr< DialogWindow> shared(this);
			// Let the system know that I am the one trying to create this window
			WinEventHandler::GetInst()->PreRegisterDlgWindow(shared);

			// create a modeless dialog
			hDialog = ::CreateDialogIndirect(hInst, lpDialogTemplate, hWndParent, 
				static_cast<DLGPROC>(WinEventHandler::ProcDlgMessage));
			WinEventHandler::GetInst()->PreRegisterDlgWindow(nullptr);

			UnlockResource(hDialogTemplate);
			if (hDialogTemplate != nullptr) {
				FreeResource(hDialogTemplate);
			}
		}

		return hDialog;
	}
	*/


	INT DialogWindow::ShowDialog() {
		

		// Missing Create call?
		assert(swktool::WinEventHandler::GetInst()->GetPreRegistedDialog() != nullptr);
		
		// start the message pumping
		::DialogBox(hInst,
			DialogTemplateName,
			hwndParent,
			swktool::WinEventHandler::ProcDialogMessage);

		return DialogResult;		
	}


	/*
	INT DialogWindow::ShowDialog() {
		bool fDone = false;
	
		// if riding on top of parent, disable the parent window
		if (hWndParent != NULL)
			EnableWindow(hWndParent, FALSE);


		// enter message loop for this dialog, don't come out until dialog gets closed
		while (fDone == false)
		{
			MSG msg;

			while (PeekMessage(&msg, hDlg, 0, 0, PM_NOREMOVE))
			{
				UINT MsgID = msg.message;

				if (MsgID == WM_CLOSE)
				{
					MsgID = 0;
				}


				if (GetMessage(&msg, NULL, 0, 0))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}

				if (msg.message == WM_NULL || msg.message == WM_CLOSE || msg.message == WM_QUIT)
				{
					fDone = true;
					//PostMessage(NULL, WM_QUIT, 0, 0);
					break;
				}
			}
		}

		// Enable Parent Window
		if (hWndParent != NULL)
			EnableWindow(hWndParent, true);

		return DialogResult;
	}
*/



	BOOL DialogWindow::OnInitDialog(HWND hwndFocus, LPARAM lParam) {

		if (!Caption_.empty()) {
			SetWindowText(hwndWindow, Caption_.c_str());
		}
		return TRUE;
	}




	LRESULT DialogWindow::OnCommand(WPARAM msg, LPARAM lParam)
	{				
		return FALSE;
	}

	 LRESULT DialogWindow::ProcessMessage(HWND hDialog, WORD wMsg, WPARAM wParam, LPARAM lParam) {		
		switch (wMsg)
		{
			PROC_DLG_MSG(hDialog, WM_INITDIALOG, OnInitDialog);
			PROC_DLG_MSG(hDialog, WM_COMMAND, OnCommand);
			PROC_DLG_MSG(hDialog, WM_CLOSE, OnClose);			
		}

		return FALSE;
	}

}