/*
 * Copyright (C) 2018 ARNERI arneri@ukr.net All rights reserved
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.

 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * Abstract: 
 */



#ifndef __GFXGRAFIX_H__
#define __GFXGRAFIX_H__

#include "stdgfx.h"
#include "gfxanim.h"
#include "gfxbmp.h"
#include "gfxcltbl.h"
#include "gfxddraw.h"
#include "gfxflic.h"
#include "gfxgdi.h"
#include "gfxgif.h"
#include "gfxgpi.h"
#include "gfxinput.h"
#include "gfxllist.h"
#include "gfxtimer.h"
#include "gfxpcx.h"
#include "gfxvesa.h"
#include "gfxxwin.h"


#define UNKNOWN   0
#define PCXFILE   1
#define GIFFILE   2
#define BMPFILE   3


#define PAL_MSWIN 1   
#define PAL_PSP   2   
#define PAL_COREL 3   


#define PAL_FADE_IN  1
#define PAL_FADE_OUT 2


#define FLIC_NEXT      ((LONG)1<<0)
#define FLIC_LOOP      ((LONG)1<<1)
#define FLIC_NOADVANCE ((LONG)1<<2)
#define FLIC_ALL       ((LONG)1<<3)

typedef BOOLEAN (*FLICCallback) ( FLICFILE *Flic );

struct XYPOINT
  {
    LONG x,y;
    LONG u,v;
  }; 

struct POLYGON
  {
    LONG NumPoints;
    XYPOINT *Points;

    POLYGON ();
    ~POLYGON ();

    VOID CreatePoints ( LONG Num );
    VOID DestroyPoints ();
  }; 

struct EDGETABLE
  {
    LONG Top;
    LONG Height;
    XYPOINT *Left;
    XYPOINT *Right;

    EDGETABLE ();
    ~EDGETABLE ();
  }; 


