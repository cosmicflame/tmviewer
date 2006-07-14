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

// TMViewer.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include <io.h> 
#include "TMViewer.h"
#include "MainFrm.h"
#include "NewSymbols.h"
#include "MergeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// The one and only CTMViewerApp object

CTMViewerApp theApp;
CMainFrame* g_pMainFrame;
CMenu m_Popups;

/////////////////////////////////////////////////////////////////////////////
// CTMViewerApp

BEGIN_MESSAGE_MAP(CTMViewerApp, CWinApp)
	//{{AFX_MSG_MAP(CTMViewerApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_MERGE_MAPS, OnMergeMaps)
	ON_COMMAND(ID_SaveDescr, OnSaveDescr)
	ON_COMMAND(ID_OpenDesc, OnOpenDesc)
	ON_COMMAND(ID_SaveDescAs, OnSaveDescAs)
	ON_UPDATE_COMMAND_UI(ID_SaveDescr, OnUpdateSaveDescr)
	ON_UPDATE_COMMAND_UI(ID_SaveDescAs, OnUpdateSaveDescAs)
	ON_UPDATE_COMMAND_UI(ID_OpenDesc, OnUpdateOpenDesc)
	ON_COMMAND(ID_NewMapDesc, OnNewMapDesc)
	ON_UPDATE_COMMAND_UI(ID_NewMapDesc, OnUpdateNewMapDesc)
	ON_COMMAND(ID_LOADPREVIOUSMAPSYMBOLS, OnLoadpreviousmapsymbols)
	ON_UPDATE_COMMAND_UI(ID_LOADPREVIOUSMAPSYMBOLS, OnUpdateLoadpreviousmapsymbols)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTMViewerApp construction

CTMViewerApp::CTMViewerApp()
{
  m_bStarted = FALSE;
}

CTMViewerApp::~CTMViewerApp()
{
  WriteProfileInt(iniOptions, iniPosX, m_map.PosX);
  WriteProfileInt(iniOptions, iniPosY, m_map.PosY);
  WriteProfileInt(iniOptions, iniZoom, m_map.m_iZoom);
  WriteProfileInt(iniOptions, iniLevel, m_map.m_iLevel);
  WriteProfileInt(iniOptions, iniFiltering, m_map.Filtering);
  WriteProfileInt(iniOptions, iniOverlayMode, m_map.Overlay);
  WriteProfileInt(iniOptions, iniShading, m_map.Shading);
  WriteProfileString(iniOptions, iniLastDescFile, m_map.Content.FileName);
}
/////////////////////////////////////////////////////////////////////////////
// CTMViewerApp initialization

BOOL CTMViewerApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

  free((void*)m_pszProfileName);
  //Change the name of the .INI file.
  //The CWinApp destructor will free the memory.

  char buf[MAX_PATH];
  GetModuleFileName(0, buf, MAX_PATH);
  char *c = strrchr(buf, '.');
  strcpy(c + 1, "ini");
  m_pszProfileName=_tcsdup(_T(buf));

	// To create the main window, this code creates a new frame window
	// object and then sets it as the application's main window object.

	g_pMainFrame = new CMainFrame;
	m_pMainWnd = g_pMainFrame;
  
	// create and load the frame with its resources

	g_pMainFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
		NULL);

  LoadStdProfileSettings(4);

  WINDOWPLACEMENT *wp;
  UINT b;

  if (GetProfileBinary(iniOptions, iniWindow, (BYTE**)&wp, &b)) {
    g_pMainFrame->SetWindowPlacement(wp);
    delete wp;
  }
	// The one and only window has been initialized, so show and update it.
	g_pMainFrame->ShowWindow(SW_SHOW);
	g_pMainFrame->UpdateWindow();

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CTMViewerApp message handlers


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	afx_msg void OnEmailClick();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg void OnFtpClick();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CFont m_UrlFont;
	CFont m_TitleFont;
  HCURSOR m_HandCur;
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
  m_HandCur = theApp.LoadCursor(IDC_CURHAND);
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_BN_CLICKED(IDC_EMAIL, OnEmailClick)
	ON_WM_SETCURSOR()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_FTP, OnFtpClick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CTMViewerApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

