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


#ifndef __G3DOBJ_H__
#define __G3DOBJ_H__

#include "stdgfx.h"
#include "g3dpoly.h"

#define OBJECT_VISIBLE ((LONG)1<<0)

struct CAMERADATA
  {
    FLPVECTOR3D CameraPoint;
    LONG NumVisibleShapes;
    G3DSHAPE **VisibleShapes;
    float ViewDistance;
    float CenterX,CenterY;
    RECTANGLE ViewPort;
    float NearClipZ;
    LONG ShadeFlags;
    LONG FaceFlags;
    BOOLEAN DepthCueing;
    float DepthScale;
    BOOLEAN DoHaze;
    float HazeScale;
    LONG HazeLevels;
  }; 

struct COLLIDEDATA
  {
    G3DSHAPE *Shape;
    float CollideT;  
  }; 

class G3DLIGHT;

class G3DOBJECT
  {
    protected :
      LONG Type;
      CHAR Name[32];
      LONG ID;
      G3DLIGHT *LightSource;
      float Radius;
      float MaxVisibleDistance;
      float ScaleFactor;
      LONG Flags;
      BOOLEAN Initialized;
      
      FLPVECTOR3D FLPAngle;
      FLPVECTOR3D FLPLocalPosition;
      FLPVECTOR3D FLPWorldPosition;
      FLPVECTOR3D FLPCameraPosition;
      FLPVECTOR3D FLPLocalCenter;
      FLPVECTOR3D FLPWorldCenter;
      FLPVECTOR3D FLPCameraCenter;
      
      G3DMATRIX LocalMatrix;
      G3DMATRIX ResultMatrix;

      LINKEDLIST<G3DOBJECT*> *SubObjectList;
      virtual VOID UpdateLocalToWorld ();
      virtual VOID UpdateWorldToCamera ( G3DMATRIX CameraMatrix,
                                         CAMERADATA *CameraData );
      virtual VOID CountShapes ( LONG *Count );
      virtual BOOLEAN ClipObject ( CAMERADATA *CameraData );
                                               
    public :
      BOOLEAN Moved;
      BOOLEAN Rotated;
    
      G3DOBJECT ();
      virtual ~G3DOBJECT ();

      VOID SetID ( LONG NewID ) { ID = NewID; };
      LONG GetID () { return ID; };

      VOID SetScaleFactor ( float ScaleFactor );
      float GetScaleFactor () { return ScaleFactor; };

      VOID SetName ( STRING NewName );
      STRING GetName () { return Name; };

      VOID SetMaxVisibleDistance ( float Distance );
      float GetMaxVisibleDistance () { return MaxVisibleDistance; };
      
      VOID SetLightSource ( G3DLIGHT *Light ) { LightSource = Light; };
      G3DLIGHT* GetLightSource () { return LightSource; };
      
      LONG GetObjectType () { return Type; };
      virtual VOID SetPosition ( float x, float y, float z );
      virtual VOID SetAngle ( float x, float y, float z );
      float GetRadius () { return Radius; };
      FLPVECTOR3D GetAngle () { return FLPAngle; };
      FLPVECTOR3D GetLocalPosition () { return FLPLocalPosition; };
      FLPVECTOR3D GetWorldPosition () { return FLPWorldPosition; };
      FLPVECTOR3D GetLocalCenter () { return FLPLocalCenter; };
      FLPVECTOR3D GetWorldCenter () { return FLPWorldCenter; };

      VOID TransformLocalToWorld ( G3DMATRIX ParentMatrix );
      VOID TransformWorldToCamera ( G3DMATRIX CameraMatrix,
                                    CAMERADATA *CameraData ); 

      VOID InitLocalMatrix ();
      virtual VOID Init ();

      BOOLEAN IsObjectVisible () { return (Flags&OBJECT_VISIBLE); };
      VOID CountNumShapes ( LONG *Count );

      G3DOBJECT* FindObjectByName ( STRING SearchName );
      G3DOBJECT* FindObjectByID ( LONG SearchID );

      VOID Move ( float xt, float yt, float zt );
      VOID Rotate ( float xa, float ya, float za );

      virtual VOID ComputeCenter ();
      virtual float ComputeRadius ();
      virtual VOID CheckCollision ( FLPVECTOR3D StartPt, FLPVECTOR3D EndPt,
                                    COLLIDEDATA* CollideList, LONG *CurNum, LONG MaxNum,
                                    float CollideDist, float Gap );
  }; 

#endif


