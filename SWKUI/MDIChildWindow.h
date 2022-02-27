#ifndef __MDI_CLIENT_WINDOW_H__
#define __MDI_CLIENT_WINDOW_H__

#pragma once

#include "AWindow.h"
#include "Window.h"

//#include <windowsx.h>

namespace swktool {

	class MDIChildWindow : public Window {
	protected:
		HWND hwndMDIFrame_;
		HWND hwndMDIClient_;

	public:
		MDIChildWindow() : 
			Window(), 
			hwndMDIFrame_(nullptr), 
			hwndMDIClient_(nullptr) {

		}

		MDIChildWindow(LPCTSTR className, LPCTSTR Title, HINSTANCE hInst) :
			Window(className, Title, hInst), 
			hwndMDIFrame_(nullptr), 
			hwndMDIClient_(nullptr) {
			;
		}

		// WM_CHILDACTIVATE
		virtual void OnSize(UINT state, int nWidth, int nHeight) override {
			Width = nWidth;
			Height = nHeight;
		}

		virtual void OnClose() override {
			// default behavior is to close the current MDI window
			//::SendMessage(hwndMDIClient_, WM_MDIDESTROY, (WPARAM)hwndWindow, 0);
			//::SendMessage(hwndMDIClient_, WM_MDIDESTROY, (WPARAM)hwndWindow, 0);
			::DestroyWindow(GetWindowHWND());
		}

		virtual BOOL QueryEndSession() {
			return TRUE;
		}

		virtual void OnDestroy() {

		}


		virtual void OnChildAcrivate() { ; }

		virtual BOOL OnCreate(LPCREATESTRUCT lpCreateStruct) override {

			hwndMDIClient_ = GetParent(GetWindowHWND());
			hwndMDIFrame_ = GetParent(hwndMDIClient_);
			 
			return TRUE;
		}

		virtual void OnMDIActivate(HWND Activating, HWND Deactivating) { ;  
		}

		virtual LRESULT ProcessMessage(WORD msg, WPARAM wParam, LPARAM lParam) override;

	};
}


#endif // __MDI_CLIENT_WINDOW_H__