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




#ifndef __G3DPOBJ_H__
#define __G3DPOBJ_H__

#include "stdgfx.h"
#include "g3dobj.h"

#define MIN_DETAIL  0
#define MED_DETAIL  1
#define MAX_DETAIL  2

#define OBJECT_TYPE_POLYHEDRON  1

#define CLOCK_WISE          TRUE
#define COUNTER_CLOCK_WISE  FALSE

struct DETAILDATA
  {
    float MaxDistance;  
    LONG ShadeFlags;     
    LONG FaceFlags;      
  }; 

struct GEOMETRYDATA
  {
    LONG NumPoints;
    LONG NumPolys;
    LONG *SharedCounts;   
    BYTE *PointVisible;
    
    G3DPOLY *Polys;
    
    FLPVECTOR3D *FLPLocalPoints;
    FLPVECTOR3D *FLPWorldPoints;
    FLPVECTOR3D *FLPCameraPoints;
    FLPVECTOR2D *FLPScreenPoints; 
    
    FLPVECTOR3D *FLPPointLocalNormals;
    FLPVECTOR3D *FLPPointWorldNormals;
    float *PointIntensities;
    float *HazeValues;
    float *InverseZ;  
    
    FLPVECTOR3D **FLPPolyLocalPoints;  
    FLPVECTOR3D **FLPPolyWorldPoints;  
    FLPVECTOR3D *FLPPolyLocalNormals;
    FLPVECTOR3D *FLPPolyWorldNormals;

    GEOMETRYDATA ();
    ~GEOMETRYDATA ();

    BOOLEAN CreatePoints ( LONG NumPoints );
    BOOLEAN CreatePolys ( LONG NumPolys );
    VOID DestroyPoints ();
    VOID DestroyPolys ();
  }; 

struct MORPHDATA
  {
    LONG NumFrames;
    LONG CurFrame;  
    FLPVECTOR3D *FLPLocalPointsDelta;
  }; 

class G3DPOLYHEDRON : public G3DOBJECT
  {
    protected :
      LONG CurDetail;
      DETAILDATA DetailData[3];
      GEOMETRYDATA GeometryData;
      MORPHDATA *MorphData;
            
      VOID InitGeometryData ();
      VOID SetDetailData ( float Distance );
      
      VOID CheckMeshVisibility ( CAMERADATA *CameraData ); 
      virtual VOID UpdateLocalToWorld ();
      virtual VOID UpdateWorldToCamera ( G3DMATRIX CameraMatrix,
                                         CAMERADATA *CameraData ); 
      virtual VOID CountShapes ( LONG *Count );

      VOID CountPointShare ();
      VOID ComputePolyLocalNormals ();
      VOID ComputePointLocalNormals ();

      VOID GetPolyMinMax ();
      float ComputeTforPoly ( FLPVECTOR3D Normal, FLPVECTOR3D Base, FLPVECTOR3D Dir );
            
    public :
      BOOLEAN ClockWised;
      BOOLEAN TextureMapped;
    
      G3DPOLYHEDRON ();
      virtual ~G3DPOLYHEDRON ();

      VOID CreateGeometryData ( LONG NumPoints, LONG NumPolys );
      VOID CreateGeometryPoints ( LONG NumPoints );
      VOID CreateGeometryPolys ( LONG NumPolys );
      VOID DestroyGeometryPoints ();
      VOID DestroyGeometryPolys ();
      VOID DestroyGeometryData ();

      DETAILDATA* GetDetailData ( LONG Level ) { return &(DetailData[Level]); };
      LONG GetCurDetail () { return CurDetail; };
      DETAILDATA* GetCurDetailData () { return &(DetailData[CurDetail]); };

      VOID SetMaterial ( G3DMATERIAL* Material );
      VOID MapTexture ( float U0, float V0,
                        float U1, float V1,
                        float U2, float V2 );

      VOID SetFaceFlags ( LONG Level, LONG Flags );
      VOID SetShadeFlags ( LONG Level, LONG Flags );
      LONG GetFaceFlags ( LONG Level ) { return DetailData[Level].FaceFlags; };
      LONG GetShadeFlags ( LONG Level ) { return DetailData[Level].ShadeFlags; };
      
      GEOMETRYDATA *GetGeometryData () { return &GeometryData; };
      virtual VOID Init ();
      
      virtual VOID ComputeCenter ();
      virtual float ComputeRadius ();
      virtual VOID CheckCollision ( FLPVECTOR3D StartPt, FLPVECTOR3D EndPt,
                                    COLLIDEDATA* CollideList, LONG *CurNum, LONG MaxNum,
                                    float CollideDist, float Gap );
      BOOLEAN CreateMorphData ( GEOMETRYDATA *TargetGeometry, LONG NumFrames );
      VOID Warp ( LONG NumSteps );
      VOID DestroyMorphData ();
  }; 

#endif


