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

// TextProps.cpp : implementation file
//

#include "stdafx.h"
#include "TMViewer.h"
#include "TextProps.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTextProps dialog


CTextProps::CTextProps(CWnd* pParent /*=NULL*/)
	: CDialog(CTextProps::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTextProps)
	m_EditText = _T("");
	//}}AFX_DATA_INIT
}


void CTextProps::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTextProps)
	DDX_Control(pDX, IDOK, m_OK);
	DDX_Text(pDX, IDC_EDIT_TEXT, m_EditText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTextProps, CDialog)
	//{{AFX_MSG_MAP(CTextProps)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTextProps message handlers

BOOL CTextProps::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
  if (!m_DlgCaption.IsEmpty())
    SetWindowText(m_DlgCaption);
  
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
