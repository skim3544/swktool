#pragma once

#include <Windows.h>

namespace swktool
{
    //
        // ------------------------------------------------------------
        //  Registration + Message Loop (unchanged)
        // ------------------------------------------------------------
        //
        class IWindowsRegister {
        protected:
            using WindowRegisterClass = WNDCLASSEX;

        public:
            virtual PCWSTR ClassName() const = 0;
            virtual void PreRegisterWindow(WindowRegisterClass& wc) = 0;
        };

        LRESULT CALLBACK SWKWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
        LRESULT CALLBACK SWKMDIFrameProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

        class WindowsDefaultRegister
        {
        protected:
            using WindowRegisterClass = WNDCLASSEX;

        public:

            //
            // ------------------------------------------------------------
            //  Standard top-level window class
            // ------------------------------------------------------------
            //
            static WindowRegisterClass CreateDefault(LPCWSTR className)
            {
                WindowRegisterClass wc = {};
                wc.cbSize = sizeof(WindowRegisterClass);
                wc.style = CS_HREDRAW | CS_VREDRAW;
                wc.lpfnWndProc = SWKWindowProc;
                wc.hInstance = GetModuleHandle(nullptr);
                wc.lpszClassName = className;
                wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
                wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
                wc.hIconSm = wc.hIcon;
                wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

                return wc;
            }


            //
            // ------------------------------------------------------------
            //  MDI Frame window class
            // ------------------------------------------------------------
            //
            static WindowRegisterClass CreateMDIFrame(LPCWSTR className)
            {
                WindowRegisterClass wc = {};
                wc.cbSize = sizeof(WindowRegisterClass);
                wc.style = CS_HREDRAW | CS_VREDRAW;
                wc.lpfnWndProc = SWKMDIFrameProc;   // your custom frame proc
                wc.hInstance = GetModuleHandle(nullptr);
                wc.lpszClassName = className;
                wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
                wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
                wc.hIconSm = wc.hIcon;
                wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

                return wc;
            }


            //
            // ------------------------------------------------------------
            //  MDI Client window class (built-in MDICLIENT)
            // ------------------------------------------------------------
            //
            static WindowRegisterClass CreateMDIClient()
            {
                WindowRegisterClass wc = {};
                wc.cbSize = sizeof(WindowRegisterClass);
                wc.style = CS_HREDRAW | CS_VREDRAW;

                //
                // IMPORTANT:
                // MDICLIENT is a built-in class. We DO NOT register it.
                //
                wc.lpfnWndProc = DefWindowProc;     // MDI client uses default proc
                wc.hInstance = GetModuleHandle(nullptr);
                wc.lpszClassName = L"MDICLIENT";    // built-in class name

                wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
                wc.hIcon = nullptr;
                wc.hIconSm = nullptr;
                wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

                return wc;
            }
        };

}