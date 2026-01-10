#pragma once

#include <Windows.h>
#include "WindowInterfaces.h"
#include "ControlBinder.h"
#include "Theme.h"


namespace swktool
{
    class ControlBinder;

    //
    // ------------------------------------------------------------
    //  WindowHandlerBase — now stores IWindow*
    // ------------------------------------------------------------
    //
    class WindowHandlerBase :
        public IWindowHandler
        , public IWindow
    {
    public:
        WindowHandlerBase() : hwnd_(nullptr), hInstance_(nullptr), hostWindow_(nullptr), dpi_(0) {}
        virtual ~WindowHandlerBase() = default;

        HWND GetHwnd() const override { return hwnd_; }
        void SetHwnd(HWND hwnd) override { hwnd_ = hwnd; }

        void Invalidate() override { ::InvalidateRect(hwnd_, nullptr, FALSE); }
        UINT GetDpi() const override { return dpi_; }   // if you track DPI

        void SetHostWindow(IWindow* w) override { hostWindow_ = w; }
        IWindow* GetHostWindow() const override { return hostWindow_; }

        HINSTANCE GetInstance() const { return hInstance_; }
        void SetInstance(HINSTANCE hinst) { hInstance_ = hinst; }

        void OnClose() override { DestroyWindow(hwnd_); }

        LRESULT OnMessage(UINT msg, WPARAM wParam, LPARAM lParam) override
        {
            switch (msg)
            {
            case WM_CTLCOLORSTATIC:
            case WM_CTLCOLORLISTBOX:
            case WM_CTLCOLORSCROLLBAR:
            case WM_CTLCOLORMSGBOX:
            {
                HDC hdc = (HDC)wParam;

                // Set text color to system color
                SetTextColor(hdc, GetSysColor(COLOR_WINDOWTEXT));

                // Set background color to system color
                SetBkColor(hdc, GetSysColor(COLOR_WINDOW));

                // Return system brush
                //return (LRESULT)GetSysColorBrush(COLOR_WINDOW);
                static HBRUSH hbrRed = CreateSolidBrush(RGB(255, 0, 0)); return (LRESULT)hbrRed;
            }
            break;

            case WM_CTLCOLORBTN:
            {
                HDC hdc = (HDC)wParam;
                SetTextColor(hdc, RGB(0, 255, 255));   // cyan text
                SetBkColor(hdc, RGB(0, 0, 128));       // dark blue background
                static HBRUSH hbrBlue = CreateSolidBrush(RGB(0, 0, 128));
                return (LRESULT)hbrBlue;
            }
            break;


            }
            return DefWindowProc(hwnd_, msg, wParam, lParam);
        }

        LRESULT OnPaint() override
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd_, &ps);
            EndPaint(hwnd_, &ps);
            return 0;
        }

    private:
        HWND hwnd_;
        HINSTANCE hInstance_;
        IWindow* hostWindow_;
        UINT dpi_;

    protected:
        Theme currentTheme_{};
        std::unique_ptr<ControlBinder> binder_;
    };


    //
    // ------------------------------------------------------------
    //  DialogHandlerBase — now stores IWindow*
    // ------------------------------------------------------------
    //
    class DialogHandlerBase :
        public IWindowHandler
        , public IWindow
    {
    public:
        DialogHandlerBase() : hwnd_(nullptr), hostWindow_(nullptr), dpi_(0) {}
        virtual ~DialogHandlerBase() = default;

        void SetHwnd(HWND hwnd) override { hwnd_ = hwnd; }
        HWND GetHwnd() const override { return hwnd_; }

        void Invalidate() override { ::InvalidateRect(hwnd_, nullptr, FALSE); }
        UINT GetDpi() const override { return dpi_; }   // if you track DPI

        void SetHostWindow(IWindow* w) override { hostWindow_ = w; }
        IWindow* GetHostWindow() const override { return hostWindow_; }

        HINSTANCE GetInstance() const
        {
            return reinterpret_cast<HINSTANCE>(GetWindowLongPtr(hwnd_, GWLP_HINSTANCE));
        }

        virtual INT_PTR OnInitDialog(HWND hwndFocusedCtrl, LPARAM lParam) override { return TRUE; }

        LRESULT OnCommand(WORD id, WORD, HWND) override
        {
            if (id == IDOK || id == IDCANCEL)
            {
                EndDialog(hwnd_, id);
                return TRUE;
            }
            return FALSE;
        }

        void OnClose() override { EndDialog(hwnd_, IDCANCEL); }

        LRESULT OnMessage(UINT, WPARAM, LPARAM) override { return FALSE; }

    private:
        HWND hwnd_;
        IWindow* hostWindow_;   // NEW
        UINT dpi_;
    };


}

