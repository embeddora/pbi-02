




#include "stdgfx.h"
#include "gfxvga.h"
#include "lgfxvga.h"
#include "lgfxdib.h"

#include "stdlib.h"

VGADRIVER::VGADRIVER ()
  {
    Width = Height = 0;
    Buffer = NULL;
    Registers[VGA_INDEX_PORT] = 0x3CE;
    Registers[VGA_DATA_PORT] = 0x3CF;
    Registers[DAC_MASK] = 0x3C6;
    Registers[DAC_READ] = 0x3C7;
    Registers[DAC_WRITE] = 0x3C8;
    Registers[DAC_DATA] = 0x3C9;
  } 

VGADRIVER::~VGADRIVER ()
  {
  } 

BOOLEAN VGADRIVER::SetUp ( DISPLAYDATA *Data )
  {
    if (Data)
      {}  
    #if defined (__FORWINDOWS__)
      return FAILURE;
    #elif defined (__FOROS2__)
      return FAILURE;
    #elif defined (__FORDOS__)
      MODEINFO ModeInfo;
      LONG ModeNumber;

      CurMode = Data->Mode;
      ModeInfo = ModeTable[Data->Mode];
      ModeNumber = ModeInfo.ModeNumber;

      union REGS Regs;
      Regs8.ah = 0;
      Regs8.al = (BYTE)ModeNumber;
      INTERRUPT ( INT_VIDEO, &Regs, &Regs );
      Width = ModeInfo.Width;
      Height = ModeInfo.Height;
      BitSize = ModeInfo.BitSize;
      if (Regs16.ax)
        {}
      SetVGASelector ( SELECTOR_VIDEO );
      return SUCCESS;
    #endif
  } 

VOID VGADRIVER::Reset ()
  {
    DISPLAYDATA Data;
    Data.Mode = M80x25x16;
    SetUp ( &Data );
  } 

VOID VGADRIVER::Clear ( HDISPLAY hDisplay, LONG Color )
  {
    if (hDisplay)
      {}

    #if defined (__FORDOS__)
      if (CurMode==M320x200x256)
        {
          BYTE* Ptr;
          #if defined (__32BIT__)
            Ptr = (BYTE*)0xA0000;
          #else
            Ptr = (BYTE*)MK_FP(SELECTOR_VIDEO,0);
          #endif
            memset ( Ptr, Color, 320*200 );
        } 
    #else
      if (Color)
        {}
    #endif
  } 

LONG VGADRIVER::GetWidth ( HDISPLAY hDisplay )
  {
    if (hDisplay)
      {}
    return Width;
  } 

LONG VGADRIVER::GetHeight ( HDISPLAY hDisplay )
  {
    if (hDisplay)
      {}
    return Height;
  } 

VOID VGADRIVER::DisplayImage ( IMAGE *Image, LONG Sx, LONG Sy,
                               LONG Wd, LONG Ht, HDISPLAY Dest,
                               LONG Cx, LONG Cy, LONG DestWd, LONG DestHt )
  {
    BYTE *SrcBuffer;
    LONG Format;

    if (Dest)
      {}
    Format = Image->GetFormat ();
    SrcBuffer = Image->SetOffset ( Sx, Sy );
    
    if (CurMode==M320x200x256)
      {
        if ((MagH==DivH)&&(MagV==DivV))
          {
            if (Format==IMAGE_8BIT)
              VGACopy256To256 ( SrcBuffer, Image->GetRowInc (), Wd, Ht,
                                Cx, Cy );
          } 
        else
          {
            BYTE *DestBuffer;
            #if defined (__32BIT__)
              DestBuffer = (BYTE*)0xA0000;
            #else
              DestBuffer = (BYTE*)MK_FP(SELECTOR_VIDEO,0);
            #endif

            DestBuffer += Cy*Width+Cx;
    
            WORD Error = 0;
            DWORD AddError = (((LONG)DivH<<16)/MagH);

            INT i; 
            if (Format==IMAGE_8BIT)
              {
                for (i=0;i<DestHt;i++)
                  {
                    ScaleBlt256To256 ( DestBuffer, SrcBuffer, DestWd, Error, AddError );
                    SrcBuffer = Image->SetOffset ( Sx, Sy+(i*DivV)/MagV );
                    DestBuffer += Width;
                  } 
              } 
          } 
      } 
  } 

VOID VGADRIVER::SetPalette ( HDISPLAY hDisplay, RGBPALETTE *Pal )
  {
    RGBCOLOR *Entry;

    if (hDisplay)
      {}
      
    if (Pal==NULL)
      return;

         Entry = Pal->GetEntry ();
         if (Entry==NULL)
      return;

    
    #if defined (__FORDOS__)
      INT i;
      outp ( Registers[DAC_MASK], 0xFF );
      outp ( Registers[DAC_WRITE], 0 );
      for (i=0;i<256;i++)
        {
          outp ( Registers[DAC_DATA], Entry[i].Red >> 2 );
          outp ( Registers[DAC_DATA], Entry[i].Green >> 2 );
          outp ( Registers[DAC_DATA], Entry[i].Blue >> 2 );
        } 
    #else
    #endif
  } 

