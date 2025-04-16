﻿// TortoiseGit - a Windows shell extension for easy version control

// Copyright (C) 2008, 2011-2013, 2015-2018, 2021-2023, 2025 - TortoiseGit
// Copyright (C) 2003-2011, 2015 - TortoiseSVN

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
#include "StandAloneDlg.h"
#include "MyGraph.h"
#include "TGitPath.h"
#include "GitRevLoglist.h"

/**
 * \ingroup TortoiseProc
 * Helper class for drawing and then saving the drawing to a meta file (wmf)
 */
class CMyMetaFileDC : public CMetaFileDC
{
public:
	HGDIOBJ SelectObject(HGDIOBJ hObject)
	{
		return (hObject != nullptr) ? ::SelectObject(m_hDC, hObject) : nullptr;
	}
};

/**
 * \ingroup TortoiseProc
 * Helper dialog showing statistics gathered from the log messages shown in the
 * log dialog.
 *
 * The function GatherData() collects statistical information and stores it
 * in the corresponding member variables. You can access the data as shown in
 * the following examples:
 * @code
 *    commits = m_commitsPerWeekAndAuthor[week_nr][author_name];
 *    filechanges = m_filechangesPerWeekAndAuthor[week_nr][author_name];
 *    commits = m_commitsPerAuthor[author_name];
 * @endcode
 */
class CStatGraphDlg : public CResizableStandAloneDialog//CResizableStandAloneDialog
{
	DECLARE_DYNAMIC(CStatGraphDlg)

public:
	CStatGraphDlg(CWnd* pParent = nullptr);
	virtual ~CStatGraphDlg();

	enum { IDD = IDD_STATGRAPH };

	std::vector<GitRevLoglist*> m_ShowList;

	// Data	passed from	the	caller of the dialog.
	CDWordArray		m_parDates;
	CDWordArray		m_parFileChanges;
	CDWordArray		m_parFileChanges2;
	CDWordArray		m_lineInc;
	CDWordArray		m_lineInc2;
	CDWordArray		m_lineDec;
	CDWordArray		m_lineDec2;
	CDWordArray		m_lineNew;
	CDWordArray		m_lineNew2;
	CDWordArray		m_lineDel;
	CDWordArray		m_lineDel2;

	CStringArray	m_parAuthors;
	CTGitPath		m_path;

protected:

	// ** Constants	**
	static	const long int		m_SecondsInWeek	= 604800; // ... a week	has	604800 seconds
	static	const long int		m_SecondsInDay = 86400;	 //	...	a day has 86400.0 seconds
	static	const int			m_CoeffAuthorShip =	2;

	// ** Data types **

	/// The types of units used in the various graphs.
	enum UnitType
	{
		Days,
		Weeks,
		Months,
		Quarters,
		Years
	};

	// Available next metrics
	enum Metrics
	{
		TextStatStart,
			AllStat,
		TextStatEnd,
		GraphicStatStart,
			PercentageOfAuthorship,
			CommitsByAuthor,
			CommitsByDate,
			LinesWByDate,
			LinesWOByDate,
		GraphicStatEnd,
	};

	//TODO: try substitute map to hash_map
	/// The mapping type used to store data per interval/week and author.
	using IntervalDataMap = std::map<int, std::map<std::wstring, LONG>>;

	//TODO: try substitute few Maps to one map, that store needs informations about Authors
	/// The mapping type used to store data per author.
	using AuthorDataMap = std::map<std::wstring, LONG>;
	/// The mapping type used to store data per Percentage Of Authorship
	using AuthorshipDataMap = std::map<std::wstring, double>;

	// *** Re-implemented member functions from CDialog
	void OnOK() override;
	void OnCancel() override;

	void DoDataExchange(CDataExchange* pDX) override;
	BOOL OnInitDialog() override;
	void ShowLabels(BOOL bShow);
	afx_msg void OnCbnSelchangeGraphcombo();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnNeedText(NMHDR *pnmh, LRESULT *pResult);
	afx_msg void OnBnClickedGraphbarbutton();
	afx_msg void OnBnClickedGraphbarstackedbutton();
	afx_msg void OnBnClickedGraphlinebutton();
	afx_msg void OnBnClickedGraphlinestackedbutton();
	afx_msg void OnBnClickedGraphpiebutton();
	afx_msg void OnFileSavestatgraphas();
	afx_msg void OnBnClickedFetchDiff();
	afx_msg void OnBnClickedCommitternames();
	afx_msg void OnBnClickedCommitdates();
	DECLARE_MESSAGE_MAP()

	// ** Member functions **

	/// Updates the variables m_nWeeks, m_nDays and m_minDate
	void UpdateWeekCount();
	/// Returns the week-of-the-year for the given time.
	int GetCalendarWeek(const CTime& time);
	/// Parses the data given to the dialog and generates mappings with statistical data.
	int GatherData(BOOL fetchdiff = FALSE, BOOL keepFetchedData = FALSE);
	/// Populates the lists passed as arguments based on the commit threshold set with the skipper.
	void FilterSkippedAuthors(std::list<std::wstring>& included_authors, std::list<std::wstring>& skipped_authors);
	/// Shows the graph Percentage Of Authorship
	void ShowPercentageOfAuthorship();
	/// Shows the graph with commit counts per author.
	void ShowCommitsByAuthor();
	/// Shows the graph with commit counts per author and date.
	void ShowByDate(int StringY, int title, IntervalDataMap &data);
	/// Shows the initial statistics page.
	void ShowStats();

