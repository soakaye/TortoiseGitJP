﻿// TortoiseGit - a Windows shell extension for easy version control

// Copyright (C) 2008-2023, 2025 - TortoiseGit
// Copyright (C) 2003-2008 - TortoiseSVN

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

#pragma once
#include "TGitPath.h"
#include "Colors.h"
#include "IconMenu.h"
#include "ProgressDlg.h"

/**
 * \ingroup TortoiseProc
 * Options which can be used to configure the way the dialog box works
 */
enum ProgressOptions
{
	ProgOptNone = 0,
	/// Don't actually do the merge - just practice it
	ProgOptDryRun = 0x04,
};

// CGitProgressList
//struct git_indexer_progress;
#define WM_SHOWCONFLICTRESOLVER (WM_APP + 100)
#define WM_PROG_CMD_FINISH		(WM_APP + 200)
#define WM_PROG_CMD_START		(WM_APP + 201)

class ProgressCommand;
class CGitProgressList : public CListCtrl
{
	DECLARE_DYNAMIC(CGitProgressList)

public:
	CGitProgressList();
	virtual ~CGitProgressList();

	void SetCommand(ProgressCommand* command) { ATLASSERT(command); m_Command = command; }
	void SetOptions(DWORD opts) {m_options = opts;}

	/**
	 * If the number of items for which the operation is done on is known
	 * beforehand, that number can be set here. It is then used to show a more
	 * accurate progress bar during the operation.
	 */
	void SetItemCountTotal(long count) { if(count) m_itemCountTotal = count; }
	void SetItemProgress(long count) { m_itemCount = count;} // do not use SetItemCount here as this overrides the ListBox method
	bool SetBackgroundImage(UINT nID);
	bool DidErrorsOccur() const { return m_bErrorsOccurred; }
	bool m_bErrorsOccurred = false;
	CWnd* m_pProgressLabelCtrl = nullptr;
	CWnd* m_pInfoCtrl = nullptr;
	CAnimateCtrl* m_pAnimate = nullptr;
	CProgressCtrl* m_pProgControl = nullptr;
	ProgressCommand* m_Command = nullptr;
	void			Cancel();
	volatile BOOL IsCancelled() const { return m_bCancelled; }
	volatile LONG IsRunning() const { return m_bThreadRunning; }
	CWinThread* m_pThread = nullptr;
	CWnd* m_pPostWnd = nullptr;
	bool m_bSetTitle = false;

public:
	using ContextMenuActionList = std::vector<std::function<void()>>;
	struct Payload
	{
		CGitProgressList* list = nullptr;
		git_repository* repo = nullptr;
	};

	class NotificationData
	{
	public:
		NotificationData()
		: color(::GetSysColor(COLOR_WINDOWTEXT))
		{};

		NotificationData(const CTGitPath &path, UINT actionTextId)
		: NotificationData()
		{
			this->path = path;
			sActionColumnText.LoadString(actionTextId);
		};

		virtual ~NotificationData() {};

		virtual void SetColorCode(CColors& /*colors*/) {};
		virtual void GetContextMenu(CIconMenu& /*popup*/, ContextMenuActionList& /*actions*/) {};
		virtual void HandleDblClick() const {}

		// The text we put into the first column (the Git action for normal items, just text for aux items)
		CString					sActionColumnText;
		CTGitPath				path;
		COLORREF				color;
		bool					colorIsDirect = false;
		bool					bAuxItem = false; // Set if this item is not a true 'Git action'
		CString					sPathColumnText;
	};

	class WC_File_NotificationData : public NotificationData
	{
	public:
		enum class Git_WC_Notify_Action
		{
			Skip,
			Add,
			Resolved,
			Revert,
			Checkout,
			LFS_Lock,
			LFS_Unlock,
		};

		WC_File_NotificationData(const CTGitPath& path, Git_WC_Notify_Action action);
		void SetColorCode(CColors& colors) override;

