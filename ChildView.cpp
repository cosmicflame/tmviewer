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

#include "stdafx.h"
#include "TMViewer.h"
#include "ChildView.h"
#include "TextProps.h"
#include "StylesListDlg.h"
#include "NewMarkDlg.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CChildView

CChildView::CChildView() : m_EditorMode(FALSE), m_NewObjectType(0), m_ObjSelCount(0),
    m_InPopup(FALSE)
{
  m_ptClick.x = -1;
  m_MagnifyCur = theApp.LoadCursor(IDC_CURMAGN);
  m_MagnifyCur2 = theApp.LoadCursor(IDC_CURMAGN2);
  m_DragHand2Cur = theApp.LoadCursor(IDC_CURDRAGHAND2);
  m_DefStyleIdx = 0;
  m_LastZoomIdx = -1;
  m_CurHideUnexplored = TRUE;
}

CChildView::~CChildView()
{
}

BEGIN_MESSAGE_MAP(CChildView,CWnd )
	//{{AFX_MSG_MAP(CChildView)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_SIZE()
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_KILLFOCUS()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_DELETE, OnDelete)
	ON_COMMAND(ID_INSERT_TEXT, OnInsertText)
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_SelectMode, OnSelectMode)
	ON_UPDATE_COMMAND_UI(ID_INSERT_TEXT, OnUpdateInsertText)
	ON_UPDATE_COMMAND_UI(ID_SelectMode, OnUpdateSelectMode)
	ON_COMMAND(ID_EditStyles, OnEditStyles)
	ON_UPDATE_COMMAND_UI(ID_EditStyles, OnUpdateEditStyles)
	ON_UPDATE_COMMAND_UI(ID_DELETE, OnUpdateDelete)
	ON_COMMAND(ID_InsertMark, OnInsertMark)
	ON_UPDATE_COMMAND_UI(ID_InsertMark, OnUpdateInsertMark)
	ON_WM_SETCURSOR()
	ON_WM_RBUTTONDBLCLK()
	ON_COMMAND(ID_HIDEUNEXPLOREDSYMBOLS, OnHideunexploredsymbols)
	ON_UPDATE_COMMAND_UI(ID_HIDEUNEXPLOREDSYMBOLS, OnUpdateHideunexploredsymbols)
	ON_COMMAND(ID_PROPERTIES, OnProperties)
	ON_UPDATE_COMMAND_UI(ID_PROPERTIES, OnUpdateProperties)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_ToFront, OnToFront)
	ON_UPDATE_COMMAND_UI(ID_ToFront, OnUpdateToFront)
	ON_COMMAND(ID_ToBack, OnToBack)
	ON_UPDATE_COMMAND_UI(ID_ToBack, OnUpdateToBack)
	//}}AFX_MSG_MAP
	ON_UPDATE_COMMAND_UI(ID_INDICATOR1, OnUpdateStatusBar)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR2, OnUpdateStatusBarFileName)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
  cs.style |= WS_HSCROLL | WS_VSCROLL;
	cs.lpszClass = AfxRegisterWndClass(CS_DBLCLKS, //CS_HREDRAW|CS_VREDRAW|
		0, 0, NULL);

	return TRUE;
}

void CChildView::OnPaint() 
{
  CPaintDC *dc;
  CDC *pdc;
  CRgn ur;
  ur.CreateRectRgn(0, 0, 0, 0);
  GetUpdateRgn(&ur);

  CRect cr;
  GetClientRect(&cr);
  if (theApp.m_bStarted && !theApp.m_map.IsEmpty()) {
//    pdc = &dc;
    m_MemDC.SelectClipRgn(&ur);
    pdc = &m_MemDC;

    theApp.m_map.DrawMap(*pdc, &cr, &ur);

    CRect r;
//    r = cr;
    ur.GetRgnBox(r);
//    r.InflateRect(1, 1);
    if (r.left < 0)
      r.left = 0;
    if (r.top < 0)
      r.top = 0;
  	dc = new CPaintDC(this); // device context for painting
    dc->BitBlt(r.left, r.top, r.Width(), r.Height(), &m_MemDC, r.left, r.top, SRCCOPY);
  }
  else {
  	dc = new CPaintDC(this); // device context for painting
    if (theApp.m_bStarted) {
      dc->SetTextAlign(TA_CENTER);
      CPoint cp = cr.CenterPoint();
      CString s;
      s.Format("There are no maps in '%s'", theApp.m_map.m_sMapsPath);
      dc->ExtTextOut(cp.x, cp.y - dc->GetOutputTextExtent("W").cy/2, ETO_OPAQUE, &cr, s, NULL);
    }
    else {
      dc->FillSolidRect(cr, 0);
    }
  }
  delete dc;
}


