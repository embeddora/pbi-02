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



#include "stdgfx.h"
#include "gfxbmp.h"

BMPFILETOOL::BMPFILETOOL ( FILEHANDLE f ) : IMAGEFILETOOL ( f )
  {
    DataBuffer = NULL;
  } 

BMPFILETOOL::~BMPFILETOOL ()
  {
    if (DataBuffer!=NULL)
      delete DataBuffer;
    DataBuffer = NULL;
  } 

BOOLEAN BMPFILETOOL::ReadHeader ()
  {
    INT ID;

    ID = ((INT)'M'<< 8) + (INT)'B';

    File.Seek ( fp, 0, FROM_BEGIN );

    #if defined (__FORWINDOWS__)
      if (File.Read ( fp, &FileHeader, 14 )==FAILURE)
        return FAILURE;
      if (FileHeader.bfType!=ID)
        return FAILURE;
    #elif defined (__FORDOS__)
      if (File.Read ( fp, &FileHeader, 14 )==FAILURE)
        return FAILURE;
      if (FileHeader.bfType!=ID)
        return FAILURE;
    #elif defined (__FORUNIX__)
      if (File.Read ( fp, &FileHeader, 14 )==FAILURE)
        return FAILURE;
      #if defined (__MSBFIRST__)
        SwapWord ( (WORD*)&FileHeader.bfType );
        SwapDWord ( (DWORD*)&FileHeader.bfSize );
        SwapDWord ( (DWORD*)&FileHeader.bfOffBits );
      #endif
      if (FileHeader.bfType!=ID)
        return FAILURE;
    #endif

    return SUCCESS;
  } 

BOOLEAN BMPFILETOOL::ReadInfo ()
  {
    if (File.Read ( fp, &InfoHeader, 40 )==FAILURE)
      return FAILURE;
      #if defined (__MSBFIRST__)
        SwapDWord ( (DWORD*)&InfoHeader.biSize );
        SwapDWord ( (DWORD*)&InfoHeader.biWidth );
        SwapDWord ( (DWORD*)&InfoHeader.biHeight );
        SwapWord ( (WORD*)&InfoHeader.biPlanes );
        SwapWord ( (WORD*)&InfoHeader.biBitCount );
        SwapDWord ( (DWORD*)&InfoHeader.biCompression );
        SwapDWord ( (DWORD*)&InfoHeader.biSizeImage );
        SwapDWord ( (DWORD*)&InfoHeader.biXPelsPerMeter );
        SwapDWord ( (DWORD*)&InfoHeader.biYPelsPerMeter );
        SwapDWord ( (DWORD*)&InfoHeader.biClrUsed );
        SwapDWord ( (DWORD*)&InfoHeader.biClrImportant );
     #endif
    return SUCCESS;
  } 

BOOLEAN BMPFILETOOL::ReadRowBW ( IMAGE *Image, INT Row )
  {
    BYTE *Buffer;

    if (!File.Read ( fp,DataBuffer,Size))
      return FAILURE;

    Buffer = Image->SetOffset ( 0, Row );

    INT i;
    INT BitShift= 1 << 7;
    BYTE Byte;

    for (i=0;i<Image->GetWidth();i++)
      {
        Byte = DataBuffer[i>>3];
        if (Byte&BitShift)
          Buffer[i] = 1;
        else
          Buffer[i] = 0;
        if (BitShift==1)
          BitShift = 1 << 7;
        else
          BitShift >>= 1;
      } 

    return TRUE;
  } 

BOOLEAN BMPFILETOOL::ReadRow16 ( IMAGE *Image, INT Row )
  {
    BYTE *Buffer;

    if (!File.Read (fp,DataBuffer,Size))
      return FAILURE;

    Buffer = Image->SetOffset ( 0, Row );

    INT i;

    for (i=0;i<Image->GetWidth();i++)
      {
        if (i&0x01) 
          Buffer[i] = (BYTE)(DataBuffer[i>>1] & 0xFF);
        else  
          Buffer[i] = (BYTE)(DataBuffer[i>>1] >> 4);
      } 

    return SUCCESS;
  } 

BOOLEAN BMPFILETOOL::ReadRow256 ( IMAGE *Image, INT Row )
  {
    BYTE *Buffer;

    Buffer = Image->SetOffset ( 0, Row );
    if (!File.Read ( fp, DataBuffer, Size ))
      return FAILURE;

    memcpy ( Buffer, DataBuffer, Image->GetWidth() );

    return SUCCESS;
  } 