VOID VGADRIVER::GetPalette ( HDISPLAY hDisplay, RGBPALETTE *Pal )
  {
    INT i;
    RGBCOLOR *Entry;

    if (hDisplay)
      {}
    if (Pal==NULL)
      return;

    Entry = Pal->GetEntry ();
         if (Entry==NULL)
                return;

    
    #if defined (__FORDOS__)
      outp ( Registers[DAC_MASK], 0xFF );
      outp ( Registers[DAC_READ], 0 );
    #endif

    for (i=0;i<256;i++)
      {
        #if defined (__FORDOS__)
          Entry[i].Red = (BYTE)inp ( Registers[DAC_DATA] );
          Entry[i].Green = (BYTE)inp ( Registers[DAC_DATA] );
          Entry[i].Blue = (BYTE)inp ( Registers[DAC_DATA] );
          Entry[i].Red <<= 2;
          Entry[i].Green <<= 2;
          Entry[i].Blue <<= 2;
        #endif
      } 
  } 

VOID VGADRIVER::DrawLine ( HDISPLAY hDisplay, LONG x1, LONG y1, LONG x2, LONG y2,
                           COLOR  Color )
  {
    INT Dx, Dy;
    INT AddX,AddY;
    INT Len;

    if (hDisplay)
      {}

    if (x2<x1)
      {
                  AddX = -1;
      } 
    else
      AddX = 1;

    Dx = abs(x2-x1);
    Dy = abs(y2-y1);

         Len = MaxValue ( Dx, Dy ) + 1;

    BYTE *Buffer;

    #if defined (__32BIT__)
      #if defined (__FORDOS4GW__)
        Buffer = (BYTE*)0xA0000;
      #endif
    #else
      Buffer = (BYTE*)MK_FP(SELECTOR_VIDEO,0);
    #endif

    AddY = Width;
    Buffer += y1*AddY;

    INT i;
    DWORD Error=0;

    DWORD x;

    if (Dy==0)
      {
        x = MinValue ( x1, x2 );
        int i;
        Buffer+=x;
        for (i=0;i<Len;i++)
          Buffer[i] = (BYTE)Color;
        
      } 
    else
      {
        x = x1;
        if (Dx>Dy)
          {
            for (i=0;i<Len;i++)
              {
                Buffer[x] = (BYTE)Color;
                Error += (DWORD)Dy;
                if (Error>=(DWORD)Dx)
                  {
                    Buffer += AddY;
                    Error -= (DWORD)Dx;
                  } 
                x += AddX;
              } 
          } 
        else
          {
            for (i=0;i<Len;i++)
              {
                Buffer[x] = (BYTE)Color;
                Error += (DWORD)Dx;
                if (Error>=(DWORD)Dy)
                  {
                    x += AddX;
                    Error -= (DWORD)Dy;
                  } 
                Buffer += AddY;
              } 
          } 
      } 
  } 

VOID VGADRIVER::DrawRect ( HDISPLAY hDisplay, LONG x1, LONG y1, LONG x2, LONG y2,
                           COLOR Color )
  {
    DrawLine ( hDisplay, x1, y2, x2, y2, Color );
    DrawLine ( hDisplay, x1, y1, x2, y1, Color );
    DrawLine ( hDisplay, x1, y1, x1, y2, Color );
    DrawLine ( hDisplay, x2, y1, x2, y2, Color );
  } 

VOID VGADRIVER::FillRect ( HDISPLAY hDisplay, LONG x1, LONG y1, LONG x2, LONG y2,
                           COLOR Color )
  {
    BYTE *Buffer;

    if (hDisplay)
      {}

    #if defined (__32BIT__)
      #if defined (__FORDOS4GW__)
        Buffer = (BYTE*)0xA0000;
      #endif
    #else
      Buffer = (BYTE*)MK_FP(0xA000,0);
    #endif

    LONG AddY = Width;
    LONG Offset;
    Offset = y1*AddY+x1;

    INT i;
    LONG Wd, Ht;

    Wd = x2-x1+1;
    Ht = y2-y1+1;

    for (i=0;i<Ht;i++)
      {
        memset ( Buffer+Offset, Color, Wd );
        Offset += AddY;
      } 
  } 

VOID VGADRIVER::DrawPixel ( HDISPLAY hDisplay, LONG x, LONG y,
                            COLOR Color )
  {
    if (hDisplay)
      {}

    if ((x<0)||(x>=Width))
      return;
    if ((y<0)||(y>=Height))
      return;

    #if defined (__32BIT__)
      #if defined (__FORDOS4GW__)
        Buffer = (BYTE*)0xA0000;
      #endif
    #else
      Buffer = (BYTE*)MK_FP(0xA000,0);
    #endif

    Buffer += y*320+x;
    *Buffer = (BYTE)Color;
  } 