void CChildView::OnLButtonDown(UINT nFlags, CPoint point) 
{
  int i;
  CPoint pt;
  CContentObjects& objs = theApp.m_map.Content.Objects;
  CContentObj *o2;
  BOOL ShiftPressed = nFlags & MK_SHIFT;

  if (theApp.m_map.IsEmpty() || m_InPopup) goto mend;

  SetFocus();

  i = m_NewObjectType;
  m_NewObjectType = 0;
  if (i)
    SelectAll(FALSE);
  switch (i) {
  case otTextLabel:
    SetArrowCursor();
    CTextProps *tp;
    tp = new CTextProps(this);
    if (tp->DoModal() == IDOK) {
      CContentText *ct = new CContentText(theApp.m_map.Content);
      ct->StyleIdx = m_DefStyleIdx;
      ct->SetText(tp->m_EditText);
      pt = ScreenToMap(point);
//      ct->SetPosition(pt.x, pt.y);
      ct->SetPosition(pt.x - ct->BoundsRect.Width()/2, pt.y - ct->BoundsRect.Height()/2);
      ct->Plane = theApp.m_map.m_iLevel;
      ct->Selected = TRUE;
      m_ObjSelCount++;
      theApp.m_map.Content.Objects.Add(ct);
      InvalidateObj(ct);
      theApp.m_map.Content.Modified = TRUE;
    }
    return;
  case otMark:
    SetArrowCursor();
    CNewMarkDlg dlg(this);
    if (dlg.DoModal() == IDOK) {
      CContentMark *cm = new CContentMark(theApp.m_map.Content);
      cm->Category = dlg.m_CategoryIdx;
      cm->ShortText = dlg.m_ShortText;
      cm->LongText = dlg.m_LongText;
      pt = ScreenToMap(point);
      cm->SetPosition(pt.x - cm->BoundsRect.Width()/2, pt.y - cm->BoundsRect.Height()/2);
      cm->Plane = theApp.m_map.m_iLevel;
      cm->Selected = TRUE;
      m_ObjSelCount++;
      theApp.m_map.Content.Objects.Add(cm);
      InvalidateObj(cm);
      theApp.m_map.Content.Modified = TRUE;
    }
    return;
  }

  pt = ScreenToMap(point);
  o2 = theApp.m_map.Content.GetObjectAt(pt, theApp.m_map.m_iLevel);

  if (!m_EditorMode) {
    if (ShiftPressed) {
      i = g_pMainFrame->m_cbZoom.GetCurSel();
      if (nFlags & MK_CONTROL) {
        m_LastZoomIdx = i;
        i = 1;
        keybd_event('~', 0, 0, 0);  // Disables kbd layout switching by Ctrl+Shift
      }
      else 
        m_LastZoomIdx = -1;
      if (i > 0) {
        ChangeZoom(point, i - 1);
      }
    }
    else {
      if (o2 && o2->GetType() == otMark && !(theApp.m_map.Content.DrawOptions & cdoHideAll)) {
        BYTE dm = ((CContentMark*)o2)->DisplayMode;
        HideMarkDescriptions();
        if (dm != dmExpanded) {
          ((CContentMark*)o2)->DisplayMode = dmExpanded;
          o2->Selected = FALSE;
          InvalidateObj(o2);
        }
      }
      else {
        m_Moving = FALSE;
        m_ptClick = point;
        SetViewCursor(m_DragHand2Cur);
        SetCapture();
      }
    }
  }
  else {
    SelClick(point); 
    m_ptClick = point;
    SetCapture();
    m_Moving = FALSE;
    if (!o2) {
      m_Selection.left = m_Selection.right = point.x;
      m_Selection.top = m_Selection.bottom = point.y;
    }
    else 
      m_Selection.SetRectEmpty();
  }
mend:  
	CWnd ::OnLButtonDown(nFlags, point);
}