BOOLEAN BMPFILETOOL::ReadImage ( IMAGE *Image )
  {
    INT i;
    BOOLEAN Ok;

    #if defined (__FOROS2__)
      BmWd = InfoHeader.cx;
      BmHt = InfoHeader.cy;
      BmBits = InfoHeader.cBitCount;
    #else
      BmWd = InfoHeader.biWidth;
      BmHt = InfoHeader.biHeight;
      BmBits = InfoHeader.biBitCount;
    #endif

    Ok = Image->Create ( IMAGE_8BIT, BmWd, BmHt);

    if (!Ok)
      return FAILURE;

    Width = Image->GetWidth();

    Width = (Width+3)/4;
    Width = Width*4;

    switch (BmBits)
      {
        case 1 :
          Size = (Width+7)/8;
          DataBuffer = new BYTE [Size];
          break;
        case 4 :
          Size = (Width+1)/2;
          DataBuffer = new BYTE [Size];
          break;
        case 8 :
          Size = Width;
          DataBuffer = new BYTE [Size];
          break;
        default :
          return FAILURE;
      } 

    if (DataBuffer==NULL)
      return FAILURE;

    for (i=0;i<Image->GetHeight();i++)
      {
        switch (BmBits)
          {
            case 1 :
              Ok = ReadRowBW ( Image, Image->GetHeight()-i-1 );
              break;
            case 4 :
              Ok = ReadRow16 ( Image, Image->GetHeight()-i-1  );
              break;
            case 8 :
              Ok = ReadRow256 ( Image, Image->GetHeight()-i-1  );
              break;
          } 

        if (!Ok)
          break;
      } 

    delete DataBuffer;

    return SUCCESS;
  } 

BOOLEAN BMPFILETOOL::ReadPalette ( RGBPALETTE *Pal )
  {
    INT i;
    INT NumColors;
    RGBCOLOR *Entry;

    if (BmBits>8) 
      return SUCCESS;

    #if defined (__FOROS2__)
      NumColors = 0;
    #else
      NumColors = InfoHeader.biClrUsed;
    #endif

    if (NumColors==0)
      NumColors = 256;

    if (Pal==NULL)
      {
        File.Seek ( fp, NumColors*sizeof(RGBQUAD), FROM_CURRENT );
        return SUCCESS;
      } 

    Entry = Pal->GetEntry ();

    for (i=0;i<NumColors;i++)
      {
        BYTE Flags;  
        Entry[i].Blue = (BYTE)File.GetCh ( fp );
        Entry[i].Green = (BYTE)File.GetCh ( fp );
        Entry[i].Red = (BYTE)File.GetCh ( fp );
        Flags = (BYTE)File.GetCh ( fp );
        if (Flags)
          {}
      } 

    return SUCCESS;
  } 

BOOLEAN BMPFILETOOL::LoadImage ( IMAGE *Image, RGBPALETTE *Pal )
  {
    BOOLEAN Ok;

    Ok = ReadHeader ();
    if (!Ok)
      return FAILURE;

    Ok = ReadInfo ();
    if (!Ok)
      return FAILURE;

    Ok = ReadPalette ( Pal );
    if (!Ok)
      return FAILURE;

    Ok = ReadImage ( Image );
    if (!Ok)
      return FAILURE;

    return SUCCESS;
  } 

BOOLEAN BMPFILETOOL::WriteHeader ()
  {
    #if defined (__FOROS2__)
      FileHeader.usType = ((INT)'M'<< 8) + (INT)'B';
    #else
      FileHeader.bfType = ((INT)'M'<< 8) + (INT)'B';
      FileHeader.bfReserved1 = 0;
      FileHeader.bfReserved2 = 0;
    #endif
    File.Seek ( fp, 0, FROM_BEGIN );

    #if defined (__MSBFIRST__)
      SwapWord ( (WORD*)&FileHeader.bfType );
      SwapDWord ( (DWORD*)&FileHeader.bfSize );
      SwapDWord ( (DWORD*)&FileHeader.bfOffBits );
    #endif

    if (File.Write ( fp, &FileHeader, 14 )==FAILURE)
      return FAILURE;
    return SUCCESS;
  } 

