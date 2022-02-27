#pragma once
#include <windows.h>

#include <vector>
#include <algorithm>

class WindowReg {
	WNDCLASSEXW mCLS = {};
public:
	WindowReg() {	
		mCLS.cbSize = sizeof(WNDCLASSEX);

		mCLS.style = CS_HREDRAW | CS_VREDRAW;
		//mCLS.lpfnWndProc = WndProc;
		mCLS.cbClsExtra = 0;
		mCLS.cbWndExtra = 0;
		//mCLS.hInstance = hInstance;
		//mCLS.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WINAPP));
		mCLS.hCursor = LoadCursor(nullptr, IDC_ARROW);
		mCLS.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
		//mCLS.lpszMenuName = MAKEINTRESOURCEW(IDC_WINAPP);
		//mCLS.lpszClassName = szWindowClass;
		mCLS.lpszClassName = L"test";
		//mCLS.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));		
	}
	~WindowReg() { ; }

	WNDCLASSEXW& GetWndClass();
};


class WindowFrame {
public: 
	WindowFrame(HINSTANCE hInst) {}
	~WindowFrame() { ; }

	ATOM Register(WindowReg& Reg) {
		WNDCLASSEXW& Cls = Reg.GetWndClass();

		return RegisterClassExW(&Cls);
		//return RegisterClass(&Cls);
	}
};


class EventConsumer {
	HWND hWND = {};


public:
	EventConsumer(HWND hWND) { ; }

	HWND  GetHWND() const {}
	void  ProcessMsg() { ; }
};


class WinEvent
{
private:
	static std::vector<EventConsumer>   MessageProcessors;

public:
	static LONG FAR PASCAL ProcMessage(HWND hWND, UINT msg, WPARAM wParam, LPARAM lParam);
	static void Register(void* Window);
};

LONG FAR PASCAL WinEvent::ProcMessage(HWND hWND, UINT msg, WPARAM wParam, LPARAM lParam) {
	std::for_each(MessageProcessors.begin(), MessageProcessors.end(),
		[hWND](EventConsumer& Other) { 
			if (Other.GetHWND() == hWND) {
				Other.ProcessMsg();
			}
		});
}


void WinEvent::Register(void* Window) {

}
