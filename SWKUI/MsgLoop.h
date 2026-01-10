#pragma once

#include <windows.h>

namespace swktool
{
    class IMsgLoop
    {
    public:
        virtual int Run() = 0;
    };

    class AppMsgLoop : public IMsgLoop
    {
    public:
        virtual ~AppMsgLoop() = default;

        int Run() override
        {
            MSG msg = {};
            while (GetMessage(&msg, nullptr, 0, 0))
            {
                if (!PreTranslateMessage(msg))
                {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }
            }
            return (int)msg.wParam;
        }

        virtual bool PreTranslateMessage(MSG&) { return false; }
    };


    class AppIdleMsgLoop : public IMsgLoop
    {
    public:
        int Run() override
        {
            MSG msg = {};

            while (true)
            {
                while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
                {
                    if (msg.message == WM_QUIT)
                        return (int)msg.wParam;

                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }

                OnIdle();
            }
        }

        virtual void OnIdle() {}
    };
}