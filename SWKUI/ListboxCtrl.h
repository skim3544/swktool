#ifndef __LIST_CTRL_7438217455543254141_H_
#define __LIST_CTRL_7438217455543254141_H_

#pragma once

#include <Windows.h>
#include <string>

#include "AWindow.h"
#include "Ctrl.h"

namespace swktool {

	const int CONTROL_STRING_BUF_SIZE = 255;
	class ListboxCtrl : public Ctrl {

	public:
		ListboxCtrl(std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, Window* pParent, UINT CtrlID);
		ListboxCtrl(UINT CtrlID, Window* pParent);
		~ListboxCtrl() { ; }

		/*
	*/

	//#define ListBox_GetText(hwndCtl, index, lpszBuffer)  ((int)(DWORD)SNDMSG((hwndCtl), LB_GETTEXT, (WPARAM)(int)(index), (LPARAM)(LPCTSTR)(lpszBuffer)))

		std::wstring GetText(int BufferSize = CONTROL_STRING_BUF_SIZE) {
			std::wstring str;
			str.resize(BufferSize, 0);
			UINT Result = GetDlgItemText(hDlg, ID, (LPWSTR)str.c_str(), (int)str.capacity());
			return str;
		}

		std::wstring GetText(int index, int BufferSize = CONTROL_STRING_BUF_SIZE) {
			std::wstring str;
			str.resize(BufferSize, 0);
			int Result = ((int)(DWORD)SendMessage((hwndCtrl), LB_GETTEXT, (WPARAM)(int)(index), (LPARAM)(LPCTSTR)(str.c_str())));

			return str;
		}

		void	SetText(const std::wstring& s) {
			SendMessage(hwndCtrl, WM_SETTEXT, 0, (LPARAM)(s.c_str()));
		}


		int	AddItem(const std::wstring& s) {
			return ((int)(DWORD)SendMessage((hwndCtrl), LB_ADDSTRING, 0L, (LPARAM)(LPCTSTR)(s.c_str())));
		}


		int	InsertItem(int Index, const std::wstring& s) {
			return ((int)(DWORD)SendMessage((hwndCtrl), LB_INSERTSTRING, (WPARAM)(int)(Index), (LPARAM)(LPCTSTR)(s.c_str())));
		}


		int DeleteItem(int index) {
			return ((int)(DWORD)SendMessage((hwndCtrl), LB_DELETESTRING, (WPARAM)(int)(index), 0L));
		}

		int GetItemTextLength(int index)
		{
			return ((int)(DWORD)SendMessage((hwndCtrl), LB_GETTEXTLEN, (WPARAM)(int)(index), 0L));
		}

		BOOL	ClearItems() {
			return (BOOL)((DWORD)::SendMessage((hwndCtrl), LB_RESETCONTENT, 0L, 0L));
		}

		// Returns count of the items in the list box or CB_ERR
		int GetItemCount() {
			int Count = ((int)(DWORD)SendMessage((hwndCtrl), LB_GETCOUNT, 0L, 0L));
			return Count;
		}

		//#define ListBox_GetItemData(hwndCtl, index)         ((LRESULT)(ULONG_PTR)SNDMSG((hwndCtl), LB_GETITEMDATA, (WPARAM)(int)(index), 0L))

		int FindItemString(int IndexStart, const std::wstring& s)
		{
			return ((int)(DWORD)SendMessage((hwndCtrl), LB_FINDSTRING, (WPARAM)(int)(IndexStart), (LPARAM)(LPCTSTR)(s.c_str())));
		}

		int SelectItem(int index)
		{
			return ((int)(DWORD)SendMessage((hwndCtrl), LB_SETSEL, (WPARAM)(BOOL)(TRUE), (LPARAM)(index)));
		}

		int UnSelectItem(int index)
		{
			return ((int)(DWORD)SendMessage((hwndCtrl), LB_SETSEL, (WPARAM)(BOOL)(FALSE), (LPARAM)(index)));
		}

		int SelectItemRange(DWORD first, DWORD last)
		{
			return ((int)(DWORD)SendMessage((hwndCtrl), LB_SELITEMRANGE, (WPARAM)(BOOL)(TRUE), MAKELPARAM((first), (last))));
		}

		int UnSelectItemRange(DWORD first, DWORD last)
		{
			return ((int)(DWORD)SendMessage((hwndCtrl), LB_SELITEMRANGE, (WPARAM)(BOOL)(FALSE), MAKELPARAM((first), (last))));
		}

		int GetItemSelectedIndex()
		{
			return ((int)(DWORD)SendMessage((hwndCtrl), LB_GETCURSEL, 0L, 0L));
		}

		int SetItemSelectedIndex(int index)
		{
			return ((int)(DWORD)SendMessage((hwndCtrl), LB_SETCURSEL, (WPARAM)(int)(index), 0L));
		}

		int SelectString(int indexStart, const std::wstring& s)
		{
			return ((int)(DWORD)SendMessage((hwndCtrl), LB_SELECTSTRING, (WPARAM)(int)(indexStart), (LPARAM)(LPCTSTR)(s.c_str())));
		}

