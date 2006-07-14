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

// EditMarkCat.cpp : implementation file
//

#include "stdafx.h"
#include "TMViewer.h"
#include "EditMarkCat.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditMarkCat dialog


CEditMarkCat::CEditMarkCat(CWnd* pParent /*=NULL*/)
	: CDialog(CEditMarkCat::IDD, pParent), m_icon(NULL)
{
	//{{AFX_DATA_INIT(CEditMarkCat)
	m_Name = _T("");
	//}}AFX_DATA_INIT
  m_Image.Create(MarkWd, MarkHt, ILC_COLOR16 | ILC_MASK, 0, 1);
}

CEditMarkCat::~CEditMarkCat()
{
  DestroyIcon(m_icon);
}

void CEditMarkCat::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditMarkCat)
	DDX_Control(pDX, IDC_MarkImg, m_Img);
	DDX_Text(pDX, IDC_edDescr, m_Name);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditMarkCat, CDialog)
	//{{AFX_MSG_MAP(CEditMarkCat)
	ON_BN_CLICKED(IDC_btSelBmp, OnbtSelBmp)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditMarkCat message handlers

void CEditMarkCat::OnbtSelBmp() 
{
  CString s;
  CFileDialog dlg(TRUE, ".bmp", NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_EXPLORER | OFN_ENABLESIZING, 
                  "Image files (*.bmp *.ico)|*.bmp;*.ico|Bitmap files (*.bmp)|*.bmp|Icon files (*.ico)|*.ico|All Files (*.*)|*.*||", this);
  dlg.m_ofn.lpstrTitle = "Open mark image";

  if (dlg.DoModal() == IDOK) {
    HANDLE h;
    UINT t;
    s = dlg.GetPathName();
    if (s.Right(4).CompareNoCase(".ico") == 0)
      t = IMAGE_ICON;
    else
      t = IMAGE_BITMAP;
    h = LoadImage(NULL, s, t, MarkWd, MarkHt, LR_LOADFROMFILE);
    if (h) {
      if (t == IMAGE_ICON) {
        m_Image.Add((HICON)h);
      }
      else {
        CDC dc;
        dc.CreateCompatibleDC(NULL);
        CBitmap *b = dc.SelectObject(CBitmap::FromHandle((HBITMAP)h));
        COLORREF c = dc.GetPixel(0, 0);
        dc.SelectObject(b);
        m_Image.Add(CBitmap::FromHandle((HBITMAP)h), c);
      }
      DeleteObject(h);
      m_icon = m_Image.ExtractIcon(0);
      m_Img.SetIcon(m_icon);
      m_Image.Remove(0);
    }
    else {
      int i;
      i = GetLastError();
      if (i == 0)
        s = "Error loading image.";
      else
        s = GetErrorMessage(i);
  		AfxMessageBox(s, MB_ICONERROR);
    }
  }
}

BOOL CEditMarkCat::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
  if (!m_Caption.IsEmpty())
    SetWindowText(m_Caption);

  LONG ws = GetWindowLong(m_Img, GWL_STYLE) & ~SS_BLACKRECT;
  SetWindowLong(m_Img, GWL_STYLE, ws | SS_ICON);
  if (m_icon)
    m_Img.SetIcon(m_icon);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEditMarkCat::OnDestroy() 
{
	CDialog::OnDestroy();
}

void CEditMarkCat::OnOK() 
{
  CString s;
  if (!m_icon) {
  	AfxMessageBox("Mark image must be specified.", MB_ICONERROR);
    return;
  }
  GetDlgItem(IDC_edDescr)->GetWindowText(s);
  if (s.IsEmpty()) {
  	AfxMessageBox("Mark name must be specified.", MB_ICONERROR);
    return;
  }
  
	CDialog::OnOK();
}
