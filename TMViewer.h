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

// TMViewer.h : main header file for the TMVIEWER application
//

#if !defined(AFX_TMVIEWER_H__BD9093FE_99CE_403A_9338_2735DB3F036C__INCLUDED_)
#define AFX_TMVIEWER_H__BD9093FE_99CE_403A_9338_2735DB3F036C__INCLUDED_

#include "Mapper.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CTMViewerApp:
// See TMViewer.cpp for the implementation of this class
//

class CTMViewerApp : public CWinApp
{
public:
	CTMViewerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTMViewerApp)
	public:
	virtual BOOL InitInstance();
	virtual BOOL OnIdle(LONG lCount);
	virtual CDocument* OpenDocumentFile(LPCTSTR lpszFileName);
	//}}AFX_VIRTUAL

// Implementation

public:
	void ClearMapDesc();
  virtual ~CTMViewerApp();
	void OpenMaps(BOOL Centered = FALSE);
	CMapper m_map;
	BOOL m_bStarted;
	//{{AFX_MSG(CTMViewerApp)
	afx_msg void OnAppAbout();
	afx_msg void OnFileOpen();
	afx_msg void OnMergeMaps();
	afx_msg BOOL OnSaveDescr();
	afx_msg void OnOpenDesc();
	afx_msg BOOL OnSaveDescAs();
	afx_msg void OnUpdateSaveDescr(CCmdUI* pCmdUI);
	afx_msg void OnUpdateSaveDescAs(CCmdUI* pCmdUI);
	afx_msg void OnUpdateOpenDesc(CCmdUI* pCmdUI);
	afx_msg void OnNewMapDesc();
	afx_msg void OnUpdateNewMapDesc(CCmdUI* pCmdUI);
	afx_msg void OnLoadpreviousmapsymbols();
	afx_msg void OnUpdateLoadpreviousmapsymbols(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
};

extern CTMViewerApp theApp;
extern CMenu m_Popups;

CString GetErrorMessage(DWORD nError);

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TMVIEWER_H__BD9093FE_99CE_403A_9338_2735DB3F036C__INCLUDED_)