void CChildView::OnMouseMove(UINT nFlags, CPoint point) 
{
  if (m_ptClick.x != -1) {
    CPoint d;
    d.x = (m_ptClick.x - point.x)*100/theApp.m_map.m_iZoom;
    d.y = (m_ptClick.y - point.y)*100/theApp.m_map.m_iZoom;

    if (!m_EditorMode) {
      theApp.m_map.ScrollMap(d.x, d.y);
      m_Moving = TRUE;
    }
    else {
      if (!m_Selection.IsRectNull()) {
        CDC *dc;
        dc = GetDC();
        DrawSelectionRect(*dc);
        m_Selection.right = point.x;
        m_Selection.bottom = point.y;
        DrawSelectionRect(*dc);
        ReleaseDC(dc);
        goto mend;
      }

      if (!m_Moving && abs(m_ptClick.x - point.x) < 4 && abs(m_ptClick.y - point.y) < 4) goto mend;

      m_Moving = TRUE;
      CContentObjects& objs = theApp.m_map.Content.Objects;
      CContentObj* o;
      int dx2, dy2;
      CRect r;
      CPoint pt;

      for (int i = 0; i < objs.GetSize(); i++) {
        o = objs[i];
        if (o->Selected) {
          InvalidateObj(o);
          o->BoundsRect.OffsetRect(-d.x, -d.y);
          GetClientRect(&r);
          pt = ScreenToMap(CPoint(r.right, r.bottom));

          if (o->BoundsRect.left < theApp.m_map.PosX)
            dx2 = o->BoundsRect.left - theApp.m_map.PosX;
          else
          if (o->BoundsRect.right > pt.x)
            dx2 = o->BoundsRect.right - pt.x;
          else
            dx2 = 0;

          if (o->BoundsRect.top < theApp.m_map.PosY)
            dy2 = o->BoundsRect.top - theApp.m_map.PosY;
          else
          if (o->BoundsRect.bottom > pt.y)
            dy2 = o->BoundsRect.bottom - pt.y;
          else
            dy2 = 0;

          if (dx2 || dy2)
            theApp.m_map.ScrollMap(dx2, dy2);
          theApp.m_map.Content.Modified = TRUE;
          InvalidateObj(o);
        }
      }
    }

    if (theApp.m_map.m_iZoom < 100) {
      if (d.x)
        m_ptClick.x = point.x;
      if (d.y)
        m_ptClick.y = point.y;
/*
      m_ptClick.x -= d.x*theApp.m_map.m_iZoom/100;
      m_ptClick.y -= d.y*theApp.m_map.m_iZoom/100;
*/
    }
    else {
      m_ptClick.x -= d.x*theApp.m_map.m_iZoom/100;
      m_ptClick.y -= d.y*theApp.m_map.m_iZoom/100;
    }
  }

mend:
  CWnd ::OnMouseMove(nFlags, point);
}

void CChildView::OnLButtonUp(UINT nFlags, CPoint point) 
{
  if (m_ptClick.x != -1) {
    if (!m_Moving)
      HideMarkDescriptions();
    m_ptClick.x = -1;
    ReleaseCapture();
    SetViewCursor(m_DefCur);

    if (!m_Selection.IsRectNull()) {
      CDC *dc;
      dc = GetDC();
      DrawSelectionRect(*dc);
      ReleaseDC(dc);

      CContentObjects& objs = theApp.m_map.Content.Objects;
      CContentObj* o;
      CRect r;
      m_Selection.NormalizeRect();
      ScreenToMapRect(&m_Selection);
      for (int i = 0; i < objs.GetSize(); i++) {
        o = objs[i];
        if (!o->Selected && o->Plane == theApp.m_map.m_iLevel && r.IntersectRect(o->BoundsRect, m_Selection) && o->IsVisible() == ovVisible) 
          if (theApp.m_map.Content.IsVisibleUser(o, o->BoundsRect)) {
            o->Selected = TRUE;
            InvalidateObj(o);
            m_ObjSelCount++;
          }
      }

      m_Selection.SetRectEmpty();
      UpdateCurrentStyle();
    }
  }
	CWnd ::OnLButtonUp(nFlags, point);
}

void CChildView::OnSize(UINT nType, int cx, int cy) 
{
	CWnd ::OnSize(nType, cx, cy);
	
  CRect r;

  GetClientRect(r);

  if (m_MemDC.m_hDC)
    m_MemDC.DeleteDC();
  if (m_MemBmp.m_hObject)
    m_MemBmp.DeleteObject();
  m_MemDC.CreateCompatibleDC(GetDC());
  m_MemBmp.CreateCompatibleBitmap(GetDC(), r.right, r.bottom);
  m_MemDC.SelectObject(&m_MemBmp);
  theApp.m_map.ScrollMap(0, 0);
  Invalidate(FALSE);
}

void CChildView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
  int i;

  switch (nSBCode) {
  case SB_THUMBPOSITION:
  case SB_THUMBTRACK:
    SCROLLINFO si;
    si.cbSize = sizeof(SCROLLINFO);
    GetScrollInfo(SB_VERT, &si, SIF_TRACKPOS);
    i = si.nTrackPos - theApp.m_map.PosY;
    break;
  case SB_LINEUP:
    i = -10;
    break;
  case SB_LINEDOWN:
    i = 10;
    break;
  case SB_PAGEDOWN:
    i = theApp.m_map.m_MapHt;
    break;
  case SB_PAGEUP:
    i = -theApp.m_map.m_MapHt;
    break;
  default:
    i = 0;
  }
  if (i != 0)
	  theApp.m_map.ScrollMap(0, i);

	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}

