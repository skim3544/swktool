#pragma once

#include "resource.h"
#include "..\SWKUI\WindowHandlers.h"
#include "..\SWKUI\WindowRegister.h"
#include "..\SWKUI\DialogWindow.h"
#include "..\SWKUI\Window.h"


class MainWindow : public  swktool::Window 
{
public:
    PCWSTR  ClassName() const {
        return L"Sample Window";
    }

    virtual void PreRegisterWindow(WindowRegisterClass& wc) override 
{
        wc.style = CS_HREDRAW | CS_VREDRAW;
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wc.hInstance = ::GetModuleHandle(NULL);
        // add menu
        wc.lpszMenuName = MAKEINTRESOURCE(IDC_SIMPLEDIALOG);
        wc.hIcon = LoadIcon(wc.hInstance, MAKEINTRESOURCE(IDI_SIMPLEDIALOG));
        wc.hCursor = LoadCursor(nullptr, IDC_ARROW);                
    }

    LRESULT OnCommand(WORD id, WORD code, HWND control) override 
    {
        // by default, let DefWinProcess to take care of this message
        LRESULT CmdHandled = FALSE;
        switch (id)
        {
        case IDM_ABOUT:
            {
                swktool::DialogWindow w(IDD_ABOUTBOX, this);
                w.SetCaption(L"About Simple Window");
                auto result = w.ShowDialog();                 
            }
            CmdHandled = TRUE;
            break;

        case IDM_EXIT:
            OnClose();
            break;
        }
        return CmdHandled;
    }

    void OnDestroy() override
    {
        PostQuitMessage(0);
    }


};