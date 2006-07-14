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

// Content.h: interface for the CContent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONTENT_H__34EE6C7C_7402_4626_9670_F80A3CA56873__INCLUDED_)
#define AFX_CONTENT_H__34EE6C7C_7402_4626_9670_F80A3CA56873__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

// Font styles
const BYTE fsBold      = 1;
const BYTE fsItalic    = 2;
const BYTE fsUnderline = 4;

// Object types
#define otTextLabel 1
#define otMark      2

class CContent;

class CContentStyle : public CObject
{
private:
	CFont* m_OldFont;
	CString m_StyleName;
public:
	BYTE FontCharSet;
	CContent* m_Owner;
	void SetZoom(int Zoom = 100);
	BYTE FontStyle;
	int FontSize;
	CString FontName;
	BOOL Shadow;
	void UnPrepareDC(CDC &dc);
	void PrepareDC(CDC &dc);
	CPen Pen;
	COLORREF TextColor;
	CBrush Brush;
	CFont Font;
  void SetStyleName(const CString& Name) { m_StyleName = Name; };
	CString& GetStyleName();
	void Serialize(CArchive& archive);
  const CContentStyle& operator =(const CContentStyle &Style);
	CContentStyle(CContent* Owner = NULL);
	virtual ~CContentStyle();
};

typedef CArray <CContentStyle*, CContentStyle*> CContentStyles;
template <> void AFXAPI DestructElements <CContentStyle*> ( CContentStyle** pElements, int nCount );

//----------------------------------------------------------------------------

class CMarkCategory : public CObject
{
public:
  CString Name;
  BOOL Visible;

	void Serialize(CArchive& archive);
  CMarkCategory() : Visible(TRUE) {};
  virtual ~CMarkCategory() {};
};

typedef CArray <CMarkCategory*, CMarkCategory*> CMarkCategories;
template <> void AFXAPI DestructElements <CMarkCategory*> ( CMarkCategory** pElements, int nCount );

//----------------------------------------------------------------------------

// Object draw options
#define odoSimple 1

// Object visibility states

#define ovVisible  1
#define ovTooLarge 2
#define ovTooSmall 3

class CContentObj : public CObject
{
protected:
	void CheckStyle();
  virtual void DoDraw(CDC &dc, BYTE Options) {}; 
public:
	virtual BYTE IsVisible();
  virtual int GetZPriority() { return 0; };
	virtual CSize GetEffectiveSize();
	int StyleIdx;
	virtual void StyleChanged();
	int Plane;
	BOOL Selected;
	CContentStyle* Style;
	void SetPosition(int x, int y);
	virtual void GetZoomedRect(CRect* r);
	CContent* Parent;
	CRect BoundsRect;
	void Draw(CDC &dc, BYTE Options); 
	virtual void Serialize(CArchive& archive);
  virtual BYTE GetType() { return 0; };
	CContentObj(CContent& Parent);
	virtual ~CContentObj();
};

typedef CArray <CContentObj*, CContentObj*> CContentObjects;
template <> void AFXAPI DestructElements <CContentObj*> ( CContentObj** pElements, int nCount );

//----------------------------------------------------------------------------

class CContentText: public CContentObj
{
private:
	void CheckZoomChanged();
	CRect m_ZoomedRect;
	int m_LastZoom;
	CSize m_EffSize;
	CString m_Text;
protected:
  virtual BYTE GetType();
	virtual void DoDraw(CDC &dc, BYTE Options); 
public:
  virtual int GetZPriority() { return (IsVisible() == ovVisible) ? 100 : 0; };
	virtual CSize GetEffectiveSize();
	virtual void StyleChanged();
	void SetText(const CString& Text);
  const CString& GetText() { return m_Text; };
	virtual void GetZoomedRect(CRect* r);
	virtual void Serialize(CArchive& archive);
	CContentText(CContent& Parent);
	virtual ~CContentText();
};

//----------------------------------------------------------------------------

// Display modes
#define dmNormal   1
#define dmExpanded 2

class CContentMark: public CContentObj
{
protected:
  virtual BYTE GetType();
	virtual void DoDraw(CDC &dc, BYTE Options); 

public:
  virtual BYTE IsVisible();
  virtual int GetZPriority() { return (Selected || DisplayMode == dmExpanded) ? 1000 : 0; };
	CMarkCategory* GetCategory();
	BYTE DisplayMode;
  int Category;
  CString ShortText;
  CString LongText;

	virtual void GetZoomedRect(CRect* r);
	virtual void Serialize(CArchive& archive);
  CContentMark(CContent& Parent);
  virtual ~CContentMark() {};
};

//----------------------------------------------------------------------------

// Content draw options
#define cdoHideLarge 0x01 
#define cdoHideSmall 0x02
#define cdoSimplify  0x40
#define cdoHideAll   0x80

#define MarkWd 20
#define MarkHt 20

typedef BOOL (*TOnDrawObject)(CContentObj *obj, CRect &rect, void *UserData);

class CContent : public CObject
{
public:
  CContentStyles Styles;
	CContentObjects Objects;
	int Zoom;

public:
  BOOL IsVisibleUser(CContentObj* obj, CRect &r) { return (!m_OnDrawObject || m_OnDrawObject(obj, r, m_UserData)); };
	void SetOnDrawObject(TOnDrawObject proc, void* UserData);
	BYTE DrawOptions;
	CWnd* m_wnd;
	CMarkCategories MarkCategories;
	CImageList MarkImages;
	BOOL Modified;
	CString FileName;
	void Clear();
	BOOL LoadFromFile(const CString &FileName);
	BOOL SaveToFile(const CString& FileName);
	CContentStyle DefStyle;
	CContentStyle MarksShortText;
	CContentStyle MarksLongText;
	CContentObj* GetObjectAt(CPoint pt, int Plane, BYTE ObjectType = 0);
	void SetZoom(int NewZoom);
	void Draw(CDC& dc, CRect& Rect, CPoint& Origin, int Plane);
	void Serialize(CArchive& archive);
	CContent();
	virtual ~CContent();

private:
	BYTE m_ArchVer;
  TOnDrawObject m_OnDrawObject;
  void *m_UserData;
};

extern CFont g_SimpleFont;

#endif // !defined(AFX_CONTENT_H__34EE6C7C_7402_4626_9670_F80A3CA56873__INCLUDED_)
