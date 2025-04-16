﻿// TortoiseGit - a Windows shell extension for easy version control

// Copyright (C) 2011-2013 - Sven Strickroth <email@cs-ware.de>
// Copyright (C) 2013-2017, 2020, 2022-2023 - TortoiseGit

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
#include "gittype.h"
#include "TGitPath.h"

class CMassiveGitTaskBase
{
public:
	CMassiveGitTaskBase(CString params, BOOL isPath = TRUE, bool ignoreErrors = false);
	~CMassiveGitTaskBase();

	void					AddFile(const CString& filename);
	void					AddFile(const CTGitPath& filename);
	bool					Execute(volatile BOOL& cancel);
	int						GetListCount() const;
	bool					IsListEmpty() const;
	static void				ConvertToCmdList(CString params, const STRING_VECTOR& pathList, STRING_VECTOR& cmdList);
protected:
	void					SetPaths(const CTGitPathList* pathList);
	bool					ExecuteCommands(volatile BOOL& cancel);
	virtual void			ReportError(const CString &out, int exitCode);
	virtual void			ReportProgress(const CTGitPath& /*path*/, int /*index*/) { }
	virtual void			ReportUserCanceled() { }
	CString					GetParams() const { return m_sParams; }
private:
	CString					GetListItem(int index) const;
	bool					m_bUnused = true;
	BOOL					m_bIsPath = TRUE;
	bool					m_bIgnoreErrors = false;
	CString					m_sParams;
	CTGitPathList			m_pathList;
	STRING_VECTOR			m_itemList;
};
