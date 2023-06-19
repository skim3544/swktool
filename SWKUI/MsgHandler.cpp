#include "pch.h"
#include "MsgHandler.h"
#include "../SWKBase/DebugStream.h"
#include <iomanip>

namespace swktool {
    WindowsMsgHandler::MsgHandlerList  WindowsMsgHandler::m_MsgHandlerList;
	DialogMsgHandler::MsgHandlerList DialogMsgHandler::m_MsgHandlerList;

	LRESULT CALLBACK WindowsMsgHandler::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		WindowsMsgHandler* pThis = nullptr;

		if (uMsg == WM_NCCREATE)
		{
			CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
			pThis = static_cast<WindowsMsgHandler*>(pCreate->lpCreateParams);
			pThis->m_hwnd = hwnd;
			pThis->m_hInstance = pCreate->hInstance;

			// into the data structure				
			WindowMsgHandlerPair pair(hwnd, pThis);
			m_MsgHandlerList.push_back(pair);
		}
		else
		{
			// find the object responsible for handling the message
			auto pMsgReceiver = std::find_if(m_MsgHandlerList.begin(), m_MsgHandlerList.end(), [&](WindowMsgHandlerPair& Other) {
				return (Other.GetWindowHandle() == hwnd); });

			//  if the msg receiver found
			if (pMsgReceiver != m_MsgHandlerList.end())
			{
				pThis = pMsgReceiver->GetHandler();
			}
		}

		// if we have a handler, rout the message to the handler, otherwise going into default processor
		if (pThis)
		{
			return pThis->HandleMessage(uMsg, wParam, lParam);
		}
		else
		{
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
	}

	DialogMsgHandlerPair* DialogMsgHandler::FindPreRegistered() 
	{
		DialogMsgHandlerPair* pPairPtr = nullptr;

		// if not in data structure, find the preregisted data & Update
		auto pRegisteredReceiverItr = std::find_if(m_MsgHandlerList.begin(), m_MsgHandlerList.end(), [&](DialogMsgHandlerPair& Other) {
			return (Other.GetWindowHandle() == 0); });

		if (pRegisteredReceiverItr != m_MsgHandlerList.end()) {
			DialogMsgHandlerPair& Pair = *pRegisteredReceiverItr;
			pPairPtr = &Pair;
		}

		return pPairPtr;
	}

	INT_PTR CALLBACK DialogMsgHandler::DialogMsgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		static bool bDebug = true;

		DialogMsgHandler* pMsgHandler = nullptr;
		if (bDebug) {
			std::stringstream os;
			os << std::ios::hex << hwnd;
			os << " msg : 0x" << std::uppercase << std::setfill('0') << std::hex << uMsg;
			os << " wParam : 0x" << std::uppercase << std::setfill('0') << std::hex << wParam;
			os << " lParam : 0x" << std::uppercase << std::setfill('0') << std::hex << lParam;
			DebugStreamA  out;

			out << os.str() << std::endl;
		}

		if (uMsg == WM_SETFONT) 
		{
			auto pPair = FindPreRegistered();
			if (pPair)
			{
				// this is the Handle of Window that goes with this preregistered object
				pPair->SetWindowHandle(hwnd);
				// this is the message receiver to use
				pMsgHandler = pPair->GetHandler();
				if (pMsgHandler) {
					pMsgHandler->SetWindowHandle(hwnd);
				}
			}
			else 
			{
				// no preregisted Dialog object
				assert(0);
				return FALSE;
			}
		}
		else if (uMsg == WM_NCDESTROY) {
			// find the object responsible for handling the message
			auto pMsgReceiverItr = std::find_if(m_MsgHandlerList.begin(), m_MsgHandlerList.end(), [&](DialogMsgHandlerPair& Other) {
				return (Other.GetWindowHandle() == hwnd); });
			//  if the msg receiver found
			if (pMsgReceiverItr != m_MsgHandlerList.end())
			{
				m_MsgHandlerList.erase(pMsgReceiverItr);
			}
		}			
		else {
			// find the object responsible for handling the message
			auto pMsgReceiverItr = std::find_if(m_MsgHandlerList.begin(), m_MsgHandlerList.end(), [&](DialogMsgHandlerPair& Other) {
				return (Other.GetWindowHandle() == hwnd); });

			//  if the msg receiver found
			if (pMsgReceiverItr != m_MsgHandlerList.end())
			{
				DialogMsgHandlerPair& Pair = *pMsgReceiverItr;
				pMsgHandler = Pair.GetHandler();

			}
		}

		if (pMsgHandler == nullptr) {
			// into the data structure				
			//DialogMsgHandlerPair pair(hwnd, pMsgHandler);
			//m_MsgHandlerList.push_back(pair);
			//assert(0);
			return FALSE;
		}
		else
		{
			return pMsgHandler->HandleMessage(uMsg, wParam, lParam);
		}
	}
}
