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

// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "TMViewer.h"
#include "MainFrm.h"
#include "TextProps.h"
#include "MarksList.h"
#include "ExportDlg.h"
#include <Shlwapi.h>

#define FirstCatButtonID 40000

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CViewerToolBar, CToolBar)
	//{{AFX_MSG_MAP(CViewerToolBar)
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_CBN_SELCHANGE(IDC_CB_ZOOM, OnSelchangeCbZoom)
	ON_CBN_SELCHANGE(IDC_CB_LEVEL, OnSelchangeCbLevel)
	ON_CBN_SELCHANGE(ID_STYLES_COMBO, OnSelchangeCbStyle)
	ON_WM_DESTROY()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_UPDATE_COMMAND_UI(ID_VIEW_NAVIGATION, OnUpdateViewNavigation)
	ON_COMMAND(ID_VIEW_NAVIGATION, OnViewNavigation)
	ON_WM_CLOSE()
	ON_COMMAND(ID_HELP_VIEWREADME, OnHelpViewreadme)
	ON_COMMAND(ID_FILE_EXPORTTOBMP, OnFileExporttobmp)
	ON_BN_CLICKED(IDC_CHECK_OVERLAY, OnCheckOverlay)
	ON_UPDATE_COMMAND_UI(IDC_CHECK_OVERLAY, OnUpdateCheckOverlay)
	ON_COMMAND(ID_VIEW_FILTERING, OnViewFiltering)
	ON_UPDATE_COMMAND_UI(ID_VIEW_FILTERING, OnUpdateViewFiltering)
	ON_UPDATE_COMMAND_UI(ID_FILE_EXPORTTOBMP, OnUpdateFileExporttobmp)
	ON_WM_SIZING()
	ON_BN_CLICKED(IDC_EDITOR_MODE, OnEditorMode)
	ON_UPDATE_COMMAND_UI(IDC_EDITOR_MODE, OnUpdateEditorMode)
	ON_COMMAND(ID_MARKCATEGORIES, OnMarkcategories)
	ON_UPDATE_COMMAND_UI(ID_MARKCATEGORIES, OnUpdateMarkcategories)
	ON_COMMAND(ID_VIEW_HIDEALLSYMBOLS, OnViewHideallsymbols)
	ON_UPDATE_COMMAND_UI(ID_VIEW_HIDEALLSYMBOLS, OnUpdateViewHideallsymbols)
	ON_COMMAND(ID_ShowAllMarks, OnShowAllMarks)
	ON_UPDATE_COMMAND_UI(ID_ShowAllMarks, OnUpdateShowAllMarks)
	ON_COMMAND(ID_HideAllMarks, OnHideAllMarks)
	ON_UPDATE_COMMAND_UI(ID_HideAllMarks, OnUpdateHideAllMarks)
	ON_COMMAND(ID_VIEW_TEXTLABELS, OnViewTextlabels)
	ON_UPDATE_COMMAND_UI(ID_VIEW_TEXTLABELS, OnUpdateViewTextlabels)
	ON_COMMAND(IDC_CHECK_OVERLAY, OnCheckOverlay)
	ON_COMMAND(IDC_EDITOR_MODE, OnEditorMode)
	ON_COMMAND(ID_VIEW_MARKS, OnViewMarks)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MARKS, OnUpdateViewMarks)
	//}}AFX_MSG_MAP
  ON_NOTIFY_EX(TTN_NEEDTEXT, 0, OnToolTipNotify)
	ON_COMMAND_RANGE(FirstCatButtonID, FirstCatButtonID + 1000, OnToggleCategory)
	ON_UPDATE_COMMAND_UI_RANGE(FirstCatButtonID, FirstCatButtonID + 1000, OnUpdateToggleCategory)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           
  ID_INDICATOR1,
  ID_INDICATOR2
};

/*------------------------------------------------------------------------------------*/
CString GetAppFileName()
{
	char sExeName[256];
	GetModuleFileName(0, sExeName, sizeof(sExeName));
	return CString(sExeName);
}

/*------------------------------------------------------------------------------------*/
CString ExtractPath(const CString FileName)
{
	CString res(FileName);
	res.MakeReverse();
	int n = res.FindOneOf("\\/");
	res.Delete(0, n);
	res.MakeReverse();
	return res;
}

