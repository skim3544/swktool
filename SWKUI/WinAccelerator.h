#ifndef ___WINDOWS_ACCELERATOR_H___
#define ___WINDOWS_ACCELERATOR_H___

#include <Windows.h>

namespace swktool {
	class WinAccelerator {
	private:
		HACCEL hAccelTable;
		HINSTANCE  hInstance;
		HWND   hWnd;

	public:
		WinAccelerator(HINSTANCE hInst, HWND hWindow, int ID) :
			hAccelTable(nullptr),
			hInstance(hInst),
			hWnd(hWindow)
		{
			hAccelTable = ::LoadAccelerators(hInstance, MAKEINTRESOURCE(ID));
		}

		WinAccelerator(HINSTANCE hInst, HWND hWindow, LPACCEL pArray, int count) : 
			hAccelTable(nullptr),
			hInstance(hInst),
			hWnd(hWindow)
		{
			hAccelTable = ::CreateAcceleratorTable(pArray, count);
		}


		~WinAccelerator() {
			::DestroyAcceleratorTable(hAccelTable);
		}

		BOOL Translate(MSG* pMsg) {
			return ::TranslateAccelerator(hWnd, hAccelTable, pMsg);
		}
	};
}


#endif //___WINDOWS_ACCELERATOR_H___