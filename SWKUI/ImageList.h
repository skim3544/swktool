#pragma once

#include <Windows.h>
#include <CommCtrl.h>



namespace swktool {
	class DeviceContext;
	class Bitmap;	
	class Icon;


	class ImageList {
	protected:
		HIMAGELIST  hImageList_;

	public:
		ImageList() : hImageList_(nullptr) {

		}

		ImageList(HIMAGELIST hImageListHandle) : hImageList_(hImageListHandle) 
		{
		}

		virtual  ~ImageList() 
		{
			try {
				Destroy();
			}
			catch (...) {
				;
			}
		}

		HIMAGELIST Create(int Width, int Height, UINT flags, int Initial, int nGrow)
		{
			hImageList_ = ImageList_Create(Width, Height, flags, Initial, nGrow);
		}



		BOOL BeginDrag(int ImageIndex, int Hotspotx, int Hotspoty) {
			return ImageList_BeginDrag(hImageList_, ImageIndex, Hotspotx, Hotspoty);
		}

		void EndDrag() {
			ImageList_EndDrag();
		}

		BOOL DragEnter(HWND hWindowLock, int x, int y)
		{
			return ImageList_DragEnter(hWindowLock, x, y);
		}

		BOOL DragLeave(HWND hWindowLock) {
			return ImageList_DragLeave(hWindowLock);
		}

		BOOL DragShowNolock(BOOL fShow) {
			return ImageList_DragShowNolock(fShow);
		}

		BOOL DragMove(int x, int y) {
			return ImageList_DragMove(x, y);
		}

		ImageList GetDragImage(POINT& pt, POINT& Hotspot) {
			return ImageList(ImageList_GetDragImage(&pt, &Hotspot));
		}

		BOOL SetDragCursorImage(int ImageIndex, int xHotspot, int yHotspot) {
			return ImageList_SetDragCursorImage(hImageList_, ImageIndex, xHotspot, yHotspot);
		}


		COLORREF GetBkColor() {
			return ImageList_GetBkColor(hImageList_);
		}

		COLORREF SetBkColor(COLORREF oColor) {
			return ImageList_SetBkColor(hImageList_, oColor);
		}

		HICON GetIcon(int i, UINT Flags) {
			return ImageList_GetIcon(hImageList_, i, Flags);
		}

		BOOL GetIconSize(int& Width, int& Height) {
			return ImageList_GetIconSize(hImageList_, &Width, &Height);
		}

		int GetImageCount() {
			return ImageList_GetImageCount(hImageList_);
		}

		BOOL GetImageInfo(int ImageIndex, IMAGEINFO& Info) {
			return ImageList_GetImageInfo(hImageList_, ImageIndex, &Info);
		}

		BOOL Remove(int ImageIndex) {
			return ImageList_Remove(hImageList_, ImageIndex);
		}

		void RemoveAll() {
			ImageList_RemoveAll(hImageList_);
		}

		BOOL SetIconSize(int Width, int Height) {
			return ImageList_SetIconSize(hImageList_, Width, Height);
		}

		BOOL SetImageCount(UINT uNewCount) {
			return ImageList_SetImageCount(hImageList_, uNewCount);
		}

		void AddIcon(HICON hIcon) {
			ImageList_AddIcon(hImageList_, hIcon);
		}

		// Windows API Doc Incorrect, the function returns HICON, not void
		HICON ExtractIcon(int ImageIndex) {
			return ImageList_ExtractIcon(NULL, hImageList_, ImageIndex);
		}

		int AddBitmap(HBITMAP hBitmap, HBITMAP hBitmapMask = NULL) {
			return ImageList_Add(hImageList_, hBitmap, hBitmapMask);
		}

		int AddMasked(HBITMAP hBitmap, COLORREF crMask) {
			return ImageList_AddMasked(hImageList_, hBitmap, crMask);
		}

		BOOL Draw(int ImageIndex, HDC hDC, int x, int y, UINT fStyle) {
			return ImageList_Draw(hImageList_, ImageIndex, hDC, x, y, fStyle);
		}

