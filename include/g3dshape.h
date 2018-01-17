




#ifndef __G3DSHAPE_H__
#define __G3DSHAPE_H__

#include "stdgfx.h"
#include "g3dmath.h"

class G3DOBJECT;

class G3DSHAPE
  {
    public :
      G3DOBJECT *ParentObject;
      LONG Ambient;
      float MinZ,MaxZ; 
      float WorldMinX,WorldMaxX;
      float WorldMinY,WorldMaxY;
      float WorldMinZ,WorldMaxZ;  
      
      G3DSHAPE ();
      virtual ~G3DSHAPE ();
  };

#endif