void CChildView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
  int i;

  switch (nSBCode) {
  case SB_THUMBPOSITION:
  case SB_THUMBTRACK:
    SCROLLINFO si;
    si.cbSize = sizeof(SCROLLINFO);
    GetScrollInfo(SB_HORZ, &si, SIF_TRACKPOS);
    i = si.nTrackPos - theApp.m_map.PosX;
    break;
  case SB_LINEUP:
    i = -10;
    break;
  case SB_LINEDOWN:
    i = 10;
    break;
  case SB_PAGEDOWN:
    i = theApp.m_map.m_MapWd;
    break;
  case SB_PAGEUP:
    i = -theApp.m_map.m_MapWd;
    break;
  default:
    i = 0;
  }
  if (i != 0)
	  theApp.m_map.ScrollMap(i, 0);

	CWnd ::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CChildView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
  int i;
	CPoint point;
	CRect r;

  if (theApp.m_map.IsEmpty()) goto mend;

  BOOL CtrlPressed;
  CtrlPressed = (GetAsyncKeyState(VK_CONTROL) & 0x8000);
  UpdateCursor();

	switch(nChar) {
    case VK_ADD:
    case VK_SUBTRACT:
    case VK_OEM_PLUS:
    case VK_OEM_MINUS:
      if (!CtrlPressed) {
	      GetClientRect(&r);
	      point.x = r.Width()/2;
	      point.y = r.Height()/2;
        i = g_pMainFrame->m_cbZoom.GetCurSel();
      }
      break;
  }
	switch(nChar) {
		case VK_UP:
			theApp.m_map.ScrollMap(0, -32);
			break;
		case VK_DOWN:
			theApp.m_map.ScrollMap(0, 32);
			break;
		case VK_LEFT:
			theApp.m_map.ScrollMap(-32, 0);
			break;
		case VK_RIGHT:
			theApp.m_map.ScrollMap(32, 0);
			break;
		case VK_NEXT:
			if (theApp.m_map.m_iLevel > theApp.m_map.m_iMinLevel) {
        SelectAll(FALSE);
				theApp.m_map.m_iLevel--;
				Invalidate(FALSE);
			  g_pMainFrame->m_cbLevel.SelectString(-1, theApp.m_map.GetLevelStr(theApp.m_map.m_iLevel));
			}
			break;
		case VK_PRIOR:
			if (theApp.m_map.m_iLevel < theApp.m_map.m_iMaxLevel) {
        SelectAll(FALSE);
				theApp.m_map.m_iLevel++;
				Invalidate(FALSE);
			  g_pMainFrame->m_cbLevel.SelectString(-1, theApp.m_map.GetLevelStr(theApp.m_map.m_iLevel));
			}
			break;
    case VK_F9:
      if (CtrlPressed) {
        theApp.m_map.m_ShowSquares = !theApp.m_map.m_ShowSquares;
		    Invalidate(FALSE);
      }
      break;
    case VK_ADD:
    case VK_OEM_PLUS:
      if (CtrlPressed) {
        if (theApp.m_map.Overlay)
          g_pMainFrame->SetShading(theApp.m_map.Shading - 1);
      }
      else
        if (i > 0) {
          ChangeZoom(point, i - 1);
          m_LastZoomIdx = -1;
        }
      break;
    case VK_SUBTRACT:
    case VK_OEM_MINUS:
      if (CtrlPressed) {
        if (theApp.m_map.Overlay)
          g_pMainFrame->SetShading(theApp.m_map.Shading + 1);
      }
      else
        if (i < g_pMainFrame->m_cbZoom.GetCount() - 1) {
          ChangeZoom(point, i + 1);
          m_LastZoomIdx = -1;
        }
      break;
    case VK_ESCAPE:
      if (m_EditorMode)
        OnSelectMode();
      break;
	}
mend:	
	CWnd ::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CChildView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
  if (theApp.m_map.IsEmpty()) goto mend;
  UpdateCursor();
mend:	
	CWnd ::OnKeyUp(nChar, nRepCnt, nFlags);
}

HCURSOR CChildView::SetViewCursor(HCURSOR cur)
{
  HCURSOR res;
  res = (HCURSOR)GetClassLong(m_hWnd, GCL_HCURSOR);
  if (res != cur) {
    res = (HCURSOR)SetClassLong(m_hWnd, GCL_HCURSOR, (LONG)cur);
    if (::GetFocus() == m_hWnd)
      SetCursor(cur);
  }
  return res;

}

void CChildView::OnKillFocus(CWnd* pNewWnd) 
{
	CWnd ::OnKillFocus(pNewWnd);
  SetViewCursor(m_DefCur);
}

void CChildView::OnRButtonDown(UINT nFlags, CPoint point) 
{
  int i;

  if (nFlags & MK_SHIFT) {
    if (nFlags & MK_CONTROL) {
      i = m_LastZoomIdx - 1;
      keybd_event('~', 0, 0, 0);  // Disables kbd layout switching by Ctrl+Shift
      if (m_LastZoomIdx == -1) goto mend;
    }
    else {
      if (m_EditorMode) goto mend;
      i = g_pMainFrame->m_cbZoom.GetCurSel();
    }
    m_LastZoomIdx = -1;
    if (i < g_pMainFrame->m_cbZoom.GetCount() - 1) {
      ChangeZoom(point, i + 1);
    }
  }
  else
  if (m_EditorMode) 
    SelClick(point);
mend:	
	CWnd ::OnRButtonDown(nFlags, point);
}

