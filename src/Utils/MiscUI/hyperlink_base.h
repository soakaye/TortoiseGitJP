﻿/*
 * Module ID: hyperlink.h
 * Title    : CHyperLink Declaration.
 *
 * Author   : Olivier Langlois <olanglois@sympatico.ca>
 * Date     : November 15, 2005
 *
 * To read the article describing this class, visit
 * http://www3.sympatico.ca/olanglois/hyperlinkdemo.htm
 *
 * Note: Strongly inspired by Neal Stublen code
 *       Minor ideas come from Chris Maunder and Paul DiLascia code
 *
 * Revision :
 *
 * 001        26-Nov-2005 - Olivier Langlois
 *            - Added changes to make CHyperLink compatible with UNICODE
 *            - Use dynamic memory allocation for the URL string
 */

#ifndef   _HYPERLINK_H_
#define   _HYPERLINK_H_

#include <Windows.h>

class CHyperLink
{
public:
	CHyperLink();
	virtual ~CHyperLink();

	BOOL ConvertStaticToHyperlink(HWND hwndCtl, LPCWSTR strURL);
	BOOL ConvertStaticToHyperlink(HWND hwndParent, UINT uiCtlId, LPCWSTR strURL);

	BOOL setURL(LPCWSTR strURL);
	LPCWSTR getURL() const { return m_strURL; }

protected:
	/*
	 * Override if you want to perform some action when the link has the focus
	 * or when the cursor is over the link such as displaying the URL somewhere.
	 */
	virtual void OnSelect()   {}
	virtual void OnDeselect() {}

	CString m_strURL; // hyperlink URL

private:
	static COLORREF g_crLinkColor, g_crVisitedColor;// Hyperlink colors
	static HCURSOR  g_hLinkCursor;                  // Cursor for hyperlink
	static HFONT    g_UnderlineFont;                // Font for underline display
	static int      g_counter;						// Global resources user counter
	BOOL     m_bOverControl = FALSE;                // cursor over control?
	BOOL     m_bVisited = FALSE;                    // Has it been visited?
	HFONT    m_StdFont = nullptr;                   // Standard font
	WNDPROC  m_pfnOrigCtlProc = nullptr;

	void createUnderlineFont();
	static void createLinkCursor();
	void createGlobalResources()
	{
		createUnderlineFont();
		createLinkCursor();
	}
	static void destroyGlobalResources()
	{
		/*
		 * No need to call DestroyCursor() for cursors acquired through
		 * LoadCursor().
		 */
		g_hLinkCursor   = nullptr;
		DeleteObject(g_UnderlineFont);
		g_UnderlineFont = nullptr;
	}

	void Navigate();

	static void DrawFocusRect(HWND hwnd);
	static LRESULT CALLBACK _HyperlinkParentProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	static LRESULT CALLBACK _HyperlinkProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
};

#endif /* _HYPERLINK_H_ */
