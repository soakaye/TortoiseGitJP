﻿// TortoiseGit - a Windows shell extension for easy version control

// Copyright (C) 2014, 2020-2023 - TortoiseGit

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
class CUpdateDownloader
{
public:
	CUpdateDownloader(HWND hwnd, const CString& sVersion, bool force = false, UINT msg = 0, CEvent* m_eventStop = nullptr);
	~CUpdateDownloader();

	DWORD	DownloadFile(const CString &url, const CString& dest, bool showProgress) const;

	struct DOWNLOADSTATUS
	{
		ULONG ulProgress;
		ULONG ulProgressMax;
	};

	CString m_sWindowsPlatform;
	CString m_sWindowsVersion;
	CString m_sWindowsServicePack;

private:
	static bool GetTrueWindowsVersion(OSVERSIONINFOEX& osVersionInfo);
	static void BruteforceGetWindowsVersionNumber(OSVERSIONINFOEX& osVersionInfo);

	HINTERNET hOpenHandle = nullptr;

	HWND	m_hWnd = nullptr;
	bool	m_bForce = false;
	UINT	m_uiMsg = 0;
	CEvent* m_eventStop = nullptr;
};
