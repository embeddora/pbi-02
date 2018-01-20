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



#include "stdgfx.h"
#include "g3dpobj.h"
#include "g3dlight.h"

#include "stdlib.h"

GEOMETRYDATA::GEOMETRYDATA ()
  {
    NumPoints = 0;
    NumPolys = 0;
    SharedCounts = NULL;
    PointVisible = NULL;
    FLPLocalPoints = NULL;
    FLPWorldPoints = NULL;
    FLPCameraPoints = NULL;
    FLPScreenPoints = NULL;
    FLPPointLocalNormals = NULL;
    FLPPointWorldNormals = NULL;
    PointIntensities = NULL;
    HazeValues = NULL;
    InverseZ = NULL;
    Polys = NULL;
    FLPPolyLocalPoints = NULL;
    FLPPolyWorldPoints = NULL;
    FLPPolyLocalNormals = NULL;
    FLPPolyWorldNormals = NULL;
  } 

GEOMETRYDATA::~GEOMETRYDATA ()
  {
    DestroyPoints ();
    DestroyPolys ();  
  } 

BOOLEAN GEOMETRYDATA::CreatePoints ( LONG NewNumPoints )
  {
    DestroyPoints ();  
    NumPoints = NewNumPoints;
    SharedCounts = new LONG [NumPoints];
    PointVisible = new BYTE [NumPoints];
    
    FLPLocalPoints = new FLPVECTOR3D [NumPoints];  
    FLPWorldPoints = new FLPVECTOR3D [NumPoints];  
    FLPCameraPoints = new FLPVECTOR3D [NumPoints];
    FLPScreenPoints = new FLPVECTOR2D [NumPoints];

    FLPPointLocalNormals = new FLPVECTOR3D [NumPoints];
    FLPPointWorldNormals = new FLPVECTOR3D [NumPoints];
    PointIntensities = new float [NumPoints];
    HazeValues = new float [NumPoints];
    InverseZ = new float [NumPoints];
    return SUCCESS;
  } 

BOOLEAN GEOMETRYDATA::CreatePolys ( LONG NewNumPolys )
  {
    DestroyPolys ();  
    NumPolys = NewNumPolys;
    Polys = (G3DPOLY*)malloc(sizeof(G3DPOLY)*NumPolys);
    LONG i;
    for (i=0;i<NumPolys;i++)
      {
        Polys[i] = G3DPOLY ();
        Polys[i].ParentObject = NULL;
      } 

    FLPPolyLocalPoints = new FLPVECTOR3D* [NumPolys];
    FLPPolyWorldPoints = new FLPVECTOR3D* [NumPolys];
    FLPPolyLocalNormals = new FLPVECTOR3D [NumPolys];
    FLPPolyWorldNormals = new FLPVECTOR3D [NumPolys];
    return SUCCESS;
  } 
  
VOID GEOMETRYDATA::DestroyPoints ()
  {
    if (FLPLocalPoints!=NULL)
      delete FLPLocalPoints;   
    FLPLocalPoints = NULL;
    
    if (FLPWorldPoints!=NULL)
      delete FLPWorldPoints;   
    FLPWorldPoints = NULL;
    
    if (FLPCameraPoints!=NULL)
      delete FLPCameraPoints;   
    FLPCameraPoints = NULL;
    
    if (FLPScreenPoints!=NULL)
      delete FLPScreenPoints;   
    FLPScreenPoints = NULL;
    
    if (FLPPointLocalNormals!=NULL)
      delete FLPPointLocalNormals;   
    FLPPointLocalNormals = NULL;
    
    if (FLPPointWorldNormals!=NULL)
      delete FLPPointWorldNormals;   
    FLPPointWorldNormals = NULL;

    if (PointIntensities!=NULL)
      delete PointIntensities;   
    PointIntensities = NULL;

    if (HazeValues!=NULL)
      delete HazeValues;   
    HazeValues = NULL;

    if (InverseZ!=NULL)
      delete InverseZ;   
    InverseZ = NULL;

    if (SharedCounts!=NULL)
      delete SharedCounts;
    SharedCounts = NULL;
    
    if (PointVisible!=NULL)
      delete PointVisible;
    PointVisible = NULL;
    
    NumPoints = 0;
  } 

VOID GEOMETRYDATA::DestroyPolys ()
  {
    if (Polys!=NULL)  
      free ( Polys );
    Polys = NULL;
    
    if (FLPPolyLocalPoints!=NULL)
      delete FLPPolyLocalPoints;   
    FLPPolyLocalPoints = NULL;
    
    if (FLPPolyWorldPoints!=NULL)
      delete FLPPolyWorldPoints;   
    FLPPolyWorldPoints = NULL;
    
    if (FLPPolyLocalNormals!=NULL)
      delete FLPPolyLocalNormals;   
    FLPPolyLocalNormals = NULL;
    
    if (FLPPolyWorldNormals!=NULL)
      delete FLPPolyWorldNormals;   
    FLPPolyWorldNormals = NULL;
    
    NumPolys = 0;
  } 








