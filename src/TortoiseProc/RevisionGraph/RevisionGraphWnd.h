﻿// TortoiseGit - a Windows shell extension for easy version control

// Copyright (C) 2003-2011 - TortoiseSVN
// Copyright (C) 2012-2019, 2022-2023, 2025 - TortoiseGit

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
#include "Future.h"
#include "ProgressDlg.h"
#include "Colors.h"
#include "SVG.h"
#include "LogDlgHelper.h"
#include "Graphviz.h"

#pragma warning(push)
#pragma warning(disable: 4100) // unreferenced formal parameter
#include <ogdf/layered/SugiyamaLayout.h>
#pragma warning(pop)

using namespace Gdiplus;
using namespace async;

constexpr int REVGRAPH_PREVIEW_WIDTH = 100;
constexpr int REVGRAPH_PREVIEW_HEIGHT = 200;

// don't draw pre-views with more than that number of nodes
constexpr int REVGRAPH_PREVIEW_MAX_NODES = 10000;

// radius of the rounded / slanted box corners  of the expand / collapse / split / join square gylphs
constexpr int CORNER_SIZE = 12;

// font sizes
constexpr int DEFAULT_ZOOM_FONT = 9; // default font size
constexpr int SMALL_ZOOM_FONT = 11; // rel. larger font size for small zoom factors
constexpr int SMALL_ZOOM_FONT_THRESHOLD = 6; // max. "small zoom" font size after scaling

// glyph display delay definitions
constexpr int GLYPH_HOVER_EVENT = 10; // timer ID for the glyph display delay
constexpr int GLYPH_HOVER_DELAY = 250; // delay until the glyphs are shown [ms]

// zoom control
constexpr float MIN_ZOOM = 0.01f;
constexpr float MAX_ZOOM = 2.0f;
constexpr float DEFAULT_ZOOM = 1.0f;
constexpr float ZOOM_STEP = 0.9f;

// don't draw shadows below this zoom level
constexpr float SHADOW_ZOOM_THRESHOLD = 0.2f;

#define MAX_TT_LENGTH			60000
#define MAX_TT_LENGTH_DEFAULT	1000

// forward declarations

class CRevisionGraphDlg;
struct AllColorsAndBrushes;
struct ColorsAndBrushes;

// simplify usage of classes from other namespaces

//using async::IJob;
//using async::CFuture;

/**
 * \ingroup TortoiseProc
 * Window class showing a revision graph.
 *
 * The analyzation of the log data is done in the child class CRevisionGraph.
 * Here, we handle the window notifications.
 */
class CRevisionGraphWnd : public CWnd //, public CRevisionGraph
{
public:
	CRevisionGraphWnd();	// standard constructor
	virtual ~CRevisionGraphWnd();
	enum
	{
		IDD = IDD_REVISIONGRAPH,
		WM_WORKERTHREADDONE = WM_APP +1
	};


	CString			m_sPath;
	GitRev			m_pegRev;

	CLogCache			m_LogCache;
	CLogDataVector		m_logEntries;
	MAP_HASH_NAME		m_HashMap;
	CString				m_CurrentBranch;
	CGitHash			m_HeadHash;
	CGit::SubmoduleInfo	m_submoduleInfo;

	BOOL		m_bCurrentBranch;
	BOOL		m_bLocalBranches;
	CString		m_FromRev;
	CString		m_ToRev;

	bool		m_bShowBranchingsMerges;
	bool		m_bShowAllTags;
	bool		m_bArrowPointToMerges;

	void ReloadHashMap()
	{
		m_HashMap.clear();
		if (g_Git.GetMapHashToFriendName(m_HashMap))
			MessageBox(g_Git.GetGitLastErr(L"Could not get all refs."), L"TortoiseGit", MB_ICONERROR);
		m_CurrentBranch=g_Git.GetCurrentBranch();
		if (g_Git.GetHash(m_HeadHash, L"HEAD"))
			MessageBox(g_Git.GetGitLastErr(L"Could not get HEAD hash."), L"TortoiseGit", MB_ICONERROR);
	}

