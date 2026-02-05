
#pragma once

#include "Ctrl.h"

namespace swktool {
	class ImageList;
	class IWindow;
	class TreeViewCtrl : public Ctrl 
	{

	public:
		TreeViewCtrl(std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, IWindow* pParent, UINT CtrlID);
		TreeViewCtrl(IWindow* pParent) : Ctrl(pParent) { ; }
		TreeViewCtrl(UINT CtrlID, IWindow* pParent);
		~TreeViewCtrl() { ; }

		BOOL GetCheckState(HTREEITEM hItem);
		BOOL SetCheckState(HTREEITEM hItem, BOOL fCheck);

		void SetImageList(const ImageList& ImgList, INT iImage = TVSIL_NORMAL);

		void DeleteAllItems();
		void DeleteItem(HTREEITEM hitem);

		void BeginEditLabel(HTREEITEM hitem);
		void EndEditLabel(BOOL bCancel);

		void EnsureVisible(HTREEITEM hitem);
		
		void Expand(HTREEITEM hitem, UINT ExpandCode = TVE_EXPAND);
		COLORREF GetBKColor();
		HTREEITEM GetChild(HTREEITEM hitem);
		UINT GetCount();
		HTREEITEM GetDropHilight();

		HWND GetEditControl();
		DWORD GetExtendedStyle();
		HTREEITEM GetFirstVisible();
		UINT GetIndent();
		COLORREF GetInsertMarkColor();
		UINT GetISearchStringCount();
		BOOL GetISearchString(LPCTSTR pstr);
		BOOL GetItem(LPTVITEM pitem);
		int GetItemHeight();
		//BOOL GetItemPartRect(HTREEITEM hitem, RECT* prc);

		BOOL GetItemRect(HTREEITEM hitem, LPRECT prc, BOOL code);
		UINT GetItemState(HTREEITEM hti, UINT mask);
		HTREEITEM GetLastVisible();
		COLORREF GetLineColor();

		HTREEITEM GetNextItem(HTREEITEM hitem, UINT code);
		HTREEITEM GetNextSelected(HTREEITEM hitem);

		// had to add _ to resolve macro name conflict
		HTREEITEM GetNextSibling_(HTREEITEM hitem);
		HTREEITEM GetNextVisible(HTREEITEM hitem);
		HTREEITEM GetParent(HTREEITEM hitem);
		HTREEITEM GetPrevSibling_(HTREEITEM hitem);
		HTREEITEM GetPrevVisible(HTREEITEM hitem);
		HTREEITEM GetRoot();
		UINT		GetScrollTime();
		DWORD		GetSelectedCount();
		HTREEITEM   GetSelection();
	};
}
