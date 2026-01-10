#pragma once

#include <windows.h>

namespace swktool 
{
    struct Theme
    {
        COLORREF TextColor = RGB(0, 0, 0);
        COLORREF BkColor = RGB(255, 255, 255);
        COLORREF BorderColor = RGB(200, 200, 200);
        bool DarkMode = false;

        // Future expansion:
        // HFONT Font;
        // int CornerRadius;
        // COLORREF AccentColor;
    };

}