int CALLBACK SelFolderCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
  if (uMsg == BFFM_INITIALIZED) {
    SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
  }
  return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CTMViewerApp message handlers

void CTMViewerApp::OnFileOpen() 
{
  if (!g_pMainFrame->CheckModified()) return;
  
  BROWSEINFO bi;
  char dir[MAX_PATH];
  LPITEMIDLIST lst;

  ZeroMemory(&bi, sizeof(BROWSEINFO));
  bi.hwndOwner = g_pMainFrame->m_hWnd;
  bi.pszDisplayName = dir;
  bi.lpszTitle = "Select where Tibia map files are located:";
  bi.lpfn = SelFolderCallbackProc;
  bi.lParam = (long)m_map.m_sMapsPath.LockBuffer();
//  bi.ulFlags = BIF_EDITBOX;
  lst = SHBrowseForFolder(&bi);
  m_map.m_sMapsPath.UnlockBuffer();
  if (lst) {
    SHGetPathFromIDList(lst, dir);
    m_map.SetMapsPath(dir);
    g_pMainFrame->SetMapInfo(100, 0);
    WriteProfileString(iniOptions, iniMapsPath, m_map.m_sMapsPath);
    m_map.Content.FileName.Empty();
    OpenMaps(TRUE);
  }
}

BOOL CTMViewerApp::OnIdle(LONG lCount) 
{
  if (!m_bStarted) {
    g_pMainFrame->m_wndView.SetFocus();

    char buf[MAX_PATH];

    GetCurrentDirectory(MAX_PATH, buf);
    m_map.SetMapsPath(GetProfileString(iniOptions, iniMapsPath, buf));
    m_map.PosX = GetProfileInt(iniOptions, iniPosX, m_map.PosX);
    m_map.PosY = GetProfileInt(iniOptions, iniPosY, m_map.PosY);
    if (g_pMainFrame->NoFiltering)
      m_map.Filtering = FALSE;
    else
      m_map.Filtering = GetProfileInt(iniOptions, iniFiltering, m_map.Filtering);
    m_map.Overlay = GetProfileInt(iniOptions, iniOverlayMode, m_map.Overlay);
    g_pMainFrame->SetShading(GetProfileInt(iniOptions, iniShading, m_map.Shading));
    g_pMainFrame->SetMapInfo(GetProfileInt(iniOptions, iniZoom, 100), GetProfileInt(iniOptions, iniLevel, 0));
    m_map.Content.FileName = GetProfileString(iniOptions, iniLastDescFile, "");
    OpenMaps();
    m_bStarted = TRUE;
	  if (m_map.m_MapRange.IsRectNull())
			g_pMainFrame->PostMessage(WM_COMMAND, ID_FILE_OPEN);
  }

	return CWinApp::OnIdle(lCount);
}

void CTMViewerApp::OpenMaps(BOOL Centered)
{
  const char* DefDescFile = "default.tmv";

  int i;
  BOOL e;
  CString s;

  m_map.CalcMapRange();
  e = !m_map.IsEmpty();
  if (e && Centered) {
    CRect r;

    m_map.m_wnd->GetClientRect(&r);
    m_map.PosX = m_map.m_MapRange.left + (m_map.m_MapRange.Width() - r.Width()) / 2;
    m_map.PosY = m_map.m_MapRange.top + (m_map.m_MapRange.Height() - r.Height()) / 2;
  }
  m_map.ScrollMap(0, 0);
  g_pMainFrame->m_cbLevel.EnableWindow(e);
  g_pMainFrame->m_cbZoom.EnableWindow(e);
  g_pMainFrame->m_IntensityText.EnableWindow(e);
  g_pMainFrame->m_ShadingSlider.EnableWindow(e);
  if (!e) {
    g_pMainFrame->m_wndView.SetViewerMode();
  }
  else {
    if (g_pMainFrame->m_wndView.m_EditorMode)
      g_pMainFrame->m_wndView.SetEditorMode();
    else
      g_pMainFrame->m_wndView.SetViewerMode();
    g_pMainFrame->m_cbLevel.EnableWindow();
    g_pMainFrame->m_cbZoom.EnableWindow();

// Filling levels combo

    g_pMainFrame->m_cbLevel.ResetContent();

    for (i = m_map.m_iMaxLevel; i >= m_map.m_iMinLevel; i--) 
      g_pMainFrame->m_cbLevel.AddString(m_map.GetLevelStr(i));

    s = m_map.Content.FileName;
    if (s.IsEmpty() || access(s, 0) != 0) {
      s = m_map.m_sMapsPath + DefDescFile;
      if (access(s, 0) != 0) {
        s = ExtractPath(GetAppFileName()) + DefDescFile;
        if (access(s, 0) != 0) 
          s.Empty();
      }
    }
    if (!s.IsEmpty()) {
      if (m_map.Content.LoadFromFile(s)) {
        g_pMainFrame->FillStylesCombo();
        AddToRecentFileList(m_map.Content.FileName);
      }
      else
        ClearMapDesc();
    }
    else 
      ClearMapDesc();
  }

  g_pMainFrame->SetMapInfo(m_map.m_iZoom, m_map.m_iLevel);
  g_pMainFrame->m_wndView.Invalidate(FALSE);
  s = m_pszAppName;
  if (!m_map.m_sMapsPath.IsEmpty())
    s += " - (" + m_map.m_sMapsPath + ")";
  g_pMainFrame->SetWindowText(s);
  g_pMainFrame->CreateMarksTB();
}

