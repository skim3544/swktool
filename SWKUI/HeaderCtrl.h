#pragma once

#include "Ctrl.h"


namespace swktool 
{
	class HeaderCtrl : public Ctrl 
	{
	public:
		HeaderCtrl(std::wstring Caption, DWORD Style, int x, int y, int Width, int Height, IWindow* pParent, UINT CtrlID);
		HeaderCtrl(IWindow* pParent);
		HeaderCtrl(UINT CtrlID, IWindow* pParent);
		~HeaderCtrl() { ; }

		virtual void OnSize(UINT state, int nWidth, int nHeight) override;


		LRESULT InsertItem(int Pos, int Width, LPTSTR lpsz) {
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
			GetClientRect(hParent, &rcParent);
			hdl.pwpos = &wp;
			hdl.prc = &rcParent;

			BOOL result = (BOOL)SendMessage(hwndCtrl, HDM_LAYOUT, 0, (LPARAM)&hdl);

			return result;
		}

	protected:
		LRESULT DoInsertItem(HWND hwndHeader, int iInsertAfter,
			int nWidth, LPCTSTR  lpsz);
	};
}