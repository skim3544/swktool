#pragma once

#include <Windows.h>
#include <memory>
#include "Ctrl.h"
#include "Window.h"
#include "WinEventHandler.h"
#include "StaticTextCtrl.h"
#include "EditCtrl.h"
#include "ButtonCtrl.h"
#include "ComboboxCtrl.h"
#include "ListboxCtrl.h"
#include "GroupboxCtrl.h"
#include "ProgressBarCtrl.h"
#include "AnimationCtrl.h"

namespace swktool
{	
	/// <summary>
	/// Model DialogWindow
	/// 
	/// Create and assign as shared pointer, Call Create to add the new dialog box to the system,
	/// Then ShowDialogbox to pass the control to the message pumping
	/// 
	/// std::shared_ptr <swktool::DialogWindow> pDlg = std::make_shared<SimpleDlg>(MAKEINTRESOURCE(IDD_DIALOG1), this);
	///	pDlg->SetCaption(TEXT("Test"));
	///	pDlg->Create(pDlg);
	///	int Result = pDlg->ShowDialog();
	///
	/// </summary>
	class DialogWindow : public AWindow
	{
	protected:
		INT				DialogResult = 0;
		LPCTSTR			DialogTemplateName = nullptr;

	public:
		DialogWindow() : AWindow() {
		}

		// Dialog on top of a Window
		DialogWindow(UINT ID, AWindow* pParent) : AWindow(pParent) {
			// convert ID to template name
			DialogTemplateName = MAKEINTRESOURCE(ID);
		}
				
		DialogWindow(LPCTSTR lpszTemplateName, AWindow* pParent) : AWindow(pParent)
		{
			DialogTemplateName = lpszTemplateName;			
		}


		DialogWindow(LPCTSTR lpszTemplateName, HINSTANCE hInstance, HWND ParentHandle)
		{
			DialogTemplateName = lpszTemplateName;
			hInst = hInstance;
			hwndParent  = ParentHandle;
		}

		// for any reason, the dialog goes out of scope and destructor gets called, clean thyself
		virtual ~DialogWindow() {

		}


		virtual void SetDialogHandle(HWND hHandle) {
			hwndWindow = hHandle;
		}


		BOOL Create(std::shared_ptr<DialogWindow> pDlg);

		virtual INT ShowDialog();


		LRESULT ProcessMessage(HWND hDialog, WORD msg, WPARAM wParam, LPARAM lParam) override;

		void End(INT Result = 0) {
			::EndDialog(hwndWindow, Result);
		}

		virtual BOOL OnInitDialog(HWND hwndFocus, LPARAM lParam);
		virtual LRESULT OnCommand(WPARAM wParam, LPARAM lParam);
		virtual void OnClose() { ::EndDialog(hwndWindow, 0L); }

	protected:
		//HWND CreateDlgIndirect(LPCTSTR lpszTemplateName, HINSTANCE hInst);
	};
}
