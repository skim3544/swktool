#pragma once

//#include <windows.h>
#include <vector>
#include <algorithm>


namespace swktool {
	class WindowsMsgHandler;
	class DialogMsgHandler;

	// this pairs the Handle of the Window with the pointer of the messaging handler
	class WindowMsgHandlerPair {
	public:
		WindowMsgHandlerPair(HWND handle, WindowsMsgHandler* ptr) :
			hwnd_(handle), pHandler_(ptr) {
			;
		}

		HWND GetWindowHandle() const { return hwnd_; }
		WindowsMsgHandler* GetHandler() const { return pHandler_; }
	private:
		HWND hwnd_;
		WindowsMsgHandler* pHandler_;
	};



	// this pairs the Handle of the Window with the pointer of the messaging handler
	class DialogMsgHandlerPair {
	public:
		DialogMsgHandlerPair(HWND handle, DialogMsgHandler* ptr) :
			hwnd_(handle), pHandler_(ptr) {
			;
		}

		HWND GetWindowHandle() const { return hwnd_; }
		void SetWindowHandle(HWND hwnd) {
			hwnd_ = hwnd;
		}

		DialogMsgHandler* GetHandler() const { return pHandler_; }


	private:
		HWND hwnd_;
		DialogMsgHandler* pHandler_;
	};


	/// <summary>
	/// Windows Message Handler
	/// It keeps static list of Window Handle and the message handler.
	/// When the message comes in through WindowsProc, it either registers a new Window Message Handler (WM_NCCREATE)
	/// or try to find the owner of the mssage by lookin up the list
	/// Anything it cannot find, it passes to the default window handler
	/// </summary>
	class WindowsMsgHandler {

	protected:
		using MsgHandlerPair = WindowMsgHandlerPair;
		using MsgHandlerList = std::vector< MsgHandlerPair >;


		HWND m_hwnd;
		HINSTANCE  m_hInstance;

		static MsgHandlerList m_MsgHandlerList;

	public:
		WindowsMsgHandler() :
			m_hwnd(NULL),
			m_hInstance(NULL) {

		}
		HWND WindowHandle() const {
			return m_hwnd;
		}

		HWND GetWindowHandle() const {
			return WindowHandle();
		}

		HINSTANCE GetInstance() const {
			return m_hInstance;
		}

		virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

		static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);		
	};


	class DialogMsgHandler {
	protected:
		using MsgHandlerPair = DialogMsgHandlerPair;
		using MsgHandlerList = std::vector< MsgHandlerPair >;

	public:
		HINSTANCE GetInstance() const { return m_hInstance; }
		HWND   GetWindowHandle() const { return m_hwnd;}
		HWND   GetParentWindowHandle() const { return m_hParent; }


	protected:
		HINSTANCE m_hInstance;
		HWND m_hwnd;
		HWND m_hParent;

		static MsgHandlerList m_MsgHandlerList;

		

		DialogMsgHandler() :
			  m_hwnd(NULL)
			, m_hParent(NULL)
			, m_hInstance(NULL)
		{

		}
		HWND WindowHandle() const {
			return m_hwnd;
		}

		virtual void SetWindowHandle(HWND hWnd) {
			m_hwnd = hWnd;
		}

		virtual INT_PTR   HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;
		static BOOL PreRegisterClass(DialogMsgHandler* ptr) {
			MsgHandlerPair pair(0, ptr);
			m_MsgHandlerList.push_back(pair);
			return  TRUE;
		}

		static DialogMsgHandlerPair* FindPreRegistered();
		static INT_PTR CALLBACK DialogMsgProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	};



	class WindowsRegister {
	protected:
		using WindowRegisterClass = WNDCLASSEX;

		virtual PCWSTR ClassName() const = 0;

		// called right before register to be able to change the registration information
		virtual void   PreRegisterWindow(WindowRegisterClass& wc) = 0;
	};
	


}
