#include "pch.h"
#include "framework.h"
#include "Ctrl.h"
#include "Window.h"


#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#else
#define DBG_NEW new
#endif


// When you are using pre-compiled headers, this source file is necessary for compilation to succeed.
#pragma comment(lib, "Comctl32.lib")




namespace swktool 
{
	Ctrl::Ctrl(IWindow* pParent) :
		parentWindow(pParent), hInst(nullptr), hwndCtrl(nullptr), hParent(nullptr), ID(-1) 
	{
		if (pParent)
		{
			hParent = pParent->GetHwnd();
			hInst = (HINSTANCE)GetWindowLongPtr(hParent, GWLP_HINSTANCE);
		}
		else
		{
			hParent = nullptr;
			hInst = GetModuleHandle(nullptr);
		}
	}


	Ctrl::Ctrl(UINT CtrlID, IWindow* pParent) :
		hInst(nullptr), hwndCtrl(nullptr), hParent(nullptr), ID(CtrlID) {

		hParent = pParent->GetHwnd();
		hInst = (HINSTANCE)GetWindowLongPtr(hParent, GWLP_HINSTANCE);

		hwndCtrl = GetDlgItem(hParent, CtrlID);

		GetWindowRect(hwndCtrl, &Rect);
	}

	Ctrl::Ctrl(HWND ExistingHandle, IWindow* pParent) :
		hInst(nullptr),
		hwndCtrl(ExistingHandle)
	{
		if (pParent)
		{
			hParent = pParent->GetHwnd();
		}
	}

	Ctrl::~Ctrl()
	{
		if (binder_)
			binder_->Remove(this);
	}


	void Ctrl::Enable() {
		::EnableWindow(hwndCtrl, TRUE);
	}

	void Ctrl::Disable() {
		::EnableWindow(hwndCtrl, FALSE);
	}

	void Ctrl::Show() {
		::ShowWindow(hwndCtrl, TRUE);
	}
	void Ctrl::Hide() {
		::ShowWindow(hwndCtrl, FALSE);
	}
}