#include "pch.h"
#include "DialogWindow.h"
#include "..\SWKBase\DebugStream.h"

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#else
#define DBG_NEW new
#endif

namespace swktool 
{
	DialogWindow::DialogWindow(UINT ID, Window* pParent)
	{
		m_ResourceID = ID;
		// convert ID to template name			
		DialogTemplateName = MAKEINTRESOURCE(ID);
		if (pParent) 
		{
			SetParent(pParent->GetHwnd());			
			SetInstance(pParent->GetInstance());
		}
	}


	DialogWindow::DialogWindow(LPCTSTR pDialogTemplateName, Window* pParent) :
		m_ResourceID(0)
	{
		if (pParent)
		{
			SetParent(pParent->GetHwnd());
			SetInstance(pParent->GetInstance());
		}
	}



	INT_PTR  DialogWindow::OnInitDialog(HWND hwndFocusedCtrl, LPARAM lParam) 
	{

		// Dialog template style detection does not work, always force it to visible unless the caller just does not call
		// Base class OnInitDialg
		// 
		//auto  dwStyle = GetWindowLong(m_hwnd, GWL_STYLE);
		//const auto DlgVisible = (dwStyle & WS_VISIBLE);

		//if (!DlgVisible) {
		//	DebugOut << "Dialog Box template not visible.  Forcing it to be visible" << std::endl;
		//	::ShowWindow(m_hwnd, SW_SHOW);
		//	::UpdateWindow(m_hwnd);
		//}

		// Always show
		::ShowWindow(GetHwnd(), SW_SHOW);
		::UpdateWindow(GetHwnd());

		// if caption set, send ir over
		if (cwCaption.length() > 0)
			SetWindowText(GetHwnd(), cwCaption.c_str());

		return (INT_PTR)TRUE;
	}

	INT_PTR DialogWindow::OnCommand(WPARAM wParam, LPARAM lParam) 
	{
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
			EndDialog(GetHwnd(), LOWORD(wParam));

		return (INT_PTR)TRUE;
	}

	INT_PTR DialogWindow::OnClose() 
	{
		::EndDialog(GetHwnd(), 0L);

		return (INT_PTR)TRUE;
	}

	INT_PTR CALLBACK SWKDialogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	INT_PTR DialogWindow::ShowDialog()
	{		
		// register THIS dialog box to receive next Model dialog box message
		//PreRegisterClass(this);

		//auto lpProc = (DLGPROC)(& DialogMsgHandler::DialogMsgProc);

		//DebugOut << std::hex << "THIS: " << this
		//	<< std::hex << " Inst: " << m_hInstance
		//	<< std::hex << " Res: " << m_ResourceID
		//	<< std::hex << " Parent: " << m_hParent
		//	<< std::hex << " Addr : " << lpProc << std::endl;
		

		DialogWindow dlgHandler;
		dlgHandler.SetInstance( GetInstance() );

		// start the message pumping, for model dialog box this call will not return until EndDialog gets called
		auto retval = DialogBoxParam(
			GetInstance(),
			MAKEINTRESOURCE(m_ResourceID),
			GetParent(),
			SWKDialogProc,
			reinterpret_cast<LPARAM>(&dlgHandler));

		auto Lasterror = GetLastError();
		return retval;
	}


	INT_PTR DialogWindow::OnDialogMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
			//PROC_DLG_MSG(m_hwnd, WM_SETFONT, OnSetFont);
			PROC_DLG_MSG(GetHwnd(), WM_INITDIALOG, OnInitDialog);
			PROC_DLG_MSG(GetHwnd(), WM_COMMAND, OnCommand);
			PROC_DLG_MSG(GetHwnd(), WM_CLOSE, OnClose);
		}

		return (INT_PTR)FALSE;
	}

} // namespace swktool