/*------------------------------------------------------------------------------------*/
CString GetErrorMessage(DWORD nError)
{
	LPTSTR lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		nError,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf,
		0,
		NULL
	);
	CString res(lpMsgBuf);
	LocalFree( lpMsgBuf );
	return res;
}

/*------------------------------------------------------------------------------------*/
void CViewerToolBar::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
  if (pScrollBar->GetDlgCtrlID() == IDC_SLIDER_SHADING) {
    g_pMainFrame->SetShading(9 - g_pMainFrame->m_ShadingSlider.GetPos());
	  g_pMainFrame->m_wndView.SetFocus();
    return;
  }
	CToolBar::OnHScroll(nSBCode, nPos, pScrollBar);
}


/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
  OSVERSIONINFO vi;
  vi.dwOSVersionInfoSize = sizeof(vi);
  GetVersionEx(&vi);
  NoFiltering = vi.dwPlatformId != VER_PLATFORM_WIN32_NT && vi.dwMajorVersion == 4 && vi.dwMinorVersion <= 10;
}

CMainFrame::~CMainFrame()
{
}

#define PACKVERSION(major,minor) MAKELONG(minor,major)
DWORD GetDllVersion(LPCTSTR lpszDllName)
{
    HINSTANCE hinstDll;
    DWORD dwVersion = 0;

    /* For security purposes, LoadLibrary should be provided with a 
       fully-qualified path to the DLL. The lpszDllName variable should be
       tested to ensure that it is a fully qualified path before it is used. */
    hinstDll = LoadLibrary(lpszDllName);
	
    if(hinstDll)
    {
        DLLGETVERSIONPROC pDllGetVersion;
        pDllGetVersion = (DLLGETVERSIONPROC)GetProcAddress(hinstDll, 
                          "DllGetVersion");

        /* Because some DLLs might not implement this function, you
        must test for it explicitly. Depending on the particular 
        DLL, the lack of a DllGetVersion function can be a useful
        indicator of the version. */

        if(pDllGetVersion)
        {
            DLLVERSIONINFO dvi;
            HRESULT hr;

            ZeroMemory(&dvi, sizeof(dvi));
            dvi.cbSize = sizeof(dvi);

            hr = (*pDllGetVersion)(&dvi);

            if(SUCCEEDED(hr))
            {
               dwVersion = PACKVERSION(dvi.dwMajorVersion, dvi.dwMinorVersion);
            }
        }

        FreeLibrary(hinstDll);
    }
    return dwVersion;
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

  m_Popups.LoadMenu(IDR_POPUPS);

	// create a view to occupy the client area of the frame
	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}

  m_BaseFont.CreatePointFont(80, "MS Sans Serif");
	
  if(GetDllVersion(TEXT("comctl32.dll")) < PACKVERSION(6, 00)) 
    m_TBStyle = TBSTYLE_FLAT;
  else
    m_TBStyle = 0;
	if (!m_ViewerTB.CreateEx(this, m_TBStyle, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, CRect(1, 1, 1, 1), AFX_IDW_CONTROLBAR_LAST))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
  if (!m_ViewerTB.LoadToolBar(IDR_VIEWER_TOOLBAR)) {
		TRACE0("Failed to load toolbar\n");
		return -1;      // fail to create
  }
  m_ViewerTB.SetWindowText("View");

  int i;
	CRect rect;

// Zoom combo
	i = m_ViewerTB.CommandToIndex(IDC_CB_ZOOM);
	m_ViewerTB.SetButtonInfo(i, IDC_CB_ZOOM, TBBS_SEPARATOR, 100);
	m_ViewerTB.GetItemRect(i, &rect);

  rect.left += 4;
  rect.right = rect.left + 36;
  rect.top = 4;
  m_ZoomText.Create("Zoom:", WS_VISIBLE, rect, &m_ViewerTB);
	m_ZoomText.SetFont(&m_BaseFont);

  rect.left = rect.right;
  rect.right = rect.left + 56;
	rect.top = 0;
	rect.bottom = rect.top + 300;
	m_cbZoom.Create(CBS_DROPDOWNLIST | WS_VISIBLE | WS_VSCROLL, rect, &m_ViewerTB, IDC_CB_ZOOM);
	m_cbZoom.SetFont(&m_BaseFont);
  m_cbZoom.AddString("400%");
  m_cbZoom.AddString("300%");
  m_cbZoom.AddString("200%");
  m_cbZoom.AddString("150%");
  m_cbZoom.AddString("100%");
  m_cbZoom.AddString("75%");
  m_cbZoom.AddString("50%");
  m_cbZoom.AddString("25%");

