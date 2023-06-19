#include "pch.h"
#include "MsgHandler.h"

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


	INT_PTR CALLBACK DialogMsgHandler::DialogMsgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		DialogMsgHandler* pMsgHandler = nullptr;

		// find the object responsible for handling the message
		auto pMsgReceiverItr = std::find_if(m_MsgHandlerList.begin(), m_MsgHandlerList.end(), [&](DialogMsgHandlerPair& Other) {
			return (Other.GetWindowHandle() == hwnd); });

		//  if the msg receiver found
		if (pMsgReceiverItr != m_MsgHandlerList.end())
		{
			DialogMsgHandlerPair& Pair = *pMsgReceiverItr;
			pMsgHandler = Pair.GetHandler();			
			
		}
		else 
		{
			// if not in data structure, find the preregisted data & Update
			auto pRegisteredReceiverItr = std::find_if(m_MsgHandlerList.begin(), m_MsgHandlerList.end(), [&](DialogMsgHandlerPair& Other) {
				return (Other.GetWindowHandle() == 0); });

			if (pRegisteredReceiverItr != m_MsgHandlerList.end()) {
				DialogMsgHandlerPair& Pair = *pRegisteredReceiverItr;
				Pair.SetWindowHandle(hwnd);

				// this is the message receiver to use
				pMsgHandler = Pair.GetHandler();
				if (pMsgHandler) {
					pMsgHandler->SetWindowHandle(hwnd);				
				}
			}
			else {
				// no preregisted Dialog object
				assert(0);  
				return FALSE;
			}
		}

		if (pMsgHandler == nullptr) {
			// into the data structure				
			//DialogMsgHandlerPair pair(hwnd, pMsgHandler);
			//m_MsgHandlerList.push_back(pair);
			assert(0);
			return FALSE;
		}
		else
		{
			return pMsgHandler->HandleMessage(uMsg, wParam, lParam);
		}
	}
}
