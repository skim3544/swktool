#pragma once

#include "resource.h"
#include "..\SWKUI\DialogWindow.h"

class MainWindow : 
    public  swktool::Window {


public:
    PCWSTR  ClassName() const {
        return L"Sample Window";
    }

    virtual void PreRegisterWindow(WindowRegisterClass& wc) override {
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wc.hInstance = ::GetModuleHandle(NULL);
        wc.lpszMenuName = MAKEINTRESOURCE(IDC_SIMPLEDIALOG);
        wc.hIcon = LoadIcon(wc.hInstance, MAKEINTRESOURCE(IDI_SIMPLEDIALOG));
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
        //(PCWSTR) L"IDC_SIMPLEDIALOG";
        //wc.lpszMenuName = ClassName();
    }

    virtual LRESULT OnCommand(WPARAM wParam, LPARAM lParam) 
    {
        auto result = 0;
        switch (wParam)
        {
        case IDM_ABOUT:
            {
                swktool::DialogWindow w(IDD_ABOUTBOX, this);
                w.SetCaption(L"About Simple Window");
                result = w.ShowDialog();
            }
            break;

        case IDM_EXIT:
            OnClose();
            break;

        default:
            return DefWindowProc(m_hwnd, WM_COMMAND, wParam, lParam);
        }
        return 0L;
    }

};