// Level combo
	i = m_ViewerTB.CommandToIndex(IDC_CB_LEVEL);
	m_ViewerTB.SetButtonInfo(i, IDC_CB_LEVEL, TBBS_SEPARATOR, 106);
	m_ViewerTB.GetItemRect(i, &rect);

  rect.left += 4;
  rect.right = rect.left + 36;
  rect.top = 4;
  m_LevelText.Create("Level:", WS_VISIBLE, rect, &m_ViewerTB);
	m_LevelText.SetFont(&m_BaseFont);

  rect.left = rect.right;
  rect.right = rect.left + 64;
	rect.top = 0;
	rect.bottom = rect.top + 300;
	m_cbLevel.Create(CBS_DROPDOWNLIST | WS_VISIBLE | WS_VSCROLL, rect, &m_ViewerTB, IDC_CB_LEVEL);
	m_cbLevel.SetFont(&m_BaseFont);

// Intensity slider and text
	i = m_ViewerTB.CommandToIndex(IDC_SLIDER_SHADING);
	m_ViewerTB.SetButtonInfo(i, IDC_SLIDER_SHADING, TBBS_SEPARATOR, 154);
	m_ViewerTB.GetItemRect(i, &rect);

  rect.left += 8;
  rect.top += 4;
  rect.right = rect.left + 44;
  m_IntensityText.Create("Intensity:", WS_VISIBLE, rect, &m_ViewerTB);
	m_IntensityText.SetFont(&m_BaseFont);

  rect.left = rect.right;
  rect.right = rect.left + 100;
  rect.top = 1;
  rect.bottom = rect.top + 20;
  m_ShadingSlider.Create(WS_VISIBLE | TBS_AUTOTICKS, rect, &m_ViewerTB, IDC_SLIDER_SHADING);
  m_ShadingSlider.SetRange(0, 9);

//---------- Editor toolbar ------------

	if (!m_EditorTB.CreateEx(this, m_TBStyle, WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, CRect(1, 1, 1, 1), AFX_IDW_CONTROLBAR_LAST - 1))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
  if (!m_EditorTB.LoadToolBar(IDR_EDITOR_TOOLBAR)) {
		TRACE0("Failed to load toolbar\n");
		return -1;      // fail to create
  }
  m_EditorTB.SetWindowText("Edit");

// Styles combo
	i = m_EditorTB.CommandToIndex(ID_STYLES_COMBO);
	m_EditorTB.SetButtonInfo(i, ID_STYLES_COMBO, TBBS_SEPARATOR, 170);
	m_EditorTB.GetItemRect(i, &rect);

	rect.top = 0;
	rect.bottom = rect.top + 300;
	m_cbStyles.Create(CBS_DROPDOWNLIST | WS_VISIBLE | WS_VSCROLL, rect, &m_EditorTB, ID_STYLES_COMBO);
	m_cbStyles.SetFont(&m_BaseFont);

