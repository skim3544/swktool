#include "pch.h"
#include "framework.h"
#include <Windows.h>
#include <stdint.h>
#include "AWindow.h"
#include "DialogWindow.h"
#include "Window.h"
#include "WinEventHandler.h"
#include "MDIChildWindow.h"
#include "MDIFrameWindow.h"


#include "..\SWKBase\DebugStream.h"

using swktool::DebugOut;


#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#else
#define DBG_NEW new
#endif


namespace swktool {
	class MDIClientWindow;

	/// <summary>
	/// Static varibles
	/// </summary>
	WinEventHandler* WinEventHandler::pInst_ = nullptr;

	std::mutex WinEventHandler::EventLockWindow;
	std::mutex WinEventHandler::EventLockDialog;
	std::mutex WinEventHandler::EventLockModelessDialog;
	
	std::shared_ptr<DialogWindow> WinEventHandler::pPreRegistered = nullptr;
	std::shared_ptr<Window> WinEventHandler::pPreRegisteredWindow = nullptr;
	std::shared_ptr<DialogWindow> WinEventHandler::pPreRegisteredModelessDlg = nullptr;

	std::shared_ptr<MDIChildWindow> WinEventHandler::pPreRegisteredMDIChild = nullptr;
	std::shared_ptr<MDIFrameWindow> WinEventHandler::pPreRegisteredMDIFrame = nullptr;

	bool WinEventHandler::bDebug_ = true; 


	/// <summary>
	///  Singlton Access method
	/// </summary>
	/// <returns></returns>
	WinEventHandler* WinEventHandler::GetInst() {
		// first time call, create
		if (pInst_ == nullptr)
		{
			pInst_ = new WinEventHandler();
		}

		return pInst_;
	}

	std::shared_ptr<DialogWindow>  WinEventHandler::GetPreRegistedDialog() {
		return pPreRegistered;
	}

	std::shared_ptr<DialogWindow>  WinEventHandler::GetPreRegistedModelessDialog() {
		return pPreRegisteredModelessDlg;
	}

	std::shared_ptr<MDIChildWindow>  WinEventHandler::GetPreRegistedMDIClient() {
		return pPreRegisteredMDIChild;
	}





	std::shared_ptr<Window>  WinEventHandler::FindWindowByHandle(HWND hWindow) {
		
		std::lock_guard<std::mutex> lock(EventLockWindow);

		std::shared_ptr<Window> pWindowMsgProcessor = nullptr;

		// walk through the item list, find the owner of the hWnd
		WinMsgProcList List = WinEventHandler::GetInst()->GetMsgProcessorList();
		WinMsgProcListItr pReceiver = std::find_if(List.begin(), List.end(),
			[hWindow](std::shared_ptr<Window>& Other) {
				return (Other->GetWindowHWND() == hWindow); });

		//  if the msg receiver found
		if (pReceiver != List.end())
		{
			pWindowMsgProcessor = *pReceiver;
		}

		return pWindowMsgProcessor;
	}

	
	std::shared_ptr<MDIChildWindow> WinEventHandler::FindMDIChildByHandle(HWND hDlg) {
		std::lock_guard<std::mutex> lock(EventLockWindow);
	
		std::shared_ptr<MDIChildWindow> pWindowMsgProcessor = nullptr;

		WinMDIChildMsgProcList List = WinEventHandler::GetInst()->GetMDIChildProcessorList();
		auto pReceiver = std::find_if(List.begin(), List.end(),
			[hDlg](std::shared_ptr<MDIChildWindow>& Other) {
				return (Other->GetDlgHWND() == hDlg); });

		if (pReceiver != List.end())
		{
			pWindowMsgProcessor = *pReceiver;
		}

		//if (bDebug_) {
		//	DebugOut << "Find HWND : " << std::ios::hex << hDlg << " ptr : " << std::ios::hex << (std::uintptr_t)pWindowMsgProcessor.get() << std::endl;
		//}

		return pWindowMsgProcessor;
	}

