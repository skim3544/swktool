#pragma once

#include "GDIObject.h"

namespace swktool {
	class DeviceContext;
	//typedef enum class OEMBMP_ : UINT {
		//	BTNCORNERS = OBM_BTNCORNERS,
		//	OLD_RESTORE = OBM_OLD_RESTORE,
		//	BTSIZE = OBM_BTSIZE,
		//	OLD_RGARROW = OBM_OLD_RGARROW,
		//	CHECK = OBM_CHECK,
		//	OLD_UPARROW = OBM_OLD_UPARROW,
		//	CHECKBOXES = OBM_CHECKBOXES,
		//	OLD_ZOOM = OBM_OLD_ZOOM,
		//	CLOSE = OBM_CLOSE,
		//	REDUCE = OBM_REDUCE,
		//	COMBO = OBM_COMBO,
		//	REDUCED = OBM_REDUCED,
		//	DNARROW = OBM_DNARROW,
		//	RESTORE = OBM_RESTORE,
		//	DNARROWD = OBM_DNARROWD,
		//	RESTORED = OBM_RESTORED,
		//	DNARROWI = OBM_DNARROWI,
		//	RGARROW = OBM_RGARROW,
		//	LFARROW = OBM_LFARROW,
		//	RGARROWD = OBM_RGARROWD,
		//	LFARROWD = OBM_LFARROWD,
		//	RGARROWI = OBM_RGARROWI,
		//	LFARROWI = OBM_LFARROWI,
		//	SIZE = OBM_SIZE,
		//	MNARROW = OBM_MNARROW,
		//	UPARROW = OBM_UPARROW,
		//	OLD_CLOSE = OBM_OLD_CLOSE,
		//	UPARROWD = OBM_UPARROWD,
		//	OLD_DNARROW = OBM_OLD_DNARROW,
		//	UPARROWI = OBM_UPARROWI,
		//	OLD_LFARROW = OBM_OLD_LFARROW,
		//	ZOOM = OBM_ZOOM,
		//	OLD_REDUCE = OBM_OLD_REDUCE,
		//	ZOOMD = OBM_ZOOMD,
		//} OEMBMP;

	class Bitmap : public GDIObject {
	public:
		Bitmap(HGDIOBJ hGDI) :
			GDIObject(hGDI)
		{
		}

		operator HBITMAP() const {
			return (HBITMAP)hGDIHandle_;
		}

		BOOL LoadOEMBitmap(UINT  nIDBitmap) {
			HINSTANCE hInst = NULL;
			Attach(::LoadBitmap(hInst, MAKEINTRESOURCE(nIDBitmap)));

			return (hGDIHandle_ != NULL) ? TRUE : FALSE;
		}

		BOOL LoadBitmap(UINT  nIDBitmap) {
			HINSTANCE hInst = NULL;
			Attach(::LoadBitmap(hInst, MAKEINTRESOURCE(nIDBitmap)));

			return (hGDIHandle_ != NULL) ? TRUE : FALSE;
		}

		void CreateBitmap(int nWidth, int nHeight, int nPlanes, UINT BitCount, const void* lpBits) {
			Attach(::CreateBitmap(nWidth, nHeight, nPlanes, BitCount, (const void*)lpBits));
		}

		void  CreateCompatibleBitmap(HDC hdc, int nWidth, int nHeight) {
			Attach(::CreateCompatibleBitmap(hdc, nWidth, nHeight));
		}
		void  CreateCompatibleBitmap(const DeviceContext& hdc, int nWidth, int nHeight);

		void CreatgeBitmapIndirect(BITMAP* pBM) {
			Attach(::CreateBitmapIndirect(pBM));
		}

		void CreateDIBitmap(HDC hdc,
			const BITMAPINFOHEADER* pbmih,
			DWORD flInit,
			const VOID* pjBits,
			const BITMAPINFO* pbmi,
			UINT  iUsage)
		{
			Attach(::CreateDIBitmap(hdc, pbmih, flInit, pjBits, pbmi, iUsage));
		}

		void CreateDIBitmap(const DeviceContext& DC,
			const BITMAPINFOHEADER* pbmih,
			DWORD flInit,
			const VOID* pjBits,
			const BITMAPINFO* pbmi,
			UINT  iUsage);

		void CreateDIBSection(
			HDC              hdc,
			const BITMAPINFO* pbmi,
			UINT             usage,
			VOID** ppvBits,
			HANDLE           hSection,
			DWORD            offset)
		{
			Attach(::CreateDIBSection(hdc, pbmi, usage, ppvBits, hSection, offset));
		}

		void CreateDIBSection(
			const DeviceContext& DC,
			const BITMAPINFO* pbmi,
			UINT             usage,
			VOID** ppvBits,
			HANDLE           hSection,
			DWORD            offset);

		BOOL GetDimensionEx(SIZE& Size) {
			::GetBitmapDimensionEx((HBITMAP)hGDIHandle_, &Size);
		}

		UINT GetDIBColorTable(
			HDC     hdc,
			UINT    iStart,
			UINT    cEntries,
			RGBQUAD* prgbq)
		{
			return ::GetDIBColorTable(hdc, iStart, cEntries, prgbq);
		}

		UINT GetDIBColorTable(
			const DeviceContext& DC,
			UINT    iStart,
			UINT    cEntries,
			RGBQUAD* prgbq);

		int GetDIBits(
			HDC          hdc,
			HBITMAP      hbm,
			UINT         start,
			UINT         cLines,
			LPVOID       lpvBits,
			LPBITMAPINFO lpbmi,
			UINT         usage) {
			return ::GetDIBits(hdc, hbm, start, cLines, lpvBits, lpbmi, usage);
		}

		int GetDIBits(
			const DeviceContext& DC,
			UINT         start,
			UINT         cLines,
			LPVOID       lpvBits,
			LPBITMAPINFO lpbmi,
			UINT         usage);


	};

}
