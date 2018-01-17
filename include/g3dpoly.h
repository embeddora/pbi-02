




#ifndef __G3DPOLY_H__
#define __G3DPOLY_H__

#include "stdgfx.h"
#include "g3dshape.h"
#include "g3dmatrl.h"

class G3DPOLY : public G3DSHAPE
  {
    public :
      BOOLEAN Visible;
      LONG Points[3];
      float Intensity;
      FLPVECTOR2D TexCoords[3];
      G3DMATERIAL *Material;
    
      G3DPOLY ();
      virtual ~G3DPOLY ();
  }; 
  
#endif  





