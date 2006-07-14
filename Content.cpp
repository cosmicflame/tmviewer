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

// Content.cpp: implementation of the CContent class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TMViewer.h"
#include "Content.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const int ShadowOffs = 2;

//CFont g_SimpleFont;
CPen g_GrayPen(PS_SOLID, 1, 0xF0F080);

//----------------------------------------------------------------------------

template <> void AFXAPI DestructElements <CContentStyle*> ( CContentStyle** pElements, int nCount )
{
  for ( int i = 0; i < nCount; i++, pElements++ )  {
    delete *pElements;
  }
}

//----------------------------------------------------------------------------

template <> void AFXAPI DestructElements <CMarkCategory*> ( CMarkCategory** pElements, int nCount )
{
  for ( int i = 0; i < nCount; i++, pElements++ )  {
    delete *pElements;
  }
}

//----------------------------------------------------------------------------

template <> void AFXAPI DestructElements <CContentObj*> ( CContentObj** pElements, int nCount )
{
  for ( int i = 0; i < nCount; i++, pElements++ )  {
    delete *pElements;
  }
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CContent::CContent() : Zoom(100), Modified(FALSE), DrawOptions(0), m_OnDrawObject(NULL)
{
//  if (!g_SimpleFont.m_hObject)
//    g_SimpleFont.CreatePointFont(100, "Arial");
  DefStyle.m_Owner = this;
  DefStyle.FontName = "Arial";
  DefStyle.FontSize = 14;
  DefStyle.SetZoom();
  DefStyle.SetStyleName("Default style");

  MarksShortText.m_Owner = this;
  MarksShortText.FontName = "Arial";
  MarksShortText.FontSize = 10;
  MarksShortText.FontStyle = fsBold;
  MarksShortText.SetZoom();

  MarksLongText.m_Owner = this;
  MarksLongText.FontName = "Arial";
  MarksLongText.FontSize = 9;
//  MarksLongText.FontStyle = fsBold;
  MarksLongText.SetZoom();

  MarkImages.Create(MarkWd, MarkHt, ILC_COLOR16 | ILC_MASK, 0, 1);
}

CContent::~CContent()
{

}

void CContent::Serialize(CArchive &archive)
{
  const WORD Signature = 0x3412;

  int i;
  if (archive.IsStoring()) {
// Writing signature;
    archive << Signature;
// Writing version;
    archive << (BYTE)30;

// Writing styles count
    archive << Styles.GetSize();
// Writing styles 
    for (i = 0; i < Styles.GetSize(); i++)
      Styles[i]->Serialize(archive);

// Writing objects count
    archive << Objects.GetSize();
// Writing objects 
    for (i = 0; i < Objects.GetSize(); i++)
      Objects[i]->Serialize(archive);

// Writing marks categories count
    i = MarkCategories.GetSize();
    archive << i;
// Writing mark images
    if (i)
      MarkImages.Write(&archive);
// Writing marks categories 
    for (i = 0; i < MarkCategories.GetSize(); i++)
      MarkCategories[i]->Serialize(archive);
  }
  else {
    WORD sig;
    CContentStyle *cs;
    CContentObj *co;
    CMarkCategory *mc;
    BYTE ot;
// Reading signature
   archive >> sig;
    if (sig != Signature) 
      throw "Invalid signature.";
// Reading version;
    archive >> m_ArchVer;
// Reading styles count
    archive >> i;
// Reading styles
    while (i > 0) {
      cs = new CContentStyle(this);
      cs->Serialize(archive);
      Styles.Add(cs);
      i--;
    }

// Reading objects count
    archive >> i;
// Reading objects
    while (i > 0) {
// Reading object type
      archive >> ot;
      switch (ot) {
      case otTextLabel:
        co = new CContentText(*this);
        break;
      case otMark:
        co = new CContentMark(*this);
        break;
      default:
        throw "Invalid file format.";
      }
// Reading object
      co->Serialize(archive);
      Objects.Add(co);
      i--;
    }

// Reading marks categories count
    archive >> i;
// Reading mark images
    if (i) {
      MarkImages.DeleteImageList(); 
      MarkImages.Read(&archive);
// Reading marks categories 
      while (i > 0) {
        mc = new CMarkCategory;
        mc->Serialize(archive);
        MarkCategories.Add(mc);
        i--;
      }
    }
    else
      MarkImages.SetImageCount(0);
  } 
}

void CContent::Draw(CDC &dc, CRect &Rect, CPoint &Origin, int Plane)
{
  if (DrawOptions & cdoHideAll) return;

  int i, j, zp, zp2;
  CRect r, r2, zr;
  CContentObj* obj;
  CPoint pt(Origin);

  pt.x = pt.x*Zoom/100;
  pt.y = pt.y*Zoom/100;
  r.left = Rect.left*Zoom/100;
  r.right = Rect.right*Zoom/100;
  r.top = Rect.top*Zoom/100;
  r.bottom = Rect.bottom*Zoom/100;
  dc.SetViewportOrg(CPoint(-pt.x, -pt.y));

  zp2 = 0;
  do {
    zp = zp2;
    for (i = 0; i < Objects.GetSize(); i++) {
      obj = Objects[i];
      j = obj->GetZPriority();
      if (j != zp) {
        if (j > zp && (j < zp2 || zp2 == zp))
          zp2 = j;
        continue;
      }
      obj->GetZoomedRect(&zr);
      if (obj->Plane == Plane && r2.IntersectRect(r, zr)) {
        BYTE v = obj->IsVisible();
        if (v == ovVisible || (DrawOptions & cdoSimplify && v == ovTooLarge))
          if (!m_OnDrawObject || m_OnDrawObject(obj, obj->BoundsRect, m_UserData)) {
            obj->Draw(dc, (v == ovTooLarge) ? odoSimple : 0);
          }
      }
    }
  }
  while (zp2 != zp);
  dc.SetViewportOrg(0, 0);
}

void CContent::SetZoom(int NewZoom)
{
  int i;
  for (i = 0; i < Styles.GetSize(); i++)
    Styles[i]->SetZoom(NewZoom);
  DefStyle.SetZoom(NewZoom);
  Zoom = NewZoom;
}

CContentObj* CContent::GetObjectAt(CPoint pt, int Plane, BYTE ObjectType)
{
  CContentObj *obj, *res;
  int i;
  CRect r;

  res = NULL;
  for (i = 0; i < Objects.GetSize(); i++) {
    obj = Objects[i];
    if (obj->IsVisible() != ovVisible) continue;
    obj->GetZoomedRect(&r);
    r.left = r.left*100/Zoom;
    r.right = (r.right*100 + Zoom - 1)/Zoom;
    r.top = r.top*100/Zoom;
    r.bottom = (r.bottom*100 + Zoom - 1)/Zoom;
    if (!IsVisibleUser(obj, r)) continue;
    if ((ObjectType == 0 || obj->GetType() == ObjectType) && obj->Plane == Plane && r.PtInRect(pt))
      if (res == NULL || res->GetZPriority() <= obj->GetZPriority())
        res = obj;
  }
  return res;
}

BOOL CContent::SaveToFile(const CString &FileName)
{
  BOOL res = FALSE;
  try {
  TRY {
    CFile f(FileName, CFile::modeCreate | CFile::modeWrite);
    CArchive ar(&f, CArchive::store);
    Serialize(ar);
    this->FileName = FileName;
    Modified = FALSE;
    res = TRUE;
  }
  CATCH (CException, e) {
    char buf[1000];
    e->GetErrorMessage(buf, 1000);
    AfxMessageBox(buf, MB_ICONERROR, 0);
  }
  END_CATCH
  }
  catch (char* ch) {
    AfxMessageBox(ch, MB_ICONERROR, 0);
  }
  catch (...) {
    AfxMessageBox("Unexpected error while writing map symbols.", MB_ICONERROR, 0);
  }
  return res;
}

BOOL CContent::LoadFromFile(const CString &FileName)
{
  BOOL res = FALSE;
  try {
  TRY {
    CFile f(FileName, CFile::modeRead);
    CArchive ar(&f, CArchive::load);
    Clear();
    Serialize(ar);
    this->FileName = FileName;
    Modified = FALSE;
    res = TRUE;
  }
  CATCH (CException, e) {
    char buf[1000];
    e->GetErrorMessage(buf, 1000);
    AfxMessageBox(buf, MB_ICONERROR, 0);
  }
  END_CATCH
  }
  catch (char* ch) {
    AfxMessageBox(ch, MB_ICONERROR, 0);
  }
  catch (...) {
    AfxMessageBox("Unexpected error while reading map symbols.", MB_ICONERROR, 0);
  }
  return res;
}

void CContent::Clear()
{
  Objects.RemoveAll();
  Styles.RemoveAll();
  MarkCategories.RemoveAll();
  MarkImages.SetImageCount(0);
}

void CContent::SetOnDrawObject(TOnDrawObject proc, void *UserData)
{
  m_OnDrawObject = proc;
  m_UserData = UserData;
}

//////////////////////////////////////////////////////////////////////
// CContentStyle Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CContentStyle::CContentStyle(CContent* Owner) : m_Owner(Owner), 
   TextColor(0xFFFFFF), Shadow(TRUE), FontCharSet(DEFAULT_CHARSET)
{
}

CContentStyle::~CContentStyle()
{

}

void CContentStyle::PrepareDC(CDC &dc)
{
  m_OldFont = dc.SelectObject(&Font);
  dc.SetBkMode(TRANSPARENT);
}

void CContentStyle::UnPrepareDC(CDC &dc)
{
  dc.SelectObject(m_OldFont);
}

void CContentStyle::SetZoom(int Zoom)
{
  LOGFONT lf;
  if (Font.m_hObject) 
    Font.DeleteObject();

  ZeroMemory(&lf, sizeof(lf));
  HDC dc = GetDC(0);
  lf.lfHeight = -FontSize*GetDeviceCaps(dc, LOGPIXELSY)/72;
  ReleaseDC(0, dc);
  if (Zoom != 100) 
    lf.lfHeight = lf.lfHeight*Zoom/100;
  if (lf.lfHeight == 0)
    lf.lfHeight = 1;
  strcpy(lf.lfFaceName, FontName);
  if (FontStyle & fsBold)
    lf.lfWeight = FW_BOLD;
  if (FontStyle & fsItalic)
    lf.lfItalic = TRUE;
  if (FontStyle & fsUnderline)
    lf.lfUnderline = TRUE;
  lf.lfCharSet = FontCharSet;
  Font.CreateFontIndirect(&lf);
}

void CContentStyle::Serialize(CArchive &archive)
{
  if (archive.IsStoring()) {

    archive << GetStyleName();
    archive << FontName;
    archive << FontSize;
    archive << FontStyle;
    archive << FontCharSet;
    archive << TextColor;
    archive << Shadow;
  }
  else {
    archive >> m_StyleName;
    archive >> FontName;
    archive >> FontSize;
    archive >> FontStyle;
    archive >> FontCharSet;
    archive >> TextColor;
    archive >> Shadow;
    SetZoom();
  }
}

CString& CContentStyle::GetStyleName()
{
  if (m_StyleName.IsEmpty()) {
    int n = 0;
m1:
    n++;
    m_StyleName.Format("Style %d", n);
    for (int i = 0; i < m_Owner->Styles.GetSize(); i++)
      if (m_Owner->Styles[i] != this && m_Owner->Styles[i]->m_StyleName == m_StyleName) 
        goto m1;
  }
  return m_StyleName;
}

const CContentStyle& CContentStyle::operator =(const CContentStyle &Style)
{
  FontName = Style.FontName;
  FontSize = Style.FontSize;
  FontStyle = Style.FontStyle;
  FontCharSet = Style.FontCharSet;
  TextColor = Style.TextColor;
  Shadow = Style.Shadow;
  m_StyleName = Style.m_StyleName;
  return *this;
}

//////////////////////////////////////////////////////////////////////
// CContentObj Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CContentObj::CContentObj(CContent& Parent) : Selected(FALSE), Plane(0), Style(NULL),
                StyleIdx(-1)
{
  BoundsRect.SetRectEmpty();
  this->Parent = &Parent;
}

CContentObj::~CContentObj()
{
}

void CContentObj::Draw(CDC &dc, BYTE Options)
{
  DoDraw(dc, Options);
  if (Selected) {
    CRect r;
    GetZoomedRect(&r);
    dc.FrameRect(&r, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));  
  }
}

