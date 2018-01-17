




#ifndef __GFXGPIDRV_H__
#define __GFXGPIDRV_H__

#include "stdgfx.h"
#include "gfxdisp.h"

class GPIDRIVER : public DISPLAYDRIVER
  {
    public :
      GPIDRIVER ();
      virtual ~GPIDRIVER ();

      virtual BOOLEAN SetUp ( DISPLAYDATA *Data );
      virtual VOID Reset ();
      virtual VOID Clear ( HDISPLAY hDisplay, LONG Color );

      virtual LONG GetWidth ( HDISPLAY hDisplay );
      virtual LONG GetHeight ( HDISPLAY hDisplay );

      virtual VOID DisplayImage ( IMAGE *Image, LONG Sx, LONG Sy,
                                  LONG Wd, LONG Ht, HDISPLAY Dest,
                                  LONG Cx, LONG Cy, LONG DestWd, LONG DestHt );
      virtual VOID SetPalette ( HDISPLAY hDisplay, RGBPALETTE *Pal );
      virtual VOID GetPalette ( HDISPLAY hDisplay, RGBPALETTE *Pal );
      virtual VOID DrawLine ( HDISPLAY hDisplay, LONG x1, LONG y1, LONG x2, LONG y2,
                              COLOR  Color );
      virtual VOID DrawRect ( HDISPLAY hDisplay, LONG x1, LONG y1, LONG x2, LONG y2,
                              COLOR Color );
      virtual VOID FillRect ( HDISPLAY hDisplay, LONG x1, LONG y1, LONG x2, LONG y2,
                              COLOR Color );
      virtual VOID DrawPixel ( HDISPLAY hDisplay, LONG x, LONG y,
                               COLOR Color );
      virtual VOID DrawEllipse ( HDISPLAY hDisplay, LONG Cx, LONG Cy, LONG Rx, LONG Ry,
                                 COLOR Color );
      virtual VOID FillEllipse ( HDISPLAY hDisplay, LONG Cx, LONG Cy, LONG Rx, LONG Ry,
                                 COLOR Color );
      virtual VOID DrawText ( HDISPLAY hDisplay, STRING Text, LONG x, LONG y,
                              COLOR Color );
  }; 

#endif

