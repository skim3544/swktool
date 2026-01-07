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

		// Handle WM_NCCREATE before registry lookup
		if (msg == WM_NCCREATE)
		{
			auto* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
			auto* handler = static_cast<IWindowHandler*>(cs->lpCreateParams);

			if (!handler)
				return DefWindowProc(hwnd, msg, wParam, lParam);

			handler->SetHwnd(hwnd);
			WindowHandlerRegistry::Instance().RegisterHandler(hwnd, handler);

			// Give the handler a chance to process creation
			handler->OnCreate(cs);

			return TRUE;
		}

		// Lookup handler for all other messages
		IWindowHandler* handler =
			WindowHandlerRegistry::Instance().GetHandler(hwnd);

		if (!handler)
			return DefWindowProc(hwnd, msg, wParam, lParam);

		switch (msg)
		{
		case WM_COMMAND:
		{
			WORD id = LOWORD(wParam);
			WORD code = HIWORD(wParam);
			HWND ctrl = reinterpret_cast<HWND>(lParam);

			return handler->OnCommand(id, code, ctrl);
		}

		case WM_NOTIFY:
		{
			int idCtrl = static_cast<int>(wParam);
			NMHDR* hdr = reinterpret_cast<NMHDR*>(lParam);

			return handler->OnNotify(idCtrl, hdr);
		}

		case WM_SETFOCUS:
			return handler->OnSetFocus(reinterpret_cast<HWND>(wParam));

		case WM_KILLFOCUS:
			return handler->OnKillFocus(reinterpret_cast<HWND>(wParam));

		case WM_PAINT:
			return handler->OnPaint();

		case WM_ERASEBKGND:
			return handler->OnEraseBkgnd(reinterpret_cast<HDC>(wParam));

		case WM_SIZE:
			return handler->OnSize((UINT)wParam, LOWORD(lParam), HIWORD(lParam));

		case WM_MOVE:
			return handler->OnMove((int)(short)LOWORD(lParam),
				(int)(short)HIWORD(lParam));

		case WM_CLOSE:
			handler->OnClose();
			return 0;

		case WM_DESTROY:
			handler->OnDestroy();
			WindowHandlerRegistry::Instance().UnregisterHandler(hwnd);
			return 0;

		case WM_NCDESTROY:
			// Final cleanup
			WindowHandlerRegistry::Instance().UnregisterHandler(hwnd);
			return 0;
		}

		// Fallback for unhandled messages
		return handler->OnMessage(msg, wParam, lParam);
	}



	// Pre‑registered dialog handler waiting for its HWND.
	inline thread_local IWindowHandler* g_pendingDialogHandler = nullptr;

	INT_PTR CALLBACK SWKDialogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) 
	{
		if (msg == WM_INITDIALOG) 
		{
			// lParam usually carries user data (often a pointer)
			auto* handler = reinterpret_cast<IWindowHandler*>(lParam);
			if (!handler)
				return FALSE;
			
			handler->SetHwnd(hwnd);
			WindowHandlerRegistry::Instance().RegisterHandler(hwnd, handler);
			
			handler->OnInitDialog(wParam, lParam);
			
			return handler->OnInitDialog(wParam, lParam);
		}

		// Lookup handler for all other messages 
		IWindowHandler* handler = WindowHandlerRegistry::Instance().GetHandler(hwnd);
		if (!handler)
			return FALSE;

		switch (msg)
		{
		case WM_COMMAND:
		{
			WORD id = LOWORD(wParam);
			WORD code = HIWORD(wParam);
			HWND ctrl = reinterpret_cast<HWND>(lParam);

			return handler->OnCommand(id, code, ctrl);
		}

		case WM_NOTIFY:
		{
			int idCtrl = static_cast<int>(wParam);
			NMHDR* hdr = reinterpret_cast<NMHDR*>(lParam);

			return handler->OnNotify(idCtrl, hdr);
		}

		case WM_CLOSE:
			handler->OnClose();
			return TRUE; // tell Windows we handled it

		case WM_DESTROY:
			handler->OnDestroy();
			WindowHandlerRegistry::Instance().UnregisterHandler(hwnd);
			return TRUE;
		}

		// Fallback for all other messages
		return handler->OnMessage(msg, wParam, lParam);
	}

	

}
