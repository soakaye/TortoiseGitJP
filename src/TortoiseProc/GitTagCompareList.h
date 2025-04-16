﻿// TortoiseGit - a Windows shell extension for easy version control

// Copyright (C) 2015-2017, 2023 - TortoiseGit

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
// GitLogList.cpp : implementation file
//
#pragma once
#include "HintCtrl.h"
#include "GitHash.h"

class CGitTagCompareList : public CHintCtrl<CListCtrl>
{
	DECLARE_DYNAMIC(CGitTagCompareList);

	struct TagEntry
	{
		CString		name;
		CString		diffstate;
		CGitHash	myHash;
		CString		myMessage;
		CGitHash	theirHash;
		CString		theirMessage;
	};

public:
	CGitTagCompareList();

	virtual ~CGitTagCompareList()
	{
	}

	void Init();

	int Fill(const CString& remote, CString& err);

protected:
	afx_msg void OnHdnItemclick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnContextMenu(CWnd *pWnd, CPoint point);
	void OnContextMenuList(CWnd *pWnd, CPoint point);
	void OnContextMenuHeader(CWnd *pWnd, CPoint point);
	ULONG GetGestureStatus(CPoint ptTouch) override;

	DECLARE_MESSAGE_MAP()

	void AddEntry(git_repository* repo, const CString& tag, const CGitHash* myHash, const CGitHash* theirHash);
	void Show();

private:
	std::vector<TagEntry>	m_TagList;
	BOOL					m_bHideEqual;
	static BOOL 			m_bSortLogical;

	CString	m_remote;

	int colTag = 0;
	int colDiff = 0;
	int colMyHash = 0;
	int colMyMessage = 0;
	int colTheirHash = 0;
	int colTheirMessage = 0;

	bool	m_bAscending = false;		///< sort direction
	int		m_nSortedColumn = -1;	///< which column to sort
};
