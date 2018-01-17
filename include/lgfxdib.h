




#ifndef __LGFXDIB_H__
#define __LGFXDIB_H__

#include "stdgfx.h"
#include "gfxtypes.h"

extern "C"
  {
    VOID TransBlt256To256 ( BYTE *Dest, BYTE *Src, LONG Wd, BYTE TransColor );
    VOID ScaleBlt256To256 ( BYTE *Dest, BYTE *Src, LONG Wd, LONG Error, DWORD AddError );
    VOID WaitForRetraceMany ( LONG Count );
    VOID WaitForRetraceOnce ();
    VOID WaitForRetraceTerminate ();
  }

#endif