void CChildView::ChangeZoom(CPoint point, int NewIdx)
{
  CPoint pt;
  CString s;
  CRect r;

  pt = ScreenToMap(point);
  g_pMainFrame->m_cbZoom.SetCurSel(NewIdx);
  g_pMainFrame->m_cbZoom.GetLBText(NewIdx, s);
  theApp.m_map.SetZoom(atoi(s));

  GetClientRect(&r);
  theApp.m_map.PosX = pt.x - r.Width()*100/theApp.m_map.m_iZoom/2;
  theApp.m_map.PosY = pt.y - r.Height()*100/theApp.m_map.m_iZoom/2;

  Invalidate(FALSE);
  theApp.m_map.ScrollMap(0, 0);
}



CPoint CChildView::ScreenToMap(const CPoint &point)
{
  CPoint res;
  res.x = theApp.m_map.PosX + point.x*100/theApp.m_map.m_iZoom;
  res.y = theApp.m_map.PosY + point.y*100/theApp.m_map.m_iZoom;
  return res;
}

CPoint CChildView::MapToScreen(const CPoint &point)
{
  CPoint res;
  res.x = (point.x - theApp.m_map.PosX)*theApp.m_map.m_iZoom/100;
  res.y = (point.y - theApp.m_map.PosY)*theApp.m_map.m_iZoom/100;
  return res;
}

void CChildView::InvalidateMapRect(CRect &rect)
{
  CRect r(rect);
  MapToScreenRect(&r);
  r.right++;
  r.bottom++;
  InvalidateRect(&r, FALSE);
}

void CChildView::SetEditorMode()
{
  m_CurHideUnexplored = theApp.m_map.HideUnexplored;
  theApp.m_map.HideUnexplored = FALSE;
  m_EditorMode = TRUE;
  m_DefCur = ::LoadCursor(NULL, IDC_ARROW);
  SetViewCursor(m_DefCur);
  ActivateDefStyle();
  g_pMainFrame->ShowControlBar(&g_pMainFrame->m_EditorTB, TRUE, FALSE);
  theApp.m_map.Content.DrawOptions |= cdoSimplify;
  HideMarkDescriptions();
  
  CMenu *menu = g_pMainFrame->GetMenu()->GetSubMenu(1);
  menu->EnableMenuItem(1, MF_BYPOSITION | MF_ENABLED);
  menu->EnableMenuItem(2, MF_BYPOSITION | MF_ENABLED);

  Invalidate(FALSE);
}

void CChildView::SetViewerMode()
{
  theApp.m_map.HideUnexplored = m_CurHideUnexplored;
  m_EditorMode = FALSE;
  m_NewObjectType = 0;
  if (theApp.m_map.IsEmpty())
    m_DefCur = ::LoadCursor(NULL, IDC_ARROW);
  else
    m_DefCur = theApp.LoadCursor(IDC_CURDRAGHAND);
  SetViewCursor(m_DefCur);
  g_pMainFrame->ShowControlBar(&g_pMainFrame->m_EditorTB, FALSE, FALSE);
  SelectAll(FALSE);
  theApp.m_map.Content.DrawOptions = cdoHideLarge | cdoHideSmall;

  CMenu *menu = g_pMainFrame->GetMenu()->GetSubMenu(1);
  menu->EnableMenuItem(1, MF_BYPOSITION | MF_GRAYED);
  menu->EnableMenuItem(2, MF_BYPOSITION | MF_GRAYED);

  Invalidate(FALSE);
}

void CChildView::OnDelete() 
{
  CContentObjects& objs = theApp.m_map.Content.Objects;
  CContentObj* o;
  int i = 0;
  while (i < objs.GetSize()) {
    o = objs[i];
    if (o->Selected) {
      InvalidateObj(o);
      objs.RemoveAt(i);
      m_ObjSelCount--;
    }
    else
      i++;
  }
  theApp.m_map.Content.Modified = TRUE;
  UpdateCurrentStyle();
}

void CChildView::DrawSelectionRect(CDC &dc)
{
  ::SelectObject(dc, (HBRUSH)GetStockObject(NULL_BRUSH));
  ::SelectObject(dc, (HPEN)GetStockObject(WHITE_PEN));
  dc.SetROP2(R2_XORPEN);
  dc.Rectangle(&m_Selection);
}

void CChildView::ScreenToMapRect(CRect *rect)
{
  CPoint pt;
  pt = ScreenToMap(CPoint(rect->left, rect->top));
  rect->left = pt.x;
  rect->top = pt.y;
  pt = ScreenToMap(CPoint(rect->right, rect->bottom));
  rect->right = pt.x;
  rect->bottom = pt.y;
}

void CChildView::MapToScreenRect(CRect *rect)
{
  CPoint pt;
  pt = MapToScreen(CPoint(rect->left, rect->top));
  rect->left = pt.x;
  rect->top = pt.y;
  pt = MapToScreen(CPoint(rect->right, rect->bottom));
  rect->right = pt.x;
  rect->bottom = pt.y;
}

