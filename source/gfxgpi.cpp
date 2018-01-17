




#include "stdgfx.h"
#include "gfxgpi.h"

GPIDRIVER::GPIDRIVER ()
  {
  } 

GPIDRIVER::~GPIDRIVER ()
  {
  } 

BOOLEAN GPIDRIVER::SetUp ( DISPLAYDATA *Data )
  {
     if (Data)
       {}  
     return SUCCESS;
  } 

VOID GPIDRIVER::Reset ()
  {
  } 

VOID GPIDRIVER::Clear ( HDISPLAY hDisplay, LONG Color )
  {
    #if defined (__FOROS2__)
      POINTL Pt;
      LONG Wd, Ht;
      RECTL Rect;

      if (hDisplay)
        {}
      GpiQueryGraphicsField ( hDisplay, &Rect );
      Wd = Rect.xRight;
      Ht = Rect.yTop;
      Pt.x = 0;
      Pt.y = 0;

      LONG OldColor;
      OldColor = GpiQueryColor ( hDisplay );
      GpiSetColor ( hDisplay, Color );
      GpiBox ( hDisplay, DRO_FILL, &Pt, Wd, Ht );
      GpiSetColor ( hDisplay, OldColor );
    #else
      if (hDisplay)
        {}
      if (Color)
        {}
    #endif
  } 

LONG GPIDRIVER::GetWidth ( HDISPLAY hDisplay )
  {
    #if defined (__FOROS2__)
      LONG Wd;
      RECTL Rect;

      if (hDisplay)
        {}
      GpiQueryGraphicsField ( hDisplay, &Rect );
      Wd = Rect.xRight;
      return Wd;
    #else
      if (hDisplay)
        {}
      return NULL;
    #endif
  } 

LONG GPIDRIVER::GetHeight ( HDISPLAY hDisplay )
  {
    #if defined (__FOROS2__)
      LONG Ht;
      RECTL Rect;

      if (hDisplay)
        {}
      GpiQueryGraphicsField ( hDisplay, &Rect );
      Ht = Rect.yTop;
      return Ht;
    #else
      if (hDisplay)
        {}
      return NULL;
    #endif
  } 

VOID GPIDRIVER::DisplayImage ( IMAGE *Image, LONG Sx, LONG Sy,
                               LONG Wd, LONG Ht, HDISPLAY Dest,
                               LONG Cx, LONG Cy, LONG DestWd, LONG DestHt )
  {
     if (Image)
       {}
     if (Sx&Sy&Wd&Ht&Cx&Cy)
       {}
     if (DestWd&DestHt)
       {}  
     if (Dest)
       {}
  } 

VOID GPIDRIVER::SetPalette ( HDISPLAY hDisplay, RGBPALETTE *Pal )
  {
     if (hDisplay)
       {}
     if (Pal)
       {}
  } 

VOID GPIDRIVER::GetPalette ( HDISPLAY hDisplay, RGBPALETTE *Pal )
  {
     if (hDisplay)
       {}
     if (Pal)
       {}
  } 

VOID GPIDRIVER::DrawLine ( HDISPLAY hDisplay, LONG x1, LONG y1, LONG x2, LONG y2,
                                COLOR  Color )
  {
     if (hDisplay)
       {}
     if (x1&y1&x2&y2)
       {}
     if (Color)
       {}
     #if defined (__FOROS2__)
       POINTL P1;
       POINTL P2;

       P1.x = x1;
       P1.y = y1;
       P2.x = x2;
       P2.y = y2;

       GpiMove ( hDisplay, &P1 );
       GpiLine ( hDisplay, &P2 );
     #endif
  } 

VOID GPIDRIVER::DrawRect ( HDISPLAY hDisplay, LONG x1, LONG y1, LONG x2, LONG y2,
                                 COLOR Color )
  {
     if (hDisplay)
       {}
     if (x1&y1&x2&y2)
       {}
     if (Color)
       {}
     #if defined (__FOROS2__)
       POINTL P1;
       POINTL P2;

       P1.x = x1;
       P1.y = y1;
       P2.x = x2;
       P2.y = y2;

       GpiMove ( hDisplay, &P1 );
       GpiBox ( hDisplay, &P2 );
     #endif
  } 

VOID GPIDRIVER::FillRect ( HDISPLAY hDisplay, LONG x1, LONG y1, LONG x2, LONG y2,
                               COLOR Color )
  {
     if (hDisplay)
       {}
     if (x1&y1&x2&y2)
       {}
     if (Color)
       {}
  } 

VOID GPIDRIVER::DrawPixel ( HDISPLAY hDisplay, LONG x, LONG y,
                                  COLOR Color )
  {
     if (hDisplay)
       {}
     if (x&y)
       {}
     if (Color)
       {}
  } 

VOID GPIDRIVER::DrawEllipse ( HDISPLAY hDisplay, LONG Cx, LONG Cy, LONG Rx, LONG Ry,
                                    COLOR Color )
  {
     if (hDisplay)
       {}
     if (Cx&Cy&Rx&Ry)
       {}
     if (Color)
       {}
  } 

VOID GPIDRIVER::FillEllipse ( HDISPLAY hDisplay, LONG Cx, LONG Cy, LONG Rx, LONG Ry,
                                  COLOR Color )
  {
     if (hDisplay)
       {}
     if (Cx&Cy&Rx&Ry)
       {}
     if (Color)
       {}
  } 

VOID GPIDRIVER::DrawText ( HDISPLAY hDisplay, STRING Text, LONG x, LONG y,
                                 COLOR Color )
  {
     if (hDisplay)
       {}
     if (x&y)
       {}
     if (Color)
       {}
     if (Text)
       {}
  } 

