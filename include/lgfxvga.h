




#ifndef __LGFXVGA_H__
#define __LGFXVGA_H__

#include "stdgfx.h"
#include "gfxtypes.h"

extern "C"
  {
    VOID SetVGASelector ( SHORT Selector );
    VOID VGACopy256To256 ( BYTE *Src, LONG AddSrc, LONG Wd, LONG Ht,
                           LONG Cx, LONG Cy );
  } 

#endif