G3DPOLYHEDRON::G3DPOLYHEDRON () : G3DOBJECT ()
  {
    Type = OBJECT_TYPE_POLYHEDRON;  
    InitGeometryData ();
    CurDetail = MAX_DETAIL;
    ClockWised = FALSE;
    DetailData[MAX_DETAIL].FaceFlags = FACE_TEXTURE;
    DetailData[MAX_DETAIL].ShadeFlags = SHADE_GOURAUD;
    DetailData[MED_DETAIL].FaceFlags = FACE_SOLID;
    DetailData[MED_DETAIL].ShadeFlags = SHADE_FLAT;
    DetailData[MIN_DETAIL].FaceFlags = FACE_WIREFRAME;
    DetailData[MIN_DETAIL].ShadeFlags = SHADE_NONE;
    MorphData = NULL;  
    TextureMapped = FALSE;
  } 

G3DPOLYHEDRON::~G3DPOLYHEDRON ()
  {
    DestroyGeometryData ();
    DestroyMorphData ();
  } 

VOID G3DPOLYHEDRON::SetMaterial ( G3DMATERIAL* Material )

  {
    LONG i;
    for (i=0;i<GeometryData.NumPolys;i++)
      {
        GeometryData.Polys[i].Material = Material;
      } 
  } 

VOID G3DPOLYHEDRON::InitGeometryData ()
  {
    GeometryData = GEOMETRYDATA ();  
  } 

VOID G3DPOLYHEDRON::CreateGeometryData ( LONG NumPoints, LONG NumPolys )
  {
    CreateGeometryPoints ( NumPoints );  
    CreateGeometryPolys ( NumPolys );  
  } 

VOID G3DPOLYHEDRON::CreateGeometryPoints ( LONG NumPoints )
  {
    GeometryData.CreatePoints ( NumPoints );
  } 

VOID G3DPOLYHEDRON::CreateGeometryPolys ( LONG NumPolys )
  {
    GeometryData.CreatePolys ( NumPolys );    
    LONG i;
    for (i=0;i<GeometryData.NumPolys;i++)
      {
        GeometryData.Polys[i].ParentObject = this;
      } 
  } 

VOID G3DPOLYHEDRON::DestroyGeometryPoints ()
  {
    GeometryData.DestroyPoints();
  } 

VOID G3DPOLYHEDRON::DestroyGeometryPolys ()
  {
    GeometryData.DestroyPolys();
  } 

VOID G3DPOLYHEDRON::DestroyGeometryData ()
  {
    DestroyGeometryPoints ();  
    DestroyGeometryPolys ();  
  } 

VOID G3DPOLYHEDRON::SetDetailData ( float Distance )
  {
    LONG i;
    CurDetail = -1;  
    for (i=MAX_DETAIL;i>=MIN_DETAIL;i--)
      {
        if (Distance<=DetailData[i].MaxDistance)
          {
            CurDetail = i;            
          } 
      } 
  } 

VOID G3DPOLYHEDRON::SetFaceFlags ( LONG Level, LONG Flags )
  {
    DetailData[Level].FaceFlags = Flags;  
  } 

VOID G3DPOLYHEDRON::SetShadeFlags ( LONG Level, LONG Flags )
  {
    DetailData[Level].ShadeFlags = Flags;  
  } 

