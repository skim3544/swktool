#pragma once
#include "Ctrl.h"


namespace swktool {
    class AWindow;

    class SysLinkCtrl :
        public Ctrl
    {
    public:
        // WS_VISIBLE | WS_CHILD | WS_TABSTOP
        SysLinkCtrl(std::wstring HtmlLinkText, DWORD Style, int x, int y, int Width, int Height, Window* pParent, UINT CtrlID);
        SysLinkCtrl(UINT CtrlID, Window* pParent);
        virtual ~SysLinkCtrl() { ; }

    };
}



