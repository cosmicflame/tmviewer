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

// ExportDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TMViewer.h"
#include "ExportDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExportDlg dialog


CExportDlg::CExportDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CExportDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExportDlg)
	m_ExportSymbols = TRUE;
	m_ExportZoom = FALSE;
	m_ExportFile = _T("");
	//}}AFX_DATA_INIT
}


void CExportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExportDlg)
	DDX_Check(pDX, IDC_ExportSymbols, m_ExportSymbols);
	DDX_Check(pDX, IDC_ExportZoom, m_ExportZoom);
	DDX_Text(pDX, IDC_ExportFile, m_ExportFile);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExportDlg, CDialog)
	//{{AFX_MSG_MAP(CExportDlg)
	ON_BN_CLICKED(IDC_Browse, OnBrowse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExportDlg message handlers

void CExportDlg::OnBrowse() 
{
  CString s;
  s.Format("Level (%s).bmp", theApp.m_map.GetLevelStr(theApp.m_map.m_iLevel));

  CFileDialog dlg(FALSE, ".bmp", s, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
                  "Bitmap files (*.bmp)|*.bmp|All Files (*.*)|*.*||", this);
  dlg.m_ofn.lpstrTitle = "Export to BMP";

  if (dlg.DoModal() == IDOK) {
    GetDlgItem(IDC_ExportFile)->SetWindowText(dlg.GetPathName());
  }
}

void CExportDlg::OnOK() 
{
  GetDlgItem(IDC_ExportFile)->GetWindowText(m_ExportFile);
  if (m_ExportFile.IsEmpty()) {
    AfxMessageBox("You must specify file to export to.", MB_ICONERROR, 0);
    return;
  }
	CDialog::OnOK();
}
