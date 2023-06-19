#pragma once
#include <Windows.h>
namespace swktool {

	enum class CheckedType {
		Checked = MF_CHECKED,
		Unchecked = MF_UNCHECKED,
	};


	class WinMenu {
		HMENU hMenu_;

	public:
		WinMenu() :hMenu_(nullptr) {

		}
		WinMenu(HMENU hNewMenu) : hMenu_(hNewMenu) {

		}
		~WinMenu() { ; }

		void attach(HMENU hMenu) {
			hMenu_ = hMenu;
		}

		HMENU Detach() {
			HMENU hCopyMenu = hMenu_;
			hMenu_ = nullptr;

			return hCopyMenu;
		}

		operator HMENU() const {
			return hMenu_;
		}


		static WinMenu CreateMenu() {
			HMENU hNewMenu = ::CreateMenu();

			WinMenu oMenu(hNewMenu);

			return oMenu;
		}

		static WinMenu CreatePopup()
		{
			HMENU hNewMenu = ::CreatePopupMenu();

			WinMenu oMenu(hNewMenu);

			return oMenu;
		}

		static WinMenu GetMenu(HWND hWnd) {
			HMENU Menu = ::GetMenu(hWnd);
			WinMenu oMenu(Menu);
			return oMenu;
		}

		static WinMenu Load(HINSTANCE hInst, LPCTSTR MenuName) {
			HMENU hNewMenu = ::LoadMenu(hInst, MenuName);
			WinMenu oMenu(hNewMenu);

			return oMenu;
		}

		static WinMenu Load(HINSTANCE hInst, int MenuName) {
			HMENU hNewMenu = ::LoadMenu(hInst, MAKEINTRESOURCE(MenuName));
			WinMenu oMenu(hNewMenu);

			return oMenu;
		}

		void CheckMenuItem(UINT MenuID, CheckedType Checked) {
			::CheckMenuItem(hMenu_, MenuID, (UINT)Checked);
		}
	};

}