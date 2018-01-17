




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
