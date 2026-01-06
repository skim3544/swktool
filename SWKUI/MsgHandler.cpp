#include "pch.h"
#include "MsgHandler.h"
#include "../SWKBase/DebugStream.h"
#include <iomanip>

namespace swktool 
{
	// For regular windows
	LRESULT CALLBACK SWKWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) 
	{
		
		const bool bDebug = false; // set this true to 

		if (bDebug) 
		{
			std::stringstream os;
			os << "HWND : 0x" << std::uppercase << std::setw(8) << std::setfill('0') << std::hex << hwnd;
			os << " msg : 0x" << std::uppercase << std::setw(8) << std::setfill('0') << std::hex << msg;
			os << " wParam : 0x" << std::uppercase << std::setw(8) << std::setfill('0') << std::hex << wParam;
			os << " lParam : 0x" << std::uppercase << std::setw(8) << std::setfill('0') << std::hex << lParam;
			DebugStreamA  out;

			out << os.str() << std::endl;
		}

		if (msg == WM_NCCREATE) {
			auto* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
			auto* handler = static_cast<IWindowHandler*>(cs->lpCreateParams);
			handler->SetHwnd(hwnd);
			WindowHandlerRegistry::Instance().RegisterHandler(hwnd, handler);
		} else if (msg == WM_NCDESTROY) {
			WindowHandlerRegistry::Instance().UnregisterHandler(hwnd);
		}

		if (auto* handler = WindowHandlerRegistry::Instance().GetHandler(hwnd)) {
			return handler->HandleMessage(msg, wParam, lParam);
		}

		return DefWindowProc(hwnd, msg, wParam, lParam);
	}



	// Pre‑registered dialog handler waiting for its HWND.
	inline thread_local IWindowHandler* g_pendingDialogHandler = nullptr;

	INT_PTR CALLBACK SWKDialogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) 
	{
		if (msg == WM_INITDIALOG) 
		{
			// lParam usually carries user data (often a pointer)
			auto* handler = reinterpret_cast<IWindowHandler*>(lParam);
			g_pendingDialogHandler = handler;
			handler->SetHwnd(hwnd);
			WindowHandlerRegistry::Instance().RegisterHandler(hwnd, handler);

			// Clear pending handler — it is no longer pending
			g_pendingDialogHandler = nullptr;
		}
		else if (msg == WM_NCDESTROY) 
		{
			WindowHandlerRegistry::Instance().UnregisterHandler(hwnd);
		}

		if (auto* handler = WindowHandlerRegistry::Instance().GetHandler(hwnd)) {
			LRESULT result = handler->HandleMessage(msg, wParam, lParam);
			// For dialogs, nonzero means message handled.
			return (msg == WM_COMMAND || msg == WM_INITDIALOG) ? static_cast<INT_PTR>(result) : result;
		}

		return FALSE;
	}

	
	//LRESULT CALLBACK WindowsMsgHandler::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	//{
	//	WindowsMsgHandler* pThis = nullptr;
	//	LRESULT lRetResult = 0;

	//	const bool bDebug = false;
	//	if (bDebug) {
	//		std::stringstream os;
	//		os << "HWND : 0x" << std::uppercase << std::setw(8) << std::setfill('0') << std::hex << hwnd;
	//		os << " msg : 0x" << std::uppercase << std::setw(8) << std::setfill('0') << std::hex << uMsg;
	//		os << " wParam : 0x" << std::uppercase << std::setw(8) << std::setfill('0') << std::hex << wParam;
	//		os << " lParam : 0x" << std::uppercase << std::setw(8) << std::setfill('0') << std::hex << lParam;
	//		DebugStreamA  out;

	//		out << os.str() << std::endl;
	//	}

	//	if (uMsg == WM_NCCREATE)
	//	{
	//		CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
	//		pThis = static_cast<WindowsMsgHandler*>(pCreate->lpCreateParams);

	//		// Store pointer in window's user data 
	//		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));

	//		// 
	//		// set HWND and Instance handle to the create structure
	//		pThis->m_hwnd = hwnd;
	//		pThis->m_hInstance = pCreate->hInstance;

	//		// into the data structure				
	//		WindowMsgHandlerPair pair(hwnd, pThis);
	//		m_MsgHandlerList.push_back(pair);
	//	}
	//	else
	//	{
	//		// find the object responsible for handling the message
	//		auto pMsgReceiver = std::find_if(m_MsgHandlerList.begin(), m_MsgHandlerList.end(), [&](WindowMsgHandlerPair& Other) {
	//			return (Other.GetHwnd() == hwnd); });

	//		//  if the msg receiver found
	//		if (pMsgReceiver != m_MsgHandlerList.end())
	//		{
	//			pThis = pMsgReceiver->GetHandler();
	//		}
	//	}

	//	// if we have a handler, rout the message to the handler, otherwise going into default processor
	//	if (pThis)
	//	{
	//		lRetResult =  pThis->HandleMessage(uMsg, wParam, lParam);
	//	}
	//	else
	//	{
	//		lRetResult = DefWindowProc(hwnd, uMsg, wParam, lParam);
	//	}

	//	return lRetResult;
	//}






	//DialogMsgHandler::MsgHandlerList DialogMsgHandler::m_MsgHandlerList;
	//CriticalSection DialogMsgHandler::cs_;


	/// <summary>
	/// Pre-registered window is a message process handler that has hwnd of 0
	/// HWND is not known until Windows starts sending the first message (WM_SETFONT for dialog boxes)
	/// </summary>
	/// <returns></returns>
	//DialogMsgHandlerPair* DialogMsgHandler::FindPreRegistered() 
	//{
	//	DialogMsgHandlerPair* pPairPtr = nullptr;

	//	// if not in data structure, find the preregisted data & Update
	//	auto pRegisteredReceiverItr = std::find_if(m_MsgHandlerList.begin(), m_MsgHandlerList.end(), [&](DialogMsgHandlerPair& Other) {
	//		return (Other.GetHwnd() == 0); });

	//	if (pRegisteredReceiverItr != m_MsgHandlerList.end()) {
	//		DialogMsgHandlerPair& Pair = *pRegisteredReceiverItr;
	//		pPairPtr = &Pair;
	//	}

	//	return pPairPtr;
	//}

	/// <summary>
	/// Dialog Message processor
	/// </summary>
	/// <param name="hwnd">Handle of Dialog Window</param>
	/// <param name="uMsg">Message ID</param>
	/// <param name="wParam">wParam</param>
	/// <param name="lParam">lParam</param>
	/// <returns></returns>
