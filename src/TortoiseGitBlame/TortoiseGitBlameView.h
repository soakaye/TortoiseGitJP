﻿// TortoiseGit - a Windows shell extension for easy version control

// Copyright (C) 2008-2013, 2015-2023, 2025 - TortoiseGit
// Copyright (C) 2003-2008, 2014 - TortoiseSVN

// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software Foundation,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//


// TortoiseGitBlameView.h : interface of the CTortoiseGitBlameView class
//


#pragma once
#include "Scintilla.h"
#include "registry.h"
#include "SciEdit.h"
#include "GitBlameLogList.h"
#include "TortoiseGitBlameData.h"
#include "Tooltip.h"

const COLORREF black = RGB(0,0,0);
const COLORREF white = RGB(0xff,0xff,0xff);
const COLORREF red = RGB(0xFF, 0, 0);
const COLORREF offWhite = RGB(0xFF, 0xFB, 0xF0);
const COLORREF darkGreen = RGB(0, 0x80, 0);
const COLORREF darkBlue = RGB(0, 0, 0x80);
const COLORREF lightBlue = RGB(0xA6, 0xCA, 0xF0);
const int blockSize = 128 * 1024;

#define BLAMESPACE 5
#define LOCATOR_WIDTH 10

class CSciEditBlame: public CSciEdit
{
	DECLARE_DYNAMIC(CSciEditBlame)
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) override
	{
		switch (nChar)
		{
			case (VK_ESCAPE):
			{
				if ((Call(SCI_AUTOCACTIVE)==0)&&(Call(SCI_CALLTIPACTIVE)==0))
				{
					::SendMessage(::AfxGetApp()->GetMainWnd()->m_hWnd, WM_CLOSE, 0, 0);
					return;
				}
			}
			break;
		}
		CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
	}
};

