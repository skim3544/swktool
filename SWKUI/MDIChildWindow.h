#pragma once


#include "Window.h"
#include "WindowRegister.h"
//#include "MDIFrameWindow.h"

namespace swktool 
{
    class MDIFrame;
	class MDIChildWindow : public Window 
	{
		inline static bool s_registered = false;

        MDIFrameWindow* Frame_ = nullptr;

	public:
        void SetMDIFrameWindow(MDIFrameWindow* pFrame);

        virtual void   PreRegisterWindow(WindowRegisterClass& wc) override
        {
            wc = WindowsDefaultRegister::CreateMDIChild(ClassName());
            //wc.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
            //wc.lpfnWndProc = DefMDIChildProc;   // CRITICAL
            //wc.hInstance = GetModuleHandle(nullptr);
            //wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
            //wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        }

        bool Register()
        {
            if (s_registered)
                return true;

            WindowRegisterClass wc{};
            wc.cbSize = sizeof(WindowRegisterClass);
            PreRegisterWindow(wc);

            if (!RegisterClassEx(&wc))
                return false;

            s_registered = true;
            return true;
        }

        bool AttachToMDI(HWND hwnd) { 
            return Subclass(hwnd); 
        }

        LRESULT OnNcDestroy() override;

	};

}