	std::unique_ptr<CFuture<bool>> updateJob;
//	CRevisionGraphState m_state;

	void			InitView();
	void			Init(CWnd * pParent, LPRECT rect);
	void			SaveGraphAs(CString sSavePath);

	bool			FetchRevisionData ( const CString& path
										, CProgressDlg* progress
										, ITaskbarList3* pTaskbarList
										, HWND hWnd);
	bool			IsUpdateJobRunning() const;

	bool			GetShowOverview() const;
	void			SetShowOverview (bool value);

	void			ScrollTo(int i, bool select);

	void			CompareRevs(const CString& revTo);
	void			UnifiedDiffRevs(bool bHead);

	CRect			GetGraphRect();
	CRect			GetClientRect();
	CRect			GetWindowRect();
	CRect			GetViewRect();
	void			DoZoom (float nZoomFactor, bool updateScrollbars = true);
	bool			CancelMouseZoom();

	void			BuildPreview();

protected:
	ULONGLONG		m_ullTicks = 0;
	CRect			m_OverviewPosRect;
	CRect			m_OverviewRect;

	bool			m_bShowOverview;

	CRevisionGraphDlg* m_parent = nullptr;

	ogdf::node		m_HeadNode = nullptr;
	ogdf::node		m_SelectedEntry1 = nullptr;
	ogdf::node		m_SelectedEntry2 = nullptr;
	int				m_nFontSize;
	CToolTipCtrl*	m_pDlgTip = nullptr;
	char			m_szTip[MAX_TT_LENGTH+1];
	wchar_t			m_wszTip[MAX_TT_LENGTH+1];
	CString			m_sTitle;

	float			m_fZoomFactor;
	CColors			m_Colors;
	bool			m_bTweakTrunkColors;
	bool			m_bTweakTagsColors;
	bool			m_bIsCanvasMove;
	CPoint			m_ptMoveCanvas;
	CPoint			m_ptRubberEnd;

	CBitmap			m_Preview;
	int				m_previewWidth;
	int				m_previewHeight;
	float			m_previewZoom;

	ogdf::node		m_hoverIndex = nullptr;
	DWORD			m_hoverGlyphs = 0;	// the glyphs shown for \ref m_hoverIndex
	mutable ogdf::node m_tooltipIndex = nullptr;	// the node index we fetched the tooltip for
	bool			m_showHoverGlyphs;	// if true, show the glyphs we currently hover over
										// (will be activated only after some delay)

	CString		GetFriendRefName(ogdf::node) const;
	STRING_VECTOR	GetFriendRefNames(ogdf::node, const CString* exclude = nullptr, CGit::REF_TYPE* onlyRefType = nullptr) const;

	ogdf::Graph	m_Graph;
	ogdf::GraphAttributes m_GraphAttr;
	ogdf::SugiyamaLayout m_SugiyamLayout;

	CRect	m_GraphRect;

