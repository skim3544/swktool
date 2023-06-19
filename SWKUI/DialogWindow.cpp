#include "pch.h"
#include "DialogWindow.h"
#include "..\SWKBase\DebugStream.h"

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#else
#define DBG_NEW new
#endif

namespace swktool {
	DialogWindow::DialogWindow(UINT ID, Window* pParent) :
		DialogMsgHandler()
	{
		m_ResourceID = ID;
		// convert ID to template name			
		DialogTemplateName = MAKEINTRESOURCE(ID);
		if (pParent) {
			m_hParent = pParent->WindowHandle();
			m_hInstance = pParent->GetInstance();
		}
	}


	DialogWindow::DialogWindow(LPCTSTR pDialogTemplateName, Window* pParent) :
		DialogMsgHandler()
		, m_ResourceID(0)
	{
		if (pParent)
		{
			m_hParent = pParent->WindowHandle();
			m_hInstance = pParent->GetInstance();
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
		::ShowWindow(m_hwnd, SW_SHOW);
		::UpdateWindow(m_hwnd);

		// if caption set, send ir over
		if (cwCaption.length() > 0)
			SetWindowText(m_hwnd, cwCaption.c_str());		

		return (INT_PTR)TRUE;
	}

	INT_PTR DialogWindow::OnCommand(WPARAM wParam, LPARAM lParam) 
	{
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
			EndDialog(m_hwnd, LOWORD(wParam));

		return (INT_PTR)TRUE;
	}

	INT_PTR DialogWindow::OnClose() 
	{
		::EndDialog(m_hwnd, 0L);

		return (INT_PTR)TRUE;
	}


	INT_PTR DialogWindow::ShowDialog()
	{		
		// register THIS dialog box to receive next Model dialog box message
		PreRegisterClass(this);

		//auto lpProc = (DLGPROC)(& DialogMsgHandler::DialogMsgProc);

		//DebugOut << std::hex << "THIS: " << this
		//	<< std::hex << " Inst: " << m_hInstance
		//	<< std::hex << " Res: " << m_ResourceID
		//	<< std::hex << " Parent: " << m_hParent
		//	<< std::hex << " Addr : " << lpProc << std::endl;

		// start the message pumping, for model dialog box this call will not return until EndDialog gets called
		auto retval = ::DialogBox(
			m_hInstance,
			//DialogTemplateName,
			MAKEINTRESOURCE(m_ResourceID),
			m_hParent,
			(DLGPROC)DialogMsgHandler::DialogMsgProc);

		auto Lasterror = GetLastError();
		return retval;
	}


	INT_PTR DialogWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
			//PROC_DLG_MSG(m_hwnd, WM_SETFONT, OnSetFont);
			PROC_DLG_MSG(m_hwnd, WM_INITDIALOG, OnInitDialog);
			PROC_DLG_MSG(m_hwnd, WM_COMMAND, OnCommand);
			PROC_DLG_MSG(m_hwnd, WM_CLOSE, OnClose);
		}

		return (INT_PTR)FALSE;
	}

} // namespace swktool
