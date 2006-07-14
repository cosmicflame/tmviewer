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

// Mapper.cpp: implementation of the CMapper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <ASSERT.H>
#include "TMViewer.h"
#include "Mapper.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

typedef struct tagBITMAPINF {
    BITMAPINFOHEADER bmiHeader;
    RGBQUAD          bmiColors[256];
} BITMAPINF;

BITMAPINF g_tm_bmi = {{sizeof(BITMAPINFOHEADER), 128, -128, 1, 8, BI_RGB, 0, 0, 0, 256, 256},
 {0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //0
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //4
#ifdef MY_DEBUG
  0,0,0,0, 0,0,0,0,  1,0,0,0, 0,0,0,0,           //8 
#else
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //8 
#endif
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //C
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //10
  0,0,0,0, 0,0,0,0,  0,102,0,0, 0,0,0,0,           //14
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //18
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //1C
  0,0,0,0, 0,0,0,0,  0,204,0,0, 0,0,0,0,           //20
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //24
  0,255,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //28
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //2C
  0,0,0,0, 0,0,0,0,  204,0,51,0, 0,0,0,0,           //30
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //34
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //38
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //3C
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //40
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //44
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //48
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //4C
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //50
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //54
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //58
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //5C
  102,102,102,0, 0,0,0,0,  204,102,102,0, 0,0,0,0,           //60
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //64
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //68
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //6C
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //70
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //74
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //78
  0,51,153,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //7C
  0,0,0,0, 0,0,0,0,  0,0,0,0, 51,102,153,0,           //80    
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //84
  0,0,0,0, 0,0,0,0,  0,0,0,0, 153,153,153,0,           //88
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //8C
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //90
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //94
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //98
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //9C
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //A0
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //A4
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //A8
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //AC
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //B0
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //B4
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //B8
  0,0,0,0, 255,255,204,0,  0,0,0,0, 0,0,0,0,           //BC
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //C0
  0,51,255,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //C4
  0,0,0,0, 0,0,0,0,  0,102,255,0, 0,0,0,0,           //C8
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //CC
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //D0
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //D4
  0,0,0,0, 153,204,255,0,  0,0,0,0, 0,0,0,0,           //D8
  0,255,255,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //DC
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //E0
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //E4
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //E8
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //EC
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //F0
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //F4
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //F8
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0            //FC
}};

// Palette for version 2

BITMAPINF g_tm_bmi2 = {{sizeof(BITMAPINFOHEADER), 128, -128, 1, 8, BI_RGB, 0, 0, 0, 256, 256},
 {0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //0
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //4
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //8 
  0,102,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //C
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //10
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //14
  0,204,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //18
  0,0,0,0, 0,0,0,0,  0,255,0,0, 0,0,0,0,           //1C
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //20
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //24
  204,0,51,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //28
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //2C
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //30
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //34
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //38
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //3C
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //40
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //44
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //48
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //4C
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //50
  0,0,0,0, 0,0,0,0,  102,102,102,0, 0,0,0,0,           //54
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //58
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //5C
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //60
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //64
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //68
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //6C
  0,0,0,0, 0,0,0,0,  0,51,153,0, 0,0,0,0,           //70
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //74
  0,0,0,0, 51,102,153,0,  0,0,0,0, 0,0,0,0,           //78
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //7C
  0,0,0,0, 153,153,153,0,  0,0,0,0, 0,0,0,0,           //80    
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //84
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //88
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //8C
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //90
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //94
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //98
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //9C
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //A0
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //A4
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //A8
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //AC
  0,0,0,0, 0,0,0,0,  0,0,0,0, 255,255,204,0,           //B0
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //B4
  0,0,0,0, 0,0,0,0,  0,51,255,0, 0,0,0,0,           //B8
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //BC
  0,102,255,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //C0
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //C4
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //C8
  0,0,0,0, 0,0,0,0,  0,0,0,0, 153,204,255,0,           //CC
  0,0,0,0, 0,0,0,0,  0,255,255,0, 0,0,0,0,           //D0
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //D4
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //D8
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //DC
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //E0
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //E4
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //E8
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //EC
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //F0
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //F4
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0,           //F8
  0,0,0,0, 0,0,0,0,  0,0,0,0, 0,0,0,0            //FC
}};

