﻿// TortoiseIDiff - an image diff viewer in TortoiseSVN

// Copyright (C) 2015-2016, 2023 - TortoiseGit
// Copyright (C) 2006-2007, 2009, 2011-2013, 2015-2016, 2020 - TortoiseSVN

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
#include "BaseWindow.h"
#include "PicWindow.h"
#include "TortoiseIDiff.h"

#include <map>
#include <CommCtrl.h>

#define SPLITTER_BORDER 2

#define WINDOW_MINHEIGHT 200
#define WINDOW_MINWIDTH 200

enum class FileType
{
    Mine        = 1,
    Theirs      = 2,
    Base        = 3,
};

/**
 * \ingroup TortoiseIDiff
 * The main window of TortoiseIDiff.
 * Hosts the two image views, the menu, toolbar, slider, ...
 */
class CMainWindow : public CWindow
{
public:
    CMainWindow(HINSTANCE hInstance, const WNDCLASSEX* wcx = nullptr) : CWindow(hInstance, wcx)
        , transparentColor(::GetSysColor(COLOR_WINDOW))
    {
        SetWindowTitle(static_cast<LPCWSTR>(ResString(hResource, IDS_APP_TITLE)));
    };

    /**
     * Registers the window class and creates the window.
     */
    bool RegisterAndCreateWindow();

    /**
     * Sets the image path and title for the left image view.
     */
    void SetLeft(const std::wstring& leftpath, const std::wstring& lefttitle)
    {
        leftpicpath = leftpath;
        leftpictitle = lefttitle;
    }
    /**
     * Sets the image path and the title for the right image view.
     */
    void SetRight(const std::wstring& rightpath, const std::wstring& righttitle)
    {
        rightpicpath = rightpath;
        rightpictitle = righttitle;
    }

    /**
     * Sets the image path and title for selection mode. In selection mode, the images
     * are shown side-by-side for the user to chose one of them. The chosen image is
     * saved at the path for \b FileTypeResult (if that path has been set) and the
     * process return value is the chosen FileType.
     */
    void SetSelectionImage(FileType ft, const std::wstring& path, const std::wstring& title);
    void SetSelectionResult(const std::wstring& path) { selectionResult = path; }

protected:
    /// the message handler for this window
    LRESULT CALLBACK                    WinMsgHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) override;
    /// Handles all the WM_COMMAND window messages (e.g. menu commands)
    LRESULT                             DoCommand(int id, LPARAM lParam);

    /// Positions the child windows. Call this after the window sizes/positions have changed.
    void                                PositionChildren(RECT* clientrect = nullptr);
    /// Shows the "Open images" dialog where the user can select the images to diff
    bool                                OpenDialog();
    static BOOL CALLBACK                OpenDlgProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);
    static bool                         AskForFile(HWND owner, wchar_t* path);

    // splitter methods
    void                                DrawXorBar(HDC hdc, int x1, int y1, int width, int height);
    LRESULT                             Splitter_OnLButtonDown(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
    LRESULT                             Splitter_OnLButtonUp(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
    LRESULT                             Splitter_OnMouseMove(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam);
    void                                Splitter_CaptureChanged();

    void                                SetTheme(bool bDark);
    int                                 m_themeCallbackId = 0;
    // toolbar
    bool                                CreateToolbar();
    HWND                                hwndTB = nullptr;
    HIMAGELIST                          hToolbarImgList = nullptr;

    // command line params
    static std::wstring                 leftpicpath;
    static std::wstring                 leftpictitle;

    static std::wstring                 rightpicpath;
    static std::wstring                 rightpictitle;

    // image data
    CPicWindow                          picWindow1 = nullptr;
    CPicWindow                          picWindow2 = nullptr;
    CPicWindow                          picWindow3 = nullptr;
    bool                                bShowInfo = false;
    COLORREF                            transparentColor;

    // splitter data
    int                                 oldx = -4;
    int                                 oldy = -4;
    bool                                bMoved = false;
    bool                                bDragMode = false;
    bool                                bDrag2 = false;
    int                                 nSplitterPos = 100;
    int                                 nSplitterPos2 = 200;

    // one/two pane view
    bool                                bSelectionMode = false;
    bool                                bOverlap = false;
    bool                                bVertical = false;
    bool                                bLinkedPositions = true;
    bool                                bFitWidths = false;
    bool                                bFitHeights = false;
    CPicWindow::BlendType               m_BlendType = CPicWindow::BlendType::Alpha;

    // selection mode data
    std::map<FileType, std::wstring>    selectionPaths;
    std::map<FileType, std::wstring>    selectionTitles;
    std::wstring                        selectionResult;

public:
    HWND			resolveMsgWnd = nullptr;
    WPARAM			resolveMsgWParam = 0;
    LPARAM			resolveMsgLParam = 0;
};