void CAboutDlg::OnEmailClick() 
{
  CString s;
  GetDlgItem(IDC_EMAIL)->GetWindowText(s);
	int i = (int)ShellExecute(0, "open", CString("mailto:") + s + "?subject=Tibia Map Viewer", NULL, NULL, SW_SHOW);
	if (i <= 32)
		MessageBox(GetErrorMessage(i), theApp.m_pszAppName, MB_ICONERROR);
}


BOOL CAboutDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
  switch(pWnd->GetDlgCtrlID()) {
    case IDC_EMAIL:
    case IDC_FTP:
  	  ::SetCursor(m_HandCur);
	    return TRUE;
	}
  return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

HBRUSH CAboutDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

  switch(pWnd->GetDlgCtrlID()) {
    case IDC_EMAIL:
    case IDC_FTP:
      pDC->SetTextColor(RGB(0,0,255));
	}
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	LOGFONT lf;

  GetFont()->GetLogFont(&lf);
  lf.lfUnderline = TRUE;
  m_UrlFont.CreateFontIndirect(&lf);
  GetDlgItem(IDC_EMAIL)->SetFont(&m_UrlFont);
  GetDlgItem(IDC_FTP)->SetFont(&m_UrlFont);
  lf.lfUnderline = FALSE;
  lf.lfWeight = FW_BOLD;
  m_TitleFont.CreateFontIndirect(&lf);
  GetDlgItem(IDC_VERSION)->SetFont(&m_TitleFont);

//------------------------------------------------------------------
  DWORD i, sz;
  char fn[MAX_PATH];
//  VS_FIXEDFILEINFO *info;
  void *info;
  void *buf;
  CString s, ss, s3;

  GetModuleFileName(0, fn, MAX_PATH);

  sz = GetFileVersionInfoSize(fn, &i);
  if (sz) {
    buf = malloc(sz);
    if (!GetFileVersionInfo(fn, 0, sz, buf)) goto gfv1;
    if (!VerQueryValue(buf, "\\StringFileInfo\\040904b0\\ProductVersion", &info, (PUINT)&i)) goto gfv1;

    GetDlgItem(IDC_VERSION)->GetWindowText(s);
    memcpy(s3.GetBuffer(i), info, i);
    s3.ReleaseBuffer(i);
    ss.Format(s, s3);
//    ss.Format(s, HIWORD(info->dwFileVersionMS), LOWORD(info->dwFileVersionMS));
    GetDlgItem(IDC_VERSION)->SetWindowText(ss);

gfv1:
    free(buf);
  }

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAboutDlg::OnFtpClick() 
{
  CString s;
  GetDlgItem(IDC_FTP)->GetWindowText(s);
	int i = (int)ShellExecute(0, "open", s, NULL, NULL, SW_SHOW);
	if (i <= 32)
		MessageBox(GetErrorMessage(i), theApp.m_pszAppName, MB_ICONERROR);
}