		int GetSelectionState(int index)
		{
			return  ((int)(DWORD)SendMessage((hwndCtrl), LB_GETSEL, (WPARAM)(int)(index), 0L));
		}

		int GetSelectionCount() {
			return       ((int)(DWORD)SendMessage((hwndCtrl), LB_GETSELCOUNT, 0L, 0L));
		}

		int GetTopVisibleIndex()
		{
			return ((int)(DWORD)SendMessage((hwndCtrl), LB_GETTOPINDEX, 0L, 0L));
		}

		int GetSelectedItemsIndexes(int ItemsCount, int lpItems[])
		{
			return ((int)(DWORD)SendMessage((hwndCtrl), LB_GETSELITEMS, (WPARAM)(int)(ItemsCount), (LPARAM)(int*)(lpItems)));
		}

		int SetTopVisibleIndex(int indexTop)
		{
			return ((int)(DWORD)SendMessage((hwndCtrl), LB_SETTOPINDEX, (WPARAM)(int)(indexTop), 0L));
		}

		void SetColumnWidth(int cxColumn)
		{
			return ((void)SendMessage((hwndCtrl), LB_SETCOLUMNWIDTH, (WPARAM)(int)(cxColumn), 0L));
		}

		int GetHorizontalExtent() {
			return ((int)(DWORD)SendMessage((hwndCtrl), LB_GETHORIZONTALEXTENT, 0L, 0L));
		}

		void SetHorizontalExtent(int cxExtent) {
			return ((void)SendMessage((hwndCtrl), LB_SETHORIZONTALEXTENT, (WPARAM)(int)(cxExtent), 0L));
		}

		int GetItemRect(int index, RECT* pRect)
		{
			return ((int)(DWORD)SendMessage((hwndCtrl), LB_GETITEMRECT, (WPARAM)(int)(index), (LPARAM)(RECT*)(pRect)));
		}

		int SetCaretIndex(int index) {
			return ((int)(DWORD)SendMessage((hwndCtrl), LB_SETCARETINDEX, (WPARAM)(int)(index), 0L));
		}

		int GetCaretIndex() {
			return ((int)(DWORD)SendMessage((hwndCtrl), LB_GETCARETINDEX, 0L, 0L));
		}

		int SetItemHeight(int index, int nHeight) {
			return ((int)(DWORD)SendMessage((hwndCtrl), LB_SETITEMHEIGHT, (WPARAM)(int)(index), MAKELPARAM((nHeight), 0)));
		}

		int GetItemHeight(int index) {
			return ((int)(DWORD)SendMessage((hwndCtrl), LB_GETITEMHEIGHT, (WPARAM)(int)(index), 0L));
		}

		void SetItemSelectedByIndex(int Index) {
			::SendMessage(hwndCtrl, LB_SETCURSEL, 0, Index);
		}

		std::wstring GetItemTextIndex(int Index)
		{
			std::wstring str;
			str.resize(CONTROL_STRING_BUF_SIZE, 0);
			::SendMessage(hwndCtrl, LB_GETTEXT, Index, (LPARAM)(str.c_str()));
		}

		std::wstring SetItemTextIndex(int Index, std::wstring s)
		{
			::SendMessage(hwndCtrl, LB_GETTEXT, Index, (LPARAM)(s.c_str()));
		}

		LRESULT GetItemData(int index) {
			return ((LRESULT)(ULONG_PTR)SendMessage((hwndCtrl), LB_GETITEMDATA, (WPARAM)(int)(index), 0L));
		}
		int SetItemData(int index, LPARAM data) {
			return ((int)(DWORD)SendMessage((hwndCtrl), LB_SETITEMDATA, (WPARAM)(int)(index), (LPARAM)(data)));
		}
		int FindItemData(int indexStart, LPARAM data) {
			return ((int)(DWORD)SendMessage((hwndCtrl), LB_FINDSTRING, (WPARAM)(int)(indexStart), (LPARAM)(data)));
		}
		int SelectItemData(int indexStart, LPARAM data) {
			return ((int)(DWORD)SendMessage((hwndCtrl), LB_SELECTSTRING, (WPARAM)(int)(indexStart), (LPARAM)(data)));
		}
		int FindStringExact(int indexStart, const std::wstring& s) {
			return ((int)(DWORD)SendMessage((hwndCtrl), LB_FINDSTRINGEXACT, (WPARAM)(int)(indexStart), (LPARAM)(LPCTSTR)(s.c_str())));
		}

		int Dir(UINT attrs, LPCTSTR lpstrPath) {
			return ((int)(DWORD)SendMessage((hwndCtrl), LB_DIR, (WPARAM)(UINT)(attrs), (LPARAM)(LPCTSTR)(lpstrPath)));
		}

		BOOL SetTabStops(int cTabCount, int lpTabs[]) {
			return ((BOOL)(DWORD)SendMessage((hwndCtrl), LB_SETTABSTOPS, (WPARAM)(int)(cTabCount), (LPARAM)(int*)(lpTabs)));
		}

	};	
}
#endif


