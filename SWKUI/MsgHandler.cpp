#include "pch.h"
#include "MsgHandler.h"
#include "../SWKBase/DebugStream.h"
#include "../SWKBase/WinRegOpenOptions.h"
#include "Window.h"
#include <iomanip>
#include "MDIFrameWindow.h"

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
			//handler->OnCreate(cs);

			return TRUE;
		}

		// Lookup handler for all other messages
		IWindowHandler* handler =
			WindowHandlerRegistry::Instance().GetHandler(hwnd);

		if (!handler)
			return DefWindowProc(hwnd, msg, wParam, lParam);

		// ============================================================
		   // ⭐ INSERT THIS BLOCK RIGHT HERE — BEFORE THE switch(msg)
		   // ============================================================
		if (auto* frame = dynamic_cast<MDIFrameWindow*>(handler))
		{
			HWND hClient = frame->GetMDIClientHWnd();

			switch (msg)
			{
				// These MUST go to DefFrameProc
			case WM_INITMENU:
			case WM_INITMENUPOPUP:
			case WM_MENUSELECT:
			case WM_MEASUREITEM:
			case WM_DRAWITEM:
			case WM_MENUCHAR:
				return DefFrameProc(hwnd, hClient, msg, wParam, lParam);

			case WM_COMMAND:
			{
				// Let DefFrameProc try first (for MDI child commands)
				if (DefFrameProc(hwnd, hClient, msg, wParam, lParam))
					return 0;

				// If DefFrameProc did NOT handle it, this is a frame command
				WORD id = LOWORD(wParam);
				WORD code = HIWORD(wParam);
				HWND ctrl = reinterpret_cast<HWND>(lParam);

				return frame->OnCommand(id, code, ctrl);
			}
			}
		}

		switch (msg)
		{
		case WM_CREATE:
		{
			auto* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
			return handler->OnCreate(cs);
		}


		case WM_COMMAND:
		{
			// If this is an MDI frame, DefFrameProc already handled it above.
			if (dynamic_cast<MDIFrameWindow*>(handler))
				break;

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

		case WM_NCCALCSIZE:
			return handler->OnNcCalcSize((BOOL)wParam, (NCCALCSIZE_PARAMS*)lParam);

		case WM_NCPAINT:
			return handler->OnNcPaint((HRGN)wParam);

		case WM_NCHITTEST:
		{
			POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
			return handler->OnNcHitTest(pt);
		}

		case WM_CLOSE:
			handler->OnClose();
			return 0;

		case WM_DESTROY:
			handler->OnDestroy();
			WindowHandlerRegistry::Instance().UnregisterHandler(hwnd);
			return 0;

		case WM_NCDESTROY:	
			auto* pHandle = handler;
			if (pHandle)
			{
				pHandle->OnNcDestroy();
			}
			WindowHandlerRegistry::Instance().UnregisterHandler(hwnd);
			return 0;
		}

		LRESULT result = handler->OnMessage(msg, wParam, lParam);
		WNDPROC orig = handler->GetOriginalWndProc();

		// if message has been handled, then return 0
		if (result != IWindowHandler::NOT_HANDLED)
		{
			return result;
		}
		
		// if message not handled, and subclassed
		if (orig)
		{
			return CallWindowProc(handler->GetOriginalWndProc(), hwnd, msg, wParam, lParam);
		}

		// Fallback for unhandled messages, no subclass		
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}


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
			
			handler->OnInitDialog((HWND)wParam, lParam);
			
			return TRUE;
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


	//LRESULT CALLBACK SWKMDIFrameProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
	//{
	//	HWND hClient = nullptr;

	//	// Retrieve the MDI client handle stored in GWLP_USERDATA or wherever SWKUI keeps it.
	//	// If you store it differently, adjust this line.
	//	hClient = (HWND)GetWindowLongPtr(hWnd, GWLP_USERDATA);

	//	switch (msg)
	//	{
	//	case WM_CREATE:
	//	{
	//		// Extract CLIENTCREATESTRUCT from CREATESTRUCT
	//		auto* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
	//		auto* ccs = reinterpret_cast<CLIENTCREATESTRUCT*>(cs->lpCreateParams);

	//		// Create the MDI client window
	//		hClient = CreateWindowEx(
	//			0,
	//			L"MDICLIENT",
	//			nullptr,
	//			WS_CHILD | WS_CLIPCHILDREN | WS_VISIBLE,
	//			0, 0, 0, 0,
	//			hWnd,
	//			nullptr,
	//			(HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
	//			ccs
	//		);

	//		if (!hClient)
	//			return -1;

	//		// Store client handle for later use
	//		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)hClient);
	//		return 0;
	//	}

	//	case WM_COMMAND:
	//		if (hClient)
	//		{
	//			// Let MDI handle menu commands (like Window → Tile, Cascade, etc.)
	//			if (DefFrameProc(hWnd, hClient, msg, wParam, lParam))
	//				return 0;
	//		}
	//		break;

	//	case WM_MENUSELECT:
	//	case WM_INITMENU:
	//	case WM_INITMENUPOPUP:
	//		if (hClient)
	//			return DefFrameProc(hWnd, hClient, msg, wParam, lParam);
	//		break;

	//	case WM_SIZE:
	//		if (hClient)
	//		{
	//			MoveWindow(hClient, 0, 0, LOWORD(lParam), HIWORD(lParam), TRUE);
	//		}
	//		return 0;

	//	case WM_CLOSE:
	//		// You can intercept shutdown here if SWKUI needs cleanup
	//		DestroyWindow(hWnd);
	//		return 0;

	//	case WM_DESTROY:
	//		PostQuitMessage(0);
	//		return 0;
	//	}

	//	// Default MDI frame message handling
	//	return DefFrameProc(hWnd, hClient, msg, wParam, lParam);
	//}

	///// <summary>
	///// Processing msg for MDI child window
	///// </summary>
	///// <param name="hwnd"></param>
	///// <param name="msg"></param>
	///// <param name="wp"></param>
	///// <param name="lp"></param>
	///// <returns></returns>
	//LRESULT CALLBACK SWKMDIChildProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
	//{
	//	using namespace swktool;
	//	Window* self = reinterpret_cast<Window*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

	//	if (self)
	//		return self->OnMessage(msg, wp, lp);

	//	return DefMDIChildProc(hwnd, msg, wp, lp);
	//}
}