VOID G3DPOLYHEDRON::UpdateLocalToWorld ()
  {
    G3DOBJECT::UpdateLocalToWorld ();

    LONG i;
    LONG NumPoints = GeometryData.NumPoints;
    LONG NumPolys = GeometryData.NumPolys;
    FLPVECTOR3D *LocalPoints;
    FLPVECTOR3D *WorldPoints;
    FLPVECTOR3D *LocalNormals;
    FLPVECTOR3D *WorldNormals;

    LocalPoints = GeometryData.FLPLocalPoints;
    WorldPoints = GeometryData.FLPWorldPoints;
    
    LocalNormals = GeometryData.FLPPointLocalNormals;
    WorldNormals = GeometryData.FLPPointWorldNormals;
    
    float x,y,z;
    float nx,ny,nz;

    float M00,M01,M02;
    float M10,M11,M12;
    float M20,M21,M22;
    float M30,M31,M32;
    float *MatrixPtr;

    MatrixPtr = &(ResultMatrix[0][0]);

    M00 = (*MatrixPtr++);
    M01 = (*MatrixPtr++);
    M02 = (*MatrixPtr++);
    MatrixPtr++;
        
    M10 = (*MatrixPtr++);
    M11 = (*MatrixPtr++);
    M12 = (*MatrixPtr++);
    MatrixPtr++;
        
    M20 = (*MatrixPtr++);
    M21 = (*MatrixPtr++);
    M22 = (*MatrixPtr++);
    MatrixPtr++;
        
    M30 = (*MatrixPtr++);
    M31 = (*MatrixPtr++);
    M32 = (*MatrixPtr);

    BOOLEAN IsGouraud;
        
    if ((DetailData[CurDetail].ShadeFlags==SHADE_GOURAUD)&&(LightSource!=NULL))
      IsGouraud = TRUE;
    else
      IsGouraud = FALSE;
      
    for (i=0;i<NumPoints;i++)
      {
        x = LocalPoints->x;       
        y = LocalPoints->y;
        z = LocalPoints->z;

        WorldPoints->x = x*M00+y*M10+z*M20+M30;
        WorldPoints->y = x*M01+y*M11+z*M21+M31;
        WorldPoints->z = x*M02+y*M12+z*M22+M32;
                
        if (IsGouraud)
          {
            nx = LocalNormals->x;       
            ny = LocalNormals->y;
            nz = LocalNormals->z;

            WorldNormals->x = nx*M00+ny*M10+nz*M20+M30 - WorldPoints->x;
            WorldNormals->y = nx*M01+ny*M11+nz*M21+M31 - WorldPoints->y;
            WorldNormals->z = nx*M02+ny*M12+nz*M22+M32 - WorldPoints->z;
            
            LocalNormals++;
            WorldNormals++;                      
          } 
        
        LocalPoints++;
        WorldPoints++;                      
      } 

    LocalNormals = GeometryData.FLPPolyLocalNormals;
    WorldNormals = GeometryData.FLPPolyWorldNormals;

    FLPVECTOR3D **PolyWorldPoints;
    G3DPOLY *Polys;
    float x0,y0,z0;
    float x1,y1,z1;
    float x2,y2,z2;
    LONG Point1,Point2;

    PolyWorldPoints = GeometryData.FLPPolyWorldPoints;
    Polys = GeometryData.Polys;
    
    for (i=0;i<NumPolys;i++)
      {
        nx = LocalNormals->x;       
        ny = LocalNormals->y;
        nz = LocalNormals->z;

        x = nx*M00+ny*M10+nz*M20+M30;
        y = nx*M01+ny*M11+nz*M21+M31;
        z = nx*M02+ny*M12+nz*M22+M32;

        x0 = (*PolyWorldPoints)->x;
        y0 = (*PolyWorldPoints)->y;
        z0 = (*PolyWorldPoints)->z;
        
        WorldNormals->x = x - x0;
        WorldNormals->y = y - y0;
        WorldNormals->z = z - z0;
        
        LocalNormals++;
        WorldNormals++;
        PolyWorldPoints++;

        Point1 = Polys->Points[1];
        Point2 = Polys->Points[2];

        x1 = GeometryData.FLPWorldPoints[Point1].x;
        y1 = GeometryData.FLPWorldPoints[Point1].y;
        z1 = GeometryData.FLPWorldPoints[Point1].z;
        
        x2 = GeometryData.FLPWorldPoints[Point2].x;
        y2 = GeometryData.FLPWorldPoints[Point2].y;
        z2 = GeometryData.FLPWorldPoints[Point2].z;

        Polys->WorldMinX = MinValue ( x0, MinValue ( x1, x2 ) );
        Polys->WorldMinY = MinValue ( y0, MinValue ( y1, y2 ) );
        Polys->WorldMinZ = MinValue ( z0, MinValue ( z1, z2 ) );
        Polys->WorldMaxX = MaxValue ( x0, MaxValue ( x1, x2 ) );
        Polys->WorldMaxY = MaxValue ( y0, MaxValue ( y1, y2 ) );
        Polys->WorldMaxZ = MaxValue ( z0, MaxValue ( z1, z2 ) );
        
        Polys++;
      } 
  } 