void CChildView::OnInsertText() 
{
  m_DefCur = ::LoadCursor(0, IDC_CROSS);
  SetViewCursor(m_DefCur);
  m_NewObjectType = otTextLabel;
}

void CChildView::SetArrowCursor()
{
  m_DefCur = ::LoadCursor(0, IDC_ARROW);
  SetViewCursor(m_DefCur);
}

void CChildView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
  if (m_EditorMode && m_ObjSelCount == 1) {
    CPoint pt;
    pt = ScreenToMap(point);
    CContentObj* obj = theApp.m_map.Content.GetObjectAt(pt, theApp.m_map.m_iLevel);
    if (obj && obj->Selected)
      EditObject(obj);
  }
	CWnd ::OnLButtonDblClk(nFlags, point);
}


void CChildView::OnSelectMode() 
{
  m_NewObjectType = 0;
  SetArrowCursor();	
}

void CChildView::OnUpdateInsertText(CCmdUI* pCmdUI) 
{
  pCmdUI->SetCheck(m_NewObjectType == otTextLabel);
  pCmdUI->Enable(m_EditorMode);
}

void CChildView::OnUpdateSelectMode(CCmdUI* pCmdUI) 
{
  pCmdUI->SetCheck(m_NewObjectType == 0);
  pCmdUI->Enable(m_EditorMode);
}

void CChildView::OnEditStyles() 
{
  CStylesListDlg sl;

  SelectAll(FALSE);
  sl.DoModal();

  g_pMainFrame->FillStylesCombo();
}

void CChildView::OnUpdateEditStyles(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(m_EditorMode);
}

void CChildView::UpdateCurrentStyle()
{
  int i, idx = -2;
  CContentObjects& objs = theApp.m_map.Content.Objects;
  CContentObj *o;
  for (i = 0; i < objs.GetSize(); i++) {
    o = objs[i];
    if (o->Selected) {
      if (idx != -2 && o->StyleIdx != idx) {
        idx = -1;
        break;
      }
      else
        idx = o->StyleIdx;
    }
  }
  if (idx != -2) {
    g_pMainFrame->m_cbStyles.SetCurSel(idx);
  }
  else {
    ActivateDefStyle();
  }
}

void CChildView::SelectAll(BOOL Select)
{
  int i, idx = -2;
  CContentObjects& objs = theApp.m_map.Content.Objects;
  CContentObj *o;
  m_ObjSelCount = 0;
  for (i = 0; i < objs.GetSize(); i++) {
    o = objs[i];
    if (o->Selected != Select) {
      o->Selected = Select;
      InvalidateObj(o);
      if (Select)
        m_ObjSelCount++;
    }
  }
  UpdateCurrentStyle();
}

void CChildView::ActivateDefStyle()
{
  int i = theApp.m_map.Content.Styles.GetSize();
  if (m_DefStyleIdx >= i)
    m_DefStyleIdx = i - 1;
  else
    if (m_DefStyleIdx < 0)
      m_DefStyleIdx = 0;
  g_pMainFrame->m_cbStyles.SetCurSel(m_DefStyleIdx);
}

void CChildView::OnUpdateDelete(CCmdUI* pCmdUI) 
{
  BOOL f = FALSE;
  int i;
  CContentObjects& objs = theApp.m_map.Content.Objects;
  for (i = 0; i < objs.GetSize(); i++) 
    if (objs[i]->Selected) {
      f = TRUE;
      break;
    }

  pCmdUI->Enable(m_EditorMode && f);
}

void CChildView::OnInsertMark() 
{
  if (theApp.m_map.Content.MarkCategories.GetSize() == 0) {
    AfxMessageBox("Before using marks you must define atleast one mark category.", MB_ICONERROR);
    return;
  }
  m_DefCur = ::LoadCursor(0, IDC_CROSS);
  SetViewCursor(m_DefCur);
  m_NewObjectType = otMark;
}

void CChildView::OnUpdateInsertMark(CCmdUI* pCmdUI) 
{
  pCmdUI->SetCheck(m_NewObjectType == otMark);
  pCmdUI->Enable(m_EditorMode);
}

void CChildView::InvalidateObj(CContentObj *obj)
{
  CRect r;
  obj->GetZoomedRect(&r);
  r.OffsetRect(-theApp.m_map.PosX*theApp.m_map.m_iZoom/100, -theApp.m_map.PosY*theApp.m_map.m_iZoom/100);
//  r.right += 4;
  InvalidateRect(r, FALSE);
}