	/// Rolling Percentage Of Authorship of author to integer
	int RollPercentageOfAuthorship(double it);

	/// Load list of drawing authors
	template <class MAP> void LoadListOfAuthors (MAP &map, bool reloadSkiper = false, bool compare = false);

	// If we have other authors, count them and their commits.
	template <class MAP>
	void DrawOthers(const std::list<std::wstring>& others, MyGraphSeries* graphData, MAP& map);


	/// Called when user checks/unchecks the "Authors case sensitive" checkbox.
	/// Recalculates statistical data because the number and names of authors
	/// can have changed. Also calls RedrawGraph().
	void AuthorsCaseSensitiveChanged();
	/// Called when user checks/unchecks the "Sort by commit count" checkbox.
	/// Calls RedrawGraph().
	void SortModeChanged();
	/// Clears the current graph and frees all data series.
	void ClearGraph();
	/// Updates the currently shown statistics page.
	void RedrawGraph();

	/// PreShow Statistic function
	bool  PreViewStat(bool fShowLabels);
	/// PreShow Graphic function
	MyGraphSeries * PreViewGraph(__in UINT GraphTitle, __in UINT YAxisLabel, __in UINT XAxisLabel = NULL);
	/// Show Selected Static metric
	void ShowSelectStat(Metrics  SelectedMetric, bool reloadSkiper = false);

	int						GetUnit(const CTime& time);
	CStatGraphDlg::UnitType	GetUnitType();
	CString					GetUnitString();
	CString					GetUnitLabel(int unit, CTime &lasttime);

	void EnableDisableMenu();

	void SaveGraph(CString sFilename);
	int  GetEncoderClsid(const WCHAR* format, CLSID* pClsid);

	void StoreCurrentGraphType();
	void ShowErrorMessage();

	// init ruler & limit its range
	void SetSkipper (bool reloadSkiper);

	//Load statistical queries
	void LoadStatQueries(__in UINT curStr, Metrics loadMetric, bool setDef = false);

	//Considers coefficient contribution author
	double CoeffContribution(int distFromEnd);

	CPtrArray		m_graphDataArray;
	MyGraph			m_graph;
	CComboBox		m_cGraphType;
	CSliderCtrl		m_Skipper;
	BOOL			m_bAuthorsCaseSensitive;
	BOOL			m_bSortByCommitCount;
	BOOL			m_bUseCommitterNames;
	BOOL			m_bUseCommitDates;
	bool			m_bDiffFetched = false;

	CMFCButton		m_btnGraphBar;
	CMFCButton		m_btnGraphBarStacked;
	CMFCButton		m_btnGraphLine;
	CMFCButton		m_btnGraphLineStacked;
	CMFCButton		m_btnGraphPie;

	MyGraph::GraphType	m_GraphType = MyGraph::GraphType::Bar;
	bool				m_bStacked = false;

	int				m_langOrder = 0;

	// ** Member variables holding the statistical data	**

	///	Number of days in the revision interval.
	int						m_nDays = -1;
	///	Number of weeks	in the revision	interval.
	int						m_nWeeks = -1;
	///	The	starting date/time for the revision	interval.
	__time64_t				m_minDate = 0;
	///	The	ending date/time for the revision interval.
	__time64_t				m_maxDate = 0;
	///	The	total number of	commits	(equals	size of	the	m_parXXX arrays).
	INT_PTR					m_nTotalCommits = 0;
	///	The	total number of	file changes.
	LONG					m_nTotalFileChanges = 0;
	///	Holds the number of	commits	per	unit and author.
	IntervalDataMap			m_commitsPerUnitAndAuthor;

	IntervalDataMap			m_LinesWPerUnitAndAuthor;
	IntervalDataMap			m_LinesWOPerUnitAndAuthor;

	///	Holds the number of	file changes per unit and author.
	IntervalDataMap			m_filechangesPerUnitAndAuthor;
	///	First interval number (key)	in the mappings.
	int						m_firstInterval = 0;
	///	Last interval number (key) in the mappings.
	int						m_lastInterval = 0;
	///	Mapping	of total commits per author, access	data via
	AuthorDataMap			m_commitsPerAuthor;
	///	Mapping	of Percentage Of Authorship	per	author
	AuthorshipDataMap		   m_PercentageOfAuthorship;

	LONG					m_nTotalLinesInc = 0;
	LONG					m_nTotalLinesDec = 0;
	LONG					m_nTotalLinesNew = 0;
	LONG					m_nTotalLinesDel = 0;

	///	The	list of	author names sorted	based on commit	count
	///	(author	with most commits is first in list).
	std::list<std::wstring> m_authorNames;
	///	unit names by week/month/quarter
	std::map<LONG, std::wstring> m_unitNames;
};