VOID G3DPOLYHEDRON::UpdateWorldToCamera ( G3DMATRIX CameraMatrix,
                                          CAMERADATA *CameraData )
  {
    LONG i;
    LONG NumPoints = GeometryData.NumPoints;
    FLPVECTOR3D *WorldPoints;
    FLPVECTOR3D *CameraPoints;
    FLPVECTOR3D *PointWorldNormals;
    FLPVECTOR2D *ScreenPoints;
    float *Intensities;
    float *HazeValues;
    float *InverseZPtr;

    G3DOBJECT::UpdateWorldToCamera ( CameraMatrix, CameraData );

    if (IsObjectVisible()==FALSE)
      return;
    
    WorldPoints = GeometryData.FLPWorldPoints;
    PointWorldNormals = GeometryData.FLPPointWorldNormals;
    CameraPoints = GeometryData.FLPCameraPoints;
    ScreenPoints = GeometryData.FLPScreenPoints;
    Intensities = GeometryData.PointIntensities;
    HazeValues = GeometryData.HazeValues;
    InverseZPtr = GeometryData.InverseZ;
    
    float M00,M01,M02;
    float M10,M11,M12;
    float M20,M21,M22;
    float M30,M31,M32;
    float *MatrixPtr;

    MatrixPtr = &(CameraMatrix[0][0]);

    M00 = (*MatrixPtr++);
    M01 = (*MatrixPtr++);
    M02 = (*MatrixPtr++);
    MatrixPtr++;
        
    M10 = (*MatrixPtr++);
    M11 = (*MatrixPtr++);
    M12 = (*MatrixPtr++);
    MatrixPtr++;
        
    M20 = (*MatrixPtr++);
    M21 = (*MatrixPtr++);
    M22 = (*MatrixPtr++);
    MatrixPtr++;
        
    M30 = (*MatrixPtr++);
    M31 = (*MatrixPtr++);
    M32 = (*MatrixPtr);

    
    BYTE *PointVisible;
    PointVisible = GeometryData.PointVisible;
    memset ( PointVisible, 0, NumPoints );

    CheckMeshVisibility ( CameraData );

    LONG ShadeFlags;
    ShadeFlags = MinValue ( DetailData[CurDetail].ShadeFlags, CameraData->ShadeFlags );
     
    float x,y,z;
    float cx,cy,cz;
    float CenterX = CameraData->CenterX;
    float CenterY = CameraData->CenterY;
    float ViewDistance = CameraData->ViewDistance;
    BOOLEAN IsGouraud;
    BOOLEAN DepthCueing;
    float DepthScale;
    BOOLEAN DoHaze;
    float HazeScale;
    float HazeLevels;

    DepthCueing = CameraData->DepthCueing;
    DepthScale = CameraData->DepthScale;
    
    DoHaze = CameraData->DoHaze;
    HazeLevels = (float)(CameraData->HazeLevels-1);
    HazeScale = CameraData->HazeScale;
    
    if ((ShadeFlags==SHADE_GOURAUD)&&(LightSource!=NULL))
      {
        IsGouraud = TRUE;  
      } 
    else
      IsGouraud = FALSE;  

    for (i=0;i<NumPoints;i++)
      {
        if (*PointVisible)
          {      
            x = WorldPoints->x;       
            y = WorldPoints->y;
            z = WorldPoints->z;

            if (IsGouraud)
              *Intensities = LightSource->ComputeIntensity ( WorldPoints, PointWorldNormals );  
            
            cx = x*M00+y*M10+z*M20+M30;
            cy = x*M01+y*M11+z*M21+M31;
            cz = x*M02+y*M12+z*M22+M32;

            if (cz==0)
              cz = (float)0.1;

            float Ratio;
            *InverseZPtr = Ratio = (float)1/cz;
            if (DepthCueing)
              {
                if (cz>DepthScale)
                  (*Intensities) *= Ratio*DepthScale;  
              } 
              
            if (DoHaze)
              {
                if (cz>HazeScale)
                  (*HazeValues) = Ratio*65536*HazeScale*HazeLevels;
                else  
                  (*HazeValues) = HazeLevels*65536;
              } 
            
            ScreenPoints->x = ViewDistance*Ratio*cx + CenterX;
            ScreenPoints->y = -ViewDistance*Ratio*cy + CenterY;

            CameraPoints->x = cx;
            CameraPoints->y = cy;
            CameraPoints->z = cz;
          } 
                                  
        PointVisible++;
        WorldPoints++;
        CameraPoints++;
        ScreenPoints++;
        PointWorldNormals++;
        Intensities++;
        HazeValues++;
        InverseZPtr++;
      } 

    GetPolyMinMax ();  
  } 

