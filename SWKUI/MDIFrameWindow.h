#pragma once
#include "Window.h"
#include "MDIFrameHelper.h"

namespace swktool 
{
    class MDIChildWindow;

    class MDIFrameWindow : public Window
    {
        using MDIChildren = std::vector<swktool::MDIChildWindow*>;

    public:
        virtual bool IsMDIFrame() const { return true; }
        HWND GetMDIClientHWnd() const { return hMdiClient_; }

        // default class registration name for the MDI frame window. 
        PCWSTR ClassName() const override { return L"SWKUI_MDI_FRAME"; }

        void PreRegisterWindow(WindowRegisterClass& wc) override;
        LRESULT OnCreate(CREATESTRUCT* cs) override;

        LRESULT OnSize(UINT, int cx, int cy) override
        {
            if (hMdiClient_)
                MoveWindow(hMdiClient_, 0, 0, cx, cy, TRUE);

            return TRUE;
        }

        LRESULT OnCommand(WORD id, WORD code, HWND control) override
        {

            // Framework default: do nothing.
            // Applications override this to handle menu commands.
            return FALSE;
        }

        void OnClose() override
        {
            DestroyWindow(GetHwnd());
        }

        void OnDestroy() override;
        void RemoveChild(MDIChildWindow* child);

    protected:
        // Applications may override this to provide a menu for the MDI client.
        virtual HMENU GetDefaultWindowMenu();

        // Applications may override this to change the first child ID.
        virtual UINT GetFirstChildId() const { return 100; }

        template<typename TChildWindow>
        TChildWindow* CreateMdiChild(const wchar_t* title)
        {
            auto* child = new TChildWindow();
            child->SetMDIFrameWindow(this);
            child->Register();            

            MDICREATESTRUCT mcs{};
            mcs.szClass = child->ClassName();
            mcs.szTitle = title;
            mcs.hOwner = GetModuleHandle(nullptr);
            mcs.x = mcs.cx = CW_USEDEFAULT;
            mcs.y = mcs.cy = CW_USEDEFAULT;
            mcs.style = WS_VISIBLE | WS_CHILD | WS_OVERLAPPEDWINDOW;

            HWND hwndChild = (HWND)SendMessage(hMdiClient_, WM_MDICREATE, 0, (LPARAM)&mcs);

            if (!hwndChild) {
                delete child;
                return nullptr;
            }

            child->Subclass(hwndChild);
            AddChild(child);
            return child;
        }

        void AddChild(MDIChildWindow* child)
        {
            children_.push_back(child);
        }



    private:
        HWND hMdiClient_ = nullptr;
        MDIChildren children_;
    };

} // namespace swktool
