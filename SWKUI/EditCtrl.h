#pragma once


#include "Ctrl.h"

//class swktool::BaseDialog;

namespace swktool {
	class AWindow;

	typedef enum class _EditCtrlStyle : DWORD {
		BORDER = WS_BORDER,
		CHLID = WS_CHILD, 
		VISIBLE = WS_VISIBLE,
		VSCHOOL = WS_VSCROLL,
		LEFT = ES_LEFT,
		CENTER = ES_CENTER,
		RIGHT = ES_RIGHT,
		LOWERCASE = ES_LOWERCASE,		
		UPPERCASE = ES_UPPERCASE,
		MULTILINE = ES_MULTILINE,
		NOHIDESEL = ES_NOHIDESEL,
		NUMBEDR = ES_NUMBER,
		OEMCONVERT = ES_OEMCONVERT,
		PASSWORD= ES_PASSWORD,
		READONLY = ES_READONLY,
		AUTO_HVSCROLL = ES_AUTOHSCROLL,
		AUTO_VSCROLL=ES_AUTOVSCROLL,
		WANTRETURN=ES_WANTRETURN
	} EditCtrlStyle;

	DEFINE_ENUM_FLAG_OPERATORS(EditCtrlStyle);

	class EditCtrl : public Ctrl {
		const int STRING_BUF_SIZE = 255;

	public:
		EditCtrl(std::wstring Caption, DWORD Style, int x, int y, int Height, int Width, Window* pParent, UINT CtrlID);
		EditCtrl(UINT CtrlID, swktool::Window* pParent) : 
			Ctrl(CtrlID, pParent) 
		{
		}
		~EditCtrl() { ; }

		void LimitText(int cchMax)  {
			((void)SendMessage((hwndCtrl), EM_LIMITTEXT, (WPARAM)(cchMax), 0L));
		}

		int GetLineCount() const {
			return ((int)(DWORD)SendMessage((hwndCtrl), EM_GETLINECOUNT, 0L, 0L));
		}

		void GetRect(RECT* lprc) const {
			((void)SendMessage((hwndCtrl), EM_GETRECT, 0L, (LPARAM)(RECT*)(lprc))); 
		}
		void SetRect(const RECT* lprc) {
			((void)SendMessage((hwndCtrl), EM_SETRECT, 0L, (LPARAM)(const RECT*)(lprc))); 
		}
		void SetRectNoPaint(const RECT* lprc) {
			((void)SendMessage((hwndCtrl), EM_SETRECTNP, 0L, (LPARAM)(const RECT*)(lprc))); 
		}


		void SetSelection(int ichStart, int ichEnd) {
			((void)SendMessage((hwndCtrl), EM_SETSEL, (ichStart), (ichEnd)));
		}

		void ReplaceSel(LPCTSTR lpszReplace) {
			((void)SendMessage((hwndCtrl), EM_REPLACESEL, 0L, (LPARAM)(LPCTSTR)(lpszReplace)));
		}

		BOOL GetModify() const {
			return ((BOOL)(DWORD)SendMessage((hwndCtrl), EM_GETMODIFY, 0L, 0L));
		}

		void SetModify(BOOL fModified)   {   
			((void)SendMessage((hwndCtrl), EM_SETMODIFY, (WPARAM)(UINT)(fModified), 0L));
		}

		BOOL ScrollCaret() const {
			return ((BOOL)(DWORD)SendMessage((hwndCtrl), EM_SCROLLCARET, 0, 0L));
		}

		int LineFromChar(int ich) {
			return ((int)(DWORD)SendMessage((hwndCtrl), EM_LINEFROMCHAR, (WPARAM)(int)(ich), 0L));
		}


		int LineIndex(int line) const { 
			return ((int)(DWORD)SendMessage((hwndCtrl), EM_LINEINDEX, (WPARAM)(int)(line), 0L)); 
		}
		int LineLength(int line) const { 
			return ((int)(DWORD)SendMessage((hwndCtrl), EM_LINELENGTH, (WPARAM)(int)(line), 0L)); 
		}

		void ScrollLine(int dv, int dh) {
			((void)SendMessage((hwndCtrl), EM_LINESCROLL, (WPARAM)(dh), (LPARAM)(dv)));
		}

		BOOL CanUndo() const {
			return ((BOOL)(DWORD)SendMessage((hwndCtrl), EM_CANUNDO, 0L, 0L));
		}
		BOOL Undo() {
			return ((BOOL)(DWORD)SendMessage((hwndCtrl), EM_UNDO, 0L, 0L));
		}
		void EmptyUndoBuffer() {
			((void)SendMessage((hwndCtrl), EM_EMPTYUNDOBUFFER, 0L, 0L));
		}
		void SetPasswordChar(UINT ch) {
			((void)SendMessage((hwndCtrl), EM_SETPASSWORDCHAR, (WPARAM)(UINT)(ch), 0L));
		}
		void SetTabStops(int cTabs, const int* lpTabs) {
			((void)SendMessage((hwndCtrl), EM_SETTABSTOPS, (WPARAM)(int)(cTabs), (LPARAM)(const int*)(lpTabs)));
		}
		BOOL FmtLines(BOOL fAddEOL) {
			((BOOL)(DWORD)SendMessage((hwndCtrl), EM_FMTLINES, (WPARAM)(BOOL)(fAddEOL), 0L));
		}
		HLOCAL GetHandle() const {
			((HLOCAL)(UINT_PTR)SendMessage((hwndCtrl), EM_GETHANDLE, 0L, 0L));
		}
		void SetHandle(HLOCAL h) {
			((void)SendMessage((hwndCtrl), EM_SETHANDLE, (WPARAM)(UINT_PTR)(HLOCAL)(h), 0L));
		}

		int GetFirstVisibleLine() const {
			return ((int)(DWORD)SendMessage((hwndCtrl), EM_GETFIRSTVISIBLELINE, 0L, 0L));
		}

		BOOL SetReadOnly(BOOL fReadOnly) {
			((BOOL)(DWORD)SendMessage((hwndCtrl), EM_SETREADONLY, (WPARAM)(BOOL)(fReadOnly), 0L));
		}

		TCHAR GetPasswordChar() const {
			return ((TCHAR)(DWORD)SendMessage((hwndCtrl), EM_GETPASSWORDCHAR, 0L, 0L));
		}

		void SetWordBreakProc(EDITWORDBREAKPROC lpfnWordBreak) {
			return ((void)SendMessage((hwndCtrl), EM_SETWORDBREAKPROC, 0L, (LPARAM)(EDITWORDBREAKPROC)(lpfnWordBreak)));
		}


		EDITWORDBREAKPROC GetWordBreakProc() const {
			((EDITWORDBREAKPROC)SendMessage((hwndCtrl), EM_GETWORDBREAKPROC, 0L, 0L));
		}


		int GetLine(int line, LPTSTR lpch, int cchMax)
		{
			WORD* pWordPtr = reinterpret_cast<WORD*>(lpch);
			*pWordPtr = (WORD)cchMax;

			return ((int)(DWORD)SendMessage((hwndCtrl), EM_GETLINE, (WPARAM)(int)(line), (LPARAM)(LPTSTR)(lpch)));		
		}
	};

}


