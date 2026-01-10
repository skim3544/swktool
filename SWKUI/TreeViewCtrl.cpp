#include "pch.h"
#include "ctrl.h"
#include "Commctrl.h"
#include "TreeViewCtrl.h"
#include "ImageList.h"

namespace swktool {
	TreeViewCtrl::TreeViewCtrl(std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, IWindow* pParent, UINT CtrlID) : Ctrl(pParent){


		// Ensure that the common control DLL is loaded. 
		InitCommonControls();

		ID = CtrlID;
		hwndCtrl = ::CreateWindowEx(0,
			WC_TREEVIEW, (LPCWSTR)Caption.c_str(),
			Style, x, y, Height, Width,
			hParent, (HMENU)(UINT_PTR)ID, hInst, NULL);
	}
	TreeViewCtrl::TreeViewCtrl(UINT CtrlID, IWindow* pParent) : Ctrl(CtrlID, pParent) {

	}

	//TreeViewCtrl::TreeViewCtrl(UINT CtrlID, DialogWindow* pParent) : Ctrl(CtrlID, pParent) {

	//}

	BOOL TreeViewCtrl::GetCheckState(HTREEITEM hItem) {
		TVITEM tvItem;

		// Prepare to receive the desired information.
		tvItem.mask = TVIF_HANDLE | TVIF_STATE;
		tvItem.hItem = hItem;
		tvItem.stateMask = TVIS_STATEIMAGEMASK;

		// Request the information.
		TreeView_GetItem(hwndCtrl, &tvItem);

		// Return zero if it's not checked, or nonzero otherwise.
		return ((BOOL)(tvItem.state >> 12) - 1);
	}


	BOOL TreeViewCtrl::SetCheckState(HTREEITEM hItem, BOOL fCheck) {
		TVITEM tvItem;

		tvItem.mask = TVIF_HANDLE | TVIF_STATE;
		tvItem.hItem = hItem;
		tvItem.stateMask = TVIS_STATEIMAGEMASK;

		// Image 1 in the tree-view check box image list is the unchecked box. 
		// Image 2 is the checked box.

		tvItem.state = INDEXTOSTATEIMAGEMASK((fCheck ? 2 : 1));

		return TreeView_SetItem(hwndCtrl, &tvItem);
	}


	void TreeViewCtrl::SetImageList(const ImageList& ImgList, INT iImage) {
		TreeView_SetImageList(hwndCtrl, ImgList.GetHandle(), iImage);
	}

	void TreeViewCtrl::DeleteAllItems() {
		TreeView_DeleteAllItems(hwndCtrl);
	}

	void TreeViewCtrl::DeleteItem(HTREEITEM hitem) {
		TreeView_DeleteItem(hwndCtrl, hitem);
	}

	void TreeViewCtrl::BeginEditLabel(HTREEITEM hitem) {
		TreeView_EditLabel(hwndCtrl, hitem);
	}

	void TreeViewCtrl::EndEditLabel(BOOL bCancel) {
		TreeView_EndEditLabelNow(hwndCtrl, bCancel);
	}

	void TreeViewCtrl::EnsureVisible(HTREEITEM hitem) {
		TreeView_EnsureVisible(hwndCtrl, hitem);
	}

	void TreeViewCtrl::Expand(HTREEITEM hitem, UINT ExpandCode) {
		TreeView_Expand(hwndCtrl, hitem, ExpandCode);
	}

	HTREEITEM TreeViewCtrl::GetChild(HTREEITEM hitem) {
		return TreeView_GetChild(hwndCtrl, hitem);
	}

	COLORREF TreeViewCtrl::GetBKColor() {
		return TreeView_GetBkColor(hwndCtrl);
	}

	UINT TreeViewCtrl::GetCount() {
		return TreeView_GetCount(hwndCtrl);
	}

	HTREEITEM TreeViewCtrl::GetDropHilight() {
		return TreeView_GetDropHilight(hwndCtrl);
	}