//---------- Statusbar ------------

	if (!m_wndStatusBar.Create(this) ||	!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
  m_wndStatusBar.SetPaneInfo(0, ID_SEPARATOR, SBPS_STRETCH, 0);
  m_wndStatusBar.SetPaneInfo(1, ID_INDICATOR1, SBPS_NORMAL, 150);
  m_wndStatusBar.SetPaneInfo(2, ID_INDICATOR2, SBPS_NORMAL, 250);

//---------- Toolbars docking ------------

	m_ViewerTB.EnableDocking(CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM);
	m_EditorTB.EnableDocking(CBRS_ALIGN_TOP | CBRS_ALIGN_BOTTOM);
	EnableDocking(CBRS_ALIGN_ANY);

	DockControlBar(&m_ViewerTB);
	DockControlBar(&m_EditorTB);
  CreateMarksTB();

	if (theApp.GetProfileInt(iniOptions, iniVersion, 100) < 301) {
		theApp.WriteProfileInt(iniOptions, iniVersion, 301);
		SaveBarState(iniToolBars);
	}
	else
		LoadBarState(iniToolBars);

  ShowControlBar(&m_EditorTB, FALSE, FALSE);

  theApp.m_map.SetWnd(m_wndView);

  FillStylesCombo();

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0, 0, 0, AfxGetApp()->LoadIcon("MAINICON"));
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers
void CMainFrame::OnSetFocus(CWnd* pOldWnd)
{
	// forward focus to the view window
	m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// let the view have first crack at the command
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// otherwise, do default handling
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CMainFrame::OnSelchangeCbZoom() 
{
  CString s;
  m_cbZoom.GetLBText(m_cbZoom.GetCurSel(), s);
  theApp.m_map.SetZoom(atoi(s));
  m_wndView.Invalidate(FALSE);
  theApp.m_map.ScrollMap(0, 0);
	m_wndView.SetFocus();
}

void CMainFrame::OnSelchangeCbLevel() 
{
  CString s;
  m_cbLevel.GetLBText(m_cbLevel.GetCurSel(), s);
  theApp.m_map.SetLevelStr(s);
  m_wndView.Invalidate(FALSE);
	m_wndView.SetFocus();
}

void CMainFrame::OnSelchangeCbStyle() 
{
  int i, idx;
  BOOL f = FALSE;
  CContentObjects& objs = theApp.m_map.Content.Objects;
  CContentObj *o;

	m_wndView.SetFocus();

  idx = m_cbStyles.GetCurSel();
  for (i = 0; i < objs.GetSize(); i++) {
    o = objs[i];
    if (o->Selected) {
      m_wndView.InvalidateObj(o);
      o->Style = NULL;
      o->StyleIdx = idx;
      o->StyleChanged();
      m_wndView.InvalidateObj(o);
      f = TRUE;
    }
  }
  if (!f) 
    m_wndView.m_DefStyleIdx = idx;
}

void CMainFrame::OnDestroy() 
{
  WINDOWPLACEMENT wp;

  wp.length = sizeof(WINDOWPLACEMENT);
  if (GetWindowPlacement(&wp))
    theApp.WriteProfileBinary(iniOptions, iniWindow, (BYTE*)&wp, wp.length); 

	CFrameWnd::OnDestroy();
}


void CMainFrame::SetMapInfo(int Zoom, int Level)
{
  char buf[10];

  theApp.m_map.SetZoom(Zoom);
  theApp.m_map.SetLevel(Level);
  itoa(Zoom, buf, 10);
  m_cbZoom.SelectString(-1, buf);
  m_cbLevel.SelectString(-1, theApp.m_map.GetLevelStr(Level));
}

void CMainFrame::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CFrameWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CMainFrame::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CFrameWnd::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CMainFrame::OnUpdateViewNavigation(CCmdUI* pCmdUI) 
{
	BOOL bVisible = ((m_ViewerTB.GetStyle() & WS_VISIBLE) != 0);
	pCmdUI->SetCheck(bVisible);
}

void CMainFrame::OnViewNavigation() 
{
	BOOL bVisible = ((m_ViewerTB.GetStyle() & WS_VISIBLE) != 0);

	ShowControlBar(&m_ViewerTB, !bVisible, FALSE);
	RecalcLayout();
}

void CMainFrame::OnCheckOverlay() 
{
  theApp.m_map.Overlay = !theApp.m_map.Overlay;
  m_wndView.Invalidate(FALSE);
	m_wndView.SetFocus();
}

void CMainFrame::OnUpdateCheckOverlay(CCmdUI* pCmdUI) 
{
  BOOL e = theApp.m_map.Overlay;
  pCmdUI->Enable(!theApp.m_map.m_MapRange.IsRectNull());
	pCmdUI->SetCheck(theApp.m_map.Overlay);
  m_ViewerTB.CheckDlgButton(IDC_CHECK_OVERLAY, theApp.m_map.Overlay);
  e = (e && !theApp.m_map.m_MapRange.IsRectNull());
  m_ShadingSlider.EnableWindow(e);
  m_IntensityText.EnableWindow(e);
}

void CMainFrame::OnViewFiltering() 
{
  theApp.m_map.Filtering = !theApp.m_map.Filtering;
  m_wndView.Invalidate(FALSE);
}

void CMainFrame::OnUpdateViewFiltering(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(theApp.m_map.Filtering);
  pCmdUI->Enable(!theApp.m_map.m_MapRange.IsRectNull() && !NoFiltering);
}
void CMainFrame::OnClose() 
{
  if (!CheckModified()) return;

  ShowControlBar(&m_EditorTB, FALSE, FALSE);
  SaveBarState(iniToolBars);

	CFrameWnd::OnClose();
}

void CMainFrame::OnHelpViewreadme() 
{
	CString s = ExtractPath(GetAppFileName()) + "readme.htm";
	int i = (int)ShellExecute(0, "open", s, NULL, "", SW_SHOW);
	if (i <= 32)
		MessageBox("Error opening '" + s + "'\n" + GetErrorMessage(i), theApp.m_pszAppName, MB_ICONERROR);
}

void CMainFrame::OnFileExporttobmp() 
{
  CExportDlg dlg;

  if (dlg.DoModal() != IDOK) return;

  if (!theApp.m_map.ExportToBmpEx(dlg.m_ExportFile, dlg.m_ExportSymbols, dlg.m_ExportZoom)) {
    CString s;
    s.Format("Error while exporting to '%s'", dlg.m_ExportFile);
  	AfxMessageBox(s, MB_ICONERROR);
  }
}

void CMainFrame::SetShading(int ShadingVal)
{
  m_ShadingSlider.SetPos(9 - ShadingVal);
  theApp.m_map.Shading = 9 - m_ShadingSlider.GetPos();
  theApp.m_map.ReInit();
  m_wndView.Invalidate(FALSE);
}

void CMainFrame::OnUpdateFileExporttobmp(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(!theApp.m_map.m_MapRange.IsRectNull() && !m_wndView.m_EditorMode);
}

void CMainFrame::OnSizing(UINT fwSide, LPRECT pRect) 
{
#define MinWidth  520
#define MinHeight 200

  if (pRect->right - pRect->left < MinWidth)
    if (fwSide == WMSZ_RIGHT || fwSide == WMSZ_TOPRIGHT || fwSide == WMSZ_BOTTOMRIGHT)
      pRect->right = pRect->left + MinWidth;
    else
      pRect->left = pRect->right - MinWidth;
  if (pRect->bottom - pRect->top < MinHeight)
    if (fwSide == WMSZ_BOTTOM || fwSide == WMSZ_BOTTOMLEFT || fwSide == WMSZ_BOTTOMRIGHT)
      pRect->bottom = pRect->top + MinHeight;
    else
      pRect->top = pRect->bottom - MinHeight;
	CFrameWnd::OnSizing(fwSide, pRect);
}


void CMainFrame::OnEditorMode() 
{
  if (m_wndView.m_EditorMode) 
    m_wndView.SetViewerMode();
  else
    m_wndView.SetEditorMode();
}


void CMainFrame::OnUpdateEditorMode(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_wndView.m_EditorMode);
  pCmdUI->Enable(!theApp.m_map.IsEmpty());
}

