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



#ifndef __GFXBMP_H__
#define __GFXBMP_H__

#include "stdgfx.h"
#include "gfxifile.h"

class BMPFILETOOL : public IMAGEFILETOOL
  {
    protected :
      BITMAPFILEHEADER FileHeader;
      BITMAPINFOHEADER InfoHeader;
      BYTE *DataBuffer;
      LONG Width;
      LONG Size;
      LONG BmWd,BmHt,BmBits;

      BOOLEAN ReadHeader ();
      BOOLEAN ReadInfo ();
      BOOLEAN ReadRowBW ( IMAGE *Image, INT Row );
      BOOLEAN ReadRow16 ( IMAGE *Image, INT Row );
      BOOLEAN ReadRow256 ( IMAGE *Image, INT Row );
      BOOLEAN ReadImage ( IMAGE *Image );
      BOOLEAN ReadPalette ( RGBPALETTE *Pal );

      BOOLEAN WriteHeader ();
      BOOLEAN WriteInfo ( INT Wd, INT Ht );
      BOOLEAN WritePalette ( RGBPALETTE *Pal );
      BOOLEAN WriteImageRow ( IMAGE *Image, INT Row, INT Sx );
      BOOLEAN WriteImage ( IMAGE *Image, INT Sx, INT Sy, INT Ht );

    public :
      BMPFILETOOL ( FILEHANDLE f );
      virtual ~BMPFILETOOL ();

      virtual BOOLEAN LoadImage ( IMAGE *Image, RGBPALETTE *Pal );
      virtual BOOLEAN SaveImage ( IMAGE *Image, LONG Sx, LONG Sy,
                                  LONG Wd, LONG Ht, RGBPALETTE *Pal );
  }; 

#endif