	std::shared_ptr<MDIFrameWindow> WinEventHandler::FindMDIFrameByHandle(HWND hDlg) {
		std::lock_guard<std::mutex> lock(EventLockWindow);

		std::shared_ptr<MDIFrameWindow> pWindowMsgProcessor = nullptr;

		WinMDIFrameMsgProcList List = WinEventHandler::GetInst()->GetMDIFrameProcessorList();
		auto pReceiver = std::find_if(List.begin(), List.end(),
			[hDlg](std::shared_ptr<MDIFrameWindow>& Other) {
				return (Other->GetDlgHWND() == hDlg); });

		if (pReceiver != List.end())
		{
			pWindowMsgProcessor = *pReceiver;
		}

		return pWindowMsgProcessor;
	}



	std::shared_ptr<DialogWindow> WinEventHandler::FindModelessDialogByHandle(HWND hDlg) {
		std::lock_guard<std::mutex> lock(EventLockWindow);

		std::shared_ptr<DialogWindow> pWindowMsgProcessor = nullptr;

		ModelessDlgMsgProcList List = WinEventHandler::GetInst()->GetModelessDlgMsgProcessorList();
		ModlessDlgMsgProcListItr pReceiver = std::find_if(List.begin(), List.end(),
			[hDlg](std::shared_ptr<DialogWindow>& Other) {
				return (Other->GetDlgHWND() == hDlg); });
		//  if the msg receiver found

		if (pReceiver != List.end())
		{
			pWindowMsgProcessor = *pReceiver;
		}

		return pWindowMsgProcessor;
	}


	std::shared_ptr<DialogWindow> WinEventHandler::FindDialogByHandle(HWND hDlg) {
		// lock the search process out of other threads
		std::lock_guard<std::mutex> lock(EventLockDialog);

		std::shared_ptr<DialogWindow> pMsgProcessor = nullptr;

		// walk through the item list, find the owner of the hWnd
		DlgMsgProcList& List = WinEventHandler::GetInst()->GetDlgMsgProcessorList();

		auto pReceiver = std::find_if(List.begin(), List.end(),
			[hDlg](std::shared_ptr<DialogWindow>& Other) {
				return (Other->GetDlgHWND() == hDlg); });

		//  if the msg receiver found
		if (pReceiver != List.end())
		{
			pMsgProcessor = *pReceiver;
		}

		return pMsgProcessor;
	 }



	LRESULT WinEventHandler::ProcWindowMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		bool bProcessed = false;
		LRESULT    lResult = 0;
		{
			// to start, we don't know which window this message belongs to
			std::shared_ptr<Window> MsgProcessor = nullptr;

			// If Window Create called
			if (msg == WM_NCCREATE) 
			{				
				if (pPreRegisteredWindow != nullptr)
				{
					// register the Window
					MsgProcessor = pPreRegisteredWindow;

					WinEventHandler::GetInst()->RegisterWindow(pPreRegisteredWindow);

					// Clear preregister Window
					WinEventHandler::GetInst()->PreRegisterWindow(nullptr);					
					{						
						MsgProcessor->SetWindowHandle(hWnd);
						bProcessed = true;

						// Route the WM_CREATE message
						lResult = MsgProcessor->ProcessMessage(msg, wParam, lParam);
					}
				}
				else {
					// Someone Constructing Window without preregistring the pointer?
					assert(FALSE);
				}
			}
			else {
				// Find the Window responsible for handling this message
				MsgProcessor = FindWindowByHandle(hWnd);
				if (MsgProcessor != nullptr)
				{
					// Route the message to the proper window handler
					bProcessed = true;
					lResult = MsgProcessor->ProcessMessage(msg, wParam, lParam);

					// if last message of THIS window, then terminate
					if (msg == WM_NCDESTROY) {
						WinEventHandler::GetInst()->UnRegisterWindow(hWnd);
					}
				}				
			}											
		}

