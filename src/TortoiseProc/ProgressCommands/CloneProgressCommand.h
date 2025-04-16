﻿// TortoiseGit - a Windows shell extension for easy version control

// Copyright (C) 2013-2014, 2019-2020, 2023 - TortoiseGit

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

#include "GitProgressList.h"
#include "FetchProgressCommand.h"

using Git_WC_Notify_Action = CGitProgressList::WC_File_NotificationData::Git_WC_Notify_Action;

class CloneProgressCommand : public FetchProgressCommand
{
protected:
	bool m_bBare = false;
	bool m_bNoCheckout = false;

	static void CheckoutCallback(const char *path, size_t cur, size_t tot, void *payload)
	{
		CTGitPath tpath = CUnicodeUtils::GetUnicode(path);
		auto list = static_cast<CGitProgressList*>(payload);
		list->m_itemCountTotal = static_cast<int>(tot);
		list->m_itemCount = static_cast<int>(cur);
		list->AddNotify(new CGitProgressList::WC_File_NotificationData(tpath, Git_WC_Notify_Action::Checkout));
	}

public:
	CloneProgressCommand() = default;

	void SetIsBare(bool b) { m_bBare = b; }
	void SetNoCheckout(bool b){ m_bNoCheckout = b; }
	bool Run(CGitProgressList* list, CString& sWindowTitle, int& m_itemCountTotal, int& m_itemCount) override;
};