VOID VGADRIVER::DrawEllipsePoint ( HDISPLAY hDisplay, LONG Cx, LONG Cy, LONG x, LONG y, COLOR Color )
  {
    INT Dx,Dy;

    Dx = x - Cx;
    Dy = Cy - y;

    DrawPixel ( hDisplay, Cx+Dx, Cy-Dy, Color );
    DrawPixel ( hDisplay, Cx+Dx, Cy+Dy, Color );
    DrawPixel ( hDisplay, Cx-Dx, Cy-Dy, Color );
    DrawPixel ( hDisplay, Cx-Dx, Cy+Dy, Color );
  } 

VOID VGADRIVER::DrawEllipse ( HDISPLAY hDisplay, LONG Cx, LONG Cy, LONG Rx, LONG Ry,
                              COLOR Color )
  {
    LONG RsqX,RsqY;
    LONG RsqXY;
    LONG X2,Y2;
    LONG x,y;
    LONG MidTest;

    if ((Rx==0)||(Ry==0))
      return;

    RsqX = Rx*Rx;
    RsqY = Ry*Ry;
    RsqXY = RsqX*RsqY;
    x = 0;
    y = Ry;

    
    DrawEllipsePoint ( hDisplay, Cx, Cy, x+Cx,
                       Cy-y, Color );

    Y2 = y*y;
    while ((x*RsqY)<=(y*RsqX))
      {
        x++;
        X2 = x*x;
        MidTest = RsqY*X2+RsqX*Y2 - RsqXY;
        if (MidTest>=0)
          {
            y--;
            Y2 = y*y;
          } 
        DrawEllipsePoint ( hDisplay, Cx, Cy, x+Cx,
                           Cy-y, Color );
      } 

    while (y>=0)
      {
        y--;
        Y2 = y*y;
        MidTest = RsqY*X2+RsqX*Y2 - RsqXY;
        if (MidTest<0)
          {
            x++;
            X2 = x*x;
          } 
        DrawEllipsePoint ( hDisplay, Cx, Cy, x+Cx,
                           Cy-y, Color );
      } 
  } 

VOID VGADRIVER::DrawHorizLine ( HDISPLAY hDisplay, LONG x1, LONG x2, LONG y, COLOR Color )
  {
    if (hDisplay)
      {}

    if (x1>x2)
      SwapValue ( &x1, &x2 );

    if (x1<0)
      x1 = 0;
    else if (x1>Width)
      return;

    if (x2<0)
      return;
    else if (x2>Width)
      x2 = Width;

    if (y<0)
      y = 0;
    else if (y>Height)
      return;

    INT Wd;

    Wd = x2 - x1 + 1;

    BYTE *Buffer;

    #if defined (__32BIT__)
      #if defined (__FORDOS4GW__)
        Buffer = (BYTE*)0xA0000;
      #endif
    #else
      Buffer = (BYTE*)MK_FP(0xA000,0);
    #endif

    LONG Offset = y*320+x1;

    memset ( Buffer+Offset, Color, Wd );
  } 

VOID VGADRIVER::FillEllipsePoint ( HDISPLAY hDisplay, LONG Cx, LONG Cy,
                                   LONG x, LONG y, COLOR Color )
  {
    INT Dx,Dy;

    if (hDisplay)
      {}
    Dx = x - Cx;
    Dy = Cy - y;

    DrawHorizLine ( hDisplay, Cx-Dx, Cx+Dx, Cy-Dy, Color );
    DrawHorizLine ( hDisplay, Cx-Dx, Cx+Dx, Cy+Dy, Color );
  } 

VOID VGADRIVER::FillEllipse ( HDISPLAY hDisplay, LONG Cx, LONG Cy,
                              LONG Rx, LONG Ry, COLOR Color )
  {
    LONG RsqX,RsqY;
    LONG RsqXY;
    LONG X2,Y2;
    LONG x,y;
    LONG MidTest;

    if ((Rx==0)||(Ry==0))
      return;

    RsqX = Rx*Rx;
    RsqY = Ry*Ry;
    RsqXY = RsqX*RsqY;
    x = 0;
    y = Ry;

    
    FillEllipsePoint ( hDisplay, Cx, Cy, x+Cx,
                       Cy-y, Color );

    Y2 = y*y;
    while ((x*RsqY)<=(y*RsqX))
      {
        x++;
        X2 = x*x;
        MidTest = RsqY*X2+RsqX*Y2 - RsqXY;
        if (MidTest>=0)
          {
            y--;
            Y2 = y*y;
          } 
        FillEllipsePoint ( hDisplay, Cx, Cy, x+Cx,
                           Cy-y, Color );
      } 

    while (y>=0)
      {
        y--;
        Y2 = y*y;
        MidTest = RsqY*X2+RsqX*Y2 - RsqXY;
        if (MidTest<0)
          {
            x++;
            X2 = x*x;
          } 
        FillEllipsePoint ( hDisplay, Cx, Cy, x+Cx,
                           Cy-y, Color );
      } 
  } 

