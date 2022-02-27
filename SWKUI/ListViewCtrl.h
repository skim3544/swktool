#pragma once

#include <CommCtrl.h>
#include "Ctrl.h"


namespace swktool {

	class AWindow;

	class ListViewCtrl : public Ctrl {
	public:
		ListViewCtrl(std::wstring Caption, DWORD Style, int x, int y, int Width, int Height,  AWindow* pParent, UINT CtrlID);
		ListViewCtrl(UINT CtrlID, AWindow* pParent);
		~ListViewCtrl() { ; }

        void SetView(DWORD dwView)
        {
            // Retrieve the current window style. 
            DWORD dwStyle = GetWindowLong(hwndCtrl, GWL_STYLE);

            // Set the window style only if the view bits changed.
            if ((dwStyle & LVS_TYPEMASK) != dwView)
            {
                SetWindowLong(hwndCtrl,
                    GWL_STYLE,
                    (dwStyle & ~LVS_TYPEMASK) | dwView);
            } 
        }

        void SetImageList(HIMAGELIST ImageList, int iImageList) 
        {
            ListView_SetImageList(hwndCtrl, ImageList, iImageList);
        }                            
	};

    struct ListViewItem : public LVITEM {

    };

    struct ListViewTileViewInfo : public LVTILEVIEWINFO {

    };

    struct ListViewColumn : public LVCOLUMN {

    };
}