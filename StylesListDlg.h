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

#if !defined(AFX_STYLESLISTDLG_H__C9AE7CCF_68C4_432A_85FB_7450D85C1096__INCLUDED_)
#define AFX_STYLESLISTDLG_H__C9AE7CCF_68C4_432A_85FB_7450D85C1096__INCLUDED_

#include "Content.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StylesListDlg.h : header file
//

void DrawSample(CDC& dc, CRect r, CContentText& Text, BOOL NoText);

/////////////////////////////////////////////////////////////////////////////
// CStylesListDlg dialog

class CStylesListDlg : public CDialog
{
// Construction
public:
	CRect m_SampleRect;
	CStylesListDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CStylesListDlg)
	enum { IDD = IDD_StylesList };
	CListBox	m_StylesList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStylesListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CStylesListDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnbtNewStyle();
	afx_msg void OnbtEditStyle();
	afx_msg void OnbtDeleteStyle();
	afx_msg void OnDblclkStylesList();
	afx_msg void OnSelchangeStylesList();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	virtual void OnOK();
private:
	CContent m_Content;
	CContentStyle m_CurStyle;
	CContentText m_SampleText;
	void UpdateButtons();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STYLESLISTDLG_H__C9AE7CCF_68C4_432A_85FB_7450D85C1096__INCLUDED_)
