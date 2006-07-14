/*
  This file is part of Tibia Map Viewer.

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

// StylesListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TMViewer.h"
#include "StylesListDlg.h"
#include "EditStyleDlg.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

void DrawSample(CDC& dc, CRect r, CContentText& Text, BOOL NoText)
{
  dc.DrawEdge(r, EDGE_SUNKEN, BF_RECT);
  r.DeflateRect(2, 2);
  dc.IntersectClipRect(r);
  switch (theApp.m_map.m_MapVersion) {
  case 1:
    if (theApp.m_map.LoadMap(250, 251, 0)) 
      StretchBlt(dc.m_hDC, r.left, r.top, r.Width(), r.Height(), theApp.m_map.m_PartMap, 0, 0, 128, r.Height()*128/r.Width(), SRCCOPY);
    else 
      goto m1;
    break;
  case 2:
    if (theApp.m_map.LoadMap(125, 125, 0)) 
      StretchBlt(dc.m_hDC, r.left, r.top, r.Width(), r.Height(), theApp.m_map.m_PartMap, 0, 128, 128, r.Height()*128/r.Width(), SRCCOPY);
    else 
      goto m1;
    break;
  default:
m1:
    dc.SelectStockObject(DKGRAY_BRUSH);
    dc.FillRect(r, dc.GetCurrentBrush());
  }

  CPoint pt;
  pt.x = r.left + (r.Width() - Text.BoundsRect.Width())/2;
  if (pt.x < r.left)
    pt.x = r.left;
  pt.y = r.top + (r.Height() - Text.BoundsRect.Height())/2;
  Text.SetPosition(pt.x, pt.y);
  Text.Draw(dc, 0);
}

/////////////////////////////////////////////////////////////////////////////
// CStylesListDlg dialog

CStylesListDlg::CStylesListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStylesListDlg::IDD, pParent), m_SampleText(m_Content),
  m_CurStyle(&m_Content)
{
	//{{AFX_DATA_INIT(CStylesListDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

  m_CurStyle.SetZoom();
  m_SampleText.Style = &m_CurStyle;
  m_SampleText.SetText("Sample text");
}


void CStylesListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStylesListDlg)
	DDX_Control(pDX, IDC_STYLES_LIST, m_StylesList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStylesListDlg, CDialog)
	//{{AFX_MSG_MAP(CStylesListDlg)
	ON_BN_CLICKED(IDC_btNewStyle, OnbtNewStyle)
	ON_BN_CLICKED(IDC_btEditStyle, OnbtEditStyle)
	ON_BN_CLICKED(IDC_btDeleteStyle, OnbtDeleteStyle)
	ON_LBN_DBLCLK(IDC_STYLES_LIST, OnDblclkStylesList)
	ON_LBN_SELCHANGE(IDC_STYLES_LIST, OnSelchangeStylesList)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStylesListDlg message handlers

BOOL CStylesListDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
  int i;
  CContentStyles &css = theApp.m_map.Content.Styles;

  for (i = 0; i < css.GetSize(); i++) 
    m_StylesList.AddString(css[i]->GetStyleName());
  m_StylesList.SetCurSel(0);
  
  CRect r;
  GetClientRect(m_SampleRect);
  m_StylesList.GetWindowRect(r);
  ScreenToClient(r);
  m_SampleRect.top = r.bottom;
  m_SampleRect.DeflateRect(10, 10);

  OnSelchangeStylesList();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CStylesListDlg::OnbtNewStyle() 
{
  CEditStyleDlg es;
  
  es.m_Style = theApp.m_map.Content.DefStyle;
  es.m_Style.SetStyleName("New style");
  es.m_Style.SetZoom(100);
  if (es.DoModal() == IDOK) {
    CContentStyle *cs = new CContentStyle(&theApp.m_map.Content);
    *cs = es.m_Style;
    cs->SetZoom(theApp.m_map.m_iZoom);
    theApp.m_map.Content.Styles.Add(cs);
    m_StylesList.SetCurSel(m_StylesList.AddString(cs->GetStyleName()));
    OnSelchangeStylesList();
    theApp.m_map.Content.Modified = TRUE;
    UpdateButtons();
  }
}

void CStylesListDlg::OnbtEditStyle() 
{
  CEditStyleDlg es;
  
  int i, idx = m_StylesList.GetCurSel();
  es.m_Style = *theApp.m_map.Content.Styles[idx];
  es.m_Style.SetZoom(100);
  es.m_Caption = "Edit style";
  if (es.DoModal() == IDOK) {
    *theApp.m_map.Content.Styles[idx] = es.m_Style;
    theApp.m_map.Content.Styles[idx]->SetZoom(theApp.m_map.m_iZoom);

    CContentObjects& objs = theApp.m_map.Content.Objects;
    CContentObj *o;

    for (i = 0; i < objs.GetSize(); i++) {
      o = objs[i];
      if (o->StyleIdx == idx) {
        g_pMainFrame->m_wndView.InvalidateObj(o);
        o->StyleChanged();
        g_pMainFrame->m_wndView.InvalidateObj(o);
      }
    }
    theApp.m_map.Content.Modified = TRUE;

    m_StylesList.DeleteString(idx);
    m_StylesList.InsertString(idx, es.m_Style.GetStyleName());
    m_StylesList.SetCurSel(idx);
    OnSelchangeStylesList();
  }
}

void CStylesListDlg::OnbtDeleteStyle() 
{
  CString s;
  int i, idx = m_StylesList.GetCurSel(); 
  s.Format("Do you want to delete styele '%s'", theApp.m_map.Content.Styles[idx]->GetStyleName());
  if (AfxMessageBox(s, MB_ICONQUESTION | MB_YESNO, 0) != IDYES) return;

  m_StylesList.DeleteString(idx);
  theApp.m_map.Content.Styles.RemoveAt(idx);
  
  CContentObjects& objs = theApp.m_map.Content.Objects;
  CContentObj *o;

  for (i = 0; i < objs.GetSize(); i++) {
    o = objs[i];
    if (o->StyleIdx == idx) {
      g_pMainFrame->m_wndView.InvalidateObj(o);
      o->StyleIdx = 0;
      o->StyleChanged();
      g_pMainFrame->m_wndView.InvalidateObj(o);
    }
    else
    if (o->StyleIdx > idx) 
      o->StyleIdx--;
  }
  theApp.m_map.Content.Modified = TRUE;
  UpdateButtons();
  OnSelchangeStylesList();
}

void CStylesListDlg::OnDblclkStylesList() 
{
  if (m_StylesList.GetCurSel() >= 0)
    OnbtEditStyle();	
}

void CStylesListDlg::UpdateButtons()
{
  BOOL e;
  e = (m_StylesList.GetCurSel() >= 0);
  GetDlgItem(IDC_btEditStyle)->EnableWindow(e);
  e = e && (theApp.m_map.Content.Styles.GetSize() > 1);
  GetDlgItem(IDC_btDeleteStyle)->EnableWindow(e);
}

void CStylesListDlg::OnSelchangeStylesList() 
{
  UpdateButtons();	

  int idx = m_StylesList.GetCurSel(); 
  if (idx >= 0) {
    m_CurStyle = *theApp.m_map.Content.Styles[idx];
    m_SampleText.SetText(m_SampleText.GetText());
  }
  InvalidateRect(m_SampleRect, FALSE);
}

void CStylesListDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

  DrawSample(dc, m_SampleRect, m_SampleText, m_StylesList.GetCurSel() < 0);
}

void CStylesListDlg::OnOK()
{
  if (GetDlgItem(IDC_btEditStyle)->IsWindowEnabled())
    OnbtEditStyle();
}
