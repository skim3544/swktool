#pragma once

#include "ctrl.h"

namespace swktool {

	class AWindow;

	class ComboboxCtrl : public Ctrl {
		const int STRING_BUF_SIZE = 255;

	public:
		ComboboxCtrl(std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, Window* pParent, UINT CtrlID);
		ComboboxCtrl(UINT CtrlID, Window* pParent);
		~ComboboxCtrl() { ; }

		std::wstring GetText() {
			std::wstring str;
			str.resize(STRING_BUF_SIZE, 0);

			const size_t size = STRING_BUF_SIZE;
			GetWindowText((hwndCtrl), ((LPWSTR)str.c_str()), (int)(size));

			return str;
		}


		int GetTextLength() {
			return GetWindowTextLength(hwndCtrl);
		}

		void	SetText(LPCWSTR s) {
			SendMessage(hwndCtrl, WM_SETTEXT, 0, (LPARAM)s);
		}

		int		LimitText(int cchLimit) {
			return ((int)(DWORD)SendMessage((hwndCtrl), CB_LIMITTEXT, (WPARAM)(int)(cchLimit), 0L));
		}

		DWORD GetEditSel() {
			return ((DWORD)SendMessage((hwndCtrl), CB_GETEDITSEL, 0L, 0L));
		}

		DWORD SetEditSel(int ichStart, int ichEnd) {
			return ((int)(DWORD)SendMessage((hwndCtrl), CB_SETEDITSEL, 0L, MAKELPARAM((ichStart), (ichEnd))));
		}

		DWORD GetCount() {
			return ((int)(DWORD)SendMessage((hwndCtrl), CB_GETCOUNT, 0L, 0L));
		}

		DWORD ResetContent() {
			return ((int)(DWORD)SendMessage((hwndCtrl), CB_RESETCONTENT, 0L, 0L));
		}

		int AddString(std::wstring s) {
			return ((int)(DWORD)SendMessage((hwndCtrl), CB_ADDSTRING, 0L, (LPARAM)(LPCTSTR)(s.c_str())));
		}

		int InsertString(int index, std::wstring s) {
			std::wstring str;
			str.resize(STRING_BUF_SIZE, 0);

			return ((int)(DWORD)SendMessage((hwndCtrl), CB_INSERTSTRING, (WPARAM)(int)(index), (LPARAM)(LPCTSTR)(s.c_str())));
		}

		int AddItemData(LPARAM data) {
			return ((int)(DWORD)SendMessage((hwndCtrl), CB_ADDSTRING, 0L, (LPARAM)(data)));
		}

		int InsertItemData(int index, LPARAM data) {
			return ((int)(DWORD)SendMessage((hwndCtrl), CB_INSERTSTRING, (WPARAM)(int)(index), (LPARAM)(data)));
		}

		int DeleteString(int index) {
			return ((int)(DWORD)SendMessage((hwndCtrl), CB_DELETESTRING, (WPARAM)(int)(index), 0L));
		}

		int GetLBTextLen(int index) {
			return ((int)(DWORD)SendMessage((hwndCtrl), CB_GETLBTEXTLEN, (WPARAM)(int)(index), 0L));
		}

		int GetLBText(int index, std::wstring& s) {
			s.resize(STRING_BUF_SIZE, 0);
			return ((int)(DWORD)SendMessage((hwndCtrl), CB_GETLBTEXT, (WPARAM)(int)(index), (LPARAM)(LPCTSTR)(s.c_str())));
		}

		LRESULT GetItemData(int index) {
			return ((LRESULT)(ULONG_PTR)SendMessage((hwndCtrl), CB_GETITEMDATA, (WPARAM)(int)(index), 0L));
		}

		int SetItemData(int index, LPARAM data) {
			return ((int)(DWORD)SendMessage((hwndCtrl), CB_SETITEMDATA, (WPARAM)(int)(index), (LPARAM)(data)));
		}

		int FindString(int indexStart, const std::string& s) {
			return ((int)(DWORD)SendMessage((hwndCtrl), CB_FINDSTRING, (WPARAM)(int)(indexStart), (LPARAM)(LPCTSTR)(s.c_str())));
		}

		int FindItemData(int indexStart, LPARAM data) {
			return ((int)(DWORD)SendMessage((hwndCtrl), CB_FINDSTRING, (WPARAM)(int)(indexStart), (LPARAM)(data)));
		}

		int GetCurSel() {
			return ((int)(DWORD)SendMessage((hwndCtrl), CB_GETCURSEL, 0L, 0L));
		}

		int SetCurSel(int index) {
			return ((int)(DWORD)SendMessage((hwndCtrl), CB_SETCURSEL, (WPARAM)(int)(index), 0L));
		}

		int SelectString(int indexStart, const std::wstring& s) {
			return ((int)(DWORD)SendMessage((hwndCtrl), CB_SELECTSTRING, (WPARAM)(int)(indexStart), (LPARAM)(LPCTSTR)(s.c_str())));
		}

		int SelectItemData(int indexStart, LPARAM data) {
			return ((int)(DWORD)SendMessage((hwndCtrl), CB_SELECTSTRING, (WPARAM)(int)(indexStart), (LPARAM)(data)));
		}

		int Dir(UINT attrs, const std::string& sPath) {
			return ((int)(DWORD)SendMessage((hwndCtrl), CB_DIR, (WPARAM)(UINT)(attrs), (LPARAM)(LPCTSTR)(sPath.c_str())));
		}

		BOOL ShowDropdown(BOOL fShow = TRUE) {
			return ((BOOL)(DWORD)SendMessage((hwndCtrl), CB_SHOWDROPDOWN, (WPARAM)(BOOL)(fShow), 0L));
		}

		int FindStringExact(int indexStart, const std::wstring& sFind) {
			return ((int)(DWORD)SendMessage((hwndCtrl), CB_FINDSTRINGEXACT, (WPARAM)(int)(indexStart), (LPARAM)(LPCTSTR)(sFind.c_str())));
		}

		BOOL GetDroppedState() {
			return ((BOOL)(DWORD)SendMessage((hwndCtrl), CB_GETDROPPEDSTATE, 0L, 0L));
		}
		void GetDroppedControlRect(RECT* lprc) {
			((void)SendMessage((hwndCtrl), CB_GETDROPPEDCONTROLRECT, 0L, (LPARAM)(RECT*)(lprc)));
		}

		int GetItemHeight() {
			((int)(DWORD)SendMessage((hwndCtrl), CB_GETITEMHEIGHT, 0L, 0L));
		}

		int SetItemHeight(int index, int cyItem) {
			((int)(DWORD)SendMessage((hwndCtrl), CB_SETITEMHEIGHT, (WPARAM)(int)(index), (LPARAM)(int)cyItem));
		}

		UINT GetExtendedUI() {
			((UINT)(DWORD)SendMessage((hwndCtrl), CB_GETEXTENDEDUI, 0L, 0L));
		}

		int SetExtendedUI(UINT flags) {
			((int)(DWORD)SendMessage((hwndCtrl), CB_SETEXTENDEDUI, (WPARAM)(UINT)(flags), 0L));
		}
	};
}