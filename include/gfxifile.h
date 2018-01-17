




#ifndef __GFXIMGFILE_H__
#define __GFXIMGFILE_H__

#include "stdgfx.h"
#include "gfximage.h"
#include "gfxpal.h"
#include "gfxfiles.h"

class IMAGEFILETOOL : public MYOBJECT
  {
    protected :
      FILEHANDLE fp;

    public :
      IMAGEFILETOOL ( FILEHANDLE f );
      virtual ~IMAGEFILETOOL ();

      virtual BOOLEAN LoadImage ( IMAGE *Image, RGBPALETTE *Pal )=0;
      virtual BOOLEAN SaveImage ( IMAGE *Image, LONG Sx, LONG Sy,
                                  LONG Wd, LONG Ht, RGBPALETTE *Pal  )=0;
  }; 

#endif
