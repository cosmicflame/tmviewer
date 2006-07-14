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

// Mapper.h: interface for the CMapper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPPER_H__EC815C81_536B_4D42_BE95_AD2FBFFFF326__INCLUDED_)
#define AFX_MAPPER_H__EC815C81_536B_4D42_BE95_AD2FBFFFF326__INCLUDED_

#include "Content.h"	// Added by ClassView
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMapper  
{
private:
	BOOL m_YnoScroll;
	BOOL m_XnoScroll;
  BYTE *gMapTable;
  BITMAPINFO* m_PBInfo;
	void InitBmp();

public:
	bool ExportToBmpEx(const CString& FileName, BOOL ExportSymbols, BOOL ExportZoom);
	void SetLevel(int Level);
	BYTE* m_VisBuf;
	BOOL HideMarks;
	BOOL HideTextLabels;
	BOOL HideUnexplored;
	void InitMemBitmap();
	BOOL SaveMapAs(int x, int y, int Level);
	CContent Content;
	BOOL m_ShowSquares;
	CString GetMapFileName(int x, int y, int Level);
  BOOL IsEmpty() { return m_MapRange.IsRectNull(); };
	int m_MapVersion;
	int m_MapHt;
	int m_MapWd;
	int Shading;
	void ReInit();
	BOOL Overlay;
  BOOL Filtering;
//	bool ExportToBmp(const CString FileName, BOOL ExportSymbols, BOOL ExportZoom);
	void SetLevelStr(const CString Level);
	CString GetLevelStr(int Level);
	void SetMapsPath(LPCSTR NewPath);
	int m_iMaxLevel;
	int m_iMinLevel;
	CRect m_MapRange;
	CWnd *m_wnd;
  int m_iWndX;
  int m_iWndY;
	CString m_sMapsPath;
  CDC m_PartMap;
  CBitmap m_bmp;
  void *bm_data;
	BYTE* m_MapBuf;
/*
  CBitmap m_MemBmp;
  CDC m_MemDC;
*/
/*
  CBitmap m_cbmp;
  CDC m_cPartMap;
  CBitmap m_MaskBmp;
  CDC m_MaskBmpDC;
  CBitmap m_DarkBmp;
  CDC m_DarkBmpDC;
  void *DarkBM_data;
  void *MaskBM_data;
*/
  int m_iLevel;
  int m_iZoom;
  int PosX;
  int PosY;

	void SetZoom(int Zoom);
	void UpdateScrollBars();
	void CalcMapRange();
	void ScrollMap(int dx, int dy);
	BOOL LoadMap(int x, int y, int Level = 1000);
	BOOL LoadMapEx(int x, int y, int Level = 1000);
	void SetWnd(CWnd &wnd);
	void DrawMap(CDC& dc, CRect *paint_rect, CRgn *upd_rgn);
  int LevelToInt(CString& level);
  CString LevelToStr(int level);
	CMapper();
	virtual ~CMapper();
};

#endif // !defined(AFX_MAPPER_H__EC815C81_536B_4D42_BE95_AD2FBFFFF326__INCLUDED_)
