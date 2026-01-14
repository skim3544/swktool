#pragma once


#include "Window.h"
#include "WindowRegister.h"

namespace swktool 
{
    class MDIFrameWindow;
	class MDIChildWindow : public Window 
	{		
        MDIFrameWindow* Frame_ = nullptr;

	public:
        void SetMDIFrameWindow(MDIFrameWindow* pFrame);
        void PreRegisterWindow(WindowRegisterClass& wc) override;
        bool Register();

        LRESULT OnNcDestroy() override;

        // used for subclassing 
        bool AttachToMDI(HWND hwnd);
	};

}
