




#ifndef __G3DWORLD_H__
#define __G3DWORLD_H__

#include "stdgfx.h"
#include "g3dpobj.h"

class G3DWORLD 
  {
    protected :
      LINKEDLIST<G3DOBJECT*> *ObjectList;
    
    public :
      G3DWORLD ();
      virtual ~G3DWORLD ();

      BOOLEAN AddObject ( G3DOBJECT * NewObject );
      VOID TransformWorldToCamera ( G3DMATRIX CameraMatrix,
                                    CAMERADATA *CameraData ); 
      BOOLEAN Init ();
      
      G3DOBJECT* FindObjectByName ( STRING SearchName );
      G3DOBJECT* FindObjectByID ( LONG SearchID );

      LONG CountNumShapes ();
      LONG CheckCollision ( FLPVECTOR3D StartPt, FLPVECTOR3D EndPt,
                            COLLIDEDATA *CollideList, LONG MaxNum,
                            float CollideDist, float Gap );
  }; 

#endif