BOOL CChildView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
  BOOL ShiftPressed = (GetAsyncKeyState(VK_SHIFT) & 0x8000);
  if (!m_EditorMode && !(theApp.m_map.Content.DrawOptions & cdoHideAll)) {
    CPoint pt;
    GetCursorPos(&pt);
    ScreenToClient(&pt);
    pt = ScreenToMap(pt);
    CContentObj *obj = theApp.m_map.Content.GetObjectAt(pt, theApp.m_map.m_iLevel, otMark);
    if (obj) {
      if (!ShiftPressed)
        SetCursor(::LoadCursor(0, IDC_ARROW));
      if (((CContentMark*)obj)->DisplayMode == dmNormal) {
        SelectAll(FALSE);
        obj->Selected = TRUE;
        InvalidateObj(obj);
      }
      return TRUE;
    }
    else
      SelectAll(FALSE);
  }
	return CWnd ::OnSetCursor(pWnd, nHitTest, message);
}

void CChildView::HideMarkDescriptions()
{
  int i;
  CPoint pt;
  CContentObjects& objs = theApp.m_map.Content.Objects;
  CContentObj *o;

  for (i = 0; i < objs.GetSize(); i++) {
    o = objs[i];
    if (o->GetType() == otMark && ((CContentMark*)o)->DisplayMode == dmExpanded) {
      InvalidateObj(o);
      ((CContentMark*)o)->DisplayMode = dmNormal;
      InvalidateObj(o);
    }
  }
}

void CChildView::OnRButtonDblClk(UINT nFlags, CPoint point) 
{
	CWnd ::OnRButtonDblClk(nFlags, point);
}

void CChildView::UpdateCursor()
{
  if (!m_EditorMode) {
    BOOL CtrlPressed;
    BOOL ShiftPressed;
    CtrlPressed = (GetAsyncKeyState(VK_CONTROL) & 0x8000);
    ShiftPressed = (GetAsyncKeyState(VK_SHIFT) & 0x8000);
    if (CtrlPressed && !ShiftPressed) {
      SetViewCursor(::LoadCursor(NULL, IDC_ARROW));
      return;
    }
    else
      if (!m_EditorMode) {
        if (!CtrlPressed && ShiftPressed) {
			    SetViewCursor(m_MagnifyCur);
          return;
        }
        else
          if (CtrlPressed && ShiftPressed) {
			      SetViewCursor(m_MagnifyCur2);
            return;
          }
      }
  }
  SetViewCursor(m_DefCur);
}

void CChildView::OnUpdateStatusBar(CCmdUI* pCmdUI) 
{
  CString s;
  pCmdUI->Enable();
  if (m_EditorMode && m_ObjSelCount) {
    if (m_ObjSelCount) 
      s.Format("Objects selected: %d", m_ObjSelCount);
  }
  pCmdUI->SetText(s);
}

void CChildView::OnUpdateStatusBarFileName(CCmdUI* pCmdUI) 
{
  CString s;

  if (theApp.m_map.Content.FileName.IsEmpty()) {
    if (m_EditorMode || theApp.m_map.Content.Modified)
      s = "Untitled";
  }
  else {
    s = theApp.m_map.Content.FileName;
  }
  if (theApp.m_map.Content.Modified && !s.IsEmpty())
    s += " *";
  if (!s.IsEmpty()) {
    CDC *dc = g_pMainFrame->m_wndStatusBar.GetDC();
    CFont *old = (CFont*)dc->SelectObject(&g_pMainFrame->m_BaseFont);
    int i;
    i = s.GetLength();
    CRect r;
    g_pMainFrame->m_wndStatusBar.GetItemRect(2, r);
    r.right -= 6;
    dc->DrawText(s.GetBuffer(i + 10), i, r, DT_PATH_ELLIPSIS | DT_MODIFYSTRING | DT_CALCRECT);
    s.ReleaseBuffer();
    dc->SelectObject(old);
    g_pMainFrame->m_wndStatusBar.ReleaseDC(dc);
  }
  pCmdUI->SetText(s);
}

void CChildView::OnHideunexploredsymbols() 
{
  theApp.m_map.HideUnexplored = !theApp.m_map.HideUnexplored;
  Invalidate(FALSE);
}

void CChildView::OnUpdateHideunexploredsymbols(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(!m_EditorMode && !theApp.m_map.IsEmpty());	
  pCmdUI->SetCheck(theApp.m_map.HideUnexplored);
}

void CChildView::SelClick(CPoint point)
{
  int i;
  CPoint pt;
  CContentObjects& objs = theApp.m_map.Content.Objects;
  CContentObj *o, *o2;
  BOOL ShiftPressed = (GetAsyncKeyState(VK_SHIFT) & 0x8000);
  pt = ScreenToMap(point);
  o2 = theApp.m_map.Content.GetObjectAt(pt, theApp.m_map.m_iLevel);

  if (ShiftPressed) {
    if (o2) {
      if (o2->Selected)
        m_ObjSelCount--;
      else
        m_ObjSelCount++;
      o2->Selected = !o2->Selected;
      InvalidateObj(o2);
    }
    else 
      goto m1;
  }
  else 
    if (!o2 || !o2->Selected) {
      for (i = 0; i < objs.GetSize(); i++) {
        o = objs[i];
        if (o->Selected) {
          o->Selected = FALSE;
          InvalidateObj(o);
          m_ObjSelCount--;
        }
      }
      if (o2) {
        o2->Selected = TRUE;
        InvalidateObj(o2);
        m_ObjSelCount++;
      }
    }
m1:
  UpdateCurrentStyle();
}

