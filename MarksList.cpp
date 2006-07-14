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

// MarksList.cpp : implementation file
//

#include "stdafx.h"
#include "TMViewer.h"
#include "MarksList.h"
#include "EditMarkCat.h"
#include "Content.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMarksList dialog


CMarksList::CMarksList(CWnd* pParent /*=NULL*/)
	: CDialog(CMarksList::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMarksList)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMarksList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMarksList)
	DDX_Control(pDX, IDC_Marks, m_Marks);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMarksList, CDialog)
	//{{AFX_MSG_MAP(CMarksList)
	ON_BN_CLICKED(IDC_btNewMark, OnbtNewMark)
	ON_BN_CLICKED(IDC_btEditMark, OnbtEditMark)
	ON_BN_CLICKED(IDC_btDeleteMark, OnbtDeleteMark)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_NOTIFY(NM_CLICK, IDC_Marks, OnClickMarks)
	ON_NOTIFY(NM_DBLCLK, IDC_Marks, OnDblclkMarks)
	ON_NOTIFY(NM_RCLICK, IDC_Marks, OnRclickMarks)
	ON_NOTIFY(NM_RDBLCLK, IDC_Marks, OnRdblclkMarks)
	ON_BN_CLICKED(IDC_MoveUp, OnMoveUp)
	ON_BN_CLICKED(IDC_MoveDown, OnMoveDown)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_Marks, OnItemchangedMarks)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMarksList message handlers

void CMarksList::OnbtNewMark() 
{
  CEditMarkCat d(this);
  if (d.DoModal() == IDOK) {
    CMarkCategory *mc = new CMarkCategory;
    int idx;// = m_Marks.GetSelectionMark();
    theApp.m_map.Content.MarkImages.Add(d.m_icon);
    mc->Name = d.m_Name;
    idx = theApp.m_map.Content.MarkCategories.Add(mc);
    m_Marks.InsertItem(idx, d.m_Name, idx);
    theApp.m_map.Content.Modified = TRUE;
    UpdateButtons();
    g_pMainFrame->CreateMarksTB();
  }
}

