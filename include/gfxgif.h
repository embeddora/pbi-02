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



#ifndef __GFXGIFFILE_h__
#define __GFXGIFFILE_h__

#include "stdgfx.h"
#include "gfxifile.h"

#define scdGCT       0x80
#define scdRES       0x70
#define scdSORT      0x08
#define scdSIZE      0x07

#define imdLCT       0x80
#define imdINTERLACE 0x40
#define imdSORT      0x20
#define imdRESV      0x18
#define imdSIZE      0x07

#define HASHSIZE  5011
#define NULLCODE  -1

struct GIFHEADER
  {
    CHAR ID[3];
    CHAR Version[3];
  }; 

struct GIFSCRDSC
  {
    SHORT Width;
    SHORT Height;
    BYTE  Flags;
    BYTE  BGColor;
    BYTE  AspectRatio;

    BOOLEAN IsGCT ()
      {
        if (Flags&scdGCT)
          return TRUE;
        return FALSE;
      }; 

    BOOLEAN IsSorted ()
      {
        if (Flags&scdSORT)
          return TRUE;
        return FALSE;
      }; 

    INT ColorDepth ()
      {
        INT Depth = Flags&scdRES;
        return (Depth >> 4) + 1;
      }; 

    INT NumColors ()
      {
        if (IsGCT())
          return 1 << ((Flags&scdSIZE)+1);
        return 0;
      }; 
  }; 

struct GIFIMGDSC
  {
    BYTE  ID;
    SHORT xLeft;
    SHORT yLeft;
    SHORT Width;
    SHORT Height;
    BYTE  Flags;

    BOOLEAN IsLCT ()
      {
        if (Flags&imdLCT)
          return TRUE;
        return FALSE;
      };

    BOOLEAN IsSorted ()
      {
        if (Flags&imdSORT)
          return TRUE;
        return FALSE;
      };

    BOOLEAN IsInterlaced ()
      {
        if (Flags&imdINTERLACE)
          return TRUE;
        return FALSE;
      };

    INT NumColors ()
      {
        if (IsLCT ())
          return 1 << ((Flags&imdSIZE)+1);
        return 0;
      };
  }; 

class GIFFILETOOL : public IMAGEFILETOOL
  {
    protected :
      GIFHEADER GIFHeader;
      GIFSCRDSC ScrDSC;
      GIFIMGDSC ImgDSC;

      
      SHORT Capacity;
      SHORT *CodeTable;
      SHORT *Prefix;
      SHORT *Suffix;

      SHORT *CodeStack;
      SHORT StackPtr;

      SHORT CurBits,MinBits;
      SHORT NextAvailCode,MaxCode;
      SHORT CurCode;
      SHORT PreCode;
      SHORT FirstCh;

      SHORT RootSize;
      SHORT ClearCode;
      SHORT EndCode;

      SHORT NumBytes;
      SHORT ByteCount;
      SHORT CurByte;
      SHORT CurByteMask;
      BYTE  *DataBlock;
      

      void DeInit ();

      
      BOOLEAN Decode ( IMAGE *Image );
      BOOLEAN InitDecoding ();
      BOOLEAN FindStr ( SHORT Code );
      void ClearTable ();
      INT GetCodeSize ();
      BOOLEAN ReadRow ( IMAGE *Image, INT Row );
      void PushCode ( SHORT Code );
      SHORT PopCode ( );
      SHORT GetCode ();
      BYTE  GetByte ();
      SHORT GetFirstCh ();
      void AddNewString ( SHORT PreCode, SHORT NewCode );
      

      
      void PutCode ( SHORT Code );
      void AddNewString ( INT Pre, INT Cur, INT Index );
      void FlushAll ();
      void PutByte ();
      BOOLEAN Encode ( IMAGE *Image, INT Sx, INT Sy, INT Wd, INT Ht );
      BOOLEAN WriteRow ( IMAGE *Image, INT Row, INT Sx, INT Wd );
      void InitTable ();
      INT FindStr ( INT Pre, INT Cur );
      BOOLEAN InitEncoding ();
      void PutCodeSize ( BYTE CodeSize );
      

      BOOLEAN ReadHeader ();
      BOOLEAN ReadScrDSC ();
      BOOLEAN ReadGCT ( RGBPALETTE *Pal );
      BOOLEAN ReadImgDSC ();
      BOOLEAN ReadLCT ( RGBPALETTE *Pal );
      BOOLEAN ReadImage ( IMAGE *Image, RGBPALETTE *Pal );

      BOOLEAN WriteHeader ();
      BOOLEAN WriteScrDSC ( INT Wd, INT Ht );
      BOOLEAN WriteGCT ( RGBPALETTE *Pal );
      BOOLEAN WriteImgDSC ( INT Wd, INT Ht );
      BOOLEAN WriteImage ( IMAGE *Image, INT Sx, INT Sy, INT Wd, INT Ht );

    public :
      GIFFILETOOL ( FILEHANDLE f );
      virtual ~GIFFILETOOL ();

      virtual BOOLEAN LoadImage ( IMAGE *Image, RGBPALETTE *Pal );
      virtual BOOLEAN SaveImage ( IMAGE *Image, LONG Sx, LONG Sy,
                                  LONG Wd, LONG Ht, RGBPALETTE *Pal );
  }; 

#endif

