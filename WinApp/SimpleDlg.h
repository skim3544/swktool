#pragma once
#include <Windows.h>
#include <memory>
#include <tchar.h>
#include "..\SWKUI\WinKernel.h"
#include "..\SWKUI\DialogWindow.h"
#include "..\SWKUI\ModelessDialogWindow.h"

#include "resource.h"
#include "..\SWKUI\Controls.h"
#include "..\SWKUI\ControlBinder.h"


#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#else
#define DBG_NEW new
#endif


class swktool::Window;

class SimpleDlg : public swktool::DialogWindow {
	swktool::ControlBinder	binder_;

	swktool::ComboboxCtrl* cbCombo_ = nullptr;
	swktool::ListboxCtrl* List_ = nullptr;

public:
	SimpleDlg(LPCTSTR lpszTemplateName, swktool::AWindow* pParent) : 
		swktool::DialogWindow(lpszTemplateName, pParent)
	{
	}

	SimpleDlg(UINT ID, swktool::AWindow* pWin) : swktool::DialogWindow(ID, pWin)
	{

	}


	BOOL OnOK() {
		::EndDialog(hwndWindow, TRUE);

		SetDlgMsgResult(hwndWindow, WM_COMMAND, 350);
		return TRUE;
	}

	BOOL OnCancel() {		
		//pProgressBar->DeltaPosition(5);
		//::EndDialog(hDlg, TRUE);

		return TRUE;
	}

	virtual BOOL OnInitDialog(HWND hwndFocus, LPARAM lParam) override {

		DialogWindow::OnInitDialog(hwndFocus, lParam);

		
		swktool::EditCtrl* txtTest = binder_.Bind<swktool::EditCtrl>(IDC_EDIT1, this);
		txtTest->SetText(std::wstring(_TEXT("Hello")));
		//txtTest->Test();
		
		
		swktool::ButtonCtrl* testBtn = binder_.Bind<swktool::ButtonCtrl>(IDOK, this);
		testBtn->SetText(_TEXT("Okey!"));
		//testBtn->Test();

		
		cbCombo_ = binder_.Bind<swktool::ComboboxCtrl>(IDC_COMBO1, this);
		cbCombo_->SetText(TEXT("Yellow"));
		cbCombo_->AddString(TEXT("1"));
		cbCombo_->AddString(TEXT("2"));
		cbCombo_->AddString(TEXT("3"));


		List_ = binder_.Bind<swktool::ListboxCtrl>(IDC_LIST1, this);
		List_->ClearItems();
		int Result = List_->Dir(DDL_ARCHIVE | DDL_DIRECTORY | DDL_DIRECTORY | DDL_EXCLUSIVE, TEXT("*.*"));		
		//List_->Test();
		
		/*
		Text1 = BindEdit(IDC_EDIT1);
		Controls.push_back(Text1);

		btnOK = BindButton(IDOK);
		Controls.push_back(btnOK);

		cbComboBox = BindCombobox(IDC_COMBO1);
		cbComboBox->SetText(TEXT("Yellow"));

		cbComboBox->AddString(TEXT("1"));
		cbComboBox->AddString(TEXT("2"));
		cbComboBox->AddString(TEXT("3"));

		Controls.push_back(cbComboBox);

		pProgressBar = BindProgressBar(IDC_PROGRESS1);
		pProgressBar->SetRange(0, 100);
		pProgressBar->SetPosition(60);
		pProgressBar->SetStep(5);
		pProgressBar->StepIt();

		lbListBox = BindListbox(IDC_LIST1);
		lbListBox->ClearItems();
		int Result = lbListBox->Dir(DDL_ARCHIVE | DDL_DIRECTORY | DDL_DIRECTORY | DDL_EXCLUSIVE, TEXT("*.*"));

		Controls.push_back(lbListBox);
		//		std::wstring str = btnOK->GetText();
		//		pGroupBox = BindGroupbox(IDC_STATIC1);
		//		pGroupBox-> SetText(TEXT("Group1"));


		btnOK->SetText(TEXT("Close"));
		Text1->SetFocus();

		//::GetWindow(IDC_PROGRESS1)
		*/

		//RECT x = Text1->GetRect();
		/*
		MoveWindow(Text1->GetCtrlHandle(),
			x.left - 1, x.top, x.right - x.left, x.bottom - x.top, true);
		*/


		return TRUE;
	}


	BOOL Notified(int id, HWND hwndCtl, UINT codeNotify)
	{

		if (codeNotify == LBN_SELCHANGE)
		{
			int index = List_->GetItemSelectedIndex();
			std::wstring Selected = List_->GetText(index, 255);

			return FALSE;
		}

		return TRUE;
	}

	BOOL NotifiedCombo(int id, HWND hwndCtl, UINT codeNotify)
	{

		if (codeNotify == CBN_SELCHANGE) {
			//int index = cbComboBox->GetCurSel();

			//cbComboBox->SetText(TEXT("TEST"));
			cbCombo_->SetText(TEXT("Notified"));
			return FALSE;
		}
		if (codeNotify == CBN_SELENDOK)
		{
			cbCombo_->SetText(TEXT("Notified"));
			//int index = cbComboBox->GetCurSel();
			//std::wstring Selected = cbComboBox->GetText();
			return FALSE;
		}


		return TRUE;
	}
	
	LRESULT OnCommand(WPARAM wParam, LPARAM lParam) override {
		int  CmdID = (int)LOWORD(wParam);
		switch (CmdID)
		{
			ON_COMMAND(IDOK, OnOK);
			ON_COMMAND(IDCANCEL, OnCancel);
			ON_COMMAND_NOTIFIED(IDC_LIST1, Notified);
			ON_COMMAND_NOTIFIED(IDC_COMBO1, NotifiedCombo);
		}
		return FALSE;
	}


};


/*
class SimpleDlgModeless : public swktool::DialogWindow {
public:
	SimpleDlgModeless(UINT ID, swktool::BaseWindow* pParent) : DialogWindow(ID, pParent)
	{		
		//swktool::WinEventHandler::GetInst()->PreRegisterDlgWindow(shared);
		hDlg = ::CreateDialog(hInst, DialogTemplateName, hWndParent, swktool::WinEventHandler::ProcDlgMessage);
	}
	~SimpleDlgModeless() {

	}

	virtual int ShowDialog() override
	{
		return ::ShowWindow(hDlg, SW_SHOW);
	}

	
};
*/