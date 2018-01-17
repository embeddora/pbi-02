




#ifndef __GFXPALETTE_H__
#define __GFXPALETTE_H__

#include "stdgfx.h"
#include "gfxobj.h"

#define COLOR  LONG

class RGBPALETTE : public MYOBJECT
  {
    protected :
      RGBCOLOR Entry[256];
      double GetColorDistance ( RGBCOLOR Col1, LONG Index );

    public :
      RGBPALETTE ();
      virtual ~RGBPALETTE ();

      RGBCOLOR* GetEntry () { return Entry; };
      LONG GetClosestColor ( RGBCOLOR Color );
      VOID SetColorScale ( RGBCOLOR Color, INT Start, INT End,
                           float StartInt, float EndInt );

      COLOR GetRGB ( BYTE R, BYTE G, BYTE B );
      COLOR GetIndex ( LONG Index );

      VOID SetWindowsPalette ();
      
      BOOLEAN IsIdentical ( RGBPALETTE *Pal );
  }; 

#endif