void CContentObj::GetZoomedRect(CRect *r)
{
  r->left = BoundsRect.left*Parent->Zoom/100;
  r->top = BoundsRect.top*Parent->Zoom/100;
  r->right = r->left + BoundsRect.Width()*Parent->Zoom/100;
  r->bottom = r->top + BoundsRect.Height()*Parent->Zoom/100;
}

void CContentObj::SetPosition(int x, int y)
{
  int wd = BoundsRect.Width();
  int ht = BoundsRect.Height();
  BoundsRect.SetRect(x, y, x + wd, y + ht);
  Parent->Modified = TRUE;
}

void CContentObj::CheckStyle()
{
  if (!Style) 
    if (StyleIdx >= 0 && StyleIdx < Parent->Styles.GetSize())
      Style = Parent->Styles[StyleIdx];
    else
      Style = &Parent->DefStyle;
}

void CContentObj::StyleChanged()
{
  Style = NULL;
  CheckStyle();
  Parent->Modified = TRUE;
}

void CContentObj::Serialize(CArchive &archive)
{
  if (archive.IsStoring()) {
    archive << GetType();
    archive << BoundsRect;
    archive << Plane;
    archive << StyleIdx;
  }
  else {
    archive >> BoundsRect;
    archive >> Plane;
    archive >> StyleIdx;
  }
}

