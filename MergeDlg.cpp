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

// MergeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TMViewer.h"
#include "MergeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMergeDlg dialog


CMergeDlg::CMergeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMergeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMergeDlg)
	m_MergeWith = _T("");
	m_Fogged = TRUE;
	//}}AFX_DATA_INIT
}


void CMergeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMergeDlg)
	DDX_Text(pDX, IDC_MergeWith, m_MergeWith);
	DDX_Check(pDX, IDC_Fogged, m_Fogged);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMergeDlg, CDialog)
	//{{AFX_MSG_MAP(CMergeDlg)
	ON_BN_CLICKED(IDC_Browse, OnBrowse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMergeDlg message handlers

int CALLBACK SelFolderCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData);

void CMergeDlg::OnBrowse() 
{
  BROWSEINFO bi;
  char dir[MAX_PATH];
  LPITEMIDLIST lst;

  ZeroMemory(&bi, sizeof(BROWSEINFO));
  bi.hwndOwner = m_hWnd;
  bi.pszDisplayName = dir;
  bi.lpszTitle = "Select maps folder to merge with:";
  bi.lpfn = SelFolderCallbackProc;
  bi.lParam = (long)m_MergeWith.LockBuffer();
  lst = SHBrowseForFolder(&bi);
  m_MergeWith.UnlockBuffer();
  if (lst) {
    SHGetPathFromIDList(lst, dir);
    m_MergeWith = dir;
    GetDlgItem(IDC_MergeWith)->SetWindowText(m_MergeWith);
  }
}

void CMergeDlg::OnOK() 
{
  GetDlgItem(IDC_MergeWith)->GetWindowText(m_MergeWith);
  if (m_MergeWith.IsEmpty()) {
    AfxMessageBox("You must specify merge path.", MB_ICONERROR, 0);
    return;
  }
  
  m_Fogged = IsDlgButtonChecked(IDC_Fogged);

  CMapper mm;
  CMapper &m_map = theApp.m_map;
  CString s;
  int x, y, lv, i, cnt;

  mm.SetMapsPath(m_MergeWith);
  mm.CalcMapRange();
  if (mm.IsEmpty()) {
    s.Format("There are no maps was found in '%s'", m_MergeWith);
    AfxMessageBox(s, MB_ICONERROR, 0);
    return;
  }
  if (mm.m_MapVersion != m_map.m_MapVersion) {
    AfxMessageBox("Can not merge maps from different versions of Tibia client.", MB_ICONERROR, 0);
    return;
  }

  CWaitCursor wc;
  BYTE *src, *dst, b;
  BOOL msk, DoSave;

  cnt = 0;
  for (y = mm.m_MapRange.top / mm.m_MapHt; y <= mm.m_MapRange.bottom / mm.m_MapHt; y++) 
    for (x = mm.m_MapRange.left / mm.m_MapWd; x <= mm.m_MapRange.right / mm.m_MapWd; x++) 
      for (lv = mm.m_iMinLevel; lv <= mm.m_iMaxLevel; lv++) 
        if (mm.LoadMap(x, y, lv)) {
          i = m_map.m_MapWd*m_map.m_MapHt;
          src = (BYTE*)mm.m_MapBuf;
          dst = (BYTE*)m_map.m_MapBuf;
          if (!m_map.LoadMap(x, y, lv)) {
            ZeroMemory(dst, i);
            if (m_map.m_MapVersion == 2)
              memset(dst + i, 0xFA, i);
          }
          DoSave = FALSE;

          do {
            i--;
            if ((i / m_map.m_MapWd) & 1) 
              msk = (i & 1);
            else
              msk = !(i & 1);
            b = (!m_Fogged || msk || *src == 0xDC) ? *src : 0;   

            if (*dst != b && (*dst == 0 || *dst == 0xA)) {
              *dst = b;
              DoSave = TRUE;
            }
            src++;
            dst++;
          } 
          while (i > 0);

          if (m_map.m_MapVersion == 2) {
            i = m_map.m_MapWd*m_map.m_MapHt;
            do {
              if (*dst != *src && *dst == 0xFA) {
                *dst = *src;
                DoSave = TRUE;
              }
              src++;
              dst++;
              i--;
            } 
            while (i > 0);
          }

          if (DoSave) {
            if (!m_map.SaveMapAs(x, y, lv)) {
              s.Format("Error saving map file %s%s", m_map.m_sMapsPath, m_map.GetMapFileName(x, y, lv));
              AfxMessageBox(s, MB_ICONERROR, 0);
              goto mend;
            };
            cnt++;
          }
        }
  theApp.OpenMaps();
  if (cnt)
    s.Format("%d map files has been merged.", cnt);
  else
    s = "No map files has been merged.\rIt's seems that the source and destination maps are identical.";
  AfxMessageBox(s, MB_ICONINFORMATION, 0);
	
mend:
	CDialog::OnOK();
}
