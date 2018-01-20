


#include "stdgfx.h"

#include "gfxdisp.h"

#include "stdlib.h"



DISPLAYDRIVER::DISPLAYDRIVER () : DRIVER ()

  {

    MagH = 1;  

    DivH = 1;  

    MagV = 1;  

    DivV = 1;  

    Info = (BITMAPINFO*)malloc(sizeof(BITMAPINFOHEADER)+sizeof(RGBQUAD)*256);

  }



DISPLAYDRIVER::~DISPLAYDRIVER ()

  {

    if (Info!=NULL)

      delete Info;

    Info = NULL;

  } // End of Destructor for DISPLAYDRIVER



VOID DISPLAYDRIVER::Clear ( HDISPLAY hDisplay, LONG Color )

  {

    if (hDisplay)

      {}

    if (Color)

      {}

  } 



VOID DISPLAYDRIVER::SetScaleFactor ( LONG MH, LONG DH, LONG MV, LONG DV )

  {

    MagH = MH;  

    DivH = DH;  

    MagV = MV;  

    DivV = DV;  

  }

  

VOID DISPLAYDRIVER::DrawText ( HDISPLAY hDisplay, STRING Text, LONG x, LONG y,

                               COLOR Color )

  {

    if (hDisplay)

      {}

    if (Text)

      {}

    if (Color)

      {}

    if (x&y)

      {}

  } 

