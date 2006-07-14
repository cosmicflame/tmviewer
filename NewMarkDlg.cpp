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

// NewMarkDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TMViewer.h"
#include "NewMarkDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewMarkDlg dialog


CNewMarkDlg::CNewMarkDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewMarkDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewMarkDlg)
	m_ShortText = _T("");
	m_LongText = _T("");
	m_CategoryIdx = 0;
	//}}AFX_DATA_INIT
}


void CNewMarkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewMarkDlg)
	DDX_Control(pDX, IDC_cbCategory, m_Category);
	DDX_Text(pDX, IDC_ShortText, m_ShortText);
	DDX_Text(pDX, IDC_LongText, m_LongText);
	DDX_CBIndex(pDX, IDC_cbCategory, m_CategoryIdx);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewMarkDlg, CDialog)
	//{{AFX_MSG_MAP(CNewMarkDlg)
	ON_NOTIFY(CBEN_GETDISPINFO, IDC_cbCategory, OnGetdispinfocbCategory)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewMarkDlg message handlers

void CNewMarkDlg::OnGetdispinfocbCategory(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

BOOL CNewMarkDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

  if (!m_Caption.IsEmpty())
    SetWindowText(m_Caption);

  m_Images.Create(&theApp.m_map.Content.MarkImages);
  m_Category.SetImageList(&m_Images);

  int i;
  COMBOBOXEXITEM ci;
  ci.mask = CBEIF_IMAGE | CBEIF_SELECTEDIMAGE | CBEIF_TEXT;
  CMarkCategories &mcs = theApp.m_map.Content.MarkCategories;
  for (i = 0; i < mcs.GetSize(); i++) {
    ci.iItem = i;
    ci.iImage = i;
    ci.iSelectedImage  = i;
    ci.pszText = mcs[i]->Name.LockBuffer();
    m_Category.InsertItem(&ci);
    mcs[i]->Name.UnlockBuffer();
  }

  m_Category.GetComboBoxCtrl()->SetCurSel(m_CategoryIdx);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CNewMarkDlg::OnDestroy() 
{
  m_Category.SetImageList(NULL);
	CDialog::OnDestroy();
}

void CNewMarkDlg::OnOK() 
{
	CDialog::OnOK();
}