#define DecClr(clr, val) (clr < val) ? clr = 0 : clr -= val

#define DivideSize 32

#define CalcVisCoord(x, y, obj) (y - obj->m_MapRange.top)/DivideSize*(obj->m_MapRange.Width()+1)/DivideSize + (x - obj->m_MapRange.left)/DivideSize

BOOL OnDrawObject(CContentObj *obj, CRect &rect, void *UserData)
{
  int x, y;
  CPoint pt;
  CRect r;
  CMapper &map = *((CMapper*)UserData);
  BYTE t;

  t = obj->GetType();
  if ((map.HideMarks && t == otMark) || (map.HideTextLabels && t == otTextLabel))
    return FALSE;

  if (!map.HideUnexplored)
    return TRUE;

  if (map.m_VisBuf && r.IntersectRect(map.m_MapRange, rect)) {
    for (y = rect.top; y < rect.bottom; y += DivideSize) 
      for (x = rect.left; x < rect.right; x += DivideSize) 
        if (map.m_VisBuf[CalcVisCoord(x, y, (&map))] == 1)
          return TRUE;
  }

  return FALSE;
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMapper::CMapper() : Overlay(true), Filtering(true), Shading(4), m_ShowSquares(FALSE),
  HideUnexplored(TRUE), HideTextLabels(FALSE), HideMarks(FALSE), m_VisBuf(NULL)
{
  gMapTable = (BYTE*)malloc(256);
  m_PBInfo = (BITMAPINFO*)malloc(sizeof(g_tm_bmi));

  Content.SetOnDrawObject(&OnDrawObject, this);
  m_MapRange.SetRectEmpty();
  PosX = 250*128;
  PosY = 250*128;
  m_iZoom = 100;
  m_iMaxLevel = 0;
  m_MapWd = 0;
  m_MapHt = 0;
  m_MapVersion = 0;
  m_MapBuf = NULL;
  m_wnd = NULL;
}

CMapper::~CMapper()
{
  free(m_PBInfo);
  free(gMapTable);
  free(m_MapBuf);
  if (m_VisBuf) 
    free(m_VisBuf);
}

int CMapper::LevelToInt(CString& level)
{
  char lev;
  int res = 0;

  switch(m_MapVersion) {
  case 1:
    lev = level[0];
	  if (lev >= '0' && lev <= '9')
		  res = -(int)(lev - '0');
	  else {
		  CharUpperBuff(&lev, 1);
		  res = (int)(lev - '@');
	  }
    break;
  case 2:
    res = 7 - atoi(level);
  }
  return res;
}

CString CMapper::LevelToStr(int level)
{
  CString res;

  switch(m_MapVersion) {
  case 1:
	  if (level <= 0)
		  res = char((-level) + '0');
	  else
		  res = char((level) + '@');
    break;
  case 2:
    res.Format("%02d", 7 - level);
  }
  return res;
}

BOOL CMapper::LoadMap(int x, int y, int Level)
{
  FILE *mf;
  int i, j;
  BYTE* buf;

  if (x*m_MapWd >= m_MapRange.right || y*m_MapHt >= m_MapRange.bottom) 
    return FALSE;

  if (Level == 1000)
    Level = m_iLevel;
  mf = fopen(m_sMapsPath + GetMapFileName(x, y, Level), "rb");
  if (!mf) return FALSE;
  if (m_MapVersion == 2) 
    j = m_MapWd*m_MapHt*2;
  else
    j = m_MapWd*m_MapHt;
  i = fread(m_MapBuf, 1, j, mf); 
  fclose(mf);
  if (i != j) return FALSE;

  if (m_MapVersion == 2) {
    BYTE *buf2;
    buf = m_MapBuf;
    for (j = 0; j < m_MapHt ; j++) {
      buf2 = (BYTE*)bm_data + j;
      for (i = m_MapWd; i > 0 ; i--) {
        *buf2 = *buf;
        buf++;
        buf2 += m_MapHt;
      }
    }
  }
  else
    memcpy(bm_data, m_MapBuf, j);

  return TRUE;
}

BOOL CMapper::LoadMapEx(int x, int y, int Level)
{
  if (!Overlay || Level == 0) 
    return LoadMap(x, y, Level);

  BOOL res = FALSE;
  int i, j, k;
  BYTE *srcbuf = (BYTE*)bm_data;
  BYTE *buf = (BYTE*)calloc(m_MapWd*m_MapHt, 1);

  if (Level == 1000)
    Level = m_iLevel;

  i = 0;
  while (1) {
    if (LoadMap(x, y, i)) {
      if (i == Level && !res)
        memcpy(buf, srcbuf, m_MapWd*m_MapHt);
      else {
        k = m_MapWd*m_MapHt;
        for (j = 0; j < k; j++) {
          if (srcbuf[j] != 0 && (m_MapVersion != 1 || srcbuf[j] != 0xA))
            if (i == Level)
              buf[j] = srcbuf[j];
            else
              buf[j] = gMapTable[srcbuf[j]];
        } 
      }
      res = TRUE;
    }
    if (i == Level) break;
    if (Level > 0) 
      i++;
    else
      i = Level;
  }
  if (res)
    memcpy(srcbuf, buf, m_MapWd*m_MapHt);

  free(buf);
  return res;
}

void CMapper::InitBmp()
{
  unsigned int i, j, k;
  BITMAPINF* bmi;

  switch(m_MapVersion) {
  case 2:
    bmi = &g_tm_bmi2;
    break;
  case 1:
  default:
    bmi = &g_tm_bmi;
  }

  memcpy(m_PBInfo, bmi, sizeof(g_tm_bmi));
  m_PBInfo->bmiHeader.biWidth = m_MapWd;
  m_PBInfo->bmiHeader.biHeight = -m_MapHt;

  k = 0;
  for (i = 0; i < bmi->bmiHeader.biClrUsed; i++) {
    j = bmi->bmiColors[i].rgbBlue + bmi->bmiColors[i].rgbGreen + bmi->bmiColors[i].rgbRed;
    if (j == 0) continue;
    do {
      k++;
      assert(k < 256);
    }
    while (k == 0xA 
         || m_PBInfo->bmiColors[k].rgbBlue != 0 
         || m_PBInfo->bmiColors[k].rgbGreen != 0
         || m_PBInfo->bmiColors[k].rgbRed != 0);

    gMapTable[i] = k;

    j = j*10 / ((Shading + 3)*10/2);
    if (j > 255)
      j = 255; 
    m_PBInfo->bmiColors[k].rgbBlue = j;

//    m_PBInfo->bmiColors[k].rgbRed = j*(10 - Shading)/10;
//    m_PBInfo->bmiColors[k].rgbGreen = j*(10 - Shading)/10;

    m_PBInfo->bmiColors[k].rgbRed = j/2;
    m_PBInfo->bmiColors[k].rgbGreen = j/2;
  }

#ifdef MY_DEBUG
  for (i = 1; i < m_PBInfo->bmiHeader.biClrUsed; i++) {
    if (m_PBInfo->bmiColors[i].rgbBlue == 0 
      && m_PBInfo->bmiColors[i].rgbGreen == 0
      && m_PBInfo->bmiColors[i].rgbRed == 0) 
    {
#ifndef _DEBUG
      m_PBInfo->bmiColors[i].rgbBlue = 255;
      m_PBInfo->bmiColors[i].rgbGreen = 255;
      m_PBInfo->bmiColors[i].rgbRed = 255;
#else
      m_PBInfo->bmiColors[i].rgbBlue = rand()*255/RAND_MAX;
      m_PBInfo->bmiColors[i].rgbGreen = rand()*255/RAND_MAX;
      m_PBInfo->bmiColors[i].rgbRed = rand()*255/RAND_MAX;
#endif
    }
  }
#endif

  m_PartMap.CreateCompatibleDC(NULL);
  m_bmp.Attach(CreateDIBSection(m_PartMap, m_PBInfo, DIB_RGB_COLORS, &bm_data, 0, 0));
  m_PartMap.SelectObject(m_bmp);

  switch (m_MapVersion) {
  case 1:
    m_MapBuf = (BYTE*)malloc(m_MapWd*m_MapHt);
    break;
  default:
    m_MapBuf = (BYTE*)malloc(m_MapWd*m_MapHt*2);
  }
/*
  m_MemDC2.CreateCompatibleDC(NULL);
  m_MemBmp2.CreateCompatibleBitmap(CDC::FromHandle(GetDC(0)), m_MapWd, m_MapHt);
  m_MemDC2.SelectObject(&m_MemBmp2);
*/  
}

void CMapper::DrawMap(CDC& dc, CRect *paint_rect, CRgn *upd_rgn)
{
  int wd, ht, xx, yy, mfx, mfy, i, j, a, b;
  CRect r, rr, cr, rb;
  DWORD tc;
  CWaitCursor *wc = NULL;

  if (m_iZoom < 100 && Filtering) 
    dc.SetStretchBltMode(HALFTONE);
  else
    dc.SetStretchBltMode(COLORONCOLOR);

  wd = m_MapWd*m_iZoom/100;
  ht = m_MapHt*m_iZoom/100;
  mfy = PosY / m_MapHt;
  yy = paint_rect->top - (PosY % m_MapHt)*m_iZoom/100;

  tc = GetTickCount();
/*
  if (HideUnexplored) 
    m_VisMap.RemoveAll();
*/
  upd_rgn->GetRgnBox(rb);

  while (yy < paint_rect->bottom) {
    mfx = PosX / m_MapWd;
    xx = paint_rect->left - (PosX % m_MapWd)*m_iZoom/100;
    while (xx < paint_rect->right) {
      r.left = xx;
      r.top = yy;
      r.right = xx + wd;
      r.bottom = yy + ht;
      if (upd_rgn->RectInRegion(&r)) {
        if (tc && GetTickCount() - tc > 300) {
          wc = new CWaitCursor;
          tc = 0;
        }
        if (LoadMapEx(mfx, mfy)) {
          if (HideUnexplored) {
            BYTE *Buf;

            for (i = 0; i < m_MapHt; i += DivideSize) {
              for (j = 0; j < m_MapWd; j += DivideSize) {
                Buf = ((BYTE*)bm_data) + i*m_MapWd + j;
                b = DivideSize;
                do {
                  a = DivideSize;
                  do {
                    if (*Buf != 0) {
                      m_VisBuf[CalcVisCoord(mfx*m_MapWd + j, mfy*m_MapHt + i, this)] = 1;
                      goto m1;
                    }
                    Buf++;
                    a--;
                  }
                  while (a > 0);
                  Buf += m_MapWd - DivideSize;
                  b--;
                }
                while (b > 0);
m1:             ;
              }
            }
          }
          cr.IntersectRect(r, rb);
          if (m_iZoom != 75) {
            rr.left = (cr.left - r.left)*100/m_iZoom;
            rr.top = (cr.top - r.top)*100/m_iZoom;
            rr.right = ((cr.right - r.left)*100 + m_iZoom - 1)/m_iZoom;
            rr.bottom = ((cr.bottom - r.top)*100 + m_iZoom - 1)/m_iZoom;
            cr.left = r.left + rr.left*m_iZoom/100;
            cr.right = r.left + rr.right*m_iZoom/100;
            cr.top = r.top + rr.top*m_iZoom/100;
            cr.bottom = r.top + rr.bottom*m_iZoom/100;
            dc.StretchBlt(cr.left, cr.top, cr.Width(), cr.Height(), &m_PartMap, rr.left, rr.top, rr.Width(), rr.Height(), SRCCOPY);
          }
          else {
            dc.StretchBlt(xx, yy, wd, ht, &m_PartMap, 0, 0, m_MapWd, m_MapHt, SRCCOPY);
/*
            cr.left = cr.left/3*3; 
            cr.right = (cr.right+2)/3*3;
            cr.top = cr.top/3*3; 
            cr.bottom = (cr.bottom+2)/3*3;

            rr.left = (cr.left - r.left)*100/m_iZoom;
            rr.top = (cr.top - r.top)*100/m_iZoom;
            rr.right = ((cr.right - r.left)*100)/m_iZoom;
            rr.bottom = ((cr.bottom - r.top)*100)/m_iZoom;
*/
          }
        }
        else
          FillRect(dc, r, (HBRUSH)GetStockObject(BLACK_BRUSH));

        if (m_ShowSquares) {
          r.left--;
          r.top--;
          FrameRect(dc, r, (HBRUSH)GetStockObject(WHITE_BRUSH));
        }
      }
      xx += wd;
      mfx++;
    }
    yy += ht;
    mfy++;
  }

  cr = rb;
  cr.left = cr.left*100/m_iZoom;
  cr.top = cr.top*100/m_iZoom;
  cr.right = cr.right*100/m_iZoom;
  cr.bottom = cr.bottom*100/m_iZoom;
  cr.InflateRect(1, 1);
  cr.OffsetRect(PosX, PosY);
  Content.Draw(dc, cr, CPoint(PosX, PosY), m_iLevel);

  if (wc)
    delete wc;
}

void CMapper::SetWnd(CWnd &wnd)
{
  m_wnd = &wnd;
  Content.m_wnd = &wnd;
  m_wnd->Invalidate(FALSE);
}

void CMapper::ScrollMap(int dx, int dy)
{
  int x, y, i;
  CRect r;

  if (m_wnd == NULL) return;
  m_wnd->GetClientRect(&r);

  if (!m_XnoScroll) {
    x = PosX + dx;
    if (x < m_MapRange.left)
      x = m_MapRange.left;
    i = r.Width()*100/m_iZoom;
    if (i > m_MapRange.Width())
      i = m_MapRange.Width();
    i = m_MapRange.right - i;
    if (x > i)
      x = i;
    PosX = x;
  }

  if (!m_YnoScroll) {
    y = PosY + dy;
    if (y < m_MapRange.top)
      y = m_MapRange.top;
    i = r.Height()*100/m_iZoom;
    if (i > m_MapRange.Height())
      i = m_MapRange.Height();
    i = m_MapRange.bottom - i;
    if (y > i)
      y = i;
    PosY = y;
  }
  x = PosX*m_iZoom/100;
  y = PosY*m_iZoom/100;
  dx = m_iWndX - x;
  dy = m_iWndY - y;
  m_wnd->ScrollWindow(dx, dy);
  m_iWndX = x;
  m_iWndY = y;
  UpdateScrollBars();
}

void CMapper::CalcMapRange()
{
  CFileFind finder;
  int i;
  CString s;

  m_MapRange.SetRectEmpty();
  if (m_VisBuf) {
    free(m_VisBuf);
    m_VisBuf = NULL;
  }
  m_iMaxLevel = 0;
  m_iMinLevel = 0;
  m_MapWd = 0;
  m_MapHt = 0;
  m_MapVersion = 0;
  BOOL bWorking = finder.FindFile(m_sMapsPath + "*.map");
  while (bWorking)
  {
    bWorking = finder.FindNextFile();
    s = finder.GetFileTitle();
    switch (s.GetLength()) {
    case 7:
      if (m_MapWd == 0) {
        m_MapWd = 128;
        m_MapHt = 128;
        m_MapVersion = 1;
      }
      if (m_MapVersion != 1) 
        continue;
      break;
    case 8:
/*
      if (m_MapVersion == 0) {
        m_wnd->Invalidate(FALSE);
        AfxMessageBox("This folder contains map files of the new Tibia client.\rThe current version of viewer does not support it.\rPlease upgrade your copy of Tibia Map Viewer.", MB_ICONERROR);
        return;
      }
*/
      if (m_MapVersion != 2) 
        if (m_MapWd == 0) {
          m_MapWd = 0x100;
          m_MapHt = 0x100;
          m_MapVersion = 2;
          m_MapRange.SetRectEmpty();
        }
      break;
    default:
      continue;
    }

    i = atoi(s.Left(3)) * m_MapWd;
    if (i == 0)
      continue;
    if (i < m_MapRange.left || m_MapRange.left == 0)
      m_MapRange.left = i;
    i += m_MapWd - 1;
    if (i > m_MapRange.right)
      m_MapRange.right = i;

    i = atoi(s.Mid(3, 3)) * m_MapHt;
    if (i == 0)
      continue;
    if (i < m_MapRange.top || m_MapRange.top == 0)
      m_MapRange.top = i;
    i += m_MapHt - 1;
    if (i > m_MapRange.bottom)
      m_MapRange.bottom = i;

    switch(m_MapVersion) {
    case 1:
      i = LevelToInt(s.Mid(6, 1));
      break;
    case 2:
    default:
      i = LevelToInt(s.Mid(6, 2));
    }
    if (i > m_iMaxLevel)
      m_iMaxLevel = i;
    if (i < m_iMinLevel)
      m_iMinLevel = i;
  }
/*
  if (m_MapVersion == 0) {
    m_MapVersion = 1;
    m_MapWd = 128;
    m_MapHt = 128;
  }
*/
  if (m_MapVersion == 0) {
    m_MapVersion = 2;
    m_MapWd = 256;
    m_MapHt = 256;
  }
  if (!m_MapRange.IsRectEmpty()) {
    m_VisBuf = (BYTE*)calloc((m_MapRange.Width() + 1)/DivideSize*(m_MapRange.Height()+1)/DivideSize, 1);
  }
  UpdateScrollBars();
  ReInit();
}

void CMapper::UpdateScrollBars()
{
  SCROLLINFO si;
  CRect r;

  if (m_wnd == NULL) return;

  m_wnd->GetClientRect(r);

  si.cbSize = sizeof(SCROLLINFO);
  si.fMask = SIF_PAGE | SIF_POS | SIF_RANGE;
  si.nMin = m_MapRange.top;
  si.nMax = m_MapRange.bottom - 1;
  si.nPage = r.Height()*100/m_iZoom;
  si.nPos = PosY;

  if ((int)si.nPage > m_MapRange.Height()) {
    m_YnoScroll = TRUE;
    PosY = m_MapRange.top - (si.nPage - m_MapRange.Height())/2;
    m_iWndY = PosY*m_iZoom/100;
  }
  else
    m_YnoScroll = FALSE;
  
  m_wnd->SetScrollInfo(SB_VERT, &si);

  m_wnd->GetClientRect(r);

  si.nMin = m_MapRange.left;
  si.nMax = m_MapRange.right - 1;
  si.nPage = r.Width()*100/m_iZoom;
  si.nPos = PosX;

  if ((int)si.nPage > m_MapRange.Width()) {
    m_XnoScroll = TRUE;
    PosX = m_MapRange.left - (si.nPage - m_MapRange.Width())/2;
    m_iWndX = PosX*m_iZoom/100;
  }
  else
    m_XnoScroll = FALSE;

  m_wnd->SetScrollInfo(SB_HORZ, &si);
}

void CMapper::SetZoom(int Zoom)
{
  m_iZoom = Zoom;
  m_iWndX = PosX*m_iZoom/100;
  m_iWndY = PosY*m_iZoom/100;
  Content.SetZoom(Zoom);
  InitMemBitmap();
}

void CMapper::SetMapsPath(LPCSTR NewPath)
{
  m_sMapsPath = NewPath;
  int len = m_sMapsPath.GetLength();
  if (len != 0)
    if (m_sMapsPath[len - 1] != '\\')
      m_sMapsPath += '\\';
}

#define GroundStr "Ground"

CString CMapper::GetLevelStr(int Level)
{
	CString res;

	if (Level == 0)
		res = GroundStr;
	else {
		itoa(Level, res.GetBuffer(10), 10);
		res.ReleaseBuffer();
		if (Level > 0)
			res = '+' + res;
	}
  return res;
}

void CMapper::SetLevelStr(const CString Level)
{
  int i;
	if (Level == GroundStr)
		i = 0;
	else
		i = atoi(Level);
  SetLevel(i);
}
/*
bool CMapper::ExportToBmp(const CString FileName, BOOL ExportSymbols, BOOL ExportZoom)
{
  CWaitCursor wc;
  FILE *fp;
  BITMAPFILEHEADER hdr;
  BITMAPINF *bi;
  bool res;
  int x, y, mx, my, i;
  long data_offs;
  char *data;

  fp = fopen(FileName, "wb");
  if (!fp) return false;

  bi = (BITMAPINF*)malloc(sizeof(BITMAPINF));
  memcpy(bi, m_PBInfo, sizeof(BITMAPINF));

  bi->bmiHeader.biWidth = m_MapRange.right - m_MapRange.left + 1;
  bi->bmiHeader.biHeight = m_MapRange.bottom - m_MapRange.top + 1;

  data = (char*)malloc(m_MapHt*bi->bmiHeader.biWidth);

  // Fill in the fields of the file header 
  hdr.bfType          = BFT_BITMAP;
  hdr.bfSize          = sizeof(BITMAPINF) + sizeof(BITMAPFILEHEADER) + bi->bmiHeader.biWidth*bi->bmiHeader.biHeight;
  hdr.bfReserved1     = 0;
  hdr.bfReserved2     = 0;
  hdr.bfOffBits       = sizeof(BITMAPINF);

  data_offs = hdr.bfOffBits + sizeof(BITMAPFILEHEADER);

  res = false;
  if (fwrite(&hdr, sizeof(hdr), 1, fp) != 1) goto m2;
  if (fwrite(bi, sizeof(BITMAPINF), 1, fp) != 1) goto m2;
  
  my = m_MapRange.top / m_MapHt;
  mx = m_MapRange.right / m_MapWd;
  for (y = m_MapRange.bottom / m_MapHt; y >= my; y--) {
    for (x = m_MapRange.left / m_MapWd; x <= mx; x++) {
      if (!LoadMapEx(x, y)) 
        memset(bm_data, 0, m_MapWd*m_MapHt);
      for (i = 0; i < m_MapHt; i++) 
        memcpy(data + (m_MapHt - 1 - i)*bi->bmiHeader.biWidth + (x*m_MapWd - m_MapRange.left), (char*)bm_data + i*m_MapHt, m_MapWd);
    }
    if (fwrite(data, m_MapHt*bi->bmiHeader.biWidth, 1, fp) != 1) goto m2;
  }
  res = true;
m2:
  free(data);
  free(bi);
  fclose(fp);
  return res;
}
*/
void CMapper::ReInit()
{
  m_PartMap.DeleteDC();
  m_bmp.DeleteObject();
  free(m_MapBuf);
/*
  m_MemDC2.DeleteDC();
  m_MemBmp.DeleteObject();
*/
  InitBmp();
  InitMemBitmap();
}

CString CMapper::GetMapFileName(int x, int y, int Level)
{
  CString res;
  res.Format("%03d%03d%s.map", x, y, LevelToStr(Level));
  return res;
}

BOOL CMapper::SaveMapAs(int x, int y, int Level)
{
  FILE *mf;
  int i, j;

  mf = fopen(m_sMapsPath + GetMapFileName(x, y, Level), "wb");
  if (!mf) return FALSE;

  if (m_MapVersion == 2) 
    j = m_MapWd*m_MapHt*2;
  else
    j = m_MapWd*m_MapHt;

  i = fwrite(m_MapBuf, 1, j, mf); 
  fclose(mf);
  if (i != j) return FALSE;

  return TRUE;
}

void CMapper::InitMemBitmap()
{
/*
  if (m_MemDC.m_hDC)
    m_MemDC.DeleteDC();
  if (m_MemBmp.m_hObject)
    m_MemBmp.DeleteObject();
  m_MemDC.CreateCompatibleDC(NULL);
  m_MemBmp.CreateCompatibleBitmap(CDC::FromHandle(GetDC(0)), m_MapWd*m_iZoom/100, m_MapHt*m_iZoom/100);
  m_MemDC.SelectObject(&m_MemBmp);
*/
}

void CMapper::SetLevel(int Level)
{
	m_iLevel = Level;
  if (m_VisBuf)
    memset(m_VisBuf, 0, (m_MapRange.Width() + 1)/DivideSize*(m_MapRange.Height() + 1)/DivideSize);
}

#define BFT_BITMAP 0x4d42   /* 'BM' */

bool CMapper::ExportToBmpEx(const CString &FileName, BOOL ExportSymbols, BOOL ExportZoom)
{
  CWaitCursor wc;
  FILE *fp;
  BITMAPFILEHEADER hdr;
  BITMAPINFO *bi = NULL;
  bool res;
  char *data;
  CBitmap bmp;
  CDC dc;
  CRgn rgn;
  CRect rect;
  int PartHt, sz;

  CPoint OldPos;
  int OldZoom = m_iZoom;
  BYTE OldDrawOptions = Content.DrawOptions;
  BOOL HiColor = ExportSymbols == TRUE;
  int bmiSize;

  fp = fopen(FileName, "wb");
  if (!fp) return false;

  if (!ExportZoom)
    SetZoom(100);
  if (!ExportSymbols) 
    Content.DrawOptions |= cdoHideAll;

  PartHt = m_MapHt*m_iZoom/100;

  if (HiColor) {
    bmiSize = sizeof(BITMAPINFOHEADER);
    bi = (BITMAPINFO*)calloc(bmiSize, 1);
    bi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bi->bmiHeader.biPlanes = 1;
    bi->bmiHeader.biBitCount = 16;
    bi->bmiHeader.biCompression = BI_RGB;
  }
  else {
    bmiSize = sizeof(BITMAPINF);
    bi = (BITMAPINFO*)malloc(bmiSize);
    memcpy(bi, m_PBInfo, sizeof(BITMAPINF));
  }

  bi->bmiHeader.biWidth = (m_MapRange.right - m_MapRange.left + 1)/m_MapWd*(m_MapWd*m_iZoom/100);
  bi->bmiHeader.biHeight = PartHt;

  dc.CreateCompatibleDC(NULL);
  bmp.Attach(CreateDIBSection(dc, bi, DIB_RGB_COLORS, (void **)&data, 0, 0));
  dc.SelectObject(bmp);

  bi->bmiHeader.biHeight = (m_MapRange.bottom - m_MapRange.top + 1)/m_MapHt*PartHt;

  /* Fill in the fields of the file header */
  hdr.bfType          = BFT_BITMAP;
  hdr.bfSize = bi->bmiHeader.biWidth*bi->bmiHeader.biHeight;
  if (HiColor)
    hdr.bfSize *= 2;
  hdr.bfSize          = hdr.bfSize + bmiSize + sizeof(BITMAPFILEHEADER);
  hdr.bfReserved1     = 0;
  hdr.bfReserved2     = 0;
  hdr.bfOffBits       = bmiSize;

  res = false;
  if (fwrite(&hdr, sizeof(hdr), 1, fp) != 1) goto m2;
  if (fwrite(bi, bmiSize, 1, fp) != 1) goto m2;

  rect.SetRect(0, 0, bi->bmiHeader.biWidth, PartHt);
  rgn.CreateRectRgn(0, 0, bi->bmiHeader.biWidth, PartHt);
  
  OldPos.x = PosX;
  OldPos.y = PosY;
  PosX = m_MapRange.left;
  sz = PartHt*bi->bmiHeader.biWidth;
  if (HiColor)
    sz *= 2;
  for (PosY = m_MapRange.bottom - m_MapHt; PosY >= m_MapRange.top - 1; PosY -= m_MapHt) {
    DrawMap(dc, &rect, &rgn);
    if (fwrite(data, sz, 1, fp) != 1) goto m3;
  }
  res = true;
m3:
  PosX = OldPos.x;
  PosY = OldPos.y;
m2:
  if (!ExportZoom)
    SetZoom(OldZoom);
  if (!ExportSymbols) 
    Content.DrawOptions = OldDrawOptions;
  if (bi)
    free(bi);
  fclose(fp);
  return res;

}