void CMainFrame::FillStylesCombo()
{
  int i, idx;
  CContentStyles &css = theApp.m_map.Content.Styles;

  idx = m_cbStyles.GetCurSel();
  m_cbStyles.ResetContent();
  for (i = 0; i < css.GetSize(); i++) 
    m_cbStyles.AddString(css[i]->GetStyleName());
  m_cbStyles.SetCurSel(idx);
}

BOOL CMainFrame::CheckModified()
{
  BOOL res;
  if (theApp.m_map.Content.Modified) {
    switch (AfxMessageBox("Map symbols was changed.\rDo you want to save it?", MB_YESNOCANCEL | MB_ICONQUESTION, 0)) {
    case IDYES:
      res = theApp.OnSaveDescr();
      break;
    case IDNO:
      res = TRUE;
      break;
    default:
      res = FALSE;
    }
  }

  return res;
}

void CMainFrame::OnMarkcategories() 
{
  CMarksList m(this);	
  m.DoModal();
}

void CMainFrame::OnUpdateMarkcategories(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_wndView.m_EditorMode);
}

void CMainFrame::OnViewHideallsymbols() 
{
  theApp.m_map.Content.DrawOptions ^= cdoHideAll;
  m_wndView.Invalidate(FALSE);	  
}

void CMainFrame::OnUpdateViewHideallsymbols(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!theApp.m_map.IsEmpty());
  pCmdUI->SetCheck((theApp.m_map.Content.DrawOptions & cdoHideAll) != 0);
}