VOID G3DPOLYHEDRON::CheckMeshVisibility ( CAMERADATA *CameraData ) 
  {
    LONG i,NumPolys;
    G3DPOLY *PolyPtr;
    FLPVECTOR3D *PointList,*Point;
    FLPVECTOR3D CameraPoint = CameraData->CameraPoint;
    LONG NumVisibleShapes = CameraData->NumVisibleShapes;
    G3DSHAPE **VisibleShapes = CameraData->VisibleShapes;

    PolyPtr = GeometryData.Polys;
    NumPolys = GeometryData.NumPolys;
    PointList = GeometryData.FLPWorldPoints;

    FLPVECTOR3D DirV;
    FLPVECTOR3D *Normals;
    float DotProduct;
    BYTE *PointVisible;
    BOOLEAN IsFlat;

    LONG ShadeFlags;
    ShadeFlags = MinValue ( DetailData[CurDetail].ShadeFlags, CameraData->ShadeFlags );
    
    if ((ShadeFlags==SHADE_FLAT)&&(LightSource!=NULL))
      IsFlat = TRUE;
    else
      IsFlat = FALSE;     

    PointVisible = GeometryData.PointVisible;
    Normals = GeometryData.FLPPolyWorldNormals;
    
    LONG List0,List1,List2;
    
    for (i=0;i<NumPolys;i++)
      {
        List0 = PolyPtr->Points[0];
        Point = &(PointList[List0]);
        FLPVectorSub ( &CameraPoint, Point, &DirV );
        DotProduct = FLPVectorDot ( Normals, &DirV );
        if (DotProduct<=0)
          PolyPtr->Visible = FALSE;
        else
          {
            PolyPtr->Visible = TRUE;
            VisibleShapes[NumVisibleShapes] = PolyPtr;
            NumVisibleShapes++;
            List1 = PolyPtr->Points[1];
            List2 = PolyPtr->Points[2];
            PointVisible[List0] = 1;
            PointVisible[List1] = 1;
            PointVisible[List2] = 1;

            if (IsFlat)
              {
                PolyPtr->Intensity =
                  LightSource->ComputeIntensity ( Point, Normals ); 
              } 
          } 
        Normals++;
        PolyPtr++;
      } 
    CameraData->NumVisibleShapes = NumVisibleShapes;  
  } 

VOID G3DPOLYHEDRON::CountPointShare ()
  {
    LONG i;
    LONG List0,List1,List2;
    
    
    for (i=0;i<GeometryData.NumPoints;i++)
      {
        GeometryData.SharedCounts[i] = 0;
        GeometryData.FLPPointLocalNormals[i].x = (float)0;
        GeometryData.FLPPointLocalNormals[i].y = (float)0;
        GeometryData.FLPPointLocalNormals[i].z = (float)0;
      } 

    
    for (i=0;i<GeometryData.NumPolys;i++)
      {
        List0 = GeometryData.Polys[i].Points[0];  
        List1 = GeometryData.Polys[i].Points[1];  
        List2 = GeometryData.Polys[i].Points[2];  
        GeometryData.SharedCounts[List0]++;  
        GeometryData.SharedCounts[List1]++;  
        GeometryData.SharedCounts[List2]++;
        GeometryData.FLPPolyLocalPoints[i] = &(GeometryData.FLPLocalPoints[List0]);
        GeometryData.FLPPolyWorldPoints[i] = &(GeometryData.FLPWorldPoints[List0]);
      } 
  } 

VOID G3DPOLYHEDRON::ComputePolyLocalNormals ()
  {
    LONG i;
    FLPVECTOR3D V1,V2,Normal;
    FLPVECTOR3D *PointLocalNormals;

    PointLocalNormals = GeometryData.FLPPointLocalNormals;

    LONG List0,List1,List2;
    
    for (i=0;i<GeometryData.NumPolys;i++)
      {
        List0 = GeometryData.Polys[i].Points[0];  
        List1 = GeometryData.Polys[i].Points[1];  
        List2 = GeometryData.Polys[i].Points[2];

        FLPVectorSub ( &(GeometryData.FLPLocalPoints[List0]),
                       &(GeometryData.FLPLocalPoints[List1]),
                       &V1 );
        FLPVectorSub ( &(GeometryData.FLPLocalPoints[List0]),
                       &(GeometryData.FLPLocalPoints[List2]),
                       &V2 );
        FLPVectorCross ( &V1, &V2, &Normal );
        FLPVectorNormalize ( &Normal );

        if (ClockWised)  
          {
            Normal.x = -Normal.x;  
            Normal.y = -Normal.y;  
            Normal.z = -Normal.z;  
          } 
        
        PointLocalNormals[List0].x += Normal.x;
        PointLocalNormals[List0].y += Normal.y;
        PointLocalNormals[List0].z += Normal.z;

        PointLocalNormals[List1].x += Normal.x;
        PointLocalNormals[List1].y += Normal.y;
        PointLocalNormals[List1].z += Normal.z;

        PointLocalNormals[List2].x += Normal.x;
        PointLocalNormals[List2].y += Normal.y;
        PointLocalNormals[List2].z += Normal.z;

        FLPVectorAdd ( &Normal, &(GeometryData.FLPLocalPoints[List0]),
                       &(GeometryData.FLPPolyLocalNormals[i]) );
      } 
  } 