	int GetLeftRightMargin() const { return 20; };
	int GetTopBottomMargin() const { return 5; };
	void			DoDataExchange(CDataExchange* pDX) override;	// DDX/DDV support
	afx_msg void	OnPaint();
	ULONG			GetGestureStatus(CPoint ptTouch) override;
	afx_msg BOOL	OnEraseBkgnd(CDC* pDC);
	afx_msg void	OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void	OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void	OnSize(UINT nType, int cx, int cy);
	afx_msg INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
	afx_msg void	OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL	OnToolTipNotify(UINT id, NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg BOOL	OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void	OnMouseHWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void	OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void	OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void	OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL	OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void	OnTimer(UINT_PTR nIDEvent);
	afx_msg void	OnCaptureChanged(CWnd *pWnd);
	afx_msg LRESULT OnWorkerThreadDone(WPARAM, LPARAM);

	DECLARE_MESSAGE_MAP()
private:

	double m_ArrowCos;
	double m_ArrowSin;
	double m_ArrowSize;

	enum MarkerPosition
	{
		mpLeft = 0,
		mpRight = 1,
	};

	enum GlyphType
	{
		NoGlyph = -1,
		ExpandGlyph = 0,	// "+"
		CollapseGlyph = 1,	// "-"
		SplitGlyph = 2,		// "x"
		JoinGlyph = 3,		// "o"
	};

	enum GlyphPosition
	{
		Above = 0,
		Right = 4,
		Below = 8,
	};

	struct GraphicsDevice
	{
		CDC* pDC = nullptr;
		Graphics* graphics = nullptr;
		SVG* pSVG = nullptr;
		Graphviz* pGraphviz = nullptr;
	};

	class SVGGrouper
	{
	public:
		SVGGrouper(SVG* pSVG)
			: m_pSVG(pSVG)
		{
			if (m_pSVG)
				m_pSVG->StartGroup();
		}
		~SVGGrouper()
		{
			if (m_pSVG)
				m_pSVG->EndGroup();
		}
	private:
		SVG*	m_pSVG = nullptr;
	};

	bool			UpdateSelectedEntry (ogdf::node clickedentry);
	void			AppendMenu (CMenu& popup, UINT title, UINT command, UINT flags = MF_ENABLED);
	void			AppendMenu(CMenu& popup, CString title, UINT command, CString* extra = nullptr, CMenu* submenu = nullptr);
	void			DoShowLog();
	void			DoSwitch(CString rev);
	void			DoBrowseRepo();
	void			DoCopyRefs();

	void			SetScrollbar (int bar, int newPos, int clientMax, int graphMax);
	void			SetScrollbars (int nVert = -1, int nHorz = -1);

	CSize			UsableTooltipRect();
	CString			DisplayableText (const CString& wholeText, const CSize& tooltipSize);
	CString			TooltipText (ogdf::node index);

	CPoint			GetLogCoordinates (CPoint point) const;
	ogdf::node		GetHitNode (CPoint point, CSize border = CSize (0, 0)) const;
	DWORD			GetHoverGlyphs (CPoint point) const;
	PointF			cutPoint(ogdf::node v, double lw, const PointF& ps, const PointF& pt) const;

	using TCutRectangle = PointF[8];
	void			CutawayPoints (const RectF& rect, float cutLen, TCutRectangle& result) const;
	enum
	{
		ROUND_UP = 0x1,
		ROUND_DOWN = 0x2,
		ROUND_BOTH = 0x3,
	};
	void			DrawRoundedRect (GraphicsDevice& graphics, const Color& penColor, int penWidth, const Pen* pen, const Color& fillColor, const Brush* brush, const RectF& rect, int mask=ROUND_BOTH) const;
	RectF			TransformRectToScreen (const CRect& rect, const CSize& offset) const;
	RectF			GetNodeRect (const ogdf::node& v, const CSize& offset) const;
	void			DrawMarker ( GraphicsDevice& graphics, const RectF& noderect
							   , MarkerPosition position, int relPosition, const Color& penColor, int num) const;
	void			DrawConnections (GraphicsDevice& graphics, const CRect& logRect, const CSize& offset) const;
	void			DrawTexts (GraphicsDevice& graphics, const CRect& logRect, const CSize& offset);
	void			DrawGraph(GraphicsDevice& graphics, const CRect& rect, int nVScrollPos, int nHScrollPos, bool bDirectDraw);
	void DrawNode(GraphicsDevice& graphics, AllColorsAndBrushes& colorsAndBrushes, ColorsAndBrushes* colors, const Gdiplus::Font& font, const CString& fontname, double height, const RectF& noderect, const CString& shortname, size_t line, size_t lines) const;

	int				GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
	void	SetNodeRect(GraphicsDevice& graphics, Gdiplus::Font& font, const Rect& commitString, ogdf::node* pnode, const CGitHash& rev);
	void	MeasureTextLength(GraphicsDevice& graphics, Gdiplus::Font& font, const CString& text, int& xmax, int& ymax) const;
};
