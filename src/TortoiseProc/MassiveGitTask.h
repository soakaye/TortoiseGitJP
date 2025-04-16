﻿// TortoiseGit - a Windows shell extension for easy version control

// Copyright (C) 2011-2013 - Sven Strickroth <email@cs-ware.de>
// Copyright (C) 2013-2014, 2016-2017, 2023 - TortoiseGit

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
#include "MassiveGitTaskBase.h"
#include "GitProgressList.h"

class CMassiveGitTask : public CMassiveGitTaskBase
{
public:
	CMassiveGitTask(CString params, BOOL isPath = TRUE, bool ignoreErrors = false);
	~CMassiveGitTask();

	void SetProgressList(CGitProgressList* notifyCallbackInstance) { m_NotifyCallbackInstance = notifyCallbackInstance; }
	void SetProgressCallback(std::function<void(const CTGitPath& path, int index)> progressCallback) { m_progressCallback = progressCallback; }
	bool					ExecuteWithNotify(CTGitPathList* pathList, volatile BOOL& cancel, CGitProgressList::WC_File_NotificationData::Git_WC_Notify_Action action, CGitProgressList* instance);

private:
	void					ReportError(const CString& out, int exitCode) override;
	void					ReportProgress(const CTGitPath& path, int index) override;
	void					ReportUserCanceled() override;
	CGitProgressList*		m_NotifyCallbackInstance = nullptr;
	CGitProgressList::WC_File_NotificationData::Git_WC_Notify_Action m_NotifyCallbackAction;
	std::function<void(const CTGitPath& path, int index)> m_progressCallback = nullptr;
};
