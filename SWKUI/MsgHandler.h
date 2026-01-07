#pragma once

//#include <windows.h>
#include <vector>
#include <algorithm>
#include <mutex>
#include <unordered_map>
#include "WinKernel.h"

namespace swktool {
	/// <summary>
	/// Base interface to cover both Dialog and Window
	/// </summary>
	class IWindowHandler 
	{
	public:
		virtual ~IWindowHandler() = default;

		// Called immediately after WM_NCCREATE or WM_INITDIALOG
		virtual void SetHwnd(HWND hwnd) = 0;
		virtual HWND GetHwnd() const = 0;

		//
		// ---- Dialog / Window Lifecycle ----
		//

		// Dialogs: return TRUE to let Windows set default focus
		// Windows: return 0 to continue default processing
		virtual BOOL OnInitDialog(WPARAM wParam, LPARAM lParam) { return TRUE; }

		// Called for WM_CREATE (non-dialog windows)
		virtual LRESULT OnCreate(CREATESTRUCT* cs) { return 0; }

		// Called for WM_DESTROY
		virtual void OnDestroy() {}

		// Called for WM_CLOSE
		virtual void OnClose() { DestroyWindow(GetHwnd()); }

		//
		// ---- Command Routing (Buttons, Menus, Accelerators) ----
		//

		// Called for WM_COMMAND
		virtual LRESULT OnCommand(WORD id, WORD code, HWND control) { return 0; }

		//
		// ---- Notifications (ListView, TreeView, etc.) ----
		//

		// Called for WM_NOTIFY
		virtual LRESULT OnNotify(int idCtrl, NMHDR* hdr) { return 0; }

		//
		// ---- Keyboard / Mouse ----
		//

		virtual LRESULT OnKeyDown(UINT vk, UINT flags) { return 0; }
		virtual LRESULT OnKeyUp(UINT vk, UINT flags) { return 0; }
		virtual LRESULT OnChar(UINT ch, UINT flags) { return 0; }

		virtual LRESULT OnMouseMove(UINT keys, int x, int y) { return 0; }
		virtual LRESULT OnLButtonDown(UINT keys, int x, int y) { return 0; }
		virtual LRESULT OnLButtonUp(UINT keys, int x, int y) { return 0; }
		virtual LRESULT OnRButtonDown(UINT keys, int x, int y) { return 0; }
		virtual LRESULT OnRButtonUp(UINT keys, int x, int y) { return 0; }

		//
		// ---- Focus ----
		//

		virtual LRESULT OnSetFocus(HWND oldFocus) { return 0; }
		virtual LRESULT OnKillFocus(HWND newFocus) { return 0; }

		//
		// ---- Painting ----
		//

		virtual LRESULT OnPaint() { return 0; }
		virtual LRESULT OnEraseBkgnd(HDC hdc) { return 0; }

		//
		// ---- Sizing / Moving ----
		//

		virtual LRESULT OnSize(UINT type, int cx, int cy) { return 0; }
		virtual LRESULT OnMove(int x, int y) { return 0; }

		//
		// ---- Default Message Router ----
		//

		// Fallback for messages not explicitly handled
		virtual LRESULT OnMessage(UINT msg, WPARAM wParam, LPARAM lParam) { return 0; }
	};

	/// <summary>
	/// Registry pairs hwnd and pointer of Handler and store in the memory store
	/// when message comes in with hwnd, this structure is used to find the ptr of handler 
	/// responsible and routs the message to the proper object
	/// </summary>
	class WindowHandlerRegistry 
	{
	public:
		static WindowHandlerRegistry& Instance()
		{
			static WindowHandlerRegistry instance; 
			return instance;
		}
		void RegisterHandler(HWND hwnd, IWindowHandler* handler)
		{
			//if (handlers_.c)
			std::lock_guard<std::mutex> lock(mutex_);
			handlers_[hwnd] = handler;
		}
		void UnregisterHandler(HWND hwnd)
		{
			std::lock_guard<std::mutex> lock(mutex_);
			handlers_.erase(hwnd);
		}

		IWindowHandler* GetHandler(HWND hwnd) 
		{ 
			std::lock_guard<std::mutex> lock(mutex_); 
			auto it = handlers_.find(hwnd); 
			return (it != handlers_.end()) ? it->second : nullptr;
		}

	private:
		WindowHandlerRegistry() = default;
		WindowHandlerRegistry(const WindowHandlerRegistry&) = delete;
		WindowHandlerRegistry& operator=(const WindowHandlerRegistry&) = delete;

		std::mutex mutex_;
		std::unordered_map<HWND, IWindowHandler*> handlers_;
	};

	
	
	class WindowHandlerBase : public IWindowHandler {
	public:
		WindowHandlerBase() : hwnd_(nullptr), hInstance_(nullptr) {}
		virtual ~WindowHandlerBase() = default;

		HWND GetHwnd() const override { return hwnd_; }
		void SetHwnd(HWND hwnd) override { hwnd_ = hwnd; }

		HINSTANCE GetInstance() const { return hInstance_; }
		void SetInstance(HINSTANCE hinst) { hInstance_ = hinst; }

		void OnClose() override { DestroyWindow(hwnd_); }

		// You implement this:
		virtual LRESULT OnMessage(UINT msg, WPARAM wParam, LPARAM lParam) override 
		{
			return DefWindowProc(hwnd_, msg, wParam, lParam);
		}

		LRESULT OnPaint() override { 
			PAINTSTRUCT ps; 
			HDC hdc = BeginPaint(hwnd_, &ps); 
			EndPaint(hwnd_, &ps); return 0; 
		}

	private:
		HWND hwnd_;
		HINSTANCE hInstance_;
	};


	class DialogHandlerBase : public IWindowHandler
	{
	public:
		DialogHandlerBase() : m_hwnd(nullptr) {}
		virtual ~DialogHandlerBase() = default;

		//
		// HWND management
		//
		void SetHwnd(HWND hwnd) override { m_hwnd = hwnd; }
		HWND GetHwnd() const override { return m_hwnd; }

		HINSTANCE GetInstance() const 
		{ 
			return reinterpret_cast<HINSTANCE>(GetWindowLongPtr(m_hwnd, GWLP_HINSTANCE)); 
		}

		//
		// Dialog initialization
		//
		BOOL OnInitDialog(WPARAM wParam, LPARAM lParam) override
		{
			// Default: allow Windows to set focus
			return TRUE;
		}

		//
		// Commands
		//
		LRESULT OnCommand(WORD id, WORD code, HWND control) override
		{
			// Default: close dialog on IDOK or IDCANCEL
			if (id == IDOK || id == IDCANCEL)
			{
				EndDialog(m_hwnd, id);
				return TRUE;
			}
			return FALSE;
		}

		//
		// Notifications
		//
		LRESULT OnNotify(int idCtrl, NMHDR* hdr) override
		{
			return FALSE;
		}

		//
		// Close / Destroy
		//
		void OnClose() override
		{
			EndDialog(m_hwnd, IDCANCEL);
		}

		void OnDestroy() override {}

		//
		// Fallback
		//
		LRESULT OnMessage(UINT msg, WPARAM wParam, LPARAM lParam) override
		{
			return FALSE; // Dialogs return BOOL, not LRESULT
		}

	protected:
		HWND m_hwnd;		
	};












	class WindowsRegister {
	protected:
		using WindowRegisterClass = WNDCLASSEX;

	public:
		virtual PCWSTR ClassName() const = 0;

		// called right before register to be able to change the registration information
		virtual void   PreRegisterWindow(WindowRegisterClass& wc) = 0;
	};	
}
