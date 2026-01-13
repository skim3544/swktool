#pragma once

#include "../SWKUI/Window.h"
#include "../SWKUI/WindowRegister.h"
#include "../SWKUI/MDIFrameWindow.h"
#include "resource.h"
#include "HelloWindow.h"
#include "RectWindow.h"

class MdiMainWindow : public swktool::MDIFrameWindow
{   
public:
    PCWSTR ClassName() const override { return L"SWKUI_MDI_MAIN_DEMO"; }
    void PreRegisterWindow(WindowRegisterClass& wc) override    
    {
        swktool::MDIFrameWindow::PreRegisterWindow(wc);

        wc.lpszMenuName = MAKEINTRESOURCE(MDIMENUHELLO); // demo menu

    }

    LRESULT OnCreate(CREATESTRUCT* cs) override 
    {
        if (!MDIFrameWindow::OnCreate(cs)) 
            return FALSE; 
        
        return TRUE; 
    }

    LRESULT OnCommand(WORD id, WORD code, HWND control) override
    {
        switch (id)
        {
        case IDM_FILE_NEWHELLO:
            CreateMdiChild<MDIHelloWindow>(TEXT("Hello"));
            return TRUE;

        case IDM_FILE_NEWRECT:
            CreateMdiChild<RectChildWindow>(TEXT("Rectangle"));
            return TRUE;

        case IDM_EXIT:
            OnClose();
            return TRUE;
        }
        return FALSE;
    }

    void OnClose() override
    {
        DestroyWindow(GetHwnd());
    }
};
