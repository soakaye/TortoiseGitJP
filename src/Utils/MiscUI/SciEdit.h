﻿// TortoiseGit - a Windows shell extension for easy version control

// Copyright (C) 2009-2023, 2025 - TortoiseGit
// Copyright (C) 2003-2008, 2013, 2018, 2020 - TortoiseSVN

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

#pragma once
#include "../SmartHandle.h"
#include "ILexer.h"
#include "scintilla.h"
#include "SciLexer.h"
#include "ProjectProperties.h"
#include "PersonalDictionary.h"
#include "LruCache.h"
#include <spellcheck.h>
// the following should be last
#pragma include_alias("hunvisapi.h", "../../ext/build/hunspell/hunvisapi.h")
#pragma include_alias("config.h", "../../ext/build/hunspell/config.h")
#include "../../ext/hunspell/src/hunspell/hunspell.hxx"

_COM_SMARTPTR_TYPEDEF(ISpellCheckerFactory, __uuidof(ISpellCheckerFactory));
_COM_SMARTPTR_TYPEDEF(ISpellChecker, __uuidof(ISpellChecker));
_COM_SMARTPTR_TYPEDEF(IEnumSpellingError, __uuidof(IEnumSpellingError));
_COM_SMARTPTR_TYPEDEF(ISpellingError, __uuidof(ISpellingError));

#define AUTOCOMPLETE_SPELLING		0
#define AUTOCOMPLETE_FILENAME		1
#define AUTOCOMPLETE_PROGRAMCODE	2
#define AUTOCOMPLETE_SNIPPET		3

//forward declaration
class CSciEdit;

/**
 * \ingroup Utils
 * This class acts as an interface so that CSciEdit can call these methods
 * on other objects which implement this interface.
 * Classes implementing this interface must call RegisterContextMenuHandler()
 * in CSciEdit to register themselves.
 */
class CSciEditContextMenuInterface
{
public:
	/**
	 * When the handler is called with this method, it can add entries
	 * to the \a mPopup context menu itself. The \a nCmd param is the command
	 * ID number the handler must use for its commands. For every added command,
	 * the handler is responsible to increment the \a nCmd param by one.
	 */
	virtual void		InsertMenuItems(CMenu& mPopup, int& nCmd);

	/**
	 * The handler is called when the user clicks on any context menu entry
	 * which isn't handled by CSciEdit itself. That means the handler might
	 * be called for entries it hasn't added itself!
	 * \remark the handler should return \a true if it handled the call, otherwise
	 * it should return \a false
	 */
	virtual bool		HandleMenuItemClick(int cmd, CSciEdit * pSciEdit);

	virtual void		HandleSnippet(int type, const CString &text, CSciEdit *pSciEdit);
};

/**
 * \ingroup Utils
 * Encapsulates the Scintilla edit control. Usable as a replacement for the
 * MFC CEdit control, but not a drop-in replacement!
 * Also provides additional features like spell checking, auto completion, ...
 */
class CSciEdit : public CWnd
{
	DECLARE_DYNAMIC(CSciEdit)
public:
	CSciEdit();
	~CSciEdit();

	void				SetAStyle(int style, COLORREF fore, COLORREF back = ::GetSysColor(COLOR_WINDOW), int size = -1, const char* face = nullptr);
	/**
	 * Set style and Scintilla configuration for read-only patch views
	 */
	void SetUDiffStyle();

	/**
	 * Initialize the scintilla control. Must be called prior to any other
	 * method!
	 */
	void		Init(const ProjectProperties& props);
	/** Initialize the scintilla control.
	 * lLanguage for initialiring spell checker: 0 = auto-detect language, -1 disable, or language code
	 */
	void		Init(LONG lLanguage = 0);
	void		SetIcon(const std::map<int, UINT> &icons);
	/**
	 * Execute a scintilla command, e.g. SCI_GETLINE.
	 */
	LRESULT		Call(UINT message, WPARAM wParam = 0, LPARAM lParam = 0);
	/**
	 * The specified text is written to the scintilla control.
	 */
	void		SetText(const CString& sText);
	/**
	 * The specified text is inserted at the cursor position. If a text is
	 * selected, that text is replaced.
	 * \param sText test to insert
	 * \param bNewLine if set to true, a newline is appended.
	 */
	void		InsertText(const CString& sText, bool bNewLine = false);
	/**
	 * Retrieves the text in the scintilla control.
	 */
	CString		GetText();
	/**
	 * Sets the font for the control.
	 */
	void		SetFont(CString sFontName, int iFontSizeInPoints);
	/**
	 * Adds a list of words for use in auto completion.
	 */
	void		SetAutoCompletionList(std::map<CString, int>&& list, wchar_t separator = ';', wchar_t typeSeparator = '?');
	/**
	 * Returns the word located under the cursor.
	 */
	CString		GetWordUnderCursor(bool bSelectWord = false, bool allchars = false);

