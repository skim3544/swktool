#pragma once

#include <Windows.h>
#include <cstdint>


namespace swktool
{

    //
    // ------------------------------------------------------------
    //  IWindow — unified abstraction for Window + Dialog hosts
    // ------------------------------------------------------------
    //
    class IWindow
    {
    public:
        virtual ~IWindow() {}

        virtual HWND GetHwnd() const = 0;
        virtual void Invalidate() = 0;
        virtual UINT GetDpi() const = 0;
    };



    //
    // ------------------------------------------------------------
    //  IWindowHandler — message sink for Window + Dialog
    //  Now aware of its IWindow host
    // ------------------------------------------------------------
    //
    class IWindowHandler
    {
    public:
        static constexpr LRESULT HANDLED = 1;
        static constexpr LRESULT NOT_HANDLED = -1;

        virtual ~IWindowHandler() = default;

        //
        // HWND lifecycle
        //
        virtual void SetHwnd(HWND hwnd) = 0;
        virtual HWND GetHwnd() const = 0;

        //
        // NEW: Host window abstraction (Window or Dialog)
        //
        virtual void SetHostWindow(IWindow* window) = 0;
        virtual IWindow* GetHostWindow() const = 0;

        //
        // ---- Dialog / Window Lifecycle ----
        //
        virtual INT_PTR OnInitDialog(HWND hwndFocusedCtrl, LPARAM lParam) { return TRUE; }
        virtual LRESULT OnCreate(CREATESTRUCT* cs) { return 0; }
        virtual void OnDestroy() {}
        virtual void OnClose() { DestroyWindow(GetHwnd()); }

        //
        // ---- Commands ----
        //
        virtual LRESULT OnCommand(WORD id, WORD code, HWND control) { return 0; }

        //
        // ---- Notifications ----
        //
        virtual LRESULT OnNotify(int idCtrl, NMHDR* hdr) { return 0; }

        //
        // ---- Keyboard / Mouse ----
        //
        virtual LRESULT OnKeyDown(UINT vk, UINT flags) { return 0; }
        virtual LRESULT OnKeyUp(UINT vk, UINT flags) { return 0; }
        virtual LRESULT OnChar(UINT ch, UINT flags) { return 0; }

        virtual LRESULT OnMouseMove(UINT keys, int x, int y) { return 0; }
        virtual LRESULT OnLButtonDown(UINT keys, int x, int y) { return 0; }
        virtual LRESULT OnLButtonUp(UINT keys, int x, int y) { return 0; }
        virtual LRESULT OnRButtonDown(UINT keys, int x, int y) { return 0; }
        virtual LRESULT OnRButtonUp(UINT keys, int x, int y) { return 0; }

        //
        // ---- Focus ----
        //
        virtual LRESULT OnSetFocus(HWND oldFocus) { return 0; }
        virtual LRESULT OnKillFocus(HWND newFocus) { return 0; }

        //
        // ---- Painting ----
        //
        virtual LRESULT OnPaint() { return 0; }
        virtual LRESULT OnEraseBkgnd(HDC hdc) { return 0; }

        //
        // ---- Sizing / Moving ----
        //
        virtual LRESULT OnSize(UINT type, int cx, int cy) { return 0; }
        virtual LRESULT OnMove(int x, int y) { return 0; }

        //
        // ---- Non-client ----
        //
        virtual LRESULT OnNcCalcSize(BOOL calcValidRects, NCCALCSIZE_PARAMS* params) { return 0; }
        virtual LRESULT OnNcPaint(HRGN hrgn) { return 0; }
        virtual LRESULT OnNcHitTest(POINT pt) { return HTCLIENT; }
        virtual LRESULT OnNcDestroy() { return 0;  };

        //
        virtual WNDPROC GetOriginalWndProc() const = 0;
        virtual bool IsMDIFrame() const { return false; }
        //
        // ---- Default Message Router ----
        //
        virtual LRESULT OnMessage(UINT msg, WPARAM wParam, LPARAM lParam) { return 0; }
    };

}