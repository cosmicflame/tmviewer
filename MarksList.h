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

#if !defined(AFX_MARKSLIST_H__F0EFB30F_6581_45D2_83BB_F099DB15D760__INCLUDED_)
#define AFX_MARKSLIST_H__F0EFB30F_6581_45D2_83BB_F099DB15D760__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MarksList.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMarksList dialog

class CMarksList : public CDialog
{
// Construction
public:
	CMarksList(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CMarksList)
	enum { IDD = IDD_MarksList };
	CListCtrl	m_Marks;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMarksList)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMarksList)
	afx_msg void OnbtNewMark();
	virtual BOOL OnInitDialog();
	afx_msg void OnbtEditMark();
	afx_msg void OnbtDeleteMark();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnClickMarks(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkMarks(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickMarks(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRdblclkMarks(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnMoveUp();
	afx_msg void OnMoveDown();
	afx_msg void OnItemchangedMarks(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:
	virtual void OnOK();
private:
	void UpdateButtonsEx(int idx);
	void FillList();
	void MoveCategory(int Idx, int Delta);
	void UpdateButtons();
	CImageList m_Images;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MARKSLIST_H__F0EFB30F_6581_45D2_83BB_F099DB15D760__INCLUDED_)
