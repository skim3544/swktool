#pragma once

#include <Windows.h>

namespace swktool
{
	LRESULT CALLBACK SWKWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	INT_PTR CALLBACK SWKDialogProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK SWKMDIFrameProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK SWKMDIChildProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
}