class CTortoiseGitBlameView : public CView
{
	enum
	{
	// needs to start with 1, since 0 is the return value if *nothing* is clicked on in the context menu
	ID_BLAMEPREVIOUS = 1,
	ID_COMPAREWITHPREVIOUS,
	ID_SHOWLOG,
	ID_COPYHASHTOCLIPBOARD,
	ID_COPYLOGTOCLIPBOARD
	};

protected: // create from serialization only
	CTortoiseGitBlameView();
	DECLARE_DYNCREATE(CTortoiseGitBlameView)

// Attributes
public:
	CTortoiseGitBlameDoc* GetDocument() const;
	int GetEncode(unsigned char * buffer, int size, int *bomoffset);

// Overrides
public:
	afx_msg void OnDraw(CDC* pDC);  // overridden to draw this view
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	BOOL PreCreateWindow(CREATESTRUCT& cs) override;
protected:
	ULONG GetGestureStatus(CPoint ptTouch) override;

// Implementation
public:
	virtual ~CTortoiseGitBlameView();
#ifdef _DEBUG
	void AssertValid() const override;
	void Dump(CDumpContext& dc) const override;
#endif

// Generated message map functions
protected:
	BOOL PreTranslateMessage(MSG* pMsg) override;
	afx_msg void OnSysColorChange();
	afx_msg void OnDestroy();
	afx_msg void OnChangeEncode(UINT nID);
	afx_msg void OnEditFind();
	afx_msg void OnEditGoto();
	afx_msg void OnFindNext();
	afx_msg void OnFindPrev();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpcs);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSciPainted(NMHDR*, LRESULT*);
	afx_msg void OnLButtonDown(UINT nFlags,CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags,CPoint point){OnLButtonDown(nFlags,point);CView::OnRButtonDown(nFlags,point);};
	afx_msg void OnSciGetBkColor(NMHDR*, LRESULT*);
	afx_msg void OnSciZoom(NMHDR*, LRESULT*);
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg LRESULT OnFindDialogMessage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnViewNext();
	afx_msg void OnViewPrev();
	afx_msg void OnViewToggleLogID();
	afx_msg void OnUpdateViewToggleLogID(CCmdUI* pCmdUI);
	afx_msg void OnViewToggleAuthor();
	afx_msg void OnUpdateViewToggleAuthor(CCmdUI *pCmdUI);
	afx_msg void OnViewToggleDate();
	afx_msg void OnUpdateViewToggleDate(CCmdUI *pCmdUI);
	afx_msg void OnViewToggleShowFilename();
	afx_msg void OnUpdateViewToggleShowFilename(CCmdUI *pCmdUI);
	afx_msg void OnViewToggleShowOriginalLineNumber();
	afx_msg void OnUpdateViewToggleShowOriginalLineNumber(CCmdUI *pCmdUI);
	afx_msg void OnViewDetectMovedOrCopiedLinesToggleDisabled();
	afx_msg void OnUpdateViewDetectMovedOrCopiedLinesToggleDisabled(CCmdUI *pCmdUI);
	afx_msg void OnViewDetectMovedOrCopiedLinesToggleWithinFile();
	afx_msg void OnUpdateViewDetectMovedOrCopiedLinesToggleWithinFile(CCmdUI *pCmdUI);
	afx_msg void OnViewDetectMovedOrCopiedLinesToggleFromModifiedFiles();
	afx_msg void OnUpdateViewDetectMovedOrCopiedLinesToggleFromModifiedFiles(CCmdUI *pCmdUI);
	afx_msg void OnViewDetectMovedOrCopiedLinesToggleFromExistingFilesAtFileCreation();
	afx_msg void OnUpdateViewDetectMovedOrCopiedLinesToggleFromExistingFilesAtFileCreation(CCmdUI *pCmdUI);
	afx_msg void OnViewDetectMovedOrCopiedLinesToggleFromExistingFiles();
	afx_msg void OnUpdateViewDetectMovedOrCopiedLinesToggleFromExistingFiles(CCmdUI *pCmdUI);
	afx_msg void OnViewToggleIgnoreWhitespace();
	afx_msg void OnUpdateViewToggleIgnoreWhitespace(CCmdUI *pCmdUI);
	afx_msg void OnViewToggleShowCompleteLog();
	afx_msg void OnUpdateViewToggleShowCompleteLog(CCmdUI *pCmdUI);
	afx_msg void OnViewToggleOnlyFirstParent();
	afx_msg void OnUpdateViewToggleOnlyFirstParent(CCmdUI* pCmdUI);
	afx_msg void OnViewToggleFollowRenames();
	afx_msg void OnUpdateViewToggleFollowRenames(CCmdUI *pCmdUI);
	afx_msg void OnViewToggleColorByAge();
	afx_msg void OnUpdateViewToggleColorByAge(CCmdUI *pCmdUI);
	afx_msg void OnViewToggleLexer();
	afx_msg void OnUpdateViewToggleDarkMode(CCmdUI* pCmdUI);
	afx_msg void OnViewToggleDarkMode();
	afx_msg void OnUpdateViewToggleLexer(CCmdUI *pCmdUI);
	afx_msg void OnViewWrapLongLines();
	afx_msg void OnUpdateViewWrapLongLines(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewCopyToClipboard(CCmdUI *pCmdUI);
	void OnViewDetectMovedOrCopiedLines(DWORD dwDetectMovedOrCopiedLines);
	void ContextMenuAction(int cmd, GitRev* pRev, GIT_REV_LIST& parentHash, const std::vector<CString>& parentFilename, int line);
	void ReloadDocument();

	DECLARE_MESSAGE_MAP()

	static UINT m_FindDialogMessage;
public:
	void ParseBlame();
	void MapLineToLogIndex();
	void UpdateInfo(int encode = 0);
	CString ResolveCommitFile(int line);
	CString ResolveCommitFile(const CString& path);
	void FocusOn(std::unordered_set<CGitHash>&& pRevs, GitRevLoglist* selected);

	CSciEditBlame		m_TextView;
	CToolTips			m_ToolTip;
	CString				m_sLastFilename;

	HINSTANCE hInstance = nullptr;
	HINSTANCE hResource = nullptr;
	HWND currentDialog = nullptr;

	BOOL bIgnoreEOL = FALSE;
	BOOL bIgnoreSpaces = FALSE;
	BOOL bIgnoreAllSpaces = FALSE;

	BOOL m_bShowLogID = FALSE;
	BOOL m_bShowAuthor = FALSE;
	BOOL m_bShowDate = FALSE;
	BOOL m_bShowFilename = FALSE;
	BOOL m_bShowOriginalLineNumber = FALSE;
	DWORD m_dwDetectMovedOrCopiedLines = 0;
	BOOL m_bIgnoreWhitespace = FALSE;
	BOOL m_bShowCompleteLog = FALSE;
	BOOL m_bOnlyFirstParent = FALSE;
	BOOL m_bFollowRenames = FALSE;
	BOOL m_bBlameOutputContainsOtherFilenames = FALSE;

	LRESULT SendEditor(UINT Msg, WPARAM wParam=0, LPARAM lParam=0);

	void SetAStyle(int style, COLORREF fore, COLORREF back = ::GetSysColor(COLOR_WINDOW), int size = -1, const char* face = nullptr);

	void InitialiseEditor();
	LONG GetBlameWidth();
	void DrawBlame(HDC hDC);
	void DrawLocatorBar(HDC hDC);
	void SetTheme(bool bDark);
	void CopyToClipboard();
	bool DoSearch(CTortoiseGitBlameData::SearchDirection direction);
	bool GotoLine(int line);
	bool ScrollToLine(long line);

	void SetSelectedLine(int line) { m_SelectedLine = line;};

	int						m_MouseLine = -1;
	std::unordered_set<CGitHash>	m_selectedHashes;
	bool					m_colorage = false;
	bool					m_bLexer = false;
	bool					m_bWrapLongLines = false;

	CTortoiseGitBlameData	m_data;
	std::vector<int>		m_lineToLogIndex;

	CLogDataVector *		GetLogData();

	BOOL m_bShowLine = TRUE;

protected:
	void CreateFont();
	void CreateNewFont(bool resize);
	void SetupColoring();
	void SetupLexer(CString filename);
	void SetupCppLexer();
	int GetLineUnderCursor(CPoint point);
	COLORREF GetLineColor(size_t line);
	COLORREF InterColor(COLORREF c1, COLORREF c2, int Slider);
	CFont					m_font;
	CFont					m_italicfont;
	LONG					m_blamewidth = 0;
	LONG					m_revwidth = 0;
	LONG					m_logidwidth = 0;
	LONG					m_datewidth = 0;
	LONG					m_authorwidth = 0;
	LONG					m_filenameWidth = 0;
	LONG					m_originalLineNumberWidth = 0;
	LONG					m_linewidth = 0;
	int						m_SelectedLine = -1; ///< zero-based

	CString m_sLogIDFormat;

	COLORREF				m_mouserevcolor;
	COLORREF				m_mouseauthorcolor;
	COLORREF				m_selectedrevcolor;
	COLORREF				m_selectedauthorcolor;
	COLORREF				m_windowcolor;
	COLORREF				m_textcolor;
	COLORREF				m_texthighlightcolor;

	CRegStdDWORD				m_regOldLinesColor;
	CRegStdDWORD				m_regNewLinesColor;
	CRegStdDWORD				m_regDarkOldLinesColor;
	CRegStdDWORD				m_regDarkNewLinesColor;

	CGitBlameLogList * GetLogList();

	CFindReplaceDialog* m_pFindDialog = nullptr;

#ifdef USE_TEMPFILENAME
	char* m_Buffer = nullptr;
#endif

	int m_themeCallbackId = 0;

	DWORD					m_DateFormat = DATE_SHORTDATE;	// DATE_SHORTDATE or DATE_LONGDATE
	bool					m_bRelativeTimes = false;	// Show relative times

	CString					m_sRev;
	CString					m_sFileName;
	CString					m_sAuthor;
	CString					m_sDate;
	CString					m_sMessage;

	CString					m_sFindText;
	bool					m_bMatchCase = false;

	bool					m_bBlockUpdates = false;
};

#ifndef _DEBUG  // debug version in TortoiseGitBlameView.cpp
inline CTortoiseGitBlameDoc* CTortoiseGitBlameView::GetDocument() const
   { return reinterpret_cast<CTortoiseGitBlameDoc*>(m_pDocument); }
#endif