VOID G3DPOLYHEDRON::ComputePointLocalNormals ()
  {
    LONG i;
    FLPVECTOR3D *LocalPoints;
    FLPVECTOR3D *Normals;

    LocalPoints = GeometryData.FLPLocalPoints;
    Normals = GeometryData.FLPPointLocalNormals;

    for (i=0;i<GeometryData.NumPoints;i++)
      {
        float Ratio;  
        if (GeometryData.SharedCounts[i]>0)
          {
            Ratio = (float)1/(float)GeometryData.SharedCounts[i];
            Normals[i].x *= Ratio;  
            Normals[i].y *= Ratio;  
            Normals[i].z *= Ratio;
          } 

        FLPVectorNormalize ( &(Normals[i]) );
        Normals[i].x += LocalPoints[i].x;
        Normals[i].y += LocalPoints[i].y;
        Normals[i].z += LocalPoints[i].z;
      } 
  } 

VOID G3DPOLYHEDRON::GetPolyMinMax ()
  {
    G3DPOLY *PolyPtr;
    FLPVECTOR3D *CameraPoints;
    
    PolyPtr = GeometryData.Polys;
    CameraPoints = GeometryData.FLPCameraPoints;

    LONG i;
    LONG List0,List1,List2;
    float z0,z1,z2;
    
    for (i=0;i<GeometryData.NumPolys;i++)
      {
        if (PolyPtr->Visible)
          {
            List0 = PolyPtr->Points[0];  
            List1 = PolyPtr->Points[1];  
            List2 = PolyPtr->Points[2];
            z0 = CameraPoints[List0].z;
            z1 = CameraPoints[List1].z;
            z2 = CameraPoints[List2].z;

            PolyPtr->MinZ = MinValue ( z0, MinValue ( z1, z2 ) );
            PolyPtr->MaxZ = MaxValue ( z0, MaxValue ( z1, z2 ) );
          } 
        PolyPtr++;
      } 
  } 

VOID G3DPOLYHEDRON::MapTexture ( float U0, float V0,
                                 float U1, float V1,
                                 float U2, float V2 )
  {
    double IntPart;
    if (U0>(float)1.0)
      U0 = modf ( (double)U0, &IntPart );
    if (V0>(float)1.0)
      V0 = modf ( (double)V0, &IntPart );
    if (U1>(float)1.0)
      U1 = modf ( (double)U1, &IntPart );
    if (V1>(float)1.0)
      V1 = modf ( (double)V1, &IntPart );
    if (U2>(float)1.0)
      U2 = modf ( (double)U2, &IntPart );
    if (V2>(float)1.0)
      V2 = modf ( (double)V2, &IntPart );
    
    LONG i;
    for (i=0;i<GeometryData.NumPolys;i++)
      {
        GeometryData.Polys[i].TexCoords[0].x = U0*255*65536;
        GeometryData.Polys[i].TexCoords[0].y = V0*255*65536;
        GeometryData.Polys[i].TexCoords[1].x = U1*255*65536;
        GeometryData.Polys[i].TexCoords[1].y = V1*255*65536;
        GeometryData.Polys[i].TexCoords[2].x = U2*255*65536;
        GeometryData.Polys[i].TexCoords[2].y = V2*255*65536;
      } 
  } 

VOID G3DPOLYHEDRON::Init ()
  {
    ComputeCenter ();
    
    LISTOBJECT<G3DOBJECT*> *ObjectNode;
    ObjectNode = SubObjectList->GetHead ();

    while (ObjectNode!=NULL)
      {
        ObjectNode->Data->Init ();
        ObjectNode = ObjectNode->NextObject;  
      } 
      
    CountPointShare ();
    ComputePolyLocalNormals ();
    ComputePointLocalNormals ();
  } 

VOID G3DPOLYHEDRON::CountShapes ( LONG *Count )
  {
    *Count += GeometryData.NumPolys;  
  } 

VOID G3DPOLYHEDRON::ComputeCenter ()
  {
    LONG i;
    float MinX,MaxX;
    float MinY,MaxY;
    float MinZ,MaxZ;

    if (GeometryData.FLPLocalPoints==NULL)
      {
        return;
      } 

    MinX = MaxX = GeometryData.FLPLocalPoints[0].x;
    MinY = MaxY = GeometryData.FLPLocalPoints[0].y;
    MinZ = MaxZ = GeometryData.FLPLocalPoints[0].z;

    for (i=1;i<GeometryData.NumPoints;i++)
      {
        if (GeometryData.FLPLocalPoints[i].x<MinX)
          MinX = GeometryData.FLPLocalPoints[i].x;
        else if (GeometryData.FLPLocalPoints[i].x>MaxX)  
          MaxX = GeometryData.FLPLocalPoints[i].x;
          
        if (GeometryData.FLPLocalPoints[i].y<MinY)
          MinY = GeometryData.FLPLocalPoints[i].y;  
        else if (GeometryData.FLPLocalPoints[i].y>MaxY)  
          MaxY = GeometryData.FLPLocalPoints[i].y;
          
        if (GeometryData.FLPLocalPoints[i].y<MinZ)
          MinZ = GeometryData.FLPLocalPoints[i].z;  
        else if (GeometryData.FLPLocalPoints[i].z>MaxZ)  
          MaxZ = GeometryData.FLPLocalPoints[i].z;
      } 

    FLPLocalCenter.x = ( MaxX + MinX ) / 2; 
    FLPLocalCenter.y = ( MaxY + MinY ) / 2; 
    FLPLocalCenter.z = ( MaxZ + MinZ ) / 2; 
  } 

