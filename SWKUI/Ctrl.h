#ifndef __WINDOWS_CONTROL_H__
#define __WINDOWS_CONTROL_H__

#include <Windows.h>
#include <windowsx.h>
#include <string>
#include "Theme.h"
#include "ControlBinder.h"


namespace swktool 
{	
	class IWindow;
	class ControlBinder;
	class Ctrl 
	{
		const int STRING_BUF_SIZE = 255;

	protected:
		HWND hwndCtrl = nullptr;
		HWND hParent = nullptr;

		// Control ID
		UINT ID = -1;

		HINSTANCE hInst = nullptr;

		RECT Rect = { };

		IWindow* parentWindow = nullptr;

	public:
		Ctrl() = default;
		Ctrl(IWindow* pParent);
		// to use with dialog template with control ID
		Ctrl(UINT CtrlID, IWindow* pParent);
		Ctrl(HWND ExistingHandle, IWindow* pParent);
		virtual ~Ctrl();

		HWND GetCtrlHandle() const {
			return hwndCtrl;
		}
		HWND GetHwnd() const {
			return GetCtrlHandle();
		}

		HWND GetParentHandle() const {
			return hParent;
		}

		UINT GetID() const {			
			return ID;
		}

		std::wstring GetText() {
			std::wstring str;
			str.resize(STRING_BUF_SIZE, 0);

			const int size = STRING_BUF_SIZE;
			GetWindowText(hwndCtrl, ((LPWSTR)str.c_str()), (int)(size));

			return str;
		}

		int GetTextLength() const {
			return GetWindowTextLength(hwndCtrl);
		}

		void	SetText(std::wstring s) {
			SendMessage(hwndCtrl, WM_SETTEXT, 0, (LPARAM)(s.c_str()));
		}


		void Enable();
		void Disable();

		void Show();
		void Hide();

		void SetFocus() {
			::SetFocus(hwndCtrl);
		}

		BOOL IsEnabled() {
			return ::IsWindowEnabled(hwndCtrl);
		}

		BOOL IsVisible() {
			return ::IsWindowVisible(hwndCtrl);
		}

		RECT GetRect() {
			GetWindowRect(hwndCtrl, &Rect);
			return Rect;
		}

		void UpdateRect() {
			GetWindowRect(hwndCtrl, &Rect);
		}

		BOOL MoveWindow(
			int  X, int  Y, int  nWidth, int  nHeight, BOOL bRepaint = TRUE) {
			return ::MoveWindow(hwndCtrl, X, Y, nWidth, nHeight, bRepaint);
		}

		virtual void OnSize(UINT state, int nWidth, int nHeight) {}
		virtual void ApplyTheme(const Theme& theme) {};
		void SetBinder(ControlBinder* binder) 
		{ 
			binder_ = binder; 
		}


	protected:
		ControlBinder* binder_ = nullptr;

	};
}

#endif //__WINDOWS_CONTROL_H__