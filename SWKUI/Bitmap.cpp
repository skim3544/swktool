#include "pch.h"
#include "DeviceContext.h"
#include "Bitmap.h"

namespace swktool {

	void  Bitmap::CreateCompatibleBitmap(const DeviceContext& DC, int nWidth, int nHeight) {
		Attach(::CreateCompatibleBitmap(DC.GetHandle(), nWidth, nHeight));
	}

	void Bitmap::CreateDIBitmap(const DeviceContext& DC,
		const BITMAPINFOHEADER* pbmih,
		DWORD flInit,
		const VOID* pjBits,
		const BITMAPINFO* pbmi,
		UINT  iUsage) 
	{

		Attach(::CreateDIBitmap(DC.GetHandle(), pbmih, flInit, pjBits, pbmi, iUsage));
	}


	void Bitmap::CreateDIBSection(
		const DeviceContext& DC,
		const BITMAPINFO* pbmi,
		UINT             usage,
		VOID** ppvBits,
		HANDLE           hSection,
		DWORD            offset) 
	{
		Attach(::CreateDIBSection(DC.GetHandle(), pbmi, usage, ppvBits, hSection, offset));
	}


	UINT Bitmap::GetDIBColorTable(
		const DeviceContext& DC,
		UINT    iStart,
		UINT    cEntries,
		RGBQUAD* prgbq) {
		return ::GetDIBColorTable(DC.GetHandle(), iStart, cEntries, prgbq);
	}

	int Bitmap::GetDIBits(
		const DeviceContext& DC,
		UINT         start,
		UINT         cLines,
		LPVOID       lpvBits,
		LPBITMAPINFO lpbmi,
		UINT         usage) {
		return ::GetDIBits(DC.GetHandle(), (HBITMAP)hGDIHandle_, start, cLines, lpvBits, lpbmi, usage);
	}


}