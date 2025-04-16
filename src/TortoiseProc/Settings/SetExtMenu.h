﻿// TortoiseGit - a Windows shell extension for easy version control

// Copyright (C) 2010, 2012, 2019, 2023, 2025 - TortoiseGit
// Copyright (C) 2008 - TortoiseSVN

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
#include "resource.h"

class CSetExtMenu : public ISettingsPropPage
{
	DECLARE_DYNAMIC(CSetExtMenu)

public:
	CSetExtMenu();
	virtual ~CSetExtMenu();

	UINT GetIconID() override { return IDI_MISC; }

// Dialog Data
	enum { IDD = IDD_SETTINGSEXTMENU };

protected:
	void DoDataExchange(CDataExchange* pDX) override;    // DDX/DDV support
	BOOL OnApply() override;
	afx_msg void OnLvnItemchangedMenulist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedSelectall();
	afx_msg void OnBnClickedRestoreDefaults();
	afx_msg void OnChange();

	DECLARE_MESSAGE_MAP()
public:
	BOOL OnInitDialog() override;

private:
	CRegStdDWORD			m_regExtmenu;
	CRegStdDWORD			m_regExtmenuhigh;

	ULARGE_INTEGER		m_extmenu;

	CImageList			m_imgList;
	CListCtrl			m_cMenuList;
	bool				m_bBlock = false;
};
