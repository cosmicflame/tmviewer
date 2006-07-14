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

// NewSymbols.cpp : implementation file
//

#include "stdafx.h"
#include "TMViewer.h"
#include "NewSymbols.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewSymbols dialog


CNewSymbols::CNewSymbols(CWnd* pParent /*=NULL*/)
	: CDialog(CNewSymbols::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewSymbols)
	m_ClearCats = TRUE;
	m_ClearStyles = TRUE;
	//}}AFX_DATA_INIT
}


void CNewSymbols::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewSymbols)
	DDX_Check(pDX, IDC_ClearCats, m_ClearCats);
	DDX_Check(pDX, IDC_ClearStyles, m_ClearStyles);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewSymbols, CDialog)
	//{{AFX_MSG_MAP(CNewSymbols)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewSymbols message handlers