void CTMViewerApp::OnMergeMaps() 
{
  CMergeDlg dlg;
  dlg.DoModal();

/*
  BROWSEINFO bi;
  char dir[MAX_PATH];
  LPITEMIDLIST lst;

  ZeroMemory(&bi, sizeof(BROWSEINFO));
  bi.hwndOwner = g_pMainFrame->m_hWnd;
  bi.pszDisplayName = dir;
  bi.lpszTitle = "Select maps folder to merge with:";
  bi.lParam = (long)m_map.m_sMapsPath.LockBuffer();
  lst = SHBrowseForFolder(&bi);
  m_map.m_sMapsPath.UnlockBuffer();
  if (lst) {
    CMapper mm;
    CString s;
    int x, y, lv, i, cnt;
    SHGetPathFromIDList(lst, dir);
    mm.SetMapsPath(dir);
    mm.CalcMapRange();
    if (mm.IsEmpty()) {
      s.Format("There are no maps was found in '%s'", dir);
      AfxMessageBox(s, MB_ICONERROR, 0);
      return;
    }
    if (mm.m_MapVersion != m_map.m_MapVersion) {
      AfxMessageBox("Can not merge maps from different versions of Tibia client.", MB_ICONERROR, 0);
      return;
    }

    CWaitCursor wc;
    BYTE *src, *dst, b;
    BOOL msk, DoSave;

    cnt = 0;
    for (y = mm.m_MapRange.top / mm.m_MapHt; y <= mm.m_MapRange.bottom / mm.m_MapHt; y++) 
      for (x = mm.m_MapRange.left / mm.m_MapWd; x <= mm.m_MapRange.right / mm.m_MapWd; x++) 
        for (lv = mm.m_iMinLevel; lv <= mm.m_iMaxLevel; lv++) 
          if (mm.LoadMap(x, y, lv)) {
            i = m_map.m_MapWd*m_map.m_MapHt;
            src = (BYTE*)mm.m_MapBuf;
            dst = (BYTE*)m_map.m_MapBuf;
            if (!m_map.LoadMap(x, y, lv)) {
              ZeroMemory(dst, i);
              if (m_map.m_MapVersion == 2)
                memset(dst + i, 0xFA, i);
            }
            DoSave = FALSE;

            do {
              i--;
              if ((i / m_map.m_MapWd) & 1) 
                msk = (i & 1);
              else
                msk = !(i & 1);
              b = (msk || *src == 0xDC) ? *src : 0;   

              if (*dst != b && (*dst == 0 || *dst == 0xA)) {
                *dst = b;
                DoSave = TRUE;
              }
              src++;
              dst++;
            } 
            while (i > 0);

            if (m_map.m_MapVersion == 2) {
              i = m_map.m_MapWd*m_map.m_MapHt;
              do {
                if (*dst != *src && *dst == 0xFA) {
                  *dst = *src;
                  DoSave = TRUE;
                }
                src++;
                dst++;
                i--;
              } 
              while (i > 0);
            }

            if (DoSave) {
              if (!m_map.SaveMapAs(x, y, lv)) {
                s.Format("Error saving map file %s%s", m_map.m_sMapsPath, m_map.GetMapFileName(x, y, lv));
                AfxMessageBox(s, MB_ICONERROR, 0);
                return;
              };
              cnt++;
            }
          }
    OpenMaps();
    if (cnt)
      s.Format("%d map files has been merged.", cnt);
    else
      s = "No map files has been merged.\rIt's seems that the source and destination maps are identical.";
    AfxMessageBox(s, MB_ICONINFORMATION, 0);
  }
*/
}

BOOL CTMViewerApp::OnSaveDescr() 
{
  if (m_map.Content.FileName.IsEmpty())
    return OnSaveDescAs();
  else
    return m_map.Content.SaveToFile(m_map.Content.FileName);	
}

void CTMViewerApp::OnUpdateSaveDescr(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(!m_map.IsEmpty() && m_map.Content.Modified);
}

