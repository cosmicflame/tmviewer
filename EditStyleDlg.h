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

#if !defined(AFX_EDITSTYLEDLG_H__E3F3C468_7C1F_4D27_8712_EB54F8C84E33__INCLUDED_)
#define AFX_EDITSTYLEDLG_H__E3F3C468_7C1F_4D27_8712_EB54F8C84E33__INCLUDED_

#include "Content.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditStyleDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditStyleDlg dialog

class CEditStyleDlg : public CDialog
{
// Construction
public:
	CString m_Caption;
	CContentStyle m_Style;
	CEditStyleDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEditStyleDlg)
	enum { IDD = IDD_EditStyle };
	CButton	m_Shadow;
	CEdit	m_FontName;
	CEdit	m_StyleName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditStyleDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEditStyleDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnbtChangeFont();
	virtual void OnOK();
	afx_msg void OnPaint();
	afx_msg void OnchkShadow();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CContentText m_SampleText;
	CRect m_SampleRect;
	CContent m_Content;
	CString GetFontStr();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITSTYLEDLG_H__E3F3C468_7C1F_4D27_8712_EB54F8C84E33__INCLUDED_)
