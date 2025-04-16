﻿// TortoiseGit - a Windows shell extension for easy version control

// Copyright (C) 2012-2019, 2021-2024 - TortoiseGit
// Copyright (C) 2003-2011, 2017 - TortoiseSVN

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
#include "registry.h"
#include "Globals.h"

#define ADMINDIRTIMEOUT 10000
#define DRIVETYPETIMEOUT 300000		// 5 min

#define DEFAULTWIN11MENUTOPENTRIES MENUSYNC|MENUCOMMIT|MENUSWITCH|MENUREVERT|MENUCONFLICTEDITOR|MENUDIFF|MENUPULL|MENUPUSH|MENUREPOBROWSE|MENULOG|MENUBLAME|MENUCLONE|MENUMERGE|MENUSTASHSAVE|MENUSETTINGS
#define DEFAULTMENUTOPENTRIES	MENUSYNC|MENUCREATEREPOS|MENUCLONE|MENUCOMMIT
#define DEFAULTMENUEXTENTRIES	MENUSVNIGNORE|MENUSTASHAPPLY|MENUSUBSYNC

using Locker = CComCritSecLock<CComCriticalSection>;

enum class Tristate
{
	/** state known to be false (the constant does not evaulate to false) */
	False = 2,
	/** state known to be true */
	True,
	/** state could be true or false */
	Unknown
};

/**
 * \ingroup TortoiseShell
 * Helper class which caches access to the registry. Also provides helper methods
 * for checks against the settings stored in the registry.
 */
class ShellCache
{
public:
	enum CacheType
	{
		none,
		exe,
		dll,
		dllFull,// same as dll except it uses commandline git tool with all status modes supported
	};

	ShellCache();
	~ShellCache();

	bool RefreshIfNeeded();

	CacheType GetCacheType();
	DWORD BlockStatus();
	unsigned __int64 GetMenuLayout();
	unsigned __int64 GetMenuLayout11();
	unsigned __int64 GetMenuExt();
	unsigned __int64 GetMenuMask();

	bool IsProcessElevated();
	BOOL IsOnlyNonElevated();

	BOOL IsRecursive();
	BOOL IsFolderOverlay();
	BOOL IsSimpleContext();
	BOOL HasShellMenuAccelerators();
	BOOL IsUnversionedAsModified();
	BOOL IsRecurseSubmodules();
	BOOL ShowUnversionedOverlay();
	BOOL ShowIgnoredOverlay();
	BOOL ShowExcludedAsNormal();
	BOOL HideMenusForUnversionedItems();

	BOOL IsRemote();
	BOOL IsFixed();
	BOOL IsCDRom();
	BOOL IsRemovable();
	BOOL IsRAM();
	BOOL IsUnknown();

	BOOL IsContextPathAllowed(const std::wstring& path);
	BOOL IsPathAllowed(LPCWSTR path);
	DWORD GetLangID();
	BOOL HasGITAdminDir(LPCWSTR path, BOOL bIsDir, CString* ProjectTopDir = nullptr);

private:
	void ExcludeContextValid();

	class CPathFilter
	{
	public:
		/// node in the lookup tree
		struct SEntry
		{
			std::wstring path;

			/// default (path spec did not end a '?').
			/// if this is not set, the default for all
			/// sub-paths is !included.
			/// This is a temporary setting an be invalid
			/// after @ref PostProcessData
			bool recursive = false;

			/// this is an "include" specification
			Tristate included = Tristate::Unknown;

			/// if @ref recursive is not set, this is
			/// the parent path status being passed down
			/// combined with the information of other
			/// entries for the same @ref path.
			Tristate subPathIncluded = Tristate::Unknown;

			/// do entries for sub-paths exist?
			bool hasSubFolderEntries = false;

			/// STL support
			/// For efficient folding, it is imperative that
			/// "recursive" entries are first
			bool operator<(const SEntry& rhs) const
			{
				int diff = _wcsicmp(path.c_str(), rhs.path.c_str());
				return (diff < 0) || ((diff == 0) && recursive && !rhs.recursive);
			}

			friend bool operator<(const SEntry& rhs, const std::pair<LPCWSTR, size_t>& lhs);
			friend bool operator<(const std::pair<LPCWSTR, size_t>& lhs, const SEntry& rhs);
		};

