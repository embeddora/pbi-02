




#ifndef __GFXDISPDRV_H__
#define __GFXDISPDRV_H__

#include "stdgfx.h"
#include "gfxdrv.h"
#include "gfxmodes.h"
#include "gfxpal.h"
#include "gfximage.h"

struct DISPLAYDATA
  {
    LONG Mode;  
    BOOLEAN UseDirectDraw;
    HWINDOW hMainWindow;
    #if defined (__FORUNIX__)
      Display *MainXDisplay;
    #endif
  }; 
 
class DISPLAYDRIVER : public DRIVER
  {
    protected :
      BITMAPINFO *Info;
      LONG MagH,MagV,DivH,DivV;
      LONG Width,Height,BitSize,BytesPerRow;      

    public :
      DISPLAYDRIVER ();
      virtual ~DISPLAYDRIVER ();

      virtual BOOLEAN SetUp ( DISPLAYDATA *Data )=0;
      virtual VOID Reset ()=0;
      virtual VOID Clear ( HDISPLAY hDisplay, LONG Color );
      VOID SetScaleFactor ( LONG MagH, LONG DivH, LONG MagV, LONG DivV );                      

      virtual LONG GetWidth ( HDISPLAY hDisplay )=0;
      virtual LONG GetHeight ( HDISPLAY hDisplay )=0;

      virtual VOID DisplayImage ( IMAGE *Image, LONG Sx, LONG Sy,
                                  LONG Wd, LONG Ht, HDISPLAY Dest,
                                  LONG Cx, LONG Cy, LONG DestWd, LONG DestHt )=0;
      virtual VOID SetPalette ( HDISPLAY hDisplay, RGBPALETTE *Pal )=0;
      virtual VOID GetPalette ( HDISPLAY hDisplay, RGBPALETTE *Pal )=0;
      virtual VOID DrawLine ( HDISPLAY hDisplay, LONG x1, LONG y1, LONG x2, LONG y2,
                              COLOR Color )=0;
      virtual VOID DrawRect ( HDISPLAY hDisplay, LONG x1, LONG y1, LONG x2, LONG y2,
                              COLOR Color )=0;
      virtual VOID FillRect ( HDISPLAY hDisplay, LONG x1, LONG y1, LONG x2, LONG y2,
                              COLOR Color )=0;
      virtual VOID DrawPixel ( HDISPLAY hDisplay, LONG x, LONG y,
                               COLOR Color )=0;
      virtual VOID DrawEllipse ( HDISPLAY hDisplay, LONG Cx, LONG Cy, LONG Rx, LONG Ry,
                                 COLOR Color )=0;
      virtual VOID FillEllipse ( HDISPLAY hDisplay, LONG Cx, LONG Cy, LONG Rx, LONG Ry,
                                 COLOR Color )=0;
      virtual VOID DrawText ( HDISPLAY hDisplay, STRING Text, LONG x, LONG y,
                              COLOR Color );
  }; 

#endif
