#ifndef ___BUTTON_CTRL_H___
#define ___BUTTON_CTRL_H___

#pragma once


#include "Ctrl.h"
#include "Theme.h"


namespace swktool 
{



	class ButtonCtrl : public Ctrl {
		const int STRING_BUF_SIZE = 255;
		Theme	currentTheme_{};

	public:
		ButtonCtrl(IWindow* pParent) : Ctrl(pParent) { ; }
		ButtonCtrl(std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, IWindow* pParent, UINT CtrlID);
		ButtonCtrl(UINT CtrlID, IWindow* pParent);
		~ButtonCtrl() { ; }

		void ApplyTheme(const Theme& theme) override;

		std::wstring GetText() const {
			std::wstring str;
			str.resize(STRING_BUF_SIZE, 0);

			const size_t size = STRING_BUF_SIZE;
			GetWindowText((hwndCtrl), ((LPWSTR)str.c_str()), (int)(size));

			return str;
		}

		int GetTextLength() const {
			return GetWindowTextLength(hwndCtrl);
		}

		void	SetText(std::wstring s) {
			SendMessage(hwndCtrl, WM_SETTEXT, 0, (LPARAM)(s.c_str()));
		}

		int GetCheck() const {
			return ((int)(DWORD)SendMessage((hwndCtrl), BM_GETCHECK, 0L, 0L));
		}

		void SetCheck(int check) {
			((void)SendMessage((hwndCtrl), BM_SETCHECK, (WPARAM)(int)(check), 0L));
		}

		UINT GetState() const {
			return ((int)(DWORD)SendMessage((hwndCtrl), BM_GETSTATE, 0L, 0L));
		}

		void SetState(int state) {
			((UINT)(DWORD)SendMessage((hwndCtrl), BM_SETSTATE, (WPARAM)(int)(state), 0L));
		}

		void SetStyle(int style, BOOL fRedraw = TRUE) {
			((void)SendMessage((hwndCtrl), BM_SETSTYLE, (WPARAM)LOWORD(style), MAKELPARAM(((fRedraw) ? TRUE : FALSE), 0)));
		}
	};


	class CheckboxCtrl : public Ctrl 
	{
	public:
		enum class CheckState {
			Checked = BST_CHECKED,
			Indeterminate = BST_INDETERMINATE,
			Unchecked = BST_UNCHECKED
		};

	public:
		CheckboxCtrl(std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, IWindow* pParent, UINT CtrlID);
		CheckboxCtrl(IWindow* pParent) : Ctrl(pParent) { ; }
		CheckboxCtrl(UINT CtrlID, IWindow* pParent);
		~CheckboxCtrl() { ; }


		CheckState  GetCheck() const
		{
			LRESULT LCheckState = ::SendMessage(hwndCtrl, BM_GETCHECK, 0L, 0L);
			return (CheckState)LCheckState;
		}

		DWORD  GetCheckState() const
		{
			auto LCheckState = ::SendMessage(hwndCtrl, BM_GETSTATE, 0L, 0L);
			return (DWORD)LCheckState;
		}

		void SetCheck(CheckState ChekState)
		{
			((void)SendMessage((hwndCtrl), BM_SETCHECK, (WPARAM)(int)(ChekState), 0L));
		}
	};


	
	class RadioCtrl : public Ctrl 
	{

	public:
		// WS_VISIBLE | WS_CHILD|BS_GROUPBOX
		RadioCtrl(std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, IWindow* pParent, UINT CtrlID);
		RadioCtrl(IWindow* pParent) : Ctrl(pParent) { ; }
		RadioCtrl(UINT CtrlID, IWindow* pParent);
		~RadioCtrl() { ; }

		void Set() {
			::SendMessage(hwndCtrl, BM_SETCHECK, 1L, 0);
		}

		void Clear()
		{
			::SendMessage(hwndCtrl, BM_SETCHECK, 0L, 0);
		}
	};
}

#endif //___BUTTON_CTRL_H___