	private:
		/// lookup by path (all entries sorted by path)
		using TData = std::vector<SEntry>;
		TData data;

		/// registry keys plus cached last content
		CRegStdString excludelist;
		std::wstring excludeliststr;

		CRegStdString includelist;
		std::wstring includeliststr;

		/// construct \ref data content
		void AddEntry(const std::wstring& s, bool include);
		void AddEntries(const std::wstring& s, bool include);

		/// for all paths, have at least one entry in data
		void PostProcessData();

		/// lookup. default result is "unknown".
		/// We must look for *every* parent path because of situations like:
		/// excluded: C:, C:\some\deep\path
		/// include: C:\some
		/// lookup for C:\some\deeper\path
		Tristate IsPathAllowed(LPCWSTR path, TData::const_iterator begin, TData::const_iterator end) const;

	public:
		/// construction
		CPathFilter();

		/// notify of (potential) registry settings
		void Refresh();

		/// data access
		Tristate IsPathAllowed(LPCWSTR path) const;
	};

	friend bool operator< (const CPathFilter::SEntry& rhs, const std::pair<LPCWSTR, size_t>& lhs);
	friend bool operator< (const std::pair<LPCWSTR, size_t>& lhs, const CPathFilter::SEntry& rhs);

	struct AdminDir_s
	{
		BOOL bHasAdminDir = false;
		std::wstring sProjectRoot;
		ULONGLONG timeout = ADMINDIRTIMEOUT;
	};

public:
	CRegStdDWORD cachetype;
	CRegStdDWORD blockstatus;
	CRegStdDWORD langid;
	CRegStdDWORD onlynonelevated;
	CRegStdDWORD showrecursive;
	CRegStdDWORD folderoverlay;
	CRegStdDWORD driveremote;
	CRegStdDWORD drivefixed;
	CRegStdDWORD drivecdrom;
	CRegStdDWORD driveremove;
	CRegStdDWORD drivefloppy;
	CRegStdDWORD driveram;
	CRegStdDWORD driveunknown;
	CRegStdQWORD menuLayout11;
	CRegStdDWORD menulayoutlow; /* Fist level mask */
	CRegStdDWORD menulayouthigh;
	CRegStdDWORD shellmenuaccelerators;
	CRegStdDWORD menuextlow;	   /* ext menu mask */
	CRegStdDWORD menuexthigh;
	CRegStdDWORD simplecontext;
	CRegStdDWORD menumasklow_lm;
	CRegStdDWORD menumaskhigh_lm;
	CRegStdDWORD menumasklow_cu;
	CRegStdDWORD menumaskhigh_cu;
	CRegStdDWORD unversionedasmodified;
	CRegStdDWORD recursesubmodules;
	CRegStdDWORD showunversionedoverlay;
	CRegStdDWORD showignoredoverlay;
	CRegStdDWORD excludedasnormal;
	CRegStdDWORD hidemenusforunversioneditems;

	CPathFilter pathFilter;

	ULONGLONG drivetypeticker = 0;
	ULONGLONG menumaskticker = 0;
	UINT  drivetypecache[27];
	wchar_t drivetypepathcache[MAX_PATH]{}; // MAX_PATH ok.
	std::map<std::wstring, AdminDir_s> admindircache;
	CRegStdString nocontextpaths;
	std::wstring excludecontextstr;
	std::vector<std::wstring> excontextvector;
	CComAutoCriticalSection m_critSec;
	HANDLE m_registryChangeEvent = nullptr;
	HKEY m_hNotifyRegKey = nullptr;
	bool isElevated = false;
};

inline bool operator<(const ShellCache::CPathFilter::SEntry& lhs, const std::pair<LPCWSTR, size_t>& rhs)
{
	return _wcsnicmp(lhs.path.c_str(), rhs.first, rhs.second) < 0;
}

inline bool operator<(const std::pair<LPCWSTR, size_t>& lhs, const ShellCache::CPathFilter::SEntry& rhs)
{
	return _wcsnicmp(lhs.first, rhs.path.c_str(), lhs.second) < 0;
}
