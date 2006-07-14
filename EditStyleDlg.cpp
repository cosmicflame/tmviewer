/*
  This file is part of Tibia Map Viewer.
  Copyright (c) 2001-2006 by Yury Sidorov.

  Tibia Map Viewer is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  Tibia Map Viewer is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Tibia Map Viewer; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

// EditStyleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TMViewer.h"
#include "EditStyleDlg.h"
#include "StylesListDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditStyleDlg dialog


CEditStyleDlg::CEditStyleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditStyleDlg::IDD, pParent), m_Style(&m_Content), 
    m_SampleText(m_Content)
{
	//{{AFX_DATA_INIT(CEditStyleDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CEditStyleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditStyleDlg)
	DDX_Control(pDX, IDC_chkShadow, m_Shadow);
	DDX_Control(pDX, IDC_edFontName, m_FontName);
	DDX_Control(pDX, IDC_edStyleName, m_StyleName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditStyleDlg, CDialog)
	//{{AFX_MSG_MAP(CEditStyleDlg)
	ON_BN_CLICKED(IDC_btChangeFont, OnbtChangeFont)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_chkShadow, OnchkShadow)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditStyleDlg message handlers

BOOL CEditStyleDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

  if (!m_Caption.IsEmpty())
    SetWindowText(m_Caption);

  m_StyleName.SetWindowText(m_Style.GetStyleName());  
  m_FontName.SetWindowText(GetFontStr());
  m_Shadow.SetCheck(m_Style.Shadow);
	
  m_Style.SetZoom();
  m_SampleText.Style = &m_Style;
  m_SampleText.SetText("Sample text");

  CRect r;
  GetDlgItem(IDC_FRAME)->GetWindowRect(m_SampleRect);
  ScreenToClient(m_SampleRect);
  m_Shadow.GetWindowRect(r);
  ScreenToClient(r);
  m_SampleRect.top = r.bottom;
  m_SampleRect.DeflateRect(13, 13);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEditStyleDlg::OnbtChangeFont() 
{
  LOGFONT lf;
  m_Style.Font.GetLogFont(&lf);

	CFontDialog fd(&lf, CF_TTONLY | CF_EFFECTS | CF_FORCEFONTEXIST | CF_INITTOLOGFONTSTRUCT | CF_SCREENFONTS, NULL, this);
  fd.m_cf.rgbColors = m_Style.TextColor;

  if (fd.DoModal() == IDOK) {
    BYTE fs = 0;
    if (fd.IsBold())
      fs |= fsBold;
    if (fd.IsItalic())
      fs |= fsItalic;
    if (fd.IsUnderline())
      fs |= fsUnderline;
    m_Style.FontName = fd.GetFaceName();
    m_Style.FontSize = fd.GetSize()/10;
    m_Style.FontStyle = fs; 
    m_Style.FontCharSet = lf.lfCharSet;
    m_Style.TextColor = fd.GetColor();
    m_Style.SetZoom();

    m_FontName.SetWindowText(GetFontStr());

    m_SampleText.SetText(m_SampleText.GetText());
    InvalidateRect(m_SampleRect, FALSE);
  }
}

CString CEditStyleDlg::GetFontStr()
{
  CString res, s;
  if (!m_Style.FontName.IsEmpty()) {
    if (m_Style.FontStyle & fsBold)
      s += ", Bold";
    if (m_Style.FontStyle & fsItalic)
      s += ", Italic";
    if (m_Style.FontStyle & fsUnderline)
      s += ", Underline";
    res.Format("%s, %d%s", m_Style.FontName, m_Style.FontSize, s);
  }
  return res;
}

void CEditStyleDlg::OnOK() 
{
  CString s;
  m_StyleName.GetWindowText(s);  
  m_Style.SetStyleName(s);
  
	CDialog::OnOK();
}

void CEditStyleDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
  DrawSample(dc, m_SampleRect, m_SampleText, FALSE);
}

void CEditStyleDlg::OnchkShadow() 
{
  m_Style.Shadow = m_Shadow.GetCheck();
  m_SampleText.SetText(m_SampleText.GetText());
  InvalidateRect(m_SampleRect, FALSE);
}