//	INT_PTR CALLBACK DialogMsgHandler::DialogMsgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
//		// debug incoming message
//		bool bDebug = false;
//
//		DialogMsgHandler* pMsgHandler = nullptr;
//		if (bDebug) {
//			std::stringstream os;
//			os << "Dialog hWnd : 0x" << std::uppercase << std::setw(8) << std::setfill('0') << std::hex << hwnd;
//			os << " msg : 0x" << std::uppercase << std::setw(8) << std::setfill('0') << std::hex << uMsg;
//			os << " wParam : 0x" << std::uppercase << std::setw(8) << std::setfill('0') << std::hex << wParam;
//			os << " lParam : 0x" << std::uppercase << std::setw(8) << std::setfill('0') << std::hex << lParam;
//			DebugStreamA  out;
//
//			out << os.str() << std::endl;
//		}
//
//		// very first message sent to the dialog is WM_SETFONT
//		if (uMsg == WM_SETFONT) 
//		{
//			CS_LockGuard guard(cs_);
//			auto pPair = FindPreRegistered();
//			if (pPair)
//			{
//				// this is the Handle of Window that goes with this preregistered object
//				pPair->SetWindowHandle(hwnd);
//				// this is the message receiver to use
//				pMsgHandler = pPair->GetHandler();
//				if (pMsgHandler) {
//					pMsgHandler->SetWindowHandle(hwnd);
//				}
//			}
//			else 
//			{
//				// no preregisted Dialog object
//				assert(0);
//				return FALSE;
//			}
//		}
//		else if (uMsg == WM_NCDESTROY) {
//			CS_LockGuard guard(cs_);
//
//			// find the object responsible for handling the message
//			auto pMsgReceiverItr = std::find_if(m_MsgHandlerList.begin(), m_MsgHandlerList.end(), [&](DialogMsgHandlerPair& Other) {
//				return (Other.GetHwnd() == hwnd); });
//			//  if the msg receiver found
//			if (pMsgReceiverItr != m_MsgHandlerList.end())
//			{
//				m_MsgHandlerList.erase(pMsgReceiverItr);
//			}
//		}			
//		else {
//			CS_LockGuard guard(cs_);
//			// find the object responsible for handling the message
//			auto pMsgReceiverItr = std::find_if(m_MsgHandlerList.begin(), m_MsgHandlerList.end(), [&](DialogMsgHandlerPair& Other) {
//				return (Other.GetHwnd() == hwnd); });
//
//			//  if the msg receiver found
//			if (pMsgReceiverItr != m_MsgHandlerList.end())
//			{
//				DialogMsgHandlerPair& Pair = *pMsgReceiverItr;
//				pMsgHandler = Pair.GetHandler();
//
//			}
//		}
//
//		if (pMsgHandler == nullptr) {
//			// into the data structure				
//			//DialogMsgHandlerPair pair(hwnd, pMsgHandler);
//			//m_MsgHandlerList.push_back(pair);
//			//assert(0);
//			return FALSE;
//		}
//		else
//		{
//			return pMsgHandler->HandleMessage(uMsg, wParam, lParam);
//		}
//	}

}