float G3DPOLYHEDRON::ComputeRadius ()
  {
    float Distance;
    float MaxDist;

    LONG i;

    MaxDist = (float)0;
    FLPVECTOR3D Center;
    FLPVECTOR3D Point;

    Center = FLPLocalCenter;
        
    for (i=0;i<GeometryData.NumPoints;i++)
      {
        Point = GeometryData.FLPLocalPoints[i];
        Distance = FLPVectorDistance ( &Center,
                                       &Point );
        if (MaxDist<Distance)
          MaxDist = Distance;          
      } 
    
    LISTOBJECT<G3DOBJECT*> *ObjectNode;
    FLPVECTOR3D SubCenter;
    G3DOBJECT *Object;
    ObjectNode = SubObjectList->GetHead ();

    while (ObjectNode!=NULL)
      {
        Object = ObjectNode->Data;
        SubCenter = Object->GetLocalCenter ();
        Distance = Object->ComputeRadius ();
        Distance += FLPVectorDistance ( &FLPLocalCenter, &SubCenter );
        if (MaxDist<Distance)
          MaxDist = Distance;
        ObjectNode = ObjectNode->NextObject;  
      } 

    Radius = MaxDist;  
    return MaxDist;  
  } 
  
float G3DPOLYHEDRON::ComputeTforPoly ( FLPVECTOR3D Normal, FLPVECTOR3D Base, FLPVECTOR3D Dir )
  {
    float T;
    float Denom,Value;
      
    Denom = Normal.x*Dir.x+Normal.y*Dir.y+Normal.z*Dir.z;
    Value = Normal.x*Base.x+Normal.y*Base.y+Normal.z*Base.z+Normal.w;
    if (Denom==0)
      return (float)(-1);

    T = (-Value)/Denom;  
    
    return T;       
  } 

VOID G3DPOLYHEDRON::CheckCollision ( FLPVECTOR3D StartPt, FLPVECTOR3D EndPt,
                                     COLLIDEDATA* CollideList, LONG *CurNum, LONG MaxNum,
                                     float CollideDist, float Gap )
  {
    if (*CurNum>=MaxNum)
      return;
        
    float MinX,MinY,MinZ;      
    float MaxX,MaxY,MaxZ;

    MinX = FLPWorldCenter.x - Radius;
    MaxX = FLPWorldCenter.x + Radius;
    MinY = FLPWorldCenter.y - Radius;
    MaxY = FLPWorldCenter.y + Radius;    
    MinZ = FLPWorldCenter.z - Radius;
    MaxZ = FLPWorldCenter.z + Radius;

    if (((StartPt.x>MinX)&&(StartPt.x<MaxX)&&
          (StartPt.y>MinY)&&(StartPt.y<MaxY)&&
           (StartPt.z>MinZ)&&(StartPt.z<MaxZ)) ||
        ((EndPt.x>MinX)&&(EndPt.x<MaxX)&&
          (EndPt.y>MinY)&&(EndPt.y<MaxY)&&
           (EndPt.z>MinZ)&&(EndPt.z<MaxZ)))
      {
        FLPVECTOR3D Dir,Normal;
        G3DPOLY *Polys;
        FLPVECTOR3D *NormalPtr;
        FLPVECTOR3D **PolyPointPtr;
        float wx,wy,wz;

        FLPVectorSub ( &StartPt, &EndPt, &Dir );
        Polys = GeometryData.Polys;
        PolyPointPtr = GeometryData.FLPPolyWorldPoints;
        NormalPtr = GeometryData.FLPPolyWorldNormals;
        LONG i;
        for (i=0;i<GeometryData.NumPolys;i++)
          {
            wx = (*PolyPointPtr)->x;  
            wy = (*PolyPointPtr)->y;  
            wz = (*PolyPointPtr)->z;
            Normal.x = NormalPtr->x;
            Normal.y = NormalPtr->y;
            Normal.z = NormalPtr->z;
            Normal.w = -(Normal.x*wx+Normal.y*wy+Normal.z*wz);
            float T;
            FLPVECTOR3D I; 
            
            T = ComputeTforPoly ( Normal, StartPt, Dir );
            if ((T>0)&&(T<CollideDist))
              {
                I.x = StartPt.x + T*Dir.x;  
                I.y = StartPt.y + T*Dir.y;  
                I.z = StartPt.z + T*Dir.z;
                if ((I.x>Polys->WorldMinX-Gap)&&(I.x<Polys->WorldMaxX+Gap)&&
                      (I.y>Polys->WorldMinY-Gap)&&(I.y<Polys->WorldMaxY+Gap)&&
                       (I.z>Polys->WorldMinZ-Gap)&&(I.z<Polys->WorldMaxZ+Gap))
                  {
                    CollideList[*CurNum].Shape = Polys;
                    CollideList[*CurNum].CollideT = T;
                    (*CurNum)++;
                    if (*CurNum>=MaxNum)
                      return;                    
                  } 
              } 
            Polys++;
            PolyPointPtr++;
            NormalPtr++;
          } 
        LISTOBJECT<G3DOBJECT*> *ObjectNode;
        G3DOBJECT *Object;
        ObjectNode = SubObjectList->GetHead ();

        while (ObjectNode!=NULL)
          {
            Object = ObjectNode->Data;
            Object->CheckCollision ( StartPt, EndPt, CollideList, CurNum, MaxNum,
                                     CollideDist, Gap );
            ObjectNode = ObjectNode->NextObject;  
          } 
      } 
  } 