	void		RegisterContextMenuHandler(CSciEditContextMenuInterface * object) {m_arContextHandlers.Add(object);}
	void		ClearContextMenuHandlers() { m_arContextHandlers.RemoveAll(); }

	CStringA	StringForControl(const CString& text);
	CString		StringFromControl(const CStringA& text);
	int			LoadFromFile(const CString& filename, UINT errorMsgId);
	void		RestyleBugIDs();
	BOOL		EnableWindow(BOOL bEnable = TRUE);
	void		SetReadOnly(bool bReadOnly);
	void		ClearUndoBuffer();
	bool		m_bNoAutomaticStyling = false;

private:
	bool IsUTF8(LPVOID pBuffer, size_t cb);
	CAutoLibrary	m_hModule;
	LRESULT		m_DirectFunction = 0;
	LRESULT		m_DirectPointer = 0;
	std::unique_ptr<Hunspell>	pChecker;
	UINT		m_spellcodepage = 0;
	std::map<CString, int> m_autolist;
	wchar_t		m_separator = '\0';
	wchar_t		m_typeSeparator = '\x01';
	CStringA	m_sCommand;
	CStringA	m_sBugID;
	CString		m_sUrl;
	CArray<CSciEditContextMenuInterface *, CSciEditContextMenuInterface *> m_arContextHandlers;
	CPersonalDictionary m_personalDict;
	bool		m_bUDiffmode = false;
	bool		m_bDoStyle = false;
	int			m_nAutoCompleteMinChars = 3;
	LruCache<std::wstring, BOOL> m_SpellingCache{2000};
	bool		m_blockModifiedHandler = false;
	bool		m_bReadOnly = false;
	int			m_themeCallbackId = 0;

protected:
	BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult) override;
	BOOL PreTranslateMessage(MSG* pMsg) override;
	ULONG GetGestureStatus(CPoint ptTouch) override;
	void		CheckSpelling(Sci_Position startpos, Sci_Position endpos);
	void		SuggestSpellingAlternatives();
	void		DoAutoCompletion(Sci_Position nMinPrefixLength);
	BOOL		LoadDictionaries(LONG lLanguageID);
	ISpellCheckerFactoryPtr m_spellCheckerFactory;
	ISpellCheckerPtr m_SpellChecker;
	BOOL		MarkEnteredBugID(Sci_Position startstylepos, Sci_Position endstylepos);
	bool		StyleEnteredText(Sci_Position startstylepos, Sci_Position endstylepos);
	void		StyleURLs(Sci_Position startstylepos, Sci_Position endstylepos);
	bool		WrapLines(Sci_Position startpos, Sci_Position endpos);
	bool		FindStyleChars(const char* line, char styler, Sci_Position& start, Sci_Position& end);
	void		SetColors(bool recolorize);
	static void	AdvanceUTF8(const char* str, int& pos);
	BOOL		IsMisspelled(const CString& sWord);
	BOOL		CheckWordSpelling(const CString& sWord);
	int			GetStyleAt(Sci_Position pos) { return static_cast<int>(Call(SCI_GETSTYLEAT, pos)) & 0x1f; }
	std::string GetWordForSpellChecker(const CString& sWord);
	CString		GetWordFromSpellChecker(const std::string& sWordA);

	static void SetWindowStylesForAutocompletionPopup();
	static BOOL CALLBACK AdjustThemeProc(HWND hwnd, LPARAM lParam);

	virtual afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnSysColorChange();
	DECLARE_MESSAGE_MAP()
};
