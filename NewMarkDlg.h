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

#if !defined(AFX_NEWMARKDLG_H__B2F6EDCB_1FE7_496B_8CF0_B8E4848C2943__INCLUDED_)
#define AFX_NEWMARKDLG_H__B2F6EDCB_1FE7_496B_8CF0_B8E4848C2943__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewMarkDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNewMarkDlg dialog

class CNewMarkDlg : public CDialog
{
// Construction
public:
	CString m_Caption;
	CNewMarkDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNewMarkDlg)
	enum { IDD = IDD_NewMark };
	CComboBoxEx	m_Category;
	CString	m_ShortText;
	CString	m_LongText;
	int		m_CategoryIdx;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewMarkDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNewMarkDlg)
	afx_msg void OnGetdispinfocbCategory(NMHDR* pNMHDR, LRESULT* pResult);
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CImageList m_Images;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWMARKDLG_H__B2F6EDCB_1FE7_496B_8CF0_B8E4848C2943__INCLUDED_)