	HWND TreeViewCtrl::GetEditControl() {
		return TreeView_GetEditControl(hwndCtrl);

	}

	DWORD TreeViewCtrl::GetExtendedStyle() {
		return TreeView_GetExtendedStyle(hwndCtrl);		
	}

	HTREEITEM TreeViewCtrl::GetFirstVisible() {
		return TreeView_GetFirstVisible(hwndCtrl);		
	}

	UINT TreeViewCtrl::GetIndent() {		
		return TreeView_GetIndent(hwndCtrl);
	}

	COLORREF TreeViewCtrl::GetInsertMarkColor() {
		return TreeView_GetInsertMarkColor(hwndCtrl);
	}

	UINT TreeViewCtrl::GetISearchStringCount() {
		return (TreeView_GetISearchString(hwndCtrl, NULL) + 1);
	}
	BOOL TreeViewCtrl::GetISearchString(LPCTSTR pStr) {
		
		return TreeView_GetISearchString(hwndCtrl, pStr);
	}

	BOOL TreeViewCtrl::GetItem(LPTVITEM	pitem) {
		return TreeView_GetItem(hwndCtrl, pitem);
	}

	int TreeViewCtrl::GetItemHeight() {
		
		return TreeView_GetItemHeight(hwndCtrl);
	}

	//BOOL TreeViewCtrl::GetItemPartRect(HTREEITEM hitem, RECT* prc) {	
	//	return TreeView_GetItemPartRect(hwndCtrl, hitem, prc, TVGIPR_BUTTON);
	//}

	BOOL TreeViewCtrl::GetItemRect(HTREEITEM hitem, LPRECT prc, BOOL code) {
		return TreeView_GetItemRect(hwndCtrl,hitem,prc,code);
	}

	UINT TreeViewCtrl::GetItemState(HTREEITEM hti, UINT mask) {
		return TreeView_GetItemState(hwndCtrl, hti, mask);		
	}

	HTREEITEM TreeViewCtrl::GetLastVisible() {
		return TreeView_GetLastVisible(hwndCtrl);
	}

	COLORREF TreeViewCtrl::GetLineColor() {
		return TreeView_GetLineColor(hwndCtrl);
	}


	HTREEITEM TreeViewCtrl::GetNextItem(HTREEITEM hitem, UINT code) {
		return TreeView_GetNextItem(hwndCtrl, hitem, code);
	}


	HTREEITEM TreeViewCtrl::GetNextSelected(HTREEITEM hitem) {
		return TreeView_GetNextSelected(hwndCtrl, hitem);
	}

	HTREEITEM TreeViewCtrl::GetNextSibling_(HTREEITEM hitem) {
		return TreeView_GetNextSibling(hwndCtrl, hitem);
	}

	HTREEITEM TreeViewCtrl::GetNextVisible(HTREEITEM hitem) {
		return TreeView_GetNextVisible(hwndCtrl, hitem);
	}

	HTREEITEM TreeViewCtrl::GetParent(HTREEITEM hitem) {
		return TreeView_GetParent(hwndCtrl, hitem);
	}

	HTREEITEM TreeViewCtrl::GetPrevSibling_(HTREEITEM hitem) {
		return TreeView_GetPrevSibling(hwndCtrl, hitem);
	}

	HTREEITEM TreeViewCtrl::GetPrevVisible(HTREEITEM hitem) {
		return TreeView_GetPrevVisible(hwndCtrl, hitem);
	}

	HTREEITEM TreeViewCtrl::GetRoot() {	
		return TreeView_GetRoot(hwndCtrl);
	}

	UINT		TreeViewCtrl::GetScrollTime() {
		return TreeView_GetScrollTime(hwndCtrl);
	}

	DWORD		TreeViewCtrl::GetSelectedCount() {
		return TreeView_GetSelectedCount(hwndCtrl);
	}

	HTREEITEM   TreeViewCtrl::GetSelection() {
		return TreeView_GetSelection(hwndCtrl);
	}




}