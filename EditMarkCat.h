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

#if !defined(AFX_EDITMARKCAT_H__B74217F3_9079_4238_ACB4_02C7BA71FB3E__INCLUDED_)
#define AFX_EDITMARKCAT_H__B74217F3_9079_4238_ACB4_02C7BA71FB3E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditMarkCat.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditMarkCat dialog

class CEditMarkCat : public CDialog
{
// Construction
public:
	CString m_Caption;
	HICON m_icon;
	CEditMarkCat(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEditMarkCat();

// Dialog Data
	//{{AFX_DATA(CEditMarkCat)
	enum { IDD = IDD_EditMarkCat };
	CStatic	m_Img;
	CString	m_Name;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditMarkCat)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEditMarkCat)
	afx_msg void OnbtSelBmp();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CImageList m_Image;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITMARKCAT_H__B74217F3_9079_4238_ACB4_02C7BA71FB3E__INCLUDED_)
