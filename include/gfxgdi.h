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



#ifndef __GFXGDIDRIVER_H__
#define __GFXGDIDRIVER_H__

#include "stdgfx.h"
#include "gfxdisp.h"

class GDIDRIVER : public DISPLAYDRIVER
  {
    protected :
      #if defined (__FORWINDOWS__)
        LOGPALETTE *LogPal;
      #endif  
        
    public :
      GDIDRIVER ();
      virtual ~GDIDRIVER ();

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