CSize CContentObj::GetEffectiveSize()
{
  return CSize(BoundsRect.Width()*Parent->Zoom/100, BoundsRect.Height()*Parent->Zoom/100);
}

BYTE CContentObj::IsVisible()
{
  BYTE res = 0;
  CSize sz = GetEffectiveSize();
  if ((Parent->DrawOptions & cdoHideLarge) && sz.cy > 60)
    res = ovTooLarge;
  else
    if ((Parent->DrawOptions & cdoHideSmall) && sz.cy < 14)
      res = ovTooSmall;
  if (Selected || res == 0)
    res = ovVisible;
  return res;
}

//////////////////////////////////////////////////////////////////////
// CContentText Class
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CContentText::CContentText(CContent& Parent) : CContentObj(Parent), m_LastZoom(0)
{
}

CContentText::~CContentText()
{
}

void CContentText::DoDraw(CDC &dc, BYTE Options)
{
  CRect r;
  GetZoomedRect(&r);
  CheckStyle();
  Style->PrepareDC(dc);
  if (Options & odoSimple) {
    dc.BeginPath();
    dc.DrawText(m_Text, r, DT_CENTER);
    dc.EndPath();
    CPen *oldpen = (CPen*)dc.SelectObject(&g_GrayPen);
//    dc.SetROP2(R2_NOT);
    dc.StrokePath();
    dc.SelectObject(oldpen);
  }
  else {
    if (Style->Shadow) {
      dc.SetTextColor(0);
      CRect rr(r);
      rr.left += ShadowOffs*2;
      rr.top += ShadowOffs;
      dc.DrawText(m_Text, rr, DT_CENTER);
    }
    dc.SetTextColor(Style->TextColor);
    dc.DrawText(m_Text, r, DT_CENTER);
  }
  Style->UnPrepareDC(dc);
}