void CMainFrame::CreateMarksTB()
{
  int i;
  CMenu *menu = GetMenu()->GetSubMenu(2)->GetSubMenu(2);

  if (!m_MarksTB.m_hWnd) {
	  m_MarksTB.CreateEx(this, m_TBStyle | TBSTYLE_WRAPABLE, WS_CHILD | CBRS_LEFT | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC, CRect(2, 2, 2, 2), AFX_IDW_CONTROLBAR_LAST - 2);
    m_MarksTB.SetWindowText("Marks");
	  m_MarksTB.EnableDocking(CBRS_ALIGN_ANY);
	  DockControlBar(&m_MarksTB, AFX_IDW_DOCKBAR_LEFT);
  }
  else {
    i = m_MarksTB.GetToolBarCtrl().GetButtonCount();
    while (i > 0) {
      i--;
      m_MarksTB.GetToolBarCtrl().DeleteButton(i);
    }
    UINT id;
    i = 0;
    while (i < (int)menu->GetMenuItemCount()) {
      id = menu->GetMenuItemID(i); 
      if (id == 0 || id >= FirstCatButtonID)
        menu->DeleteMenu(i, MF_BYPOSITION);
      else
        i++;
    }
  }
  CToolBarCtrl &tbc = m_MarksTB.GetToolBarCtrl();
  tbc.SetImageList(NULL);
  m_MarksTB_Images.DeleteImageList();
  m_MarksTB_Images.Create(&theApp.m_map.Content.MarkImages);
  CBitmap bmp;
  bmp.LoadBitmap(IDB_Show);
  m_MarksTB_Images.Add(&bmp, RGB(0, 255, 0));
  bmp.DeleteObject();
  bmp.LoadBitmap(IDB_Hide);
  m_MarksTB_Images.Add(&bmp, RGB(0, 255, 0));
  bmp.DeleteObject();
  tbc.SetImageList(&m_MarksTB_Images);

  TBBUTTON tb;
  CString s;

  tb.fsStyle = TBSTYLE_CHECK;
  tb.dwData = 0;

  i = 0;
  while (i < theApp.m_map.Content.MarkCategories.GetSize()) {
    s = theApp.m_map.Content.MarkCategories[i]->Name + '\0';
    tb.iString = tbc.AddStrings(s);
    tb.iBitmap = i;
    tb.idCommand = FirstCatButtonID + i;
    tb.fsState = (theApp.m_map.Content.MarkCategories[i]->Visible) ? TBSTATE_ENABLED | TBSTATE_CHECKED : TBSTATE_ENABLED;
    tbc.AddButtons(1, &tb);

    menu->InsertMenu(i, MF_BYPOSITION | MF_STRING, tb.idCommand, theApp.m_map.Content.MarkCategories[i]->Name);

    i++;
  }
  if (i > 0) {
    menu->InsertMenu(i, MF_BYPOSITION | MF_SEPARATOR);

    tb.fsState = TBSTATE_ENABLED;
    tb.fsStyle = TBSTYLE_SEP;
    tb.iBitmap = 16;
    tb.idCommand = 0;
    tbc.AddButtons(1, &tb);

    tb.fsStyle = TBSTYLE_BUTTON;
    tb.iBitmap = i;
    tb.idCommand = ID_ShowAllMarks;
    s.LoadString(tb.idCommand);
    tb.iString = tbc.AddStrings(s + '\0');
    tbc.AddButtons(1, &tb);

    i++;
    tb.iBitmap = i;
    tb.idCommand = ID_HideAllMarks;
    s.LoadString(tb.idCommand);
    tb.iString = tbc.AddStrings(s + '\0');
    tbc.AddButtons(1, &tb);
  }

  tbc.SetButtonSize(CSize(MarkWd + 8, MarkHt + 6));

  ShowControlBar(&m_MarksTB, theApp.m_map.Content.MarkCategories.GetSize() > 0 && !theApp.m_map.IsEmpty(), FALSE);
}