class GRAFIX : public DCIClass
  {
    protected :
      DISPLAYDRIVER *DisplayDriver;
      LONG MagH,MagV,DivH,DivV;

      INT GetImageFileType ( FILEHANDLE f );
      BOOLEAN ClipRect ( RECTANGLE *ViewPort, LONG *Sx, LONG *Sy, LONG *Wd, LONG *Ht );
      BOOLEAN ClipDest ( RECTANGLE *ViewPort, LONG *Sx, LONG *Sy, LONG *Wd, LONG *Ht,
                         LONG *Cx, LONG *Cy );
      LONG GetCode ( RECTANGLE Rect, LONG x, LONG y );
      BOOLEAN ClipLine ( RECTANGLE Rect, LONG *x1, LONG *y1, LONG *x2, LONG *y2 );
      VOID DrawEllipsePoint ( IMAGE *Dest, LONG Cx, LONG Cy, LONG x, LONG y );
      VOID DrawHorizLine ( IMAGE *Dest, LONG x1, LONG x2, LONG y );
      VOID FillEllipsePoint ( IMAGE *Dest, LONG Cx, LONG Cy, LONG x, LONG y );
      VOID FindIntersect ( XYPOINT *In, XYPOINT *Out, XYPOINT *I, RECTANGLE ViewPort, LONG Code );
      VOID CreateEdgeTable ( POLYGON *Poly, EDGETABLE *EdgeTable );     
      VOID ClipPolygon ( POLYGON *SrcPoly, POLYGON *ClipPoly, RECTANGLE ViewPort );

    public :
      COLOR FGColor;
      COLOR BGColor;

      GRAFIX ();
      virtual ~GRAFIX ();

      virtual BOOLEAN Init ();
      virtual VOID DeInit ();

      BOOLEAN SetDisplay ( DISPLAYDATA *DataPtr );
      VOID ResetDisplay ();
      VOID ClearDisplay ( HDISPLAY hDisplay, LONG Color );
      VOID SetScaleFactor ( LONG MagH, LONG DivH, LONG MagV, LONG DivV );                      

      
      VOID CopyImage ( IMAGE *Src, LONG Sx, LONG Sy, LONG Wd, LONG Ht,
                       IMAGE *Dest, LONG Cx, LONG Cy );
      VOID ScaleImage ( IMAGE *Src, LONG Sx, LONG Sy, LONG Wd, LONG Ht,
                        IMAGE *Dest, LONG Cx, LONG Cy );
      VOID RotateImage ( IMAGE *Src, LONG Sx, LONG Sy, LONG Wd, LONG Ht,
                         LONG CenterX, LONG CenterY, float Angle,
                         IMAGE *Dest, LONG Cx, LONG Cy );
      VOID DisplayImage ( HDISPLAY hDisplay, IMAGE *Image, LONG Sx, LONG Sy,
                          LONG Wd, LONG Ht, LONG Cx, LONG Cy );
      VOID ConvertImage ( IMAGE *Image, COLORTABLE *MatchTable );
      VOID AdjustImageSize ( IMAGE *Image, LONG Wd, LONG Ht );
      BOOLEAN LoadImage ( STRING FileName, IMAGE *Image, RGBPALETTE *Pal );
      BOOLEAN SaveImage ( STRING FileName, SHORT ImageType, IMAGE *Image,
                          LONG Sx, LONG Sy, LONG Wd, LONG Ht, RGBPALETTE *Pal );

      
      VOID PlayFLIC ( FLICFILE *Flic, RECTANGLE Region, LONG Cx, LONG Cy,
                      LONG StartFrame, LONG EndFrame, LONG Flags, FLICCallback Callback ); 
      
      
      VOID SetPalette ( HDISPLAY hDisplay, RGBPALETTE *Pal );
      VOID GetPalette ( HDISPLAY hDisplay, RGBPALETTE *Pal );
      VOID ClearPalette ( HDISPLAY hDisplay, BYTE R, BYTE G, BYTE B );
      VOID FadePalette ( HDISPLAY hDisplay, RGBPALETTE *Pal,
                         LONG Direction, LONG NumSteps );
      BOOLEAN SavePalette ( STRING FileName, RGBPALETTE *Pal, LONG PalFormat );
      BOOLEAN LoadPalette ( STRING FileName, RGBPALETTE *Pal );

      
      VOID DrawPixel ( IMAGE *Dest, LONG x, LONG y );
      VOID DrawPixel ( HDISPLAY hDisplay, LONG x, LONG y );
      VOID DrawLine ( IMAGE *Dest, LONG x1, LONG y1, LONG x2, LONG y2 );
      VOID DrawLine ( HDISPLAY hDisplay, LONG x1, LONG y1, LONG x2, LONG y2 );
      VOID DrawRect ( IMAGE *Dest, LONG x1, LONG y1, LONG x2, LONG y2 );
      VOID DrawRect ( HDISPLAY hDisplay, LONG x1, LONG y1, LONG x2, LONG y2 );
      VOID FillRect ( IMAGE *Dest, LONG x1, LONG y1, LONG x2, LONG y2 );
      VOID FillRect ( HDISPLAY hDisplay, LONG x1, LONG y1, LONG x2, LONG y2 );
      VOID DrawEllipse ( IMAGE *Dest, LONG Cx, LONG Cy, LONG Rx, LONG Ry );
      VOID DrawEllipse ( HDISPLAY hDisplay, LONG Cx, LONG Cy, LONG Rx, LONG Ry );
      VOID FillEllipse ( IMAGE *Dest, LONG Cx, LONG Cy, LONG Rx, LONG Ry );
      VOID FillEllipse ( HDISPLAY hDisplay, LONG Cx, LONG Cy, LONG Rx, LONG Ry );
      VOID DrawPolygon ( IMAGE *Dest, POLYGON *Poly );
      VOID FillPolygon ( IMAGE *Dest, POLYGON *Poly );
      VOID DrawText ( HDISPLAY hDisplay, STRING Text, LONG x, LONG y );

      LONG GetWidth ( HDISPLAY hDisplay );
      LONG GetHeight ( HDISPLAY hDisplay );

      VOID WaitForRetrace ( LONG Count );
      VOID WaitForRetrace ();
      VOID WaitForRetraceEnd ();
  }; 

extern GRAFIX Grafix;

#endif




