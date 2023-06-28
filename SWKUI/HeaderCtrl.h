#pragma once

#include "Ctrl.h"


namespace swktool {

	class HeaderCtrl : public Ctrl {
	public:
		HeaderCtrl(std::wstring Caption, DWORD Style, int x, int y, int Width, int Height, Window* pParent, UINT CtrlID);
		HeaderCtrl(Window* pParent);
		HeaderCtrl(DialogWindow* pParent);

		HeaderCtrl(UINT CtrlID, Window* pParent);
		HeaderCtrl(UINT CtrlID, DialogWindow* pParent);
		~HeaderCtrl() { ; }

		virtual void OnSize(UINT state, int nWidth, int nHeight) override;


		int InsertItem(int Pos, int Width, LPTSTR lpsz) {
			return DoInsertItem(hwndCtrl, Pos, Width, lpsz);
		}

		BOOL DeleteItem(int index) {
			BOOL result = (BOOL) SendMessage(hwndCtrl, HDM_DELETEITEM,
				(WPARAM)index, (LPARAM)0);
			return result;
		}

		int GetItemCount() {
			int count = (int)SendMessage((hwndCtrl), HDM_GETITEMCOUNT, 0, 0L);
			return count;
		}

		BOOL GetLayout(WINDOWPOS& wp, RECT& rcParent) {
			HDLAYOUT hdl{};
			GetClientRect(hDlg, &rcParent);
			hdl.pwpos = &wp;
			hdl.prc = &rcParent;

			BOOL result = (BOOL)SendMessage(hwndCtrl, HDM_LAYOUT, 0, (LPARAM)&hdl);

			return result;
		}



	protected:
		int DoInsertItem(HWND hwndHeader, int iInsertAfter,
			int nWidth, LPTSTR lpsz);
	};
}