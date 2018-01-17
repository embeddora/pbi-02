




#ifndef __GFXVGADRV_H__
#define __GFXVGADRV_H__

#include "stdgfx.h"
#include "gfxdisp.h"

#define NUM_VGA_REGS 6


#define VGA_INDEX_PORT 0
#define VGA_DATA_PORT  1
#define DAC_MASK       2
#define DAC_READ       3
#define DAC_WRITE      4
#define DAC_DATA       5

#if defined (__FORPWPK16__)
  #define SELECTOR_VIDEO  0x0A000
#else
  #define SELECTOR_VIDEO  0x0A000  
#endif

#define HVGA 1

class VGADRIVER : public DISPLAYDRIVER
  {
    protected :
      LONG Registers[NUM_VGA_REGS];
      BYTE *Buffer;
      LONG CurMode;

      virtual VOID DrawHorizLine ( HDISPLAY hDisplay, LONG x1,
                                   LONG x2, LONG y, COLOR Color );

      VOID DrawEllipsePoint ( HDISPLAY hDisplay, LONG Cx, LONG Cy,
                              LONG x, LONG y, COLOR Color );
      VOID FillEllipsePoint ( HDISPLAY hDisplay, LONG Cx, LONG Cy,
                              LONG x, LONG y, COLOR Color );

    public :
      VGADRIVER ();
      virtual ~VGADRIVER ();

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
  }; 

#endif