BOOL CMainFrame::OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult )
{
    TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;
    UINT nID =pNMHDR->idFrom;

    m_MarksTB.SendMessage(TB_GETBUTTONTEXT, nID, (LPARAM)pTTT->szText);
    pTTT->hinst = 0;
    return TRUE;
}

void CMainFrame::OnToggleCategory(UINT id) 
{
  id -= FirstCatButtonID;
  theApp.m_map.Content.MarkCategories[id]->Visible = !theApp.m_map.Content.MarkCategories[id]->Visible;

  CContentObjects& objs = theApp.m_map.Content.Objects;
  CContentObj *o;
  int i;

  for (i = 0; i < objs.GetSize(); i++) {
    o = objs[i];
    if (o->GetType() == otMark && ((CContentMark*)o)->Category == (int)id) 
      m_wndView.InvalidateObj(o);
  }
}

void CMainFrame::OnUpdateToggleCategory(CCmdUI* pCmdUI) 
{
  int i = pCmdUI->m_nID - FirstCatButtonID;
  BOOL e = i < theApp.m_map.Content.MarkCategories.GetSize();
  pCmdUI->Enable(e);
  if (e)
	  pCmdUI->SetCheck(theApp.m_map.Content.MarkCategories[i]->Visible);
}

void CMainFrame::OnShowAllMarks() 
{
  int i;
  for (i = 0; i < theApp.m_map.Content.MarkCategories.GetSize(); i++)
    theApp.m_map.Content.MarkCategories[i]->Visible = TRUE;
  m_wndView.Invalidate(FALSE);
}

void CMainFrame::OnUpdateShowAllMarks(CCmdUI* pCmdUI) 
{
  int i;
  BOOL e = FALSE;
  for (i = 0; i < theApp.m_map.Content.MarkCategories.GetSize(); i++)
    if (!theApp.m_map.Content.MarkCategories[i]->Visible) {
      e = TRUE;
      break;
    }
  pCmdUI->Enable(e);    
}

void CMainFrame::OnHideAllMarks() 
{
  int i;
  for (i = 0; i < theApp.m_map.Content.MarkCategories.GetSize(); i++)
    theApp.m_map.Content.MarkCategories[i]->Visible = FALSE;
  m_wndView.Invalidate(FALSE);
}

void CMainFrame::OnUpdateHideAllMarks(CCmdUI* pCmdUI) 
{
  int i;
  BOOL e = FALSE;
  for (i = 0; i < theApp.m_map.Content.MarkCategories.GetSize(); i++)
    if (theApp.m_map.Content.MarkCategories[i]->Visible) {
      e = TRUE;
      break;
    }
  pCmdUI->Enable(e);    
}

void CMainFrame::OnViewTextlabels() 
{
	theApp.m_map.HideTextLabels = !theApp.m_map.HideTextLabels;
  theApp.m_map.Content.DrawOptions &= ~cdoHideAll;
  m_wndView.Invalidate(FALSE);
}

void CMainFrame::OnUpdateViewTextlabels(CCmdUI* pCmdUI) 
{
  pCmdUI->SetCheck(!theApp.m_map.HideTextLabels);
  pCmdUI->Enable(!theApp.m_map.IsEmpty());
}

void CMainFrame::OnViewMarks() 
{
	theApp.m_map.HideMarks = !theApp.m_map.HideMarks;
  theApp.m_map.Content.DrawOptions &= ~cdoHideAll;
  m_wndView.Invalidate(FALSE);
}

void CMainFrame::OnUpdateViewMarks(CCmdUI* pCmdUI) 
{
  pCmdUI->SetCheck(!theApp.m_map.HideMarks);
  pCmdUI->Enable(!theApp.m_map.IsEmpty());
}