BOOL CMarksList::OnInitDialog() 
{
	CDialog::OnInitDialog();

  m_Images.Create(&theApp.m_map.Content.MarkImages);
  m_Marks.SetImageList(&m_Images, LVSIL_SMALL);
	
  CRect r;
  m_Marks.GetClientRect(r);
  m_Marks.InsertColumn(0, "", LVCFMT_LEFT, r.Width() - GetSystemMetrics(SM_CYVSCROLL));
  m_Marks.SetExtendedStyle(m_Marks.GetExtendedStyle() | LVS_EX_FULLROWSELECT);

  FillList();

  UpdateButtons();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMarksList::OnbtEditMark() 
{
  CEditMarkCat d(this);
  int idx = m_Marks.GetSelectionMark();
  CMarkCategory *mc;
  d.m_icon = theApp.m_map.Content.MarkImages.ExtractIcon(idx);
  mc = theApp.m_map.Content.MarkCategories[idx];
  d.m_Name = mc->Name;
  d.m_Caption = "Edit marks category";

  if (d.DoModal() == IDOK) {
    theApp.m_map.Content.MarkImages.Replace(idx, d.m_icon);
    mc->Name = d.m_Name;
    m_Marks.SetItem(idx, 0, LVIF_TEXT, d.m_Name, idx, 0, 0, 0);
    m_Marks.Update(idx);
    theApp.m_map.Content.Modified = TRUE;

    CContentObjects& objs = theApp.m_map.Content.Objects;
    CContentObj *o;

    for (int i = 0; i < objs.GetSize(); i++) {
      o = objs[i];
      if (o->GetType() == otMark && ((CContentMark*)o)->Category == idx) 
        g_pMainFrame->m_wndView.InvalidateObj(o);
    }

    UpdateButtons();
    g_pMainFrame->CreateMarksTB();
  }
}

void CMarksList::OnbtDeleteMark() 
{
  CMarkCategories &mcs = theApp.m_map.Content.MarkCategories;
  int idx = m_Marks.GetSelectionMark();
  CString s;
  s.Format("Do you want to delete mark category '%s' ?", mcs[idx]->Name);
  if (AfxMessageBox(s, MB_ICONQUESTION | MB_YESNO, 0) != IDYES) return;

  BOOL del;
  if (mcs.GetSize() > 1) 
    del = (AfxMessageBox("Delete all objects of this category ?", MB_ICONQUESTION | MB_YESNO, 0) == IDYES);
  else
    del = TRUE;

  CContentObjects& objs = theApp.m_map.Content.Objects;
  CContentObj *o;

  int i = 0;
  while (i < objs.GetSize()) {
    o = objs[i];
    if (o->GetType() == otMark && ((CContentMark*)o)->Category == idx) {
      g_pMainFrame->m_wndView.InvalidateObj(o);
      if (del)
        objs.RemoveAt(i);
      else {
        ((CContentMark*)o)->Category = 0;
        i++;
      }
    }
    else {
      if (((CContentMark*)o)->Category > idx) 
        ((CContentMark*)o)->Category--;
      i++;
    }
  }
  
  mcs.RemoveAt(idx);
  m_Marks.DeleteItem(idx);
  theApp.m_map.Content.MarkImages.Remove(idx);
  theApp.m_map.Content.Modified = TRUE;

  LVITEM it;
  it.mask = LVIF_IMAGE;
  for (i = 0; i < m_Marks.GetItemCount(); i++) {
    it.iItem = i;
    it.iImage = i;
    m_Marks.SetItem(&it);
  }

  UpdateButtons();
  g_pMainFrame->CreateMarksTB();
}

void CMarksList::OnClose() 
{
	CDialog::OnClose();
}

void CMarksList::OnDestroy() 
{
  m_Marks.SetImageList(NULL, LVSIL_SMALL);
	CDialog::OnDestroy();
}

void CMarksList::UpdateButtons()
{
  int idx = m_Marks.GetSelectionMark();
  if (idx < 0 || m_Marks.GetItemState(idx, LVIS_SELECTED) != LVIS_SELECTED)
    idx = -1;
  UpdateButtonsEx(idx);
}

void CMarksList::UpdateButtonsEx(int idx)
{
  BOOL e = (idx >= 0);

  GetDlgItem(IDC_btEditMark)->EnableWindow(e);
  GetDlgItem(IDC_btDeleteMark)->EnableWindow(e);
  GetDlgItem(IDC_MoveUp)->EnableWindow(e && idx > 0);
  GetDlgItem(IDC_MoveDown)->EnableWindow(e && idx < m_Marks.GetItemCount() - 1);
}
void CMarksList::OnClickMarks(NMHDR* pNMHDR, LRESULT* pResult) 
{
  UpdateButtons();
	*pResult = 0;
}


void CMarksList::OnDblclkMarks(NMHDR* pNMHDR, LRESULT* pResult) 
{
  UpdateButtons();
  if (GetDlgItem(IDC_btEditMark)->IsWindowEnabled())
    OnbtEditMark();
	*pResult = 0;
}

void CMarksList::OnRclickMarks(NMHDR* pNMHDR, LRESULT* pResult) 
{
  UpdateButtons();
	*pResult = 0;
}

void CMarksList::OnRdblclkMarks(NMHDR* pNMHDR, LRESULT* pResult) 
{
  UpdateButtons();
	*pResult = 0;
}

void CMarksList::OnOK()
{
  UpdateButtons();
  if (GetDlgItem(IDC_btEditMark)->IsWindowEnabled())
    OnbtEditMark();
}

void CMarksList::OnMoveUp() 
{
  MoveCategory(m_Marks.GetSelectionMark(), -1);
}

void CMarksList::OnMoveDown() 
{
  MoveCategory(m_Marks.GetSelectionMark(), 1);
}

void CMarksList::MoveCategory(int Idx, int Delta)
{
  CContentObjects& objs = theApp.m_map.Content.Objects;
  CContentObj *o;
  CContentMark* m;
  int i, idx2, cat, d;
  CMarkCategory *mc;

  CMarkCategories &mcs = theApp.m_map.Content.MarkCategories;
  mc = mcs[Idx];
  mcs[Idx] = NULL;
  mcs.RemoveAt(Idx);
  idx2 = Idx + Delta;
  mcs.InsertAt(idx2, mc);

  d = abs(Delta)/Delta;

  CImageList &il = theApp.m_map.Content.MarkImages;
  HICON ico = il.ExtractIcon(Idx);
  HICON ico2;
  i = idx2;
  do {
    ico2 = il.ExtractIcon(i);
    i -= d;
    il.Replace(i, ico2);
    DestroyIcon(ico2);
  }
  while (i != Idx);

  il.Replace(idx2, ico);
  DestroyIcon(ico);

  for (i = Idx; i != idx2 + d; i += d)
    m_Marks.SetItem(i, 0, LVIF_IMAGE | LVIF_TEXT, mcs[i]->Name, i, 0, 0, NULL);

  m_Marks.SetSelectionMark(idx2);
  m_Marks.SetItemState(idx2, LVIS_SELECTED, LVIS_SELECTED);

  for (i = 0; i < objs.GetSize(); i++) {
    o = objs[i];
    if (o->GetType() == otMark) {
      m = (CContentMark*)o;
      cat = m->Category;
      if (cat == Idx)
        m->Category = idx2;
      else
      if (cat >= idx2 && cat < Idx)
        m->Category++;
      else
      if (cat <= idx2 && cat > Idx)
        m->Category--;
    }
  }

  theApp.m_map.Content.Modified = TRUE;
  UpdateButtons();
  g_pMainFrame->CreateMarksTB();
}

void CMarksList::FillList()
{
  int i;
  CMarkCategories &mcs = theApp.m_map.Content.MarkCategories;
  m_Marks.DeleteAllItems();
  for (i = 0; i < mcs.GetSize(); i++)
    m_Marks.InsertItem(i, mcs[i]->Name, i);
}

void CMarksList::OnItemchangedMarks(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	
  if (pNMListView->uNewState & LVIS_SELECTED)
    UpdateButtonsEx(pNMListView->iItem);

	*pResult = 0;
}
