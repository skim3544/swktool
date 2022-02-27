#pragma once
#include <Windows.h>
#include <assert.h>

namespace swktool {
	/// <summary>
	///  AWindow
	///  Abstract Windows to hold App Instance, ParentHwnd, and Window Handle
	///  Used as proxy of actual window, but it only carries the minimal information
	///  to work with a window (Or dialog)
	/// </summary>
	class AWindow {
	protected:
		// Instance Handle
		HINSTANCE hInst;

		// Window Handle of the parent
		HWND hwndParent;

		// Window Handle
		HWND hwndWindow;

		std::wstring  Caption_;

	public:
		AWindow() : hInst(nullptr), hwndParent(nullptr), hwndWindow(nullptr) {}
		AWindow(AWindow* pParent) : 
			hInst(nullptr), 
			hwndParent(nullptr), 
			hwndWindow(nullptr) 
		{
			if (pParent != nullptr) {
				hInst = pParent->GetInstanceHandle();
				hwndParent = pParent->GetWindowHWND();
			}
		}

		virtual ~AWindow() { ; }

		virtual HWND GetParentHWND() const {
			return hwndParent;
		}

		virtual HWND GetWindowHWND() const {
			return hwndWindow;
		}

		virtual HWND GetDlgHWND() const {
			return GetWindowHWND();
		}


		virtual HINSTANCE GetInstanceHandle() const {
			return hInst;
		}


		// Support for Modeless dialog
		virtual BOOL IsModelessMessage(MSG* pMsg) {
			return ::IsDialogMessage(hwndWindow, pMsg);
		}

		virtual LRESULT ProcessMessage(HWND hDialog, WORD msg, WPARAM wParam, LPARAM lParam) { return 1; }

		void SetCaption(const std::wstring& Caption) {
			Caption_ = Caption;
			// delayed set, usually caption is set right after allocation of dialog box
			// at which point Window is not created and cannot set caption
			// we store the caption, and call later
			if (hwndWindow != nullptr)
				SetWindowText(hwndWindow, Caption.c_str());
		}

		void SetCaption(LPCWSTR Caption) {
			// delayed set, usually caption is set right after allocation of dialog box
			// at which point Window is not created and cannot set caption
			// we store the caption, and call later

			Caption_ = Caption;
			if (hwndWindow != nullptr)
				SetWindowText(hwndWindow, Caption);
		}
	};
}

