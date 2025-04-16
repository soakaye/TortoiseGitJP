﻿// TortoiseGit - a Windows shell extension for easy version control

// Copyright (C) 2003-2008 - TortoiseSVN
// Copyright (C) 2009-2021, 2023-2025 - TortoiseGit

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

#include "stdafx.h"
#include "TortoiseProc.h"
#include "AboutDlg.h"
#include "PathUtils.h"
#define NEED_SIGNING_KEY
#include "../version.h"
#include "VersioncheckParser.h"
#include "AppUtils.h"
#include "Git.h"
#include "DPIAware.h"

//IMPLEMENT_DYNAMIC(CAboutDlg, CStandAloneDialog)
CAboutDlg::CAboutDlg(CWnd* pParent /*=nullptr*/)
	: CStandAloneDialog(CAboutDlg::IDD, pParent)
{
}

CAboutDlg::~CAboutDlg()
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CStandAloneDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_WEBLINK, m_cWebLink);
	DDX_Control(pDX, IDC_SUPPORTLINK, m_cSupportLink);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CStandAloneDialog)
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	ON_BN_CLICKED(IDC_UPDATE, OnBnClickedUpdate)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

static CString Lf2Crlf(const CString& text)
{
	CString s;
	if (text.IsEmpty())
		return s;

	wchar_t c = L'\0';
	for (int i = 0; i < text.GetLength(); i++)
	{
		if (text[i] == L'\n' && c != L'\r')
			s += L"\r\n";
		else
			s += text[i];
		c = text[i];
	}
	return s;
}

BOOL CAboutDlg::OnInitDialog()
{
	CStandAloneDialog::OnInitDialog();
	CAppUtils::MarkWindowAsUnpinnable(m_hWnd);

	// set the version string
	CString temp;

	CString out, err;
	if (g_Git.Run(L"git.exe --version", &out, &err, CP_UTF8))
		out = L"git not found (" + err + L')';
	out.Trim();

	if (!CGit::ms_LastMsysGitDir.IsEmpty())
	{
		out += L" (" + CGit::ms_LastMsysGitDir;
		out += L"; " + CGit::ms_MsysGitRootDir;
		out += L"; " + g_Git.GetGitSystemConfig();
		if (CGit::ms_bMsys2Git)
			out += L"; with msys2 hack";
		else if (CGit::ms_bCygwinGit)
			out += L"; with cygwin hack";
		out += L')';
	}

	CString tortoisegitprocpath;
	CString additionalVersionInformation;
#if PREVIEW
	additionalVersionInformation = _T(PREVIEW_INFO);
#else
	if (CString hotfix = CPathUtils::GetAppDirectory() + L"hotfix.ini"; PathFileExists(hotfix))
	{
		CVersioncheckParser parser;
		if (parser.Load(hotfix, err))
		{
			auto version = parser.GetTortoiseGitVersion();
			if (version.major == TGIT_VERMAJOR && version.minor == TGIT_VERMINOR && version.micro == TGIT_VERMICRO && version.build > TGIT_VERBUILD)
				additionalVersionInformation = L"Hotfix " + parser.GetTortoiseGitVersion().version;
		}
	}
#endif
	if (!additionalVersionInformation.IsEmpty())
		tortoisegitprocpath.Format(L"(%s; %s)", static_cast<LPCWSTR>(additionalVersionInformation), static_cast<LPCWSTR>(CPathUtils::GetAppDirectory().TrimRight(L'\\')));
	else
		tortoisegitprocpath.Format(L"(%s)", static_cast<LPCWSTR>(CPathUtils::GetAppDirectory().TrimRight(L'\\')));
	temp.Format(IDS_ABOUTVERSION, TGIT_VERMAJOR, TGIT_VERMINOR, TGIT_VERMICRO, TGIT_VERBUILD, static_cast<LPCWSTR>(tortoisegitprocpath), static_cast<LPCWSTR>(out));
	SetDlgItemText(IDC_VERSIONABOUT, Lf2Crlf(temp));

	this->SetWindowText(L"TortoiseGit");

	// we can only put up to 256 chars into the resource file, so fill it here with the full list
	SetDlgItemText(IDC_STATIC_AUTHORS, L"Sven Strickroth <email@cs-ware.de> (Current Maintainer), Sup Yut Sum <ch3cooli@gmail.com>, Frank Li <lznuaa@gmail.com> (Founder), Yue Lin Ho <b8732003@student.nsysu.edu.tw>, Colin Law <clanlaw@googlemail.com>, Myagi <snowcoder@gmail.com>, Johan 't Hart <johanthart@gmail.com>, Laszlo Papp <djszapi@archlinux.us>");

	LoadSVGLogoAndStartAnimation();

	m_cWebLink.SetURL(L"https://tortoisegit.org/");
	m_cSupportLink.SetURL(L"https://tortoisegit.org/donate");

	SetTheme(CTheme::Instance().IsDarkTheme());

	CenterWindow(CWnd::FromHandle(GetExplorerHWND()));
	GetDlgItem(IDOK)->SetFocus();
	return FALSE;
}