BOOLEAN G3DPOLYHEDRON::CreateMorphData ( GEOMETRYDATA *TargetGeometry, LONG NumFrames )
  {
    if (NumFrames<=0)
      return FAILURE;
        
    FLPVECTOR3D *NewPoints;
    FLPVECTOR3D *OldPoints;

    NewPoints = TargetGeometry->FLPLocalPoints;
    OldPoints = GeometryData.FLPLocalPoints;

    DestroyMorphData ();
    MorphData = new MORPHDATA;
    if (MorphData==NULL)
      return FAILURE;
    MorphData->FLPLocalPointsDelta = new FLPVECTOR3D [TargetGeometry->NumPoints];
    if (MorphData->FLPLocalPointsDelta==NULL)
      DestroyMorphData ();
    MorphData->NumFrames = NumFrames;

    LONG i;
    float Ratio;
    Ratio = (float)1/NumFrames;
    FLPVECTOR3D *Deltas;

    Deltas = MorphData->FLPLocalPointsDelta;
    for (i=0;i<TargetGeometry->NumPoints;i++)
      {
        Deltas->x = (NewPoints->x-OldPoints->x)*Ratio;
        Deltas->y = (NewPoints->x-OldPoints->y)*Ratio;
        Deltas->z = (NewPoints->x-OldPoints->z)*Ratio;
        Deltas++;
        NewPoints++;
        OldPoints++;
      } 
    MorphData->CurFrame = 0;
    return 0;
  } 
  
VOID G3DPOLYHEDRON::DestroyMorphData ()
  {
    if (MorphData!=NULL)
      {
        if (MorphData->FLPLocalPointsDelta!=NULL)
          delete MorphData->FLPLocalPointsDelta;  
        delete MorphData;
      } 
    MorphData = NULL;
  } 

VOID G3DPOLYHEDRON::Warp ( LONG NumSteps )
  {
    if (MorphData==NULL)
      return;
    if (MorphData->FLPLocalPointsDelta==NULL)
      return;

    FLPVECTOR3D *Deltas;
    FLPVECTOR3D *Points;
    LONG i;
    
    Deltas = MorphData->FLPLocalPointsDelta;
    Points = GeometryData.FLPLocalPoints;

    if (NumSteps+MorphData->CurFrame>=MorphData->NumFrames)
      NumSteps = MorphData->NumFrames-MorphData->CurFrame-1;
      
    if (NumSteps<=0)
      return;

    if (NumSteps==1)
      {
        for (i=0;i<GeometryData.NumPoints;i++)
          {
            Points->x += Deltas->x;  
            Points->y += Deltas->y;  
            Points->z += Deltas->z;
            Points++;
            Deltas++;
          } 
      } 
    else
      {
        for (i=0;i<GeometryData.NumPoints;i++)
          {
            Points->x += Deltas->x*NumSteps;  
            Points->y += Deltas->y*NumSteps;  
            Points->z += Deltas->z*NumSteps;
            Points++;
            Deltas++;
          } 
      } 
      
    MorphData->CurFrame++;
    if (MorphData->CurFrame>=MorphData->NumFrames-1)
      DestroyMorphData ();
  } 


