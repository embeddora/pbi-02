/*
 * Copyright (C) 2018 ARNERI arneri@ukr.net All rights reserved
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.

 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * Abstract: 
 */




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