void CAboutDlg::LoadSVGLogoAndStartAnimation()
{
	HRSRC hRes = ::FindResource(nullptr, MAKEINTRESOURCE(IDR_TGITLOGO), RT_RCDATA);
	if (!hRes)
		return;
	DWORD sz = ::SizeofResource(nullptr, hRes);
	HGLOBAL hData = ::LoadResource(nullptr, hRes);
	if (!hData)
		return;

	int width = CDPIAware::Instance().ScaleX(GetSafeHwnd(), 468);
	int height = CDPIAware::Instance().ScaleY(GetSafeHwnd(), 64);
	std::string_view svg(reinterpret_cast<const char*>(::LockResource(hData)), sz);
	SCOPE_EXIT { ::UnlockResource(hData); };
	if (!m_renderSrc.Create32BitFromSVG(svg, width, height) || !m_renderDest.Create32BitFromSVG(svg, width, height))
	{
		m_renderSrc.DeleteObject();
		m_renderDest.DeleteObject();
		return;
	}

	m_waterEffect.Create(width, height);
	SetTimer(ID_EFFECTTIMER, 40, nullptr);
	SetTimer(ID_DROPTIMER, 1500, nullptr);
}

void CAboutDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == ID_EFFECTTIMER)
	{
		m_waterEffect.Render(static_cast<DWORD*>(m_renderSrc.GetDIBits()), static_cast<DWORD*>(m_renderDest.GetDIBits()));
		CClientDC dc(this);
		CPoint ptOrigin(CDPIAware::Instance().ScaleX(GetSafeHwnd(), 15), CDPIAware::Instance().ScaleY(GetSafeHwnd(), 20));
		m_renderDest.Draw(&dc,ptOrigin);
	}
	if (nIDEvent == ID_DROPTIMER)
	{
		CRect r;
		r.left = CDPIAware::Instance().ScaleX(GetSafeHwnd(), 15);
		r.top = CDPIAware::Instance().ScaleY(GetSafeHwnd(), 20);
		r.right = r.left + m_renderSrc.GetWidth();
		r.bottom = r.top + m_renderSrc.GetHeight();
		m_waterEffect.Blob(random(r.left,r.right), random(r.top, r.bottom), 5, 800, m_waterEffect.m_iHpage);
	}
	CStandAloneDialog::OnTimer(nIDEvent);
}

void CAboutDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	CRect r;
	r.left = CDPIAware::Instance().ScaleX(GetSafeHwnd(), 15);
	r.top = CDPIAware::Instance().ScaleY(GetSafeHwnd(), 20);
	r.right = r.left + m_renderSrc.GetWidth();
	r.bottom = r.top + m_renderSrc.GetHeight();

	if(r.PtInRect(point) == TRUE)
	{
		// dibs are drawn upside down...
		point.y -= CDPIAware::Instance().ScaleY(GetSafeHwnd(), 20);
		point.y = CDPIAware::Instance().ScaleY(GetSafeHwnd(), 64) - point.y;

		if (nFlags & MK_LBUTTON)
			m_waterEffect.Blob(point.x - CDPIAware::Instance().ScaleX(GetSafeHwnd(), 15), point.y, 10, 1600, m_waterEffect.m_iHpage);
		else
			m_waterEffect.Blob(point.x - CDPIAware::Instance().ScaleX(GetSafeHwnd(), 15), point.y, 5, 50, m_waterEffect.m_iHpage);
	}

	CStandAloneDialog::OnMouseMove(nFlags, point);
}

void CAboutDlg::OnBnClickedUpdate()
{
	CAppUtils::RunTortoiseGitProc(L"/command:updatecheck /visible", false, false);
}

void CAboutDlg::OnClose()
{
	KillTimer(ID_EFFECTTIMER);
	KillTimer(ID_DROPTIMER);

	__super::OnClose();
}
