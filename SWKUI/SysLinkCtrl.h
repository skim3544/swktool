#pragma once
#include "Ctrl.h"


namespace swktool {

    class SysLinkCtrl :
        public Ctrl
    {
    public:
        // WS_VISIBLE | WS_CHILD | WS_TABSTOP
        SysLinkCtrl(std::wstring HtmlLinkText, DWORD Style, int x, int y, int Width, int Height, IWindow* pParent, UINT CtrlID);
        SysLinkCtrl(UINT CtrlID, IWindow* pParent);
        virtual ~SysLinkCtrl() { ; }

    };
}



