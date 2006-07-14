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

// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__1F484E14_CD49_4296_8F7E_7005AE25F271__INCLUDED_)
#define AFX_MAINFRM_H__1F484E14_CD49_4296_8F7E_7005AE25F271__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ChildView.h"
#include "Mapper.h"	// Added by ClassView
#include <afxadv.h>

//----------------------------------------------------------------------------------
#define iniToolBars "ToolBars"
#define iniOptions "Options"
#define iniMapsPath "MapsPath"
#define iniZoom "Zoom"
#define iniLevel "Level"
#define iniWindow "Window"
#define iniPosX "PosX"
#define iniPosY "PosY"
#define iniVersion "Version"
#define iniFiltering "Filtering"
#define iniOverlayMode "OverlayMode"
#define iniShading "Shading"
#define iniLastDescFile "LastDescFile"

/*------------------------------------------------------------------------------------*/
class CViewerToolBar : public CToolBar
{
	//{{AFX_MSG(CViewerToolbar)
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/*------------------------------------------------------------------------------------*/
class CMainFrame : public CFrameWnd
{
	
public:
	CMainFrame();
protected: 
	DECLARE_DYNAMIC(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members

public:
	CStatusBar  m_wndStatusBar;
	CToolBar m_MarksTB;
	void CreateMarksTB();
	BOOL CheckModified();
	void FillStylesCombo();
	CToolBar m_EditorTB;
	CViewerToolBar m_ViewerTB;
	void SetShading(int ShadingVal);
	CSliderCtrl m_ShadingSlider;
  CStatic m_IntensityText;
	void SetMapInfo(int Zoom, int Level);
	CComboBox m_cbZoom;
  CStatic m_ZoomText;
  CStatic m_ZoomPercText;
  CComboBox m_cbLevel;
  CStatic m_LevelText;
  CComboBox m_cbStyles;
	CChildView    m_wndView;
	BOOL NoFiltering;
	CFont m_BaseFont;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	afx_msg void OnSelchangeCbZoom();
	afx_msg void OnSelchangeCbLevel();
	afx_msg void OnSelchangeCbStyle();
	afx_msg void OnDestroy();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnUpdateViewNavigation(CCmdUI* pCmdUI);
	afx_msg void OnViewNavigation();
	afx_msg void OnClose();
	afx_msg void OnHelpViewreadme();
	afx_msg void OnFileExporttobmp();
	afx_msg void OnCheckOverlay();
	afx_msg void OnUpdateCheckOverlay(CCmdUI* pCmdUI);
	afx_msg void OnViewFiltering();
	afx_msg void OnUpdateViewFiltering(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileExporttobmp(CCmdUI* pCmdUI);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnEditorMode();
	afx_msg void OnUpdateEditorMode(CCmdUI* pCmdUI);
	afx_msg void OnMarkcategories();
	afx_msg void OnUpdateMarkcategories(CCmdUI* pCmdUI);
	afx_msg void OnViewHideallsymbols();
	afx_msg void OnUpdateViewHideallsymbols(CCmdUI* pCmdUI);
	afx_msg void OnShowAllMarks();
	afx_msg void OnUpdateShowAllMarks(CCmdUI* pCmdUI);
	afx_msg void OnHideAllMarks();
	afx_msg void OnUpdateHideAllMarks(CCmdUI* pCmdUI);
	afx_msg void OnViewTextlabels();
	afx_msg void OnUpdateViewTextlabels(CCmdUI* pCmdUI);
	afx_msg void OnViewMarks();
	afx_msg void OnUpdateViewMarks(CCmdUI* pCmdUI);
	//}}AFX_MSG
  afx_msg BOOL OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult );
  afx_msg void OnToggleCategory(UINT id);
  afx_msg void OnUpdateToggleCategory(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()
private:
	DWORD m_TBStyle;
	CImageList m_MarksTB_Images;
};

extern CMainFrame* g_pMainFrame;

CString ExtractPath(const CString FileName);
CString GetAppFileName();

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__1F484E14_CD49_4296_8F7E_7005AE25F271__INCLUDED_)