void CTMViewerApp::OnOpenDesc() 
{
  if (!g_pMainFrame->CheckModified()) return;
  CFileDialog dlg(TRUE, ".tmv", NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_EXPLORER | OFN_ENABLESIZING, 
                  "Map symbols files (*.tmv)|*.tmv|All Files (*.*)|*.*||", g_pMainFrame);
  dlg.m_ofn.lpstrTitle = "Load map symbols";

  if (dlg.DoModal() == IDOK) {
    if (m_map.Content.LoadFromFile(dlg.GetPathName())) {
      g_pMainFrame->FillStylesCombo();
      g_pMainFrame->CreateMarksTB();
      AddToRecentFileList(m_map.Content.FileName);
    }
    else
      ClearMapDesc();
    g_pMainFrame->m_wndView.Invalidate(FALSE);
  }
}

void CTMViewerApp::OnUpdateOpenDesc(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(!m_map.IsEmpty());
}
BOOL CTMViewerApp::OnSaveDescAs() 
{
  CString s;
  if (m_map.Content.FileName.IsEmpty())
    s = "Untitled.tmv";
  else
    s = m_map.Content.FileName;
  CFileDialog dlg(FALSE, ".tmv", s, OFN_PATHMUSTEXIST | OFN_EXPLORER | OFN_ENABLESIZING | OFN_OVERWRITEPROMPT, 
                  "Map symbols files (*.tmv)|*.tmv|All Files (*.*)|*.*||", g_pMainFrame);
  
  dlg.m_ofn.lpstrTitle = "Save map symbols as";

  if (dlg.DoModal() == IDOK) {
    return m_map.Content.SaveToFile(dlg.GetPathName());
  }
  else
    return FALSE;
}

void CTMViewerApp::OnUpdateSaveDescAs(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(!m_map.IsEmpty() && g_pMainFrame->m_wndView.m_EditorMode);
}

void CTMViewerApp::OnNewMapDesc() 
{
  if (!g_pMainFrame->CheckModified()) return;

  CNewSymbols ns;

  if (ns.DoModal() != IDOK) return;

  m_map.Content.Objects.RemoveAll();
  if (ns.m_ClearStyles) {
    m_map.Content.Styles.RemoveAll();
    CContentStyle *cs = new CContentStyle(&m_map.Content);
    *cs = m_map.Content.DefStyle;
    cs->SetZoom(m_map.m_iZoom);
    m_map.Content.Styles.Add(cs);
    g_pMainFrame->FillStylesCombo();
  }
  if (ns.m_ClearCats) {
    m_map.Content.MarkCategories.RemoveAll();
    m_map.Content.MarkImages.SetImageCount(0);
    g_pMainFrame->CreateMarksTB();
  }

  m_map.Content.FileName.Empty();
  g_pMainFrame->m_wndView.Invalidate(FALSE);
}

void CTMViewerApp::OnUpdateNewMapDesc(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(!m_map.IsEmpty() && g_pMainFrame->m_wndView.m_EditorMode);
}


CDocument* CTMViewerApp::OpenDocumentFile(LPCTSTR lpszFileName) 
{
  if (g_pMainFrame->CheckModified()) {
    if (m_map.Content.LoadFromFile(lpszFileName)) {
      g_pMainFrame->FillStylesCombo();
      g_pMainFrame->CreateMarksTB();
      AddToRecentFileList(m_map.Content.FileName);
    }
    else
      ClearMapDesc();
    g_pMainFrame->m_wndView.Invalidate(FALSE);
  }
	return (CDocument*)1;
}

void CTMViewerApp::ClearMapDesc()
{
  m_map.Content.Clear();    
  m_map.Content.FileName.Empty();
  g_pMainFrame->CreateMarksTB();
  CContentStyle *cs = new CContentStyle(&m_map.Content);
  *cs = m_map.Content.DefStyle;
  cs->SetZoom(m_map.m_iZoom);
  m_map.Content.Styles.Add(cs);
  g_pMainFrame->FillStylesCombo();
  g_pMainFrame->m_wndView.Invalidate(FALSE);
}

void CTMViewerApp::OnLoadpreviousmapsymbols() 
{
  g_pMainFrame->SendMessage(WM_COMMAND, ID_FILE_MRU_FILE2);
}

void CTMViewerApp::OnUpdateLoadpreviousmapsymbols(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable((*m_pRecentFileList)[1].GetLength() != 0 && !m_map.IsEmpty());
}