		BOOL Draw(int ImageIndex, const DeviceContext& hDC, int x, int y, UINT fStyle);

		BOOL DrawEx(int ImageIndex, HDC hDC, int x, int y, int Width, int Height, COLORREF BKColor, COLORREF FGColor, UINT fStyle) {
			return ImageList_DrawEx(hImageList_, ImageIndex, hDC, x, y, Width, Height, BKColor, FGColor, fStyle);
		}
		BOOL DrawEx(int ImageIndex, const DeviceContext& hDC, int x, int y, int Width, int Height, COLORREF BKColor, COLORREF FGColor, UINT fStyle);

		BOOL DrawIndirect(IMAGELISTDRAWPARAMS& imldp) {
			return ImageList_DrawIndirect(&imldp);
		}

		ImageList Duplicate() {
			return ImageList(ImageList_Duplicate(hImageList_));
		}

		void LoadBitmap(HINSTANCE hInstance, LPCTSTR lpBitmap, int Width, int nGrow, COLORREF crMask) {
			ImageList_LoadBitmap(hInstance, lpBitmap, Width, nGrow, crMask);
		}

		void LoadBitmapA(HINSTANCE hInstance, LPCSTR lpBitmap, int Width, int nGrow, COLORREF crMask, UINT uType, UINT uFlags) {
			ImageList_LoadImageA(hInstance, lpBitmap, Width, nGrow, crMask, IMAGE_BITMAP, uFlags);
		}

		void LoadBitmapW(HINSTANCE hInstance, LPCWSTR lpBitmap, int Width, int nGrow, COLORREF crMask, UINT uType, UINT uFlags) {
			ImageList_LoadImageW(hInstance, lpBitmap, Width, nGrow, crMask, IMAGE_BITMAP, uFlags);
		}

		ImageList Merge(ImageList ImageList1, int ImageIndex1, ImageList ImageList2, int ImageIndex2, int Width, int Height)
		{
			return ImageList(Merge2(ImageList1.hImageList_, ImageIndex1, ImageList2.hImageList_, ImageIndex2, Width, Height));
		}

		ImageList Read(IStream& Stream) {
			return ImageList(ImageList_Read(&Stream));
		}

		// // dwFlags = ILP_NORMAL or ILP_DOWNLEVEL;
		HRESULT ReadEx(DWORD   dwFlags, IStream& Stream, REFIID riid, PVOID* ppv) {
			return ImageList_ReadEx(dwFlags, &Stream, riid, ppv);
		}

		BOOL Replace(int ImageIndex, HBITMAP hBitmap, HBITMAP hMask) {
			return ImageList_Replace(hImageList_, ImageIndex, hBitmap, hMask);
		}
		int ReplaceIcon(int ImageIndex, HICON hIcon) {
			return ImageList_ReplaceIcon(hImageList_, ImageIndex, hIcon);
		}

		BOOL SetOverlayImage(int ImageIndex, int OverlayMask) {
			return ImageList_SetOverlayImage(hImageList_, ImageIndex, OverlayMask);
		}

		BOOL Write(IStream& Stream) {
			return ImageList_Write(hImageList_, &Stream);
		}

		// dwFlags = ILP_NORMAL or ILP_DOWNLEVEL;
		HRESULT WriteEx(DWORD dwFlags, IStream& Stream) {
			return ImageList_WriteEx(hImageList_, dwFlags , &Stream);
		}


	protected:
		HIMAGELIST Merge2(HIMAGELIST himl1, int ImageIndex1, HIMAGELIST himl2, int ImageIndex2, int Width, int Height) {
			return ImageList_Merge(himl1, ImageIndex1, himl2, ImageIndex2, Width, Height);
		}

		void Destroy() {
			if (hImageList_ != nullptr) {
				ImageList_Destroy(hImageList_);
				hImageList_ = nullptr;
			}
		}
	};

}