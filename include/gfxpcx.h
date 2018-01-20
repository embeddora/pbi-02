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


#ifndef __GFXPCX_H__
#define __GFXPCX_H__

#include "stdgfx.h"
#include "gfxifile.h"

struct PCXHEADER
  {
    BYTE  ID;
    BYTE  Version;
    BYTE  Encoding;
    BYTE  BitsPerPixel;
    SHORT XMin;
    SHORT YMin;
    SHORT XMax;
    SHORT YMax;
    SHORT XResolution;
    SHORT YResolution;
    RGBCOLOR Palette[16];
    BYTE  Reserved;
    BYTE  NumPlanes;
    SHORT BytesPerRow;
    SHORT PalType;  
    SHORT XScreen;
    SHORT YScreen;
    BYTE  Filter[54];
  }; 

class PCXFILETOOL : public IMAGEFILETOOL
  {
    protected :
      PCXHEADER PCXHeader;
      BYTE *DataBuffer;

      BOOLEAN ReadHeader ();
      BOOLEAN ReadImageRow2 ( IMAGE *Image, INT Row );
      BOOLEAN ReadImageRow4 ( IMAGE *Image, INT Row );
      BOOLEAN ReadImageRow16 ( IMAGE *Image, INT Row );
      BOOLEAN ReadImageRow256 ( IMAGE *Image, INT Row );
      BOOLEAN ReadImage ( IMAGE *Image );
      BOOLEAN ReadPalette ( RGBPALETTE *Pal );

      BOOLEAN WriteHeader ( INT Wd, INT Ht );
      BOOLEAN WriteImageRow256 ( IMAGE *Image, INT Sx, INT Wd, INT WhichRow );
      BOOLEAN WriteImage ( IMAGE *Image, INT Sx, INT Sy, INT Wd, INT Ht  );
      BOOLEAN WritePalette ( RGBPALETTE *Pal );

    public :
      PCXFILETOOL ( FILEHANDLE f );
      virtual ~PCXFILETOOL ();

      virtual BOOLEAN LoadImage ( IMAGE *Image, RGBPALETTE *Pal );
      virtual BOOLEAN SaveImage ( IMAGE *Image, LONG Sx, LONG Sy,
                                  LONG Wd, LONG Ht, RGBPALETTE *Pal  );
  }; 

#endif