		Git_WC_Notify_Action action;

		void GetContextMenu(CIconMenu& popup, ContextMenuActionList& actions) override;
		void HandleDblClick() const override;
	};

	void AddNotify(NotificationData* data, CColors::Colors color = CColors::COLOR_END);
	int UpdateProgress(const git_indexer_progress* stat);

	void SetProgressLabelText(const CString& str);

protected:
	DECLARE_MESSAGE_MAP()

public:
	void SetWindowTitle(UINT id, const CString& urlorpath, CString& dialogname);

protected:
	afx_msg void	OnNMCustomdrawSvnprogress(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void	OnLvnGetdispinfoSvnprogress(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void	OnNMDblclkSvnprogress(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void	OnHdnItemclickSvnprogress(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void	OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void	OnTimer(UINT_PTR nIDEvent);
	afx_msg void	OnSize(UINT nType, int cx, int cy);
	LRESULT			OnShowConflictResolver(WPARAM, LPARAM);
	afx_msg void	OnLvnBegindragSvnprogress(NMHDR *pNMHDR, LRESULT *pResult);

	void			Sort();
	static bool		SortCompare(const NotificationData* pElem1, const NotificationData* pElem2);

	static BOOL		m_bAscending;
	static int		m_nSortedColumn;

private:
	static UINT ProgressThreadEntry(LPVOID pVoid);
	UINT		ProgressThread();

public:
	void		ReportGitError();
	void		ReportUserCanceled();
	void		ReportError(const CString& sError);
	void		ReportWarning(const CString& sWarning);
	void		ReportNotification(const CString& sNotification);
	void		ReportCmd(const CString& sCmd);
	void		ReportString(CString sMessage, const CString& sMsgKind, bool colorIsDirect, COLORREF color = CTheme::Instance().IsDarkTheme() ? CTheme::darkTextColor : GetSysColor(COLOR_WINDOWTEXT));

private:
	void		AddItemToList();
	CString		BuildInfoString();

	/**
	 * Resizes the columns of the progress list so that the headings are visible.
	 */
	void		ResizeColumns();

	/// Predicate function to tell us if a notification data item is auxiliary or not
	static bool NotificationDataIsAux(const NotificationData* pData);

private:
	using NotificationDataVect = std::vector<NotificationData*>;

	NotificationDataVect	m_arData;

	volatile LONG			m_bThreadRunning = 0;

	int						m_options = ProgOptNone;	// Use values from the ProgressOptions enum


	wchar_t					m_columnbuf[MAX_PATH];

public:
	volatile BOOL			m_bCancelled = FALSE;

private:
	int						iFirstResized = 0;
	BOOL					bSecondResized = FALSE;
	int						nEnsureVisibleCount = 0;

	CString					m_sTotalBytesTransferred;
	size_t					m_TotalBytesTransferred = 0;

	CColors					m_Colors;

	bool					m_bFinishedItemAdded = false;
	bool					m_bLastVisible = false;

public:
	int						m_itemCount = -1;
	int						m_itemCountTotal = -1;

public:
	CComPtr<ITaskbarList3>	m_pTaskbarList;
	void Init();
	void ShowProgressBar();

protected:
	afx_msg void OnClose();
	BOOL PreTranslateMessage(MSG* pMsg) override;
	afx_msg void OnSysColorChange();
	ULONG GetGestureStatus(CPoint ptTouch) override;
};

class ProgressCommand
{
protected:
	CTGitPathList		m_targetPathList;

public:
	PostCmdCallback	m_PostCmdCallback = nullptr;

	ProgressCommand() = default;

	void SetPathList(const CTGitPathList& pathList) { m_targetPathList = pathList; }
	virtual bool Run(CGitProgressList* list, CString& sWindowTitle, int& m_itemCountTotal, int& m_itemCount) = 0;
	virtual bool ShowInfo(CString& /*info*/) { return false; }
	virtual ~ProgressCommand() {}
};

