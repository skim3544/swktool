#ifndef __WINDOWS_CONTROL_H__
#define __WINDOWS_CONTROL_H__

#include <Windows.h>
#include <windowsx.h>
#include <string>



namespace swktool {
	class AWindow;

	class Ctrl {
		const int STRING_BUF_SIZE = 255;

	protected:
		HWND hwndCtrl = nullptr;
		HWND hDlg = nullptr;

		// Control ID
		UINT ID = -1;

		HINSTANCE hInst;

		RECT Rect = { };

	public:
		Ctrl() : hInst(nullptr), hwndCtrl (nullptr), hDlg(nullptr), ID(-1) { ; }
		Ctrl(UINT CtrlID, HINSTANCE h, HWND hDialog, HWND hControl) :
			hInst(h), hwndCtrl(hControl), hDlg(hDialog), ID(CtrlID) {
			;

			GetWindowRect(hwndCtrl, &Rect);
		}
		Ctrl(HWND hwnd) : 
			hInst(nullptr), hwndCtrl(hwnd), hDlg(nullptr), ID(-1) { ; }
		// Used for createing a child control dynamically (whithin resource file)
		Ctrl(Window* pParent);

		Ctrl(UINT CtrlID, Window* pParent);		
		Ctrl(UINT CtrlID, DialogWindow* pParent);
		virtual ~Ctrl() { ; }

		HWND GetCtrlHandle() const {
			return hwndCtrl;
		}

		HWND GetDlgHandle() const {
			return hDlg;
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

		HWND GetCtrlHandle() {
			return hwndCtrl;
		}

		RECT GetRect() {
			GetWindowRect(hwndCtrl, &Rect);
			return Rect;
		}

		BOOL MoveWindow(
			int  X, int  Y, int  nWidth, int  nHeight, BOOL bRepaint = TRUE) {
			return ::MoveWindow(hwndCtrl, X, Y, nWidth, nHeight, bRepaint);
		}

		//void Test() {
		//	HDC hdc = GetDC(hwndCtrl);

		//	COLORREF Color = 0x000000FF;	
		//	::SetTextColor(hdc, Color);

		//	::ReleaseDC(hwndCtrl, hdc);
		//}

		virtual void OnSize(UINT state, int nWidth, int nHeight) {}
	};
}

#endif //__WINDOWS_CONTROL_H__