#pragma once
#include "Ctrl.h"
namespace swktool {


	//class swktool::BaseDialog;

	class GroupboxCtrl : public Ctrl {
		const int STRING_BUF_SIZE = 255;

	public:
		GroupboxCtrl(std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, Window* pParent, UINT CtrlID);
		GroupboxCtrl(UINT CtrlID, Window* pParent);
		~GroupboxCtrl() { ; }


		std::wstring GetText() {
			std::wstring str;
			str.resize(STRING_BUF_SIZE, 0);
			UINT Result = GetDlgItemText(hDlg, ID, (LPWSTR)str.c_str(), (int)str.capacity());
			return str;
		}

		void	SetText(std::wstring s) {
			SendMessage(hwndCtrl, WM_SETTEXT, 0, (LPARAM)(s.c_str()));
		}
	};
}