		// if no one processed the msg, ask default window proc to process it
		if (bProcessed == false) {
			lResult = ::DefWindowProc(hWnd, msg, wParam, lParam);
		}
		return lResult;
	}


	/// <summary>
	/// Message processor for Dialog Messages
	/// </summary>
	/// <param name="hWND"></param>
	/// <param name="msg"></param>
	/// <param name="wParam"></param>
	/// <param name="lParam"></param>
	/// <returns></returns>
	INT_PTR CALLBACK WinEventHandler::ProcDialogMessage(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
	{		
		std::shared_ptr<DialogWindow> MsgProcessor = nullptr;
		LRESULT    Result = 1;
		{			
			MsgProcessor = FindDialogByHandle(hDlg);
			
			// If the receiver of the message was identified, call its message handler
			if (MsgProcessor != nullptr) {
				Result = MsgProcessor->ProcessMessage(hDlg, msg, wParam, lParam);

				if (msg == WM_NCDESTROY) {
					WinEventHandler::GetInst()->UnRegisterDlgWindow(hDlg);			
				}
			}
			else {
				if (pPreRegistered != nullptr) {
					MsgProcessor = pPreRegistered;
					// If there was a preregistered Dialog, use it to create the object
					pPreRegistered->SetDialogHandle(hDlg);
					WinEventHandler::GetInst()->RegisterDlgWindow(pPreRegistered);
					pPreRegistered = nullptr;

					// Process THIS message only, starting with the next one it will be handled properly
					Result = MsgProcessor->ProcessMessage(hDlg, msg, wParam, lParam);
				}
			}
		}

		return Result;
	}

	// Adds the Window into the list to receive the message
	void WinEventHandler::RegisterWindow(std::shared_ptr<Window> pWindow)
	{
		std::lock_guard<std::mutex> lock(EventLockWindow);
		MessageProcessors.push_back(pWindow);

		
	}

	void WinEventHandler::RegisterMDIFrameWindow(std::shared_ptr<MDIFrameWindow> pWindow) {
		std::lock_guard<std::mutex> lock(EventLockWindow);
		MDIFrameProcessors_.push_back(pWindow);

		if (bDebug_) 
			DebugOut << "MDIRegistering : " << std::ios::hex << pWindow->GetWindowHWND() 
			<< " ptr : " << std::ios::hex << (std::uintptr_t)pWindow.get() << std::endl;
	}

	void WinEventHandler::RegisterMDIChildWindow(std::shared_ptr<MDIChildWindow> pWindow) {
		std::lock_guard<std::mutex> lock(EventLockWindow);
		MDIChildProcessors_.push_back(pWindow);

		if (bDebug_)
			DebugOut << "MDI Child Registering : " << std::ios::hex << pWindow->GetWindowHWND()
			<< " ptr : " << std::ios::hex << (std::uintptr_t)pWindow.get() << std::endl;
	}

	void WinEventHandler::UnRegisterWindow(HWND hWnd)
	{
		{
			std::lock_guard<std::mutex> lock(EventLockWindow);

			// walk through the item list, find the owner of the hWnd
			WinMsgProcList& List = WinEventHandler::GetInst()->GetMsgProcessorList();
			WinMsgProcListItr pReceiver = std::find_if(List.begin(), List.end(),
				[hWnd](std::shared_ptr<Window>& Other) {
					return (Other->GetWindowHWND() == hWnd); });

			//  if the msg receiver found
			if (pReceiver != List.end())
			{
				List.erase(pReceiver);
			}
		}
	}


	void WinEventHandler::UnRegisterMDIChildWindow(HWND hWnd)
	{
		{
			std::lock_guard<std::mutex> lock(EventLockWindow);

			// walk through the item list, find the owner of the hWnd
			WinMDIChildMsgProcList& List = WinEventHandler::GetInst()->GetMDIChildProcessorList();
			WinMDIChildMsgProcListItr pReceiver = std::find_if(List.begin(), List.end(),
				[hWnd](std::shared_ptr<MDIChildWindow>& Other) {
					return (Other->GetWindowHWND() == hWnd); });

			//  if the msg receiver found
			if (pReceiver != List.end())
			{
				if (bDebug_) DebugOut
					<< "MDI Child UNRegistering : " << std::ios::hex << hWnd << std::endl;
				List.erase(pReceiver);
			}
		}
	}

	void WinEventHandler::UnRegisterMDIFrameWindow(HWND hWnd)
	{
		{
			std::lock_guard<std::mutex> lock(EventLockWindow);

			// walk through the item list, find the owner of the hWnd
			WinMDIFrameMsgProcList& List = WinEventHandler::GetInst()->GetMDIFrameProcessorList();
			WinMDIFrameMsgProcListItr pReceiver = std::find_if(List.begin(), List.end(),
				[hWnd](std::shared_ptr<MDIFrameWindow>& Other) {
					return (Other->GetWindowHWND() == hWnd); });

			//  if the msg receiver found
			if (pReceiver != List.end())
			{
				if (bDebug_) DebugOut
					<< "MDIUNRegistering : " << std::ios::hex << hWnd << std::endl;
				List.erase(pReceiver);
			}
		}
	}


	// Register Window to receive the message
	void WinEventHandler::PreRegisterDlgWindow(std::shared_ptr<DialogWindow> pDialog)
	{
		// should be controlled by Semiphore as we have storage for only 1
		std::lock_guard<std::mutex> lock(EventLockDialog);
		pPreRegistered = pDialog;

		//if (pDialog != nullptr) {
		//	std::wstring DialogHandle = TEXT("PreRegistering: ") + std::to_wstring((int)pDialog->GetDialogHandle());
		//	OutputDebugString(DialogHandle.c_str());
		//}
	}

	void WinEventHandler::PreRegisterModelessDlgWindow(std::shared_ptr<DialogWindow> pDialog)
	{
		pPreRegisteredModelessDlg = pDialog;
	}

	void WinEventHandler::PreRegisterWindow(std::shared_ptr<Window> pDialog) {
		std::lock_guard<std::mutex> lock(EventLockDialog);
		pPreRegisteredWindow = pDialog;
	}

	void WinEventHandler::PreRegisterMDIChildWindow(std::shared_ptr<MDIChildWindow> pMDIChildWindow) {
		std::lock_guard<std::mutex> lock(EventLockDialog);
		pPreRegisteredMDIChild = pMDIChildWindow;

		if (bDebug_&& pMDIChildWindow != nullptr) {
			HWND hWnd = pMDIChildWindow->GetWindowHWND();
			DebugOut << "Preregistering: " << std::ios::hex << (std::uintptr_t)pMDIChildWindow.get() << std::endl;
		}
	}


	void WinEventHandler::PreRegisterMDIFramedWindow(std::shared_ptr<MDIFrameWindow> pMDIFrameWindow) {
		std::lock_guard<std::mutex> lock(EventLockDialog);
		pPreRegisteredMDIFrame = pMDIFrameWindow;

		if (bDebug_ && pMDIFrameWindow != nullptr) {		
			DebugOut << "Preregistering: " << std::ios::hex << (std::uintptr_t)pMDIFrameWindow.get() << std::endl;
		}
	}



	void WinEventHandler::RegisterDlgWindow(std::shared_ptr<DialogWindow> pDialog) {
		std::lock_guard<std::mutex> lock(EventLockDialog);

		DialogProcessors.push_back(pDialog);		

		//if (pDialog != nullptr) {
		//	std::wstring DialogHandle = TEXT("Registering: ") + std::to_wstring((int)pDialog->GetDialogHandle());
		//	OutputDebugString(DialogHandle.c_str());
		//}
	}

	void WinEventHandler::UnRegisterModelessDlgWindow(HWND hWnd) {
		{
			std::lock_guard<std::mutex> lock(EventLockDialog);

			ModelessDlgMsgProcList& List = WinEventHandler::GetInst()->GetModelessDlgMsgProcessorList();
			auto pReceiver = std::find_if(List.begin(), List.end(),
				[hWnd](std::shared_ptr<DialogWindow>& Other) {
					return (Other->GetDlgHWND() == hWnd); });
			//  if the msg receiver found
			if (pReceiver != List.end())
			{
				List.erase(pReceiver);
			}
		}
	}

	void WinEventHandler::UnRegisterDlgWindow(HWND hWnd) {		
		{
			std::lock_guard<std::mutex> lock(EventLockDialog);

			DlgMsgProcList& List = WinEventHandler::GetInst()->GetDlgMsgProcessorList();
			auto pReceiver = std::find_if(List.begin(), List.end(),
				[hWnd](std::shared_ptr<DialogWindow>& Other) {
					return (Other->GetDlgHWND() == hWnd); });
			//  if the msg receiver found
			if (pReceiver != List.end())			
			{							
				//BaseDlgPtr pDialog = *pReceiver;
				//std::wstring DialogHandle = TEXT("UnRegistering: ") + std::to_wstring((int)pDialog->GetDialogHandle());
				//OutputDebugString(DialogHandle.c_str());

				List.erase(pReceiver);
			}		
		}
	}

	BOOL       WinEventHandler::IsDialogMessage(MSG* pMsg) {

		ModelessDlgMsgProcList& List = WinEventHandler::GetInst()->GetModelessDlgMsgProcessorList();
		BOOL Result = FALSE;

		if (!List.empty()) {
			std::vector<HWND>  HwndList;

			{   
				std::lock_guard<std::mutex> lock(EventLockDialog);
	
				// Build list of HWND for all the Modeless Dialog
				auto SaveWinhandle = [&HwndList](std::shared_ptr<DialogWindow>& Other) {
					HWND hWin = Other->GetDlgHWND();
					HwndList.push_back(hWin); };

				std::for_each(List.begin(), List.end(), SaveWinhandle);
			}

			//  For all the collected HWNDm call IsDialogMessage
			std::vector<HWND>::iterator itr = std::find_if(HwndList.begin(), HwndList.end(), [pMsg](HWND Data) {
				return (Data != nullptr) ? ::IsDialogMessage(Data, pMsg) : FALSE;
				});
			if (itr != HwndList.end()) {
				Result = TRUE;
			}
		}

		return Result;				
	}


	INT_PTR CALLBACK WinEventHandler::ProcModelessDlgMessage(HWND hWND, UINT msg, WPARAM wParam, LPARAM lParam) {

		std::shared_ptr<DialogWindow> MsgProcessor = nullptr;
		LRESULT    Result = 1;
		{
			MsgProcessor = FindModelessDialogByHandle(hWND);

			// If the receiver of the message was identified, call its message handler
			if (MsgProcessor != nullptr) {
				Result = MsgProcessor->ProcessMessage(hWND, msg, wParam, lParam);
				if (msg == WM_CLOSE) {
					WinEventHandler::GetInst()->UnRegisterModelessDlgWindow(hWND);
				}
			}
			else {
				if (pPreRegisteredModelessDlg != nullptr) {
					MsgProcessor = pPreRegisteredModelessDlg;
					// If there was a preregistered Dialog, use it to create the object
					pPreRegistered->SetDialogHandle(hWND);
					//WinEventHandler::GetInst()->RegisterDlgWindow(pPreRegisteredModelessDlg);
					pPreRegisteredModelessDlg = nullptr;

					// Process THIS message only, starting with the next one it will be handled properly
					Result = MsgProcessor->ProcessMessage(hWND, msg, wParam, lParam);
				}
			}
		}

		return Result;

	}


	void WinEventHandler::DbgShowChainedDlgHandles() {
		DlgMsgProcList& List = WinEventHandler::GetInst()->GetDlgMsgProcessorList();

		std::wstringstream out;
		std::wstring label = TEXT("Dialog Handles: ");

		out << label << std::ends;
		for (std::shared_ptr<DialogWindow> p : List) {

			HWND DialogHandle = (HWND)p->GetWindowHWND();
			out << std::hex << DialogHandle << std::endl;

			OutputDebugString(out.str().c_str());
		}
	}


	LRESULT WinEventHandler::ProcMDIFrameMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		bool bProcessed = false;
		LRESULT    lResult = 0;
		{
			// to start, we don't know which window this message belongs to
			std::shared_ptr<MDIFrameWindow> MsgProcessor = FindMDIFrameByHandle(hWnd);
			if (MsgProcessor != nullptr) {
				// Route the message to the proper window handler
				bProcessed = true;
				lResult = MsgProcessor->ProcessMessage(msg, wParam, lParam);

				// if last message of THIS window, then terminate
				if (msg == WM_NCDESTROY) {
					WinEventHandler::GetInst()->UnRegisterMDIFrameWindow(hWnd);
				}
			}
			else {
				if (pPreRegisteredMDIFrame != nullptr) {
					// register the Window
					MsgProcessor = pPreRegisteredMDIFrame;
					WinEventHandler::GetInst()->RegisterMDIFrameWindow(pPreRegisteredMDIFrame);

					// Clear preregister Window
					WinEventHandler::GetInst()->PreRegisterMDIFramedWindow(nullptr);
					{
						MsgProcessor->SetWindowHandle(hWnd);
						bProcessed = true;

						// Route the message
						lResult = MsgProcessor->ProcessMessage(msg, wParam, lParam);
					}
				}
				else {
					assert(FALSE);
				}			
			}
		}

		// if no one processed the msg, ask default window proc to process it
		if (bProcessed == false) {
			lResult = ::DefFrameProc(hWnd, NULL, msg, wParam, lParam);
		}
		return lResult;
	}



	LRESULT WinEventHandler::ProcMDIChildMessage(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		bool bProcessed = false;
		LRESULT    lResult = 0;
		{
			// to start, we don't know which window this message belongs to
			std::shared_ptr<MDIChildWindow> MsgProcessor = FindMDIChildByHandle(hWnd);

			// if found the message responder, rout the message to the processor
			if (MsgProcessor != nullptr) {
				bProcessed = true;
				lResult = MsgProcessor->ProcessMessage(msg, wParam, lParam);

				// if last message of THIS window, then terminate
				if (msg == WM_NCDESTROY) {
					if (bDebug_) DebugOut << "MDIChild Destroying: " << std::ios::hex << hWnd << std::endl;
					WinEventHandler::GetInst()->UnRegisterMDIChildWindow(hWnd);
				}
			}
			else
			{

				if (pPreRegisteredMDIChild != nullptr)
				{
					if (bDebug_) DebugOut << "MDIChild Creating: " << std::ios::hex << hWnd << std::endl;

					// copy
					MsgProcessor = pPreRegisteredMDIChild;

					MsgProcessor->SetWindowHandle(hWnd);

					// Clear preregister Window
					WinEventHandler::GetInst()->PreRegisterMDIChildWindow(nullptr);

					// register the Window
					WinEventHandler::GetInst()->RegisterMDIChildWindow(MsgProcessor);

					bProcessed = true;

					// Route the WM_CREATE message
					lResult = MsgProcessor->ProcessMessage(msg, wParam, lParam);
				}
				else {
					// Someone Constructing Window without preregistring the pointer?
					assert(FALSE);
				}
			}
		}
		// if no one processed the msg, ask default window proc to process it
		if (bProcessed == false) {			
			lResult = ::DefMDIChildProc(hWnd, msg, wParam, lParam);
		}
		return lResult;
	}

}