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



#ifndef __GFXIMAGE_H__
#define __GFXIMAGE_H__

#include "stdgfx.h"
#include "gfxerror.h"


#define IMAGE_8BIT     1
#define IMAGE_16BIT    2
#define IMAGE_24BIT    3


#define IMAGE_TOPDOWN  1
#define IMAGE_BOTTOMUP 2


#define ROW_DOWN  1
#define ROW_UP    2


#define NOTRANSPARENT  -1

#if defined (__FORWINDOWS__)
#elif defined (__FOROS2__)
  #define BI_NONE 0
  #define BI_RGB  0
  #define BI_RLE4 2
  #define BI_RLE8 1
  #define HBITMAP HANDLE

  struct RGBQUAD
    {
      BYTE rgbBlue;
      BYTE rgbGreen;
      BYTE rgbRed;
      BYTE rgbReserved;
    }; 

#elif defined (__FORDOS__)
  #define BI_NONE 0
  #define BI_RGB  0
  #define BI_RLE4 2
  #define BI_RLE8 1
  #define HBITMAP HANDLE

  struct BITMAPINFOHEADER
    {
      DWORD biSize;
      LONG  biWidth;
      LONG  biHeight;
      WORD  biPlanes;
      WORD  biBitCount;
      DWORD biCompression;
      DWORD biSizeImage;
      LONG  biXPelsPerMeter;
      LONG  biYPelsPerMeter;
      DWORD biClrUsed;
      DWORD biClrImportant;
    }; 

  struct RGBQUAD
    {
      BYTE rgbBlue;
      BYTE rgbGreen;
      BYTE rgbRed;
      BYTE rgbReserved;
    }; 

  struct BITMAPINFO
    {
      BITMAPINFOHEADER bmiHeader;
      RGBQUAD bmiColors[1];
    }; 
      
  struct BITMAPFILEHEADER
    {
      USHORT bfType;
      DWORD  bfSize;
      USHORT bfReserved1;
      USHORT bfReserved2;
      DWORD  bfOffBits;
    }; 

#elif defined (__FORUNIX__)
  #define BI_NONE 0
  #define BI_RGB  0
  #define BI_RLE4 2
  #define BI_RLE8 1
  #define HBITMAP HANDLE

  struct BITMAPINFOHEADER
    {
      DWORD biSize;
      LONG  biWidth;
      LONG  biHeight;
      WORD  biPlanes;
      WORD  biBitCount;
      DWORD biCompression;
      DWORD biSizeImage;
      LONG  biXPelsPerMeter;
      LONG  biYPelsPerMeter;
      DWORD biClrUsed;
      DWORD biClrImportant;
    }; 

  struct RGBQUAD
    {
      BYTE rgbBlue;
      BYTE rgbGreen;
      BYTE rgbRed;
      BYTE rgbReserved;
    }; 

  struct BITMAPINFO
    {
      BITMAPINFOHEADER bmiHeader;
      RGBQUAD bmiColors[1];
    }; 
      
  struct BITMAPFILEHEADER
    {
      USHORT bfType;
      DWORD  bfSize;
      USHORT bfReserved1;
      USHORT bfReserved2;
      DWORD  bfOffBits;
    }; 
#endif

class IMAGE : public MYOBJECT
  {
    protected :
      BYTE *Buffer;
      LONG Offset;
      LONG Width;
      LONG Height;
      LONG BytesPerRow;      
      LONG BytesPerPixel;
      LONG RowInc;
      LONG Format;
      LONG Bits;
      LONG Direction;
      RECTANGLE ViewPort;
      BOOLEAN ForDebug;
      
      
      HMEMDISPLAY hMemDisplay;
      HBITMAP DIBSection;
      HBITMAP OldBitmap;
      BOOLEAN Attached;

    public :
      LONG Transparent;

      IMAGE ();
      IMAGE ( LONG Direction );
      virtual ~IMAGE ();

      virtual BOOLEAN Create ( LONG Format, LONG Wd, LONG Ht );
      virtual BOOLEAN Create ( HDISPLAY hDisplay, LONG Format, LONG Wd, LONG Ht );
      virtual VOID Destroy ();
      
      virtual BYTE *SetOffset ( LONG x, LONG y );
      virtual BYTE *GetNextRow ( LONG Direction );
      virtual LONG GetRowInc () { return RowInc; };

      virtual VOID Clear ( LONG Color );
      virtual BOOLEAN IsCreated ();

      BOOLEAN IsAttached () { return Attached; };
      HMEMDISPLAY GetMemDisplay () { return hMemDisplay; };
      
      VOID SetViewPort ( LONG x1, LONG y1, LONG x2, LONG y2 );
      RECTANGLE GetViewPort () { return ViewPort; };

      BYTE *GetBuffer () { return Buffer; };
      BYTE *GetOffset () { return Buffer+Offset; };
      LONG GetWidth () { return Width; };
      LONG GetBytesPerRow () { return BytesPerRow; };
      LONG GetBytesPerPixel () { return BytesPerPixel; };
      LONG GetHeight () { return Height; };
      LONG GetBits () { return Bits; };
      LONG GetFormat () { return Format; };
      LONG GetDirection () { return Direction; };

      VOID SetDebugFlag ( BOOLEAN IsForDebug );
  }; 

#endif

