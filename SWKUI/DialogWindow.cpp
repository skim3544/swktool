#include "pch.h"
#include "Msg_filter.h"
#include "MsgHandler.h"
#include "DialogWindow.h"
#include "..\SWKBase\DebugStream.h"

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#else
#define DBG_NEW new
#endif

namespace swktool
{
	DialogWindow::DialogWindow(UINT ID, IWindow* pParent) :
		parentWindow_(pParent)
	{
		m_ResourceID = ID;
		// convert ID to template name			
		DialogTemplateName = MAKEINTRESOURCE(ID);
		if (pParent)
		{
			//SetParent(pParent->GetHwnd());			
			//SetInstance(pParent->GetInstance());
		}
	}


	DialogWindow::DialogWindow(LPCTSTR pDialogTemplateName, IWindow* pParent) :
		m_ResourceID(0),
		parentWindow_(pParent)
	{

	}


	INT_PTR DialogWindow::OnInitDialog(HWND hwndFocusedCtrl, LPARAM lParam)	
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
		//::ShowWindow(GetHwnd(), SW_SHOW);
		//::UpdateWindow(GetHwnd());

		// if caption set, send ir over
		if (cwCaption.length() > 0)
			SetWindowText(GetHwnd(), cwCaption.c_str());

		return (INT_PTR)TRUE;
	}

	LRESULT DialogWindow::OnCommand(WORD id, WORD code, HWND control)
	{
		if (id == IDOK || id == IDCANCEL)
		{
			EndDialog(GetHwnd(), id);
			return TRUE;
		}

		return (LRESULT)FALSE;
	}


	INT_PTR CALLBACK SWKDialogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	INT_PTR DialogWindow::ShowDialog()
	{
		//const bool bDebug = false;
		//if (bDebug) {
		//	DebugOut << std::hex << "THIS: " << this
		//		<< std::hex << " Inst: " << m_hInstance
		//		<< std::hex << " Res: " << m_ResourceID
		//		<< std::hex << " Parent: " << m_hParent
		//		<< std::hex << " Addr : " << lpProc << std::endl;
		//}

		HWND hParent = parentWindow_ ? parentWindow_->GetHwnd() : nullptr;

		// start the message pumping, for model dialog box this call will not return until EndDialog gets called
		auto retval = DialogBoxParam(
			GetInstance(),
			DialogTemplateName ? DialogTemplateName : MAKEINTRESOURCE(m_ResourceID),
			hParent,
			SWKDialogProc,
			reinterpret_cast<LPARAM>(this));

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
