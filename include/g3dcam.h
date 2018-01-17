




#ifndef __G3DCAMERA_H__
#define __G3DCAMERA_H__

#include "stdgfx.h"
#include "g3dobj.h"

#define OBJECT_TYPE_CAMERA  3

class G3DCAMERA : public G3DOBJECT
  {
    protected :      
    public :      
      G3DCAMERA ();
      virtual ~G3DCAMERA ();

      virtual VOID SetPosition ( float x, float y, float z );
  }; 

#endif