BOOLEAN BMPFILETOOL::WriteInfo ( INT Wd, INT Ht )
  {
    #if defined (__FOROS2__)
      InfoHeader.cbFix = sizeof(BITMAPINFOHEADER);
      InfoHeader.cx = Wd;
      InfoHeader.cy = Ht;
      InfoHeader.cPlanes = 1;
      InfoHeader.cBitCount = 8;
      if (File.Write ( fp, &InfoHeader, 12 )==FAILURE)
        return FAILURE;
    #else
      InfoHeader.biSize = sizeof(BITMAPINFOHEADER);
      InfoHeader.biWidth = Wd;
      InfoHeader.biHeight = Ht;
      InfoHeader.biPlanes = 1;
      InfoHeader.biBitCount = 8;
      InfoHeader.biCompression = 0;
      InfoHeader.biXPelsPerMeter = 0;
      InfoHeader.biYPelsPerMeter = 0;
      InfoHeader.biClrUsed = 0;
      InfoHeader.biClrImportant = 0;
      InfoHeader.biSizeImage = (LONG)Wd*(LONG)Ht;

      #if defined (__MSBFIRST__)
        SwapDWord ( (DWORD*)&InfoHeader.biSize );
        SwapDWord ( (DWORD*)&InfoHeader.biWidth );
        SwapDWord ( (DWORD*)&InfoHeader.biHeight );
        SwapWord ( (WORD*)&InfoHeader.biPlanes );
        SwapWord ( (DWORD*)&InfoHeader.biBitCount );
        SwapDWord ( (DWORD*)&InfoHeader.biCompression );
        SwapDWord ( (DWORD*)&InfoHeader.biSizeImage );
        SwapDWord ( (DWORD*)&InfoHeader.biXPelsPerMeter );
        SwapDWord ( (DWORD*)&InfoHeader.biYPelsPerMeter );
        SwapDWord ( (DWORD*)&InfoHeader.biClrUsed );
        SwapDWord ( (DWORD*)&InfoHeader.biClrImportant );
      #endif
      if (File.Write ( fp, &InfoHeader, 40 )==FAILURE)
        return FAILURE;
    #endif
    return SUCCESS;
  } 

BOOLEAN BMPFILETOOL::WritePalette ( RGBPALETTE *Pal )
  {
    RGBQUAD RGBQ;
    RGBCOLOR *Entry;
    INT i;

    if (Pal==NULL)
      return FAILURE;

    Entry = Pal->GetEntry ();

    for (i=0;i<256;i++)
      {
        RGBQ.rgbBlue = Entry[i].Blue;
        RGBQ.rgbGreen = Entry[i].Green;
        RGBQ.rgbRed = Entry[i].Red;
        RGBQ.rgbReserved = 0;

        File.Write ( fp, &RGBQ, 4 );
      } 

    return SUCCESS;
  } 

BOOLEAN BMPFILETOOL::WriteImageRow ( IMAGE *Image, INT Row, INT Sx )
  {
    BOOLEAN Ok;
    memcpy ( DataBuffer, Image->SetOffset ( 0, Row ), Image->GetWidth() );
    Ok = File.Write ( fp, DataBuffer+Sx, Width );
    if (!Ok)
      return FAILURE;
    return SUCCESS;
  } 

BOOLEAN BMPFILETOOL::WriteImage ( IMAGE *Image, INT Sx, INT Sy, INT Ht )
  {
    INT i;
    BOOLEAN Ok;
    for (i=Sy;i<Sy+Ht;i++)
      {
        Ok = WriteImageRow ( Image, Image->GetHeight()-i-1, Sx );
        if (!Ok)
          return FAILURE;
      } 
    return SUCCESS;
  } 

BOOLEAN BMPFILETOOL::SaveImage ( IMAGE *Image, LONG Sx, LONG Sy,
                                 LONG Wd, LONG Ht, RGBPALETTE *Pal )
  {
    BOOLEAN Ok;

    if (DataBuffer!=NULL)
      delete DataBuffer;

    DataBuffer = NULL;
    if (Image==NULL)
      {
        return FAILURE;
      } 

    Ok = WriteHeader ();
    if (!Ok)
      return FAILURE;

    Ok = WriteInfo ( Wd, Ht );
    if (!Ok)
      return FAILURE;

    Ok = WritePalette ( Pal );
    if (!Ok)
      return FAILURE;

    Width = Wd;

    Width = (Width+3)/4;
    Width = Width*4;

    if (Width>Image->GetWidth())
      DataBuffer = new BYTE [Width];
    else
      DataBuffer = new BYTE [Image->GetWidth()];

    if (DataBuffer==NULL)
      return FAILURE;

    Ok = WriteImage ( Image, Sx, Sy, Ht );
    if (!Ok)
      return FAILURE;

    delete DataBuffer;

    DWORD Size;

    Size = File.GetPos ( fp );

    #if defined (__FOROS2__)
      FileHeader.cbSize = Size;
      FileHeader.offBits = sizeof (BITMAPFILEHEADER) +
                              sizeof (BITMAPINFOHEADER) +
                               sizeof (RGBQUAD)*256;
    #else
      FileHeader.bfSize = Size;
      FileHeader.bfOffBits = sizeof (BITMAPFILEHEADER) +
                              sizeof (BITMAPINFOHEADER) +
                               sizeof (RGBQUAD)*256;
    #endif

    Ok = WriteHeader ();
    if (!Ok)
      return FAILURE;

    DataBuffer = NULL;
    return SUCCESS;
  } 