void CContentText::SetText(const CString &Text)
{
  if (m_Text != Text)
    Parent->Modified = TRUE;
  m_LastZoom = 0;
  m_Text = Text;
  CheckStyle();
  Style->SetZoom();
  CDC dc;
  dc.Attach(GetDC(0));
  Style->PrepareDC(dc);
  CRect r(BoundsRect);
  dc.DrawText(m_Text, BoundsRect, DT_CALCRECT);
  Style->UnPrepareDC(dc);

  Style->SetZoom(Parent->Zoom);

//  BoundsRect.right += 8;
  if (Style->Shadow) {
    BoundsRect.right += ShadowOffs;
    BoundsRect.bottom += ShadowOffs;
  }
  BoundsRect.OffsetRect((r.Width() - BoundsRect.Width())/2, (r.Height() - BoundsRect.Height())/2);
}

void CContentText::StyleChanged()
{
  CContentObj::StyleChanged();
  SetText(m_Text);
}

void CContentText::GetZoomedRect(CRect *r)
{
  CheckZoomChanged();
  *r = m_ZoomedRect;
  r->OffsetRect(BoundsRect.left*Parent->Zoom/100, BoundsRect.top*Parent->Zoom/100);
}

void CContentText::Serialize(CArchive &archive)
{
  CContentObj::Serialize(archive);
  if (archive.IsStoring()) {
    archive << m_Text;
  }
  else {
    archive >> m_Text;
    SetText(m_Text);
  }
}

BYTE CContentText::GetType()
{
  return otTextLabel;
}

CSize CContentText::GetEffectiveSize()
{
  CheckZoomChanged();
  return m_EffSize;
}

void CContentText::CheckZoomChanged()
{
  if (m_LastZoom != Parent->Zoom) {
    CDC dc;
    dc.Attach(GetDC(0));
    Style->PrepareDC(dc);
    m_EffSize = dc.GetTextExtent("Ay");

    m_ZoomedRect.SetRectEmpty();
    dc.DrawText(m_Text, m_ZoomedRect, DT_CALCRECT);
    if (Style->Shadow) {
      m_ZoomedRect.right += ShadowOffs;
      m_ZoomedRect.bottom += ShadowOffs;
    }
    m_ZoomedRect.right += 2;

    Style->UnPrepareDC(dc);
    m_LastZoom = Parent->Zoom;
  }
}

//////////////////////////////////////////////////////////////////////
// CMarkCategory Class
//////////////////////////////////////////////////////////////////////

