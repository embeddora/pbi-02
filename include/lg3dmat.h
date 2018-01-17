




#ifndef __LG3DMATRIX_H__
#define __LG3DMATRIX_H__

#include "stdgfx.h"
#include "g3ddef.h"

extern "C"
  {    
    VOID _FLPMultMatrix ( G3DMATRIX Mat1, G3DMATRIX Mat2, G3DMATRIX Result );
  }
  
VOID _FLPMultMatrix ( G3DMATRIX Mat1, G3DMATRIX Mat2, G3DMATRIX Result );

#endif  