void CChildView::OnProperties() 
{
  CContentObjects& objs = theApp.m_map.Content.Objects;
  CContentObj *o;
  int i;

  for (i = 0; i < objs.GetSize(); i++) {
    o = objs[i];
    if (o->Selected) {
      EditObject(o);
      return;
    }
  }
}

void CChildView::OnUpdateProperties(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(m_EditorMode && m_ObjSelCount == 1);	
  if (pCmdUI->m_pMenu && m_InPopup)
    pCmdUI->m_pMenu->SetDefaultItem(ID_PROPERTIES);
}

void CChildView::EditObject(CContentObj *obj)
{
  if (obj) {
    switch (obj->GetType()) {
    case otTextLabel:
      CTextProps *tp;
      tp = new CTextProps(this);
      tp->m_DlgCaption = "Edit text label";
      tp->m_EditText = ((CContentText*)obj)->GetText();
      if (tp->DoModal() == IDOK) {
        InvalidateObj(obj);
        ((CContentText*)obj)->SetText(tp->m_EditText);
        InvalidateObj(obj);
        theApp.m_map.Content.Modified = TRUE;
      }
      break;
    case otMark:
      SetArrowCursor();
      CNewMarkDlg *dlg;
      CContentMark *cm;
      cm = (CContentMark*)obj;
      dlg = new CNewMarkDlg(this);
      dlg->m_CategoryIdx = cm->Category;
      dlg->m_ShortText = cm->ShortText;
      dlg->m_LongText = cm->LongText;
      dlg->m_Caption = "Edit mark";
      if (dlg->DoModal() == IDOK) {
        InvalidateObj(cm);
        cm->Category = dlg->m_CategoryIdx;
        cm->ShortText = dlg->m_ShortText;
        cm->LongText = dlg->m_LongText;
        InvalidateObj(cm);
        theApp.m_map.Content.Modified = TRUE;
      }
      break;
    }
  }
}

void CChildView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
  if (theApp.m_map.IsEmpty()) return;
#ifdef MY_DEBUG
  if ((GetAsyncKeyState(VK_CONTROL) & 0x8000) && !(GetAsyncKeyState(VK_SHIFT) & 0x8000)) {
    CPoint pt;
    BYTE b;

    ScreenToClient(&point);
    pt = ScreenToMap(point);

    if (theApp.m_map.LoadMap(pt.x / theApp.m_map.m_MapWd, pt.y / theApp.m_map.m_MapHt)) {
      b = *((BYTE*)theApp.m_map.bm_data + (pt.y % theApp.m_map.m_MapHt)*theApp.m_map.m_MapWd + (pt.x % theApp.m_map.m_MapWd));
    }
    else
      b = 0;

    CString s;
    s.Format("X: %d;  Y: %d\nValue: 0x%X\nMap file: %s", pt.x, pt.y, b, 
             theApp.m_map.GetMapFileName(pt.x / theApp.m_map.m_MapWd, pt.y / theApp.m_map.m_MapHt, theApp.m_map.m_iLevel));
    MessageBox(s, "Info", MB_ICONINFORMATION);
  }
  else
#endif
  if (m_EditorMode) {
    m_InPopup = TRUE;
    m_Popups.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y, AfxGetMainWnd());
    m_InPopup = FALSE;
  }
}

void CChildView::OnToFront() 
{
  CContentObjects& objs = theApp.m_map.Content.Objects;
  CContentObj *o;
  int i, cnt;

  i = 0;
  cnt = 0;
  while (i < objs.GetSize() - cnt) {
    o = objs[i];
    if (o->Selected) {
      objs[i] = NULL;
      objs.Add(o);
      objs.RemoveAt(i);
      cnt++;
      InvalidateObj(o);
      theApp.m_map.Content.Modified = TRUE;
    }
    else
      i++;
  }
}

void CChildView::OnUpdateToFront(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(m_EditorMode && m_ObjSelCount > 0);
}

void CChildView::OnToBack() 
{
  CContentObjects& objs = theApp.m_map.Content.Objects;
  CContentObj *o;
  int i, cnt;

  i = objs.GetSize() - 1;
  cnt = 0;
  while (i >= cnt) {
    o = objs[i];
    if (o->Selected) {
      objs[i] = NULL;
      objs.RemoveAt(i);
      objs.InsertAt(0, o);
      cnt++;
      InvalidateObj(o);
      theApp.m_map.Content.Modified = TRUE;
    }
    else
      i--;
  }
}

void CChildView::OnUpdateToBack(CCmdUI* pCmdUI) 
{
  pCmdUI->Enable(m_EditorMode && m_ObjSelCount > 0);
}