void CMarkCategory::Serialize(CArchive &archive)
{
  if (archive.IsStoring()) {
    archive << Name;
  }
  else {
    archive >> Name;
  }
}

//////////////////////////////////////////////////////////////////////
// CContentMark Class
//////////////////////////////////////////////////////////////////////

CContentMark::CContentMark(CContent& Parent) : CContentObj(Parent), Category(0),
     DisplayMode(dmNormal)
{
  BoundsRect.SetRect(0, 0, 20, 20);
}

void CContentMark::Serialize(CArchive &archive)
{
  CContentObj::Serialize(archive);
  if (archive.IsStoring()) {
    archive << ShortText;
    archive << LongText;
    archive << Category;
  }
  else {
    archive >> ShortText;
    archive >> LongText;
    archive >> Category;
  }
}

BYTE CContentMark::GetType()
{
  return otMark;
}

void CContentMark::DoDraw(CDC &dc, BYTE Options)
{
  CRect r;
  GetZoomedRect(&r);
  CMarkCategory *mc = GetCategory();
  CString s;

  if (mc) {
    if (!mc->Visible) return;
    Parent->MarkImages.Draw(&dc, Category, CPoint(r.left + 2, r.top + 2), ILD_TRANSPARENT);
    if (ShortText.IsEmpty())
      s = mc->Name;
    else
      s = ShortText;
  }
  if (DisplayMode == dmExpanded && !s.IsEmpty()) {
    int i;
    CRect rr(r);
    rr.left += MarkWd + 4;
    dc.DrawEdge(rr, BDR_RAISEDINNER, BF_RECT);
//    FrameRect(dc, rr, (HBRUSH)GetStockObject(LTGRAY_BRUSH));
    rr.DeflateRect(1, 1);
    dc.FillSolidRect(rr, GetSysColor(COLOR_INFOBK));

    Parent->MarksShortText.PrepareDC(dc);
    dc.SetTextColor(GetSysColor(COLOR_INFOTEXT));
    rr.top += 3;
    rr.DeflateRect(4, 0);
    i = dc.DrawText(s, rr, DT_CENTER);
    Parent->MarksShortText.UnPrepareDC(dc);

    Parent->MarksLongText.PrepareDC(dc);
    dc.SetTextColor(GetSysColor(COLOR_INFOTEXT));
    rr.top += 6 + i;
    dc.DrawText(LongText, rr, 0);
    Parent->MarksLongText.UnPrepareDC(dc);
  }
}

void CContentMark::GetZoomedRect(CRect *r)
{
  r->left = BoundsRect.left*Parent->Zoom/100;
  r->left += (BoundsRect.right*Parent->Zoom/100 - r->left - 20)/2;
  r->top = BoundsRect.top*Parent->Zoom/100;
  r->top += (BoundsRect.bottom*Parent->Zoom/100 - r->top - 20)/2;
  if (DisplayMode == dmNormal) {
    r->right = r->left + MarkWd + 4;
    r->bottom = r->top + MarkHt + 4;
  }
  else {
    CRect rr;
    CString s;
    r->right = r->left + 200;
    r->bottom = r->top;
    CDC *dc = Parent->m_wnd->GetDC();

    if (!ShortText.IsEmpty())
      s = ShortText;
    else 
      if (Category < Parent->MarkCategories.GetSize()) 
        s = Parent->MarkCategories[Category]->Name;
    if (!s.IsEmpty()) {
      Parent->MarksShortText.PrepareDC(*dc);
      dc->DrawText(s, r, DT_CALCRECT);
      Parent->MarksShortText.UnPrepareDC(*dc);
    }

    if (!LongText.IsEmpty()) {
      rr.SetRectEmpty();
      rr.right = r->Width();
      Parent->MarksLongText.PrepareDC(*dc);
      dc->DrawText(LongText, rr, DT_CALCRECT);
      Parent->MarksLongText.UnPrepareDC(*dc);
      r->bottom += rr.bottom + 6;
      if (rr.right > r->Width())
        r->right = r->left + rr.right;
    }
    Parent->m_wnd->ReleaseDC(dc);
    r->right += MarkWd;
    if (r->bottom != r->top) {
      r->right += 4 + 10;
      r->bottom += 10;
    }
  }
}

CMarkCategory* CContentMark::GetCategory()
{
  CMarkCategory *cat;
  if (Category < Parent->MarkCategories.GetSize()) 
    cat = Parent->MarkCategories[Category];
  else
    cat = NULL;
  return cat;
}

BYTE CContentMark::IsVisible()
{
  CMarkCategory *cat = GetCategory();
  return (cat && cat->Visible) ? ovVisible : 0;
}
