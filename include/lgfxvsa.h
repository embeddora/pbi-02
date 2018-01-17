




#ifndef __LGFXVESA_H__
#define __LGFXVESA_H__

#include "stdgfx.h"
#include "gfxtypes.h"

extern "C"
  {
    VOID SetVESASelector ( SHORT Selector );
    VOID SetVESAData ( LONG Width, LONG Height, LONG RowSize );
    VOID VESAClear256 ( BYTE Color );
    VOID VESACopy256To256 ( BYTE *Src, LONG AddSrc, LONG Wd, LONG Ht,
                            LONG Cx, LONG Cy );
  } 

#endif
