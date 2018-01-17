




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
