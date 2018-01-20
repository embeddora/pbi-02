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
#include "g3ddraw.h"
#include "lg3dras.h"
#include "math.h"
#if defined (__FORWATCOM__)
  #include "lg3d256.h"
#endif

#define CLIP_TOP    (LONG)(1<<0)
#define CLIP_LEFT   (LONG)(1<<1)
#define CLIP_BOTTOM (LONG)(1<<2)
#define CLIP_RIGHT  (LONG)(1<<3)

# if 0

LONG FXPMult ( LONG Val1, LONG MulBy );
#pragma aux FXPMult = \
  "imul EDX" \
  "shrd EAX, EDX, 16" \
  parm [EAX] [EDX] \
  value [EAX];

LONG FXPDiv ( LONG Val1, LONG DivBy );
#pragma aux FXPDiv = \
  "cdq" \
  "shld EDX, EAX, 16" \
  "sal  EAX, 16" \
  "idiv EBX" \
  parm [EAX] [EBX] \
  modify [EDX] \
  value [EAX];

LONG FXPMultDiv ( LONG Val1, LONG MulBy, LONG DivBy );
#pragma aux FXPMultDiv = \
  "imul EDX" \
  "idiv EBX" \
  parm [EAX] [EDX] [EBX] \
  value [EAX];

#else

LONG FXPMult ( LONG Val1, LONG MulBy )
{
	return Val1 * MulBy; 
}

LONG FXPDiv ( LONG Val1, LONG DivBy )
{
	if (0 != DivBy)
		return 0;
	else
		return Val1 / DivBy;
	
}

LONG FXPMultDiv ( LONG Val1, LONG MulBy, LONG DivBy )
{
	if (0 != DivBy)
		return 0;
	else
		return ( (Val1 * MulBy ) / DivBy );
}

#endif 







FLPEDGE *FLPEdge1 = NULL;
FLPEDGE *FLPEdge2 = NULL;
FLPEDGETABLE *FLPEdgeTable = NULL;

FXPEDGE *FXPEdge1 = NULL;
FXPEDGE *FXPEdge2 = NULL;
FXPEDGETABLE FXPEdgeTable;

FXPPOINT2D TopClipPoly[8];
FXPPOINT2D LeftClipPoly[8];
FXPPOINT2D BottomClipPoly[8];
FXPPOINT2D RightClipPoly[8];
FXPPOINT2D *ClipPoly = NULL;

float *DivTable = NULL; ;

G3DDRAW::G3DDRAW ()
  {
    ShadeFlags = SHADE_GOURAUD;
    FaceFlags = FACE_TEXTURE;
    Dest = NULL;
    ShadeTable = NULL;
    CenterX = 160;
    CenterY = 100;
    ViewDistance = (float)100;
    ViewPort.x1 = (LONG)0;
    ViewPort.y1 = (LONG)0;
    ViewPort.x2 = (LONG)319;
    ViewPort.y2 = (LONG)199;
    MIN_Z_DISTANCE = (float)10.0;
    DoHaze = FALSE;

    FLPEdgeTable = new FLPEDGETABLE();
    FLPEdge1 = new FLPEDGE[768];
    FLPEdge2 = new FLPEDGE[768];

    FXPEdgeTable = FXPEDGETABLE();
    FXPEdge1 = new FXPEDGE[768];
    FXPEdge2 = new FXPEDGE[768];

    INT i;

    DivTable = new float [30000];
    DivTable[0] = (float)0; 
    for (i=1;i<30000;i++)
      {
        DivTable[i] = (float)1/i;  
      } 

    InitFunctionTable ();
  } 

G3DDRAW::~G3DDRAW ()
  {
    if (FLPEdge1!=NULL)
      delete FLPEdge1;
    FLPEdge1 = NULL;

    if (FLPEdge2!=NULL)
      delete FLPEdge2;
    FLPEdge2 = NULL;

    if (FLPEdgeTable!=NULL)
      delete FLPEdgeTable;
    FLPEdgeTable = NULL;

    if (FXPEdge1!=NULL)
      delete FXPEdge1;
    FXPEdge1 = NULL;

    if (FXPEdge2!=NULL)
      delete FXPEdge2;
    FXPEdge2 = NULL;

    if (DivTable!=NULL)
      delete DivTable;
    DivTable = NULL;
  } 
      
VOID G3DDRAW::SetDestination ( IMAGE *Destination )
  {
    Dest = Destination;
    if (Dest!=NULL)
      ViewPort = Dest->GetViewPort ();
  } 
  
VOID G3DDRAW::SetViewDistance ( float VDistance )
  {
    ViewDistance = VDistance;  
  } 

VOID G3DDRAW::SetScreenCenter ( LONG Cx, LONG Cy )
  {
    CenterX = Cx;  
    CenterY = Cy;  
  } 
  
VOID G3DDRAW::DrawShape ( G3DSHAPE *Shape )
  {
    G3DOBJECT *Object;
    Object = Shape->ParentObject;
    ViewPort = Dest->GetViewPort ();
printf(" >> G3DDRAW::DrawShape Object = %p, ViewPort = %p\n", Object, ViewPort );
    if (Object->GetObjectType()==OBJECT_TYPE_POLYHEDRON)
      {
        DrawPoly ( (G3DPOLYHEDRON*)Object, (G3DPOLY*)Shape );  
      } 
  } 

VOID G3DDRAW::ZIntersect ( CLIPPOINT3D *In, CLIPPOINT3D *Out, CLIPPOINT3D *Intersect,
                           LONG DrawShadeFlags, LONG DrawFaceFlags )
  {
    float Ratio;

    Ratio = (float)(MIN_Z_DISTANCE-Out->z) / (float)(In->z-Out->z);

    Intersect->x = (In->x-Out->x)*Ratio + Out->x;
    Intersect->y = (In->y-Out->y)*Ratio + Out->y;
    Intersect->z = MIN_Z_DISTANCE;

    if (DrawShadeFlags==SHADE_GOURAUD)
      {
        Intersect->I = (In->I-Out->I)*Ratio + Out->I;  
      } 
      
    if (DrawFaceFlags==FACE_TEXTURE)
      {
        Intersect->u = (In->u-Out->u)*Ratio + Out->u;  
        Intersect->v = (In->v-Out->v)*Ratio + Out->v;  
      } 
  } 
  
LONG G3DDRAW::ZClip ( CLIPPOINT3D *Points, CLIPPOINT3D *ClippedPoints,
                      LONG Code, LONG DrawShadeFlags, LONG DrawFaceFlags )
  {
    LONG NumPoints;  
    switch (Code)
      {
        case 1 : 
          ZIntersect ( &(Points[1]), &(Points[0]), &(ClippedPoints[0]), DrawShadeFlags, DrawFaceFlags );
          ClippedPoints[1] = Points[1];
          ClippedPoints[2] = Points[2];
          ZIntersect ( &(Points[2]), &(Points[0]), &(ClippedPoints[3]), DrawShadeFlags, DrawFaceFlags );
          NumPoints = 4;
          break;
        case 2 : 
          ClippedPoints[0] = Points[0];
          ZIntersect ( &(Points[0]), &(Points[1]), &(ClippedPoints[1]), DrawShadeFlags, DrawFaceFlags );
          ZIntersect ( &(Points[2]), &(Points[1]), &(ClippedPoints[2]), DrawShadeFlags, DrawFaceFlags );
          ClippedPoints[3] = Points[2];
          NumPoints = 4;
          break;
        case 3 : 
          ZIntersect ( &(Points[2]), &(Points[1]), &(ClippedPoints[0]), DrawShadeFlags, DrawFaceFlags );
          ClippedPoints[1] = Points[2];
          ZIntersect ( &(Points[2]), &(Points[0]), &(ClippedPoints[2]), DrawShadeFlags, DrawFaceFlags );
          NumPoints = 3;
          break;
        case 4 : 
          ClippedPoints[0] = Points[0];
          ClippedPoints[1] = Points[1];
          ZIntersect ( &(Points[1]), &(Points[2]), &(ClippedPoints[2]), DrawShadeFlags, DrawFaceFlags );
          ZIntersect ( &(Points[0]), &(Points[2]), &(ClippedPoints[3]), DrawShadeFlags, DrawFaceFlags );
          NumPoints = 4;
          break;
        case 5 : 
          ZIntersect ( &(Points[1]), &(Points[0]), &(ClippedPoints[0]), DrawShadeFlags, DrawFaceFlags );
          ClippedPoints[1] = Points[1];
          ZIntersect ( &(Points[1]), &(Points[2]), &(ClippedPoints[2]), DrawShadeFlags, DrawFaceFlags );
          NumPoints = 3;
          break;
        case 6 : 
          ClippedPoints[0] = Points[0];
          ZIntersect ( &(Points[0]), &(Points[1]), &(ClippedPoints[1]), DrawShadeFlags, DrawFaceFlags );
          ZIntersect ( &(Points[0]), &(Points[2]), &(ClippedPoints[2]), DrawShadeFlags, DrawFaceFlags );
          NumPoints = 3;
          break;    
      } 
    return NumPoints;  
  } 

VOID G3DDRAW::DrawPoly ( G3DPOLYHEDRON *Object, G3DPOLY *Poly )
  {
    FXPPOINT2D Points[4];
    FLPVECTOR2D *ScreenPoints;
    FLPVECTOR3D *CameraPoints;
    float *PointIntensities;
    float *HazeValues;
    DETAILDATA *DetailData;
    GEOMETRYDATA *GeometryData;
    LONG List0,List1,List2;
    LONG DrawShadeFlags;
    LONG DrawFaceFlags;
    LONG Ambient;
    G3DMATERIAL *Material;

    G3DLIGHT *Light;

printf("   G3DDRAW::DrawPoly 0 \n");
    Light = Object->GetLightSource();
    if (Light!=NULL)
      {
        ShadeTable = Light->GetShadeTable ();
      } 
    else
      ShadeTable = NULL;

    Material = Poly->Material;

printf("   G3DDRAW::DrawPoly 1 \n");
    _BlendBuffer = NULL;
    
    if (Material)
      {
        Ambient = Material->Ambient;
        if ((Material->Attributes&MATERIAL_TRANSPARENT)&&(BlendTable!=NULL))
          _BlendBuffer = BlendTable->GetTable()+Material->Transparency*256*256;
      } 
    else
      Ambient = Poly->Ambient;  

printf("   G3DDRAW::DrawPoly 2 \n");
    _HazeBuffer = NULL;
    if (DoHaze)
      {
        if (HazeTable!=NULL)
          _HazeBuffer = HazeTable->GetTable ();  
      } 
    
    LONG CurDetail = Object->GetCurDetail ();
printf("   G3DDRAW::DrawPoly 3 \n");
    if (CurDetail<MIN_DETAIL)
      return;
printf("   G3DDRAW::DrawPoly 4 \n");      
    DetailData = Object->GetDetailData ( CurDetail );
    GeometryData = Object->GetGeometryData ();
    ScreenPoints = GeometryData->FLPScreenPoints;
    CameraPoints = GeometryData->FLPCameraPoints;
    PointIntensities = GeometryData->PointIntensities;
    HazeValues = GeometryData->HazeValues;

printf("   G3DDRAW::DrawPoly 5 \n");
    List0 = Poly->Points[0];
    List1 = Poly->Points[1];
    List2 = Poly->Points[2];

    float z0,z1,z2;
    float MaxVisibleDistance;

printf("   G3DDRAW::DrawPoly 6 \n");
    MaxVisibleDistance = Object->GetMaxVisibleDistance ();
    
    LONG ClipCode;
    z0 = CameraPoints[List0].z;
    z1 = CameraPoints[List1].z;
    z2 = CameraPoints[List2].z;

printf("   G3DDRAW::DrawPoly 7 \n");
    if ((z0>MaxVisibleDistance)&&(z1>MaxVisibleDistance)&&(z2>MaxVisibleDistance))
      return;
      
printf("   G3DDRAW::DrawPoly 8 \n");
    ClipCode = 0;
    if (z0<MIN_Z_DISTANCE)
      ClipCode |= ((LONG)1<<0);
    if (z1<MIN_Z_DISTANCE)
      ClipCode |= ((LONG)1<<1);
    if (z2<MIN_Z_DISTANCE)
      ClipCode |= ((LONG)1<<2);
      
    DrawShadeFlags = MinValue ( DetailData->ShadeFlags, ShadeFlags );
    DrawFaceFlags = MinValue ( DetailData->FaceFlags, FaceFlags );
    
    ANIMIMAGE *Texture=NULL;    
printf("   G3DDRAW::DrawPoly 9 \n");
    if (ClipCode==0)
      {    
printf("   G3DDRAW::DrawPoly 9 999 \n");
        Points[0].x = (LONG)(ScreenPoints[List0].x)<<16; 
        Points[0].y = (LONG)(ScreenPoints[List0].y);
    
        Points[1].x = (LONG)(ScreenPoints[List1].x)<<16;
        Points[1].y = (LONG)(ScreenPoints[List1].y);
    
        Points[2].x = (LONG)(ScreenPoints[List2].x)<<16; 
        Points[2].y = (LONG)(ScreenPoints[List2].y);

        if (DoHaze)
          {
printf("   G3DDRAW::DrawPoly 9 999  HAZE\n");
            Points[0].Haze = (LONG)(HazeValues[List0]);  
            Points[1].Haze = (LONG)(HazeValues[List1]);  
            Points[2].Haze = (LONG)(HazeValues[List2]);  
          } 
        
        if (DrawFaceFlags>=FACE_SOLID)
          {
printf("   G3DDRAW::DrawPoly 9 999  SOLID\n");
            if (DrawShadeFlags==SHADE_GOURAUD)
              {              
printf("   G3DDRAW::DrawPoly 9 999  SOLID  SHADE_GOURAUD\n");
                Points[0].I = (LONG)(PointIntensities[List0]);  
                Points[1].I = (LONG)(PointIntensities[List1]);  
                Points[2].I = (LONG)(PointIntensities[List2]);  
              } 
            if ((DrawFaceFlags==FACE_TEXTURE)&&Material!=NULL)
              {
printf("   G3DDRAW::DrawPoly 9 999  SOLID  TEXTURE\n");
                Points[0].u = (LONG)(Poly->TexCoords[0].x);  
                Points[0].v = (LONG)(Poly->TexCoords[0].y);  
                Points[1].u = (LONG)(Poly->TexCoords[1].x);  
                Points[1].v = (LONG)(Poly->TexCoords[1].y);  
                Points[2].u = (LONG)(Poly->TexCoords[2].x);  
                Points[2].v = (LONG)(Poly->TexCoords[2].y);

                Texture = Material->GetTexture ();
              } 
printf("   G3DDRAW::DrawPoly 9 999  about to draw POLY fast \n");
            DrawPolyFast ( Points, Texture, (LONG)(Poly->Intensity), Ambient, DrawShadeFlags,
                           DrawFaceFlags );
          } 
        else 
          {
printf("   G3DDRAW::DrawPoly 9 666 \n");
            Grafix.FGColor = Ambient;  
            Grafix.DrawLine ( Dest, Points[0].x>>16, Points[0].y,
                                    Points[1].x>>16, Points[1].y );
            Grafix.DrawLine ( Dest, Points[1].x>>16, Points[1].y,
                                    Points[2].x>>16, Points[2].y );
            Grafix.DrawLine ( Dest, Points[2].x>>16, Points[2].y,
                                    Points[0].x>>16, Points[0].y );
          } 
      } 
    else if (ClipCode==7) 
      {
        return;  
      } 
    else 
      {
        CLIPPOINT3D SrcPoints[3];
        CLIPPOINT3D ClippedPoints[4];
        LONG NumClippedPoints;

        SrcPoints[0].x = CameraPoints[List0].x;
        SrcPoints[0].y = CameraPoints[List0].y;
        SrcPoints[0].z = z0;
        SrcPoints[1].x = CameraPoints[List1].x;
        SrcPoints[1].y = CameraPoints[List1].y;
        SrcPoints[1].z = z1;
        SrcPoints[2].x = CameraPoints[List2].x;
        SrcPoints[2].y = CameraPoints[List2].y;
        SrcPoints[2].z = z2;
        
        if (DoHaze)
          {
            SrcPoints[0].Haze = HazeValues[List0];  
            SrcPoints[1].Haze = HazeValues[List1];  
            SrcPoints[2].Haze = HazeValues[List2];  
          } 
        if (DrawShadeFlags==SHADE_GOURAUD)
          {              
            SrcPoints[0].I = PointIntensities[List0];  
            SrcPoints[1].I = PointIntensities[List1];  
            SrcPoints[2].I = PointIntensities[List2];  
          } 
        if ((DrawFaceFlags==FACE_TEXTURE)&&Material!=NULL)
          {
            SrcPoints[0].u = Poly->TexCoords[0].x;  
            SrcPoints[0].v = Poly->TexCoords[0].y;  
            SrcPoints[1].u = Poly->TexCoords[1].x;  
            SrcPoints[1].v = Poly->TexCoords[1].y;  
            SrcPoints[2].u = Poly->TexCoords[2].x;  
            SrcPoints[2].v = Poly->TexCoords[2].y;
          } 
          
        NumClippedPoints = ZClip ( SrcPoints, ClippedPoints,
                                   ClipCode, DrawShadeFlags, DrawFaceFlags );
                                           
        Points[0].x = (LONG)(ClippedPoints[0].x*ViewDistance/ClippedPoints[0].z + CenterX)<<16;
        Points[0].y = (LONG)(-ClippedPoints[0].y*ViewDistance/ClippedPoints[0].z + CenterY);
        Points[1].x = (LONG)(ClippedPoints[1].x*ViewDistance/ClippedPoints[1].z + CenterX)<<16;
        Points[1].y = (LONG)(-ClippedPoints[1].y*ViewDistance/ClippedPoints[1].z + CenterY);
        Points[2].x = (LONG)(ClippedPoints[2].x*ViewDistance/ClippedPoints[2].z + CenterX)<<16;
        Points[2].y = (LONG)(-ClippedPoints[2].y*ViewDistance/ClippedPoints[2].z + CenterY);
                
        if (DrawFaceFlags>=FACE_SOLID)
          {
            if (DoHaze)
              {
                Points[0].Haze = (LONG)(ClippedPoints[0].Haze);  
                Points[1].Haze = (LONG)(ClippedPoints[1].Haze);  
                Points[2].Haze = (LONG)(ClippedPoints[2].Haze);  
              } 
        
            if (DrawShadeFlags==SHADE_GOURAUD)
              {              
                Points[0].I = (LONG)(ClippedPoints[0].I);  
                Points[1].I = (LONG)(ClippedPoints[1].I);  
                Points[2].I = (LONG)(ClippedPoints[2].I);  
              } 
            if ((DrawFaceFlags==FACE_TEXTURE)&&Material!=NULL)
              {
                Points[0].u = (LONG)(ClippedPoints[0].u);  
                Points[0].v = (LONG)(ClippedPoints[0].v);  
                Points[1].u = (LONG)(ClippedPoints[1].u);  
                Points[1].v = (LONG)(ClippedPoints[1].v);  
                Points[2].u = (LONG)(ClippedPoints[2].u);  
                Points[2].v = (LONG)(ClippedPoints[2].v);  

                Texture = Material->GetTexture ();
              } 
            DrawPolyFast ( Points, Texture, (LONG)(Poly->Intensity), Ambient, DrawShadeFlags,
                           DrawFaceFlags );
            if (NumClippedPoints==4)
              {
                Points[1] = Points[2];
                Points[2].x = (LONG)(ClippedPoints[3].x*ViewDistance/ClippedPoints[3].z + CenterX)<<16;
                Points[2].y = (LONG)(-ClippedPoints[3].y*ViewDistance/ClippedPoints[3].z + CenterY);
                Points[2].u = (LONG)(ClippedPoints[3].u);  
                Points[2].v = (LONG)(ClippedPoints[3].v);                  
                Points[2].I = (LONG)(ClippedPoints[3].I);
                Points[2].I = (LONG)(ClippedPoints[3].Haze);
                
                DrawPolyFast ( Points, Texture, (LONG)(Poly->Intensity), Ambient, DrawShadeFlags,
                               DrawFaceFlags );
              } 
          } 
        else
          {
            if (NumClippedPoints<4)
              {
                Grafix.FGColor = Ambient;  
                Grafix.DrawLine ( Dest, Points[0].x>>16, Points[0].y,
                                        Points[1].x>>16, Points[1].y );
                Grafix.DrawLine ( Dest, Points[1].x>>16, Points[1].y,
                                        Points[2].x>>16, Points[2].y );
                Grafix.DrawLine ( Dest, Points[2].x>>16, Points[2].y,
                                        Points[0].x>>16, Points[0].y );                  
              } 
            else
              {
                Grafix.FGColor = Ambient;  
                Grafix.DrawLine ( Dest, Points[0].x>>16, Points[0].y,
                                        Points[1].x>>16, Points[1].y );
                Grafix.DrawLine ( Dest, Points[1].x>>16, Points[1].y,
                                        Points[2].x>>16, Points[2].y );
                Grafix.DrawLine ( Dest, Points[2].x>>16, Points[2].y,
                                        Points[3].x>>16, Points[3].y );                  
                Grafix.DrawLine ( Dest, Points[3].x>>16, Points[3].y,
                                        Points[0].x>>16, Points[0].y );                  
              } 
          } 
      } 
  } 

LONG MIDDLEPOINTTABLE[4][4] = { { 1, 2, 1, 0 }, { 2, 0, 0, 0 },
                                { 1, 0, 0, 0 }, { 0, 0, 0, 0 } };
FXPPOINT2D ExtraPoint;
LONG AddedY;

BOOLEAN G3DDRAW::SortPolyHorz  ( FXPPOINT2D *Points, FXPDRAWSTRUCT *DrawStruct )
  {
    LONG TopIndex,BottomIndex,MiddleIndex;
    FXPPOINT2D *PointPtr;    
    FXPPOINT2D *Top,*Bottom,*Middle;    
    FXPPOINT2D *Left,*Right;    
    LONG MinY,MaxY;
    LONG i;
    
    TopIndex = BottomIndex = 0;
    PointPtr = Points;
    MinY = MaxY = PointPtr->y;
    for (i=1;i<3;i++)
      {
        LONG y;
        PointPtr++;
        y = PointPtr->y;
        if (y<MinY)
          {
            MinY = y;
            TopIndex = i;
          } 
        else if (y>MaxY)
          {
            MaxY = y;
            BottomIndex = i;
          } 
      } 
      
    if (MinY>ViewPort.y2)
      return FALSE;
    else if (MaxY<ViewPort.y1)
      return FALSE;

    MiddleIndex = MIDDLEPOINTTABLE[TopIndex][BottomIndex];
        
    Top = DrawStruct->Top = &(Points[TopIndex]);
    Bottom = DrawStruct->Bottom = &(Points[BottomIndex]);
    Middle = DrawStruct->Middle = &(Points[MiddleIndex]);

    LONG Case;
    
    if (Top->y==Bottom->y)
      {
        Case = DRAW_FLAT;
        if (Top->x<Bottom->x)
          {
            if (Top->x<Middle->x)
              {
                Left = Top;
                if (Middle->x<Bottom->x)
                  Right = Bottom;
                else
                  Right = Middle;  
              } 
            else 
              {
                Left = Middle;
                Right = Bottom;
              } 
          } 
        else 
          {
            if (Bottom->x<Middle->x)
              {
                Left = Bottom;
                if (Middle->x<Top->x)
                  Right = Top;
                else
                  Right = Middle;  
              } 
            else
              {
                Left = Middle;
                Right = Top;
              } 
          } 
      } 
    else if (Top->y==Middle->y)
      {
        Case = DRAW_FLAT_TOP;
        if (Top->x<Middle->x)
          {
            Left = Top;
            Right = Middle;
          } 
        else  
          {
            Left = Middle;
            Right = Top;
          } 
      } 
    else if (Middle->y==Bottom->y)
      {
        Case = DRAW_FLAT_BOTTOM;
        if (Bottom->x<Middle->x)
          {
            Left = Bottom;
            Right = Middle;
          } 
        else  
          {
            Left = Middle;
            Right = Bottom;
          } 
      } 
    else
      {
        Case = DRAW_GENERAL;       
        float Ratio;
        Ratio = (float)(Middle->y-MinY)*DivTable[MaxY-MinY];
        ExtraPoint.x = Top->x + (LONG)((Bottom->x-Top->x)*Ratio);
        ExtraPoint.y = Bottom->y;

        if (ExtraPoint.x<Middle->x)
          {
            Left = &(ExtraPoint);
            Right = Middle;
          } 
        else  
          {
            Left = Middle;
            Right = &(ExtraPoint);
          } 
      } 

    DrawStruct->Left = Left;
    DrawStruct->Right = Right;
    DrawStruct->Case = Case;
    DrawStruct->Length = MaxY-MinY+1;
    return TRUE;
  } 

typedef VOID (*DrawRoutineType) ( LONG Color, LONG LoopCount, LONG SkipRow,
                                   LONG SideClipped );
                        
VOID G3DDRAW::DrawPolyFast ( FXPPOINT2D *Points, ANIMIMAGE *Texture,
                             LONG Intensity, LONG Color, LONG DrawShadeFlags,
                             LONG DrawFaceFlags )
  {    
    FXPDRAWSTRUCT DrawStruct;
    BOOLEAN Texmap=FALSE;
    BOOLEAN Gouraud=FALSE;
    BOOLEAN Flat=FALSE;
    BOOLEAN Transparent = FALSE;
    BOOLEAN Haze = FALSE;
    LONG DrawColor;
    DrawRoutineType DrawFunction;

printf(" >> G3DDRAW::DrawPolyFast 0 \n");
    DrawColor = Color+(Color<<8)+(Color<<16)+(Color<<24);

    if (DrawFaceFlags==FACE_TEXTURE)
      Texmap = TRUE;
      
printf(" >> G3DDRAW::DrawPolyFast 2 \n");
    if (DoHaze)
      {
        if ((Points[0].Haze==Points[1].Haze)&&(Points[0].Haze==Points[2].Haze)&&
             (Points[0].Haze==HazeTable->GetNumLevels()))
          {
            Haze = FALSE;  
          } 
        else
          {
            Haze = TRUE;
          } 
      } 
      
    if (_BlendBuffer!=NULL)
      {
        Transparent = TRUE;
        Haze = FALSE;
      } 
      
    if (DrawShadeFlags==SHADE_GOURAUD)
      {
        if (ShadeTable!=NULL)
          {
            _ShadeBuffer = ShadeTable->GetTable ();
            Gouraud = TRUE;
          } 
      } 
    else if (DrawShadeFlags==SHADE_FLAT)
      {
        _LeftI = 0;
        if (ShadeTable!=NULL)
          {
            _ShadeBuffer = ShadeTable->GetTable ();
            Color = _ShadeBuffer[((LONG)Intensity>>16)*256+Color];
            DrawColor = Color+(Color<<8)+(Color<<16)+(Color<<24);
            Flat = TRUE;
            _LeftI = Intensity;  
          } 
      } 

    _DivTable = DivTable;
      
    _SetViewPortData ( ViewPort.x1, ViewPort.y1, ViewPort.x2, ViewPort.y2 );
    if (SortPolyHorz ( Points,  &DrawStruct )==FALSE)
      return;
        
    float LeftRatio,RightRatio;
    LONG Tx,Ty,Bx,By,Lx,Ly,Rx,Ry;
    LONG Tu,Tv,Bu,Bv,Lu,Lv,Ru,Rv;
    LONG TI,BI,LI,RI;
    LONG THaze,BHaze,LHaze,RHaze;
    LONG My;
    LONG Height;

    Tx = DrawStruct.Top->x;
    Ty = DrawStruct.Top->y;
    Bx = DrawStruct.Bottom->x;
    By = DrawStruct.Bottom->y;
    Lx = DrawStruct.Left->x;
    Ly = DrawStruct.Left->y;
    Rx = DrawStruct.Right->x;
    Ry = DrawStruct.Right->y;

    if (Texmap)
      {
        _TextureBuffer = Texture->GetBuffer ();
        Tu = DrawStruct.Top->u;
        Tv = DrawStruct.Top->v;
        Bu = DrawStruct.Bottom->u;
        Bv = DrawStruct.Bottom->v;
        Lu = DrawStruct.Left->u;
        Lv = DrawStruct.Left->v;
        Ru = DrawStruct.Right->u;
        Rv = DrawStruct.Right->v;

        if (Gouraud)
          {
            TI = DrawStruct.Top->I;
            BI = DrawStruct.Bottom->I;
            LI = DrawStruct.Left->I;
            RI = DrawStruct.Right->I;
            
            if (Transparent)
              DrawFunction = _DrawTextureGrdTransHorz;
            else
              {
                if (Haze)
                  {
                    THaze = DrawStruct.Top->Haze;
                    BHaze = DrawStruct.Bottom->Haze;
                    LHaze = DrawStruct.Left->Haze;
                    RHaze = DrawStruct.Right->Haze;            
                    DrawFunction = _DrawTextureGrdHazeHorz;
                  } 
                else                    
                  DrawFunction = _DrawTextureGrdHorz;
              } 
          } 
        else if (Flat)
          {
            if (Transparent)
              DrawFunction = _DrawTextureFlatTransHorz;
            else
              {
                if (Haze)
                  {
                    THaze = DrawStruct.Top->Haze;
                    BHaze = DrawStruct.Bottom->Haze;
                    LHaze = DrawStruct.Left->Haze;
                    RHaze = DrawStruct.Right->Haze;            
                    DrawFunction = _DrawTextureFlatHazeHorz;
                  } 
                else                    
                  DrawFunction = _DrawTextureFlatHorz;
              } 
          } 
        else
          {
            if (Transparent)
              DrawFunction = _DrawTextureNoTransHorz;
            else
              {
                if (Haze)
                  {
                    THaze = DrawStruct.Top->Haze;
                    BHaze = DrawStruct.Bottom->Haze;
                    LHaze = DrawStruct.Left->Haze;
                    RHaze = DrawStruct.Right->Haze;            
                    DrawFunction = _DrawTextureNoHazeHorz;
                  } 
                else                    
                  DrawFunction = _DrawTextureNoHorz;
              } 
          } 
      } 
    else
      {
        if (Gouraud)
          {
            TI = DrawStruct.Top->I;
            BI = DrawStruct.Bottom->I;
            LI = DrawStruct.Left->I;
            RI = DrawStruct.Right->I;
             
            if (Transparent)
              DrawFunction = _DrawSolidGrdTransHorz;
            else
              {
                if (Haze)
                  {
                    THaze = DrawStruct.Top->Haze;
                    BHaze = DrawStruct.Bottom->Haze;
                    LHaze = DrawStruct.Left->Haze;
                    RHaze = DrawStruct.Right->Haze;            
                    DrawFunction = _DrawSolidGrdHazeHorz;
                  } 
                else                    
                  DrawFunction = _DrawSolidGrdHorz;
              } 
          } 
        else
          {
            if (Transparent)
              DrawFunction = _DrawSolidNoTransHorz;
            else
              {
                if (Haze)
                  {
                    THaze = DrawStruct.Top->Haze;
                    BHaze = DrawStruct.Bottom->Haze;
                    LHaze = DrawStruct.Left->Haze;
                    RHaze = DrawStruct.Right->Haze;            
                    DrawFunction = _DrawSolidNoHazeHorz;
                  } 
                else                    
                  DrawFunction = _DrawSolidNoHorz;
              } 
          } 
      } 
        
    _DestBuffer = Dest->SetOffset ( 0, 0 );
    _AddDest = Dest->GetRowInc ();
    _DestBuffer += _AddDest*Ty;

    BOOLEAN SideClipped = FALSE;
    LONG SkipRow;
    LONG ClipTx,ClipBx,ClipRx,ClipLx;
    ClipTx = Tx>>16;
    ClipBx = Bx>>16;
    ClipLx = Lx>>16;
    ClipRx = Rx>>16;
    
    if ((ClipTx<ViewPort.x1)||(ClipTx>ViewPort.x2)||
         (ClipLx<ViewPort.x1)||(ClipLx>ViewPort.x2)||
          (ClipRx<ViewPort.x1)||(ClipRx>ViewPort.x2)||
           (ClipBx<ViewPort.x1)||(ClipBx>ViewPort.x2))
      SideClipped = TRUE;

    switch (DrawStruct.Case)
      {
        case DRAW_FLAT_TOP :
          Height = DrawStruct.Length;
          LeftRatio = DivTable[By-Ly];
          RightRatio = DivTable[By-Ry];

          SkipRow = 0;
          if (Ty<ViewPort.y1)
            {
              SkipRow = ViewPort.y1-Ty;
            } 
          if (By>ViewPort.y2)
            Height = ViewPort.y2-Ty+1;

          _LeftX = (LONG)(Lx);
          _RightX = (LONG)(Rx);
          _LeftDx = (LONG)((Bx-Lx)*LeftRatio);
          _RightDx = (LONG)((Bx-Rx)*RightRatio);

          if (Texmap)
            {
              _LeftU = (LONG)(Lu);
              _RightU = (LONG)(Ru);
              _LeftDu = (LONG)((Bu-Lu)*LeftRatio);
              _RightDu = (LONG)((Bu-Ru)*RightRatio);

              _LeftV = (LONG)(Lv);
              _RightV = (LONG)(Rv);
              _LeftDv = (LONG)((Bv-Lv)*LeftRatio);
              _RightDv = (LONG)((Bv-Rv)*RightRatio);
            } 

          if (Gouraud)
            {
              _LeftI = (LONG)(LI);
              _RightI = (LONG)(RI);
              _LeftDI = (LONG)((BI-LI)*LeftRatio);
              _RightDI = (LONG)((BI-RI)*RightRatio);
            } 
            
          if (Haze)
            {
              _LeftH = (LONG)(LHaze);
              _RightH = (LONG)(RHaze);
              _LeftDh = (LONG)((BHaze-LHaze)*LeftRatio);
              _RightDh = (LONG)((BHaze-RHaze)*RightRatio);
            } 
            
          DrawFunction ( DrawColor, Height, SkipRow, SideClipped );
          break;

        case DRAW_FLAT_BOTTOM :
          Height = DrawStruct.Length;
          LeftRatio = DivTable[Ly-Ty];
          RightRatio = DivTable[Ry-Ty];

          SkipRow = 0;
          if (Ty<ViewPort.y1)
            {
              SkipRow = ViewPort.y1-Ty;
            } 
          if (By>ViewPort.y2)
            Height = ViewPort.y2-Ty+1;

          _LeftX = (LONG)(Tx);
          _RightX = _LeftX;
          _LeftDx = (LONG)((Lx-Tx)*LeftRatio);
          _RightDx = (LONG)((Rx-Tx)*RightRatio);

          if (Texmap)
            {
              _LeftU = (LONG)(Tu);
              _RightU = _LeftU;
              _LeftDu = (LONG)((Lu-Tu)*LeftRatio);
              _RightDu = (LONG)((Ru-Tu)*RightRatio);
          
              _LeftV = (LONG)(Tv);
              _RightV = _LeftV;
              _LeftDv = (LONG)((Lv-Tv)*LeftRatio);
              _RightDv = (LONG)((Rv-Tv)*RightRatio);
            } 
          
          if (Gouraud)
            {
              _LeftI = (LONG)(TI);
              _RightI = _LeftI;
              _LeftDI = (LONG)((LI-TI)*LeftRatio);
              _RightDI = (LONG)((RI-TI)*RightRatio);
            } 
            
          if (Haze)
            {
              _LeftH = (LONG)(THaze);
              _RightH = _LeftH;
              _LeftDh = (LONG)((LHaze-THaze)*LeftRatio);
              _RightDh = (LONG)((RHaze-THaze)*RightRatio);
            } 
            
          DrawFunction ( DrawColor, Height, SkipRow, SideClipped );
          break;

        case DRAW_FLAT :
          Height = 1;
          SkipRow = 0;
          
          _LeftX = (LONG)(Lx);
          _RightX = (LONG)(Rx);
          _LeftDx = 0;
          _RightDx = 0;
          
          if (Texmap)
            {
              _LeftU = (LONG)(Lu);
              _RightU = (LONG)(Ru);
              _LeftDu = 0;
              _RightDu = 0;
          
              _LeftV = (LONG)(Lv);
              _RightV = (LONG)(Rv);
              _LeftDv = 0;
              _RightDv = 0;
            } 
          
          if (Gouraud)
            {
              _LeftI = (LONG)(LI);
              _RightI = (LONG)(RI);
              _LeftDI = 0;
              _RightDI = 0;
            } 
            
          if (Haze)
            {
              _LeftH = (LONG)(LHaze);
              _RightH = (LONG)(RHaze);
              _LeftDh = 0;
              _RightDh = 0;
            } 
            
          DrawFunction ( DrawColor, Height, SkipRow, SideClipped );
          break;

        case DRAW_GENERAL :
          
          My = DrawStruct.Middle->y;
          Height = My-Ty;
          LeftRatio = DivTable[Ly-Ty];
          RightRatio = DivTable[Ry-Ty];

          SkipRow = 0;
          if (Ty<ViewPort.y1)
            {
              SkipRow = ViewPort.y1-Ty;
            } 
          if (My>ViewPort.y2)
            Height = ViewPort.y2-Ty+1;

          _LeftX = (LONG)(Tx);
          _RightX = _LeftX;
          
          if (Rx==ExtraPoint.x)
            {
              _LeftDx = (LONG)((Lx-Tx)*LeftRatio);
              _RightDx = (LONG)((Bx-Tx)*RightRatio);
                      
              if (Texmap)
                {
                  _LeftU = (LONG)(Tu);
                  _RightU = _LeftU;
          
                  _LeftV = (LONG)(Tv);
                  _RightV = _LeftV;
          
                  _LeftDu = (LONG)((Lu-Tu)*LeftRatio);
                  _RightDu = (LONG)((Bu-Tu)*RightRatio);
                  _LeftDv = (LONG)((Lv-Tv)*LeftRatio);
                  _RightDv = (LONG)((Bv-Tv)*RightRatio);
                } 
              if (Gouraud)
                {
                  _LeftI = (LONG)(TI);
                  _RightI = _LeftI;
                  _LeftDI = (LONG)((LI-TI)*LeftRatio);
                  _RightDI = (LONG)((BI-TI)*RightRatio);
                } 
              if (Haze)
                {
                  _LeftH = (LONG)(THaze);
                  _RightH = _LeftH;
                  _LeftDh = (LONG)((LHaze-THaze)*LeftRatio);
                  _RightDh = (LONG)((BHaze-THaze)*RightRatio);
                } 
            } 
          else
            {
              _LeftDx = (LONG)((Bx-Tx)*LeftRatio);
              _RightDx = (LONG)((Rx-Tx)*RightRatio);
                      
              if (Texmap)
                {
                  _LeftU = (LONG)(Tu);
                  _RightU = _LeftU;
          
                  _LeftV = (LONG)(Tv);
                  _RightV = _LeftV;
          
                  _LeftDu = (LONG)((Bu-Tu)*LeftRatio);
                  _RightDu = (LONG)((Ru-Tu)*RightRatio);
                  _LeftDv = (LONG)((Bv-Tv)*LeftRatio);
                  _RightDv = (LONG)((Rv-Tv)*RightRatio);
                } 
              if (Gouraud)
                {
                  _LeftI = (LONG)(TI);
                  _RightI = _LeftI;
                  _LeftDI = (LONG)((BI-TI)*LeftRatio);
                  _RightDI = (LONG)((RI-TI)*RightRatio);
                } 
              if (Haze)
                {
                  _LeftH = (LONG)(THaze);
                  _RightH = _LeftH;
                  _LeftDh = (LONG)((BHaze-THaze)*LeftRatio);
                  _RightDh = (LONG)((RHaze-THaze)*RightRatio);
                } 
            } 
            
           if (SkipRow<=Height)
            { 
              DrawFunction ( DrawColor, Height, SkipRow, SideClipped );
              SkipRow = 0;
            } 
           else
            {
              SkipRow -= Height;
              DrawFunction ( DrawColor, Height, Height, SideClipped );
            } 

          
          Height = By-My+1;

          if (By>ViewPort.y2)
            Height = ViewPort.y2-My+1;
          
          if (Rx==ExtraPoint.x)
            {
              LeftRatio = DivTable[By-Ly];
              _LeftDx = (LONG)((Bx-Lx)*LeftRatio);
              _LeftX = (LONG)(Lx);

              if (Gouraud)
                {
                  _LeftI = (LONG)(LI);
                  _LeftDI = (LONG)((BI-LI)*LeftRatio);
                } 
              if (Haze)
                {
                  _LeftH = (LONG)(LHaze);
                  _LeftDh = (LONG)((BHaze-LHaze)*LeftRatio);
                } 
              if (Texmap)
                {
                  _LeftU = (LONG)(Lu);
                  _LeftV = (LONG)(Lv);
                  _LeftDu = (LONG)((Bu-Lu)*LeftRatio);
                  _LeftDv = (LONG)((Bv-Lv)*LeftRatio);
                } 
            } 
          else
            {
              RightRatio = DivTable[By-Ry];
              _RightDx = (LONG)((Bx-Rx)*RightRatio);
              _RightX = (LONG)(Rx);

              if (Gouraud)
                {
                  _RightI = (LONG)(RI);
                  _RightDI = (LONG)((BI-RI)*RightRatio);
                } 
              if (Haze)
                {
                  _RightH = (LONG)(RHaze);
                  _RightDh = (LONG)((BHaze-RHaze)*RightRatio);
                } 
              if (Texmap)
                {
                  _RightU = (LONG)(Ru);
                  _RightV = (LONG)(Rv);
                  _RightDu = (LONG)((Bu-Ru)*RightRatio);
                  _RightDv = (LONG)((Bv-Rv)*RightRatio);
                } 
            } 

          DrawFunction ( DrawColor, Height, SkipRow, SideClipped );
          break;
      } 
  } 

RECTANGLE G3DDRAW::GetViewPort ()
  {
    RECTANGLE VPort;
      
    if (Dest==NULL)
      return VPort;
    VPort = Dest->GetViewPort ();
    return VPort;    
  } 

float G3DDRAW::GetNearClipZ ()
  {
    return MIN_Z_DISTANCE;   
  } 
  
VOID G3DDRAW::SetViewPort ( LONG x1, LONG y1, LONG x2, LONG y2 )
  {
    if (Dest!=NULL)
      {
        Dest->SetViewPort ( x1, y1, x2, y2 );
        ViewPort = Dest->GetViewPort ();
      } 
  } 
  
VOID G3DDRAW::SetNearClipZ ( float Z )
  {
    MIN_Z_DISTANCE = Z;  
  } 



typedef FXPEDGE* (*ScanEdgeType) ( FXPPOINT2D *Point1, FXPPOINT2D *Point2,
                                   FXPEDGE *EdgePtr, LONG ExtraHt );
typedef VOID (*DrawPolyType) ( LONG Color, LONG Intensity,
                               LONG Skip, LONG LoopCount );

struct DRAWROUTINE
  {
    ScanEdgeType ScanEdgePtr;
    DrawPolyType DrawPolyPtr;
  }; 

                    
DRAWROUTINE DrawRoutine [3]   [3]  [2]  [2];

VOID G3DDRAW::InitFunctionTable ()
  {
    #if defined (__FORWATCOM__)
    DrawRoutine[SHADE_NONE][FACE_WIREFRAME][FALSE][FALSE].ScanEdgePtr = _ScanEdgeSolid;
    DrawRoutine[SHADE_NONE][FACE_WIREFRAME][FALSE][FALSE].DrawPolyPtr = _DrawSolidPoly;

      DrawRoutine[SHADE_NONE][FACE_WIREFRAME][FALSE][TRUE].ScanEdgePtr = _ScanEdgeSolid;
      DrawRoutine[SHADE_NONE][FACE_WIREFRAME][FALSE][TRUE].DrawPolyPtr = _DrawSolidPoly;

      DrawRoutine[SHADE_NONE][FACE_WIREFRAME][TRUE][FALSE].ScanEdgePtr = _ScanEdgeSolid;
      DrawRoutine[SHADE_NONE][FACE_WIREFRAME][TRUE][FALSE].DrawPolyPtr = _DrawSolidPoly;

      DrawRoutine[SHADE_NONE][FACE_WIREFRAME][TRUE][TRUE].ScanEdgePtr = _ScanEdgeSolid;
      DrawRoutine[SHADE_NONE][FACE_WIREFRAME][TRUE][TRUE].DrawPolyPtr = _DrawSolidPoly;

      DrawRoutine[SHADE_NONE][FACE_SOLID][FALSE][FALSE].ScanEdgePtr = _ScanEdgeSolid;
      DrawRoutine[SHADE_NONE][FACE_SOLID][FALSE][FALSE].DrawPolyPtr = _DrawSolidPoly;

      DrawRoutine[SHADE_NONE][FACE_SOLID][FALSE][TRUE].ScanEdgePtr = _ScanEdgeSolid;
      DrawRoutine[SHADE_NONE][FACE_SOLID][FALSE][TRUE].DrawPolyPtr = _DrawSolidPoly;

      DrawRoutine[SHADE_NONE][FACE_SOLID][TRUE][FALSE].ScanEdgePtr = _ScanEdgeSolid;
      DrawRoutine[SHADE_NONE][FACE_SOLID][TRUE][FALSE].DrawPolyPtr = _DrawSolidPoly;

      DrawRoutine[SHADE_NONE][FACE_SOLID][TRUE][TRUE].ScanEdgePtr = _ScanEdgeSolid;
      DrawRoutine[SHADE_NONE][FACE_SOLID][TRUE][TRUE].DrawPolyPtr = _DrawSolidPoly;

      DrawRoutine[SHADE_NONE][FACE_TEXTURE][FALSE][FALSE].ScanEdgePtr = _ScanEdgeTexture;
      DrawRoutine[SHADE_NONE][FACE_TEXTURE][FALSE][FALSE].DrawPolyPtr = _DrawTexturePoly;

      DrawRoutine[SHADE_NONE][FACE_TEXTURE][FALSE][TRUE].ScanEdgePtr = _ScanEdgeTexture;
      DrawRoutine[SHADE_NONE][FACE_TEXTURE][FALSE][TRUE].DrawPolyPtr = _DrawTexturePoly;

      DrawRoutine[SHADE_NONE][FACE_TEXTURE][TRUE][FALSE].ScanEdgePtr = _ScanEdgeTexture;
      DrawRoutine[SHADE_NONE][FACE_TEXTURE][TRUE][FALSE].DrawPolyPtr = _DrawTexturePoly;

      DrawRoutine[SHADE_NONE][FACE_TEXTURE][TRUE][TRUE].ScanEdgePtr = _ScanEdgeTexture;
      DrawRoutine[SHADE_NONE][FACE_TEXTURE][TRUE][TRUE].DrawPolyPtr = _DrawTexturePoly;

    DrawRoutine[SHADE_FLAT][FACE_WIREFRAME][FALSE][FALSE].ScanEdgePtr = _ScanEdgeSolid;
    DrawRoutine[SHADE_FLAT][FACE_WIREFRAME][FALSE][FALSE].DrawPolyPtr = _DrawSolidPoly;

      DrawRoutine[SHADE_FLAT][FACE_WIREFRAME][FALSE][TRUE].ScanEdgePtr = _ScanEdgeSolid;
      DrawRoutine[SHADE_FLAT][FACE_WIREFRAME][FALSE][TRUE].DrawPolyPtr = _DrawSolidPoly;

      DrawRoutine[SHADE_FLAT][FACE_WIREFRAME][TRUE][FALSE].ScanEdgePtr = _ScanEdgeSolid;
      DrawRoutine[SHADE_FLAT][FACE_WIREFRAME][TRUE][FALSE].DrawPolyPtr = _DrawSolidPoly;

      DrawRoutine[SHADE_FLAT][FACE_WIREFRAME][TRUE][TRUE].ScanEdgePtr = _ScanEdgeSolid;
      DrawRoutine[SHADE_FLAT][FACE_WIREFRAME][TRUE][TRUE].DrawPolyPtr = _DrawSolidPoly;

      DrawRoutine[SHADE_FLAT][FACE_SOLID][FALSE][FALSE].ScanEdgePtr = _ScanEdgeSolid;
      DrawRoutine[SHADE_FLAT][FACE_SOLID][FALSE][FALSE].DrawPolyPtr = _DrawSolidPoly;

      DrawRoutine[SHADE_FLAT][FACE_SOLID][FALSE][TRUE].ScanEdgePtr = _ScanEdgeSolid;
      DrawRoutine[SHADE_FLAT][FACE_SOLID][FALSE][TRUE].DrawPolyPtr = _DrawSolidPoly;

      DrawRoutine[SHADE_FLAT][FACE_SOLID][TRUE][FALSE].ScanEdgePtr = _ScanEdgeSolid;
      DrawRoutine[SHADE_FLAT][FACE_SOLID][TRUE][FALSE].DrawPolyPtr = _DrawSolidPoly;

      DrawRoutine[SHADE_FLAT][FACE_SOLID][TRUE][TRUE].ScanEdgePtr = _ScanEdgeSolid;
      DrawRoutine[SHADE_FLAT][FACE_SOLID][TRUE][TRUE].DrawPolyPtr = _DrawSolidPoly;

      DrawRoutine[SHADE_FLAT][FACE_TEXTURE][FALSE][FALSE].ScanEdgePtr = _ScanEdgeSolid;
      DrawRoutine[SHADE_FLAT][FACE_TEXTURE][FALSE][FALSE].DrawPolyPtr = _DrawSolidPoly;

      DrawRoutine[SHADE_FLAT][FACE_TEXTURE][FALSE][TRUE].ScanEdgePtr = _ScanEdgeSolid;
      DrawRoutine[SHADE_FLAT][FACE_TEXTURE][FALSE][TRUE].DrawPolyPtr = _DrawSolidPoly;

      DrawRoutine[SHADE_FLAT][FACE_TEXTURE][TRUE][FALSE].ScanEdgePtr = _ScanEdgeSolid;
      DrawRoutine[SHADE_FLAT][FACE_TEXTURE][TRUE][FALSE].DrawPolyPtr = _DrawSolidPoly;

      DrawRoutine[SHADE_FLAT][FACE_TEXTURE][TRUE][TRUE].ScanEdgePtr = _ScanEdgeSolid;
      DrawRoutine[SHADE_FLAT][FACE_TEXTURE][TRUE][TRUE].DrawPolyPtr = _DrawSolidPoly;

    DrawRoutine[SHADE_GOURAUD][FACE_WIREFRAME][FALSE][FALSE].ScanEdgePtr = _ScanEdgeSolid;
    DrawRoutine[SHADE_GOURAUD][FACE_WIREFRAME][FALSE][FALSE].DrawPolyPtr = _DrawSolidPoly;

      DrawRoutine[SHADE_GOURAUD][FACE_WIREFRAME][FALSE][TRUE].ScanEdgePtr = _ScanEdgeSolid;
      DrawRoutine[SHADE_GOURAUD][FACE_WIREFRAME][FALSE][TRUE].DrawPolyPtr = _DrawSolidPoly;

      DrawRoutine[SHADE_GOURAUD][FACE_WIREFRAME][TRUE][FALSE].ScanEdgePtr = _ScanEdgeSolid;
      DrawRoutine[SHADE_GOURAUD][FACE_WIREFRAME][TRUE][FALSE].DrawPolyPtr = _DrawSolidPoly;

      DrawRoutine[SHADE_GOURAUD][FACE_WIREFRAME][TRUE][TRUE].ScanEdgePtr = _ScanEdgeSolid;
      DrawRoutine[SHADE_GOURAUD][FACE_WIREFRAME][TRUE][TRUE].DrawPolyPtr = _DrawSolidPoly;

      DrawRoutine[SHADE_GOURAUD][FACE_SOLID][FALSE][FALSE].ScanEdgePtr = _ScanEdgeSolidGouraud;
      DrawRoutine[SHADE_GOURAUD][FACE_SOLID][FALSE][FALSE].DrawPolyPtr = _DrawSolidGouraudPoly;

      DrawRoutine[SHADE_GOURAUD][FACE_SOLID][FALSE][TRUE].ScanEdgePtr = _ScanEdgeSolidGouraud;
      DrawRoutine[SHADE_GOURAUD][FACE_SOLID][FALSE][TRUE].DrawPolyPtr = _DrawSolidGouraudPoly;

      DrawRoutine[SHADE_GOURAUD][FACE_SOLID][TRUE][FALSE].ScanEdgePtr = _ScanEdgeSolidGouraud;
      DrawRoutine[SHADE_GOURAUD][FACE_SOLID][TRUE][FALSE].DrawPolyPtr = _DrawSolidGouraudPoly;

      DrawRoutine[SHADE_GOURAUD][FACE_SOLID][TRUE][TRUE].ScanEdgePtr = _ScanEdgeSolidGouraud;
      DrawRoutine[SHADE_GOURAUD][FACE_SOLID][TRUE][TRUE].DrawPolyPtr = _DrawSolidGouraudPoly;

      DrawRoutine[SHADE_GOURAUD][FACE_TEXTURE][FALSE][FALSE].ScanEdgePtr = _ScanEdgeSolid;
      DrawRoutine[SHADE_GOURAUD][FACE_TEXTURE][FALSE][FALSE].DrawPolyPtr = _DrawSolidPoly;

      DrawRoutine[SHADE_GOURAUD][FACE_TEXTURE][FALSE][TRUE].ScanEdgePtr = _ScanEdgeSolid;
      DrawRoutine[SHADE_GOURAUD][FACE_TEXTURE][FALSE][TRUE].DrawPolyPtr = _DrawSolidPoly;

      DrawRoutine[SHADE_GOURAUD][FACE_TEXTURE][TRUE][FALSE].ScanEdgePtr = _ScanEdgeSolid;
      DrawRoutine[SHADE_GOURAUD][FACE_TEXTURE][TRUE][FALSE].DrawPolyPtr = _DrawSolidPoly;

      DrawRoutine[SHADE_GOURAUD][FACE_TEXTURE][TRUE][TRUE].ScanEdgePtr = _ScanEdgeSolid;
      DrawRoutine[SHADE_GOURAUD][FACE_TEXTURE][TRUE][TRUE].DrawPolyPtr = _DrawSolidPoly;
    #endif
  } 


VOID G3DDRAW::TestDrawTriangle ( FXPPOINT2D *Points, ANIMIMAGE *Texture,
                                 LONG Intensity, LONG Color, LONG DrawShadeFlags,
                                 LONG DrawFaceFlags, COLORTABLE *BlendTBL, COLORTABLE *ShadeTBL,
                                 COLORTABLE *HazeTBL )
  {
    ShadeTable = ShadeTBL;
    HazeTable = HazeTBL;
    if (BlendTBL!=NULL)
      _BlendBuffer = NULL;
    else
      _BlendBuffer = NULL;  

    DrawPolyFast ( Points, Texture, Intensity, Color, DrawShadeFlags, DrawFaceFlags );
  } 

FLPPOINT2D* FLPEdge1Index[8];
FLPPOINT2D* FLPEdge2Index[8];
FXPPOINT2D* FXPEdge1Index[8];
FXPPOINT2D* FXPEdge2Index[8];
LONG NumEdge1;
LONG NumEdge2;

VOID G3DDRAW::CreateFLPEdgeTable ( FLPPOINT2D *Points, LONG NumPoints )
  {
    LONG i;
    LONG TopIndex,BottomIndex;
    float TopY,BottomY;
    FLPPOINT2D *PointPtr;

    PointPtr = Points;

    FLPPOINT2D *FirstPtr,*LastPtr;
    FirstPtr = Points;
    LastPtr = &(Points[NumPoints-1]);

    TopY = PointPtr->y;
    BottomY = TopY;

    PointPtr++;

    TopIndex = BottomIndex = 0;

    float y;
    for (i=0;i<NumPoints;i++)
      {
        y = PointPtr->y;
        if (y<TopY)
          {
            TopIndex = i;
            TopY = y;
          } 
        else if (y>BottomY)
          {
            BottomIndex = i;
            BottomY = y;
          } 
        PointPtr++;
      }; 

    if (BottomY==TopY)
      return;

    FLPEdgeTable->Start = TopY;
    FLPEdgeTable->Length = (LONG)(BottomY-TopY)+1;

    FLPPOINT2D *NextPtr;
    FLPPOINT2D **PointPtrPtr;

    
    NumEdge1 = 0;
    NextPtr = &(Points[TopIndex]);
    y = TopY;
    PointPtrPtr = FLPEdge1Index;

    while (y<BottomY)
      {
        PointPtr = NextPtr;
        if (NextPtr<LastPtr)
          NextPtr++;
        else
          NextPtr = FirstPtr;
        if ((y==TopY)&&(NextPtr->y!=TopY))
          {
            *(PointPtrPtr++) = PointPtr;
            y = PointPtr->y;
            NumEdge1++;
          } 
      } 

    
    NumEdge2 = 0;
    NextPtr = &(Points[TopIndex]);
    y = TopY;
    PointPtrPtr = FLPEdge2Index;

    while (y<BottomY)
      {
        PointPtr = NextPtr;
        if (NextPtr>FirstPtr)
          NextPtr--;
        else
          NextPtr = LastPtr;
        if ((y==TopY)&&(NextPtr->y!=TopY))
          {
            *(PointPtrPtr++) = PointPtr;
            y = PointPtr->y;
            NumEdge2++;
          } 
      } 

    float x;
    float Dx,Dy;
    LONG EdgeLength;
    FLPPOINT2D *Point1,*Point2;
    FLPEDGE *EdgePtr;

    
    EdgePtr = FLPEdge1;
    for (i=0;i<NumEdge1-1;i++)
      {
        Point1 = FLPEdge1Index[i];
        Point2 = FLPEdge1Index[i+1];
        Dy = Point2->y-Point1->y;
        x = Point1->x;
        Dx = Point2->x-x;

        EdgeLength = (LONG)Dy;
        if (i==NumEdge1-2)
          EdgeLength++;

        Dy = (float)1/Dy;
        Dx *= Dy;

        while (EdgeLength--)
          {
            EdgePtr->x = x;
            x += Dx;
            EdgePtr++;
          } 
      } 

    
    EdgePtr = FLPEdge2;
    for (i=0;i<NumEdge2-1;i++)
      {
        Point1 = FLPEdge2Index[i];
        Point2 = FLPEdge2Index[i+1];
        Dy = Point2->y-Point1->y;
        x = Point1->x;
        Dx = Point2->x-x;

        EdgeLength = (LONG)Dy;
        if (i==NumEdge1-2)
          EdgeLength++;

        Dy = (float)1/Dy;
        Dx *= Dy;

        while (EdgeLength--)
          {
            EdgePtr->x = x;
            x += Dx;
            EdgePtr++;
          } 
      } 

    
    FLPEdgeTable->Edge1 = FLPEdge1;
    FLPEdgeTable->Edge2 = FLPEdge2;

    for (i=0;FLPEdgeTable->Length;i++)
      {
        if (FLPEdge1[i].x<FLPEdge2[i].x)
          {
            FLPEdgeTable->Edge1 = FLPEdge1;
            FLPEdgeTable->Edge2 = FLPEdge2;
            break;
          } 
        else if (FLPEdge1[i].x<FLPEdge2[i].x)
          {
            FLPEdgeTable->Edge1 = FLPEdge2;
            FLPEdgeTable->Edge2 = FLPEdge1;
            break;
          } 
      } 
  } 

LONG Vx1,Vy1,Vx2,Vy2;

VOID G3DDRAW::XYIntersect ( FXPPOINT2D *In, FXPPOINT2D *Out,
                            FXPPOINT2D *Intersect, LONG Code, LONG ShadeFlags,
                            LONG FaceFlags )
  {    
    LONG LRatio;
    LONG Vx;
    LONG Ix,Iy,Ox,Oy;
    Ix = In->x;
    Iy = In->y;
    Ox = Out->x;
    Oy = Out->y;

    switch (Code)
      {
        case CLIP_TOP :
          LRatio = ((Vy1-Oy)<<16)/(Iy-Oy);
          Intersect->x = (LONG)(FXPMult((Ix-Ox),LRatio)+Ox);
          Intersect->y = Vy1;
          break;
        case CLIP_LEFT :
          Vx = Vx1 << 16;
          LRatio = (LONG)FXPDiv((Vx-Ox),(Ix-Ox));
          Intersect->x = Vx;
          Intersect->y = (((Iy-Oy)*LRatio)>>16)+Oy;
          break;
        case CLIP_BOTTOM :
          LRatio = ((Vy2-Oy)<<16)/(Iy-Oy);
          Intersect->x = (LONG)(FXPMult((Ix-Ox),LRatio)+Ox);
          Intersect->y = Vy2;
          break;
        case CLIP_RIGHT :
          Vx = Vx2 << 16;
          LRatio = (LONG)FXPDiv((Vx-Ox),(Ix-Ox));
          Intersect->x = Vx;
          Intersect->y = (((Iy-Oy)*LRatio)>>16)+Oy;
          break;
      } 

    if (ShadeFlags==SHADE_GOURAUD)
      {}
    if (FaceFlags==FACE_TEXTURE)
      {}
  } 

LONG G3DDRAW::ClipFXPPolygon ( FXPPOINT2D *Points, LONG NumPoints, LONG Code, 
                               LONG ShadeFlags, LONG FaceFlags )
  {
    FXPPOINT2D *P1,*P2;
    FXPPOINT2D *LastPoint;
    FXPPOINT2D *ClipPolyPtr;
    FXPPOINT2D *SrcPoly;
    LONG NewNumPoints;
    LONG x1,y1,x2,y2;

    if (Code==0)  
      {
        ClipPoly = Points;
        return NumPoints;
      } 

    x1 = Vx1 << 16;
    x2 = Vx2 << 16;
    y1 = Vy1;
    y2 = Vy2;

    SrcPoly = Points;

    
    if (Code&CLIP_TOP)
      {
        NewNumPoints = 0;
        ClipPolyPtr = TopClipPoly;
        P1 = LastPoint = &(SrcPoly[NumPoints-1]);  
        P2 = SrcPoly;

        do
          {
            if (P1->y>=y1) 
              {
                if (P2->y>=y1)  
                  {
                    *(ClipPolyPtr++) = *P2;  
                    NewNumPoints++;
                  } 
                else
                  {
                    
                    XYIntersect ( P1, P2, ClipPolyPtr++, CLIP_TOP,
                                  ShadeFlags, FaceFlags );
                    NewNumPoints++;
                  } 
               } 
            else 
              {
                if (P2->y>=y1)  
                  {
                    
                    XYIntersect ( P2, P1, ClipPolyPtr++, CLIP_TOP,
                                  ShadeFlags, FaceFlags );
                    NewNumPoints++;
                    *(ClipPolyPtr++) = *P2;  
                    NewNumPoints++;
                  } 
              } 
            P1 = P2;
          } while (P2++<LastPoint); 
        SrcPoly = TopClipPoly;
        NumPoints = NewNumPoints;
      } 

    if (NumPoints<3)
      return NumPoints;

    
    if (Code&CLIP_LEFT)
      {
        NewNumPoints = 0;
        ClipPolyPtr = LeftClipPoly;
        P1 = LastPoint = &(SrcPoly[NumPoints-1]);  
        P2 = SrcPoly;

        do
          {
            if (P1->x>=x1) 
              {
                if (P2->x>=x1)  
                  {
                    *(ClipPolyPtr++) = *P2;  
                    NewNumPoints++;
                  } 
                else
                  {
                    
                    XYIntersect ( P1, P2, ClipPolyPtr++, CLIP_LEFT,
                                  ShadeFlags, FaceFlags );
                    NewNumPoints++;
                  } 
               } 
            else 
              {
                if (P2->x>=x1)  
                  {
                    
                    XYIntersect ( P2, P1, ClipPolyPtr++, CLIP_LEFT,
                                  ShadeFlags, FaceFlags );
                    NewNumPoints++;
                    *(ClipPolyPtr++) = *P2;  
                    NewNumPoints++;
                  } 
              } 
            P1 = P2;
          } while (P2++<LastPoint); 
        SrcPoly = LeftClipPoly;
        NumPoints = NewNumPoints;
      } 

    if (NumPoints<3)
      return NumPoints;

    
    if (Code&CLIP_BOTTOM)
      {
        NewNumPoints = 0;
        ClipPolyPtr = BottomClipPoly;
        P1 = LastPoint = &(SrcPoly[NumPoints-1]);  
        P2 = SrcPoly;

        do
          {
            if (P1->y<=y2) 
              {
                if (P2->y<=y2)  
                  {
                    *(ClipPolyPtr++) = *P2;  
                    NewNumPoints++;
                  } 
                else
                  {
                    
                    XYIntersect ( P1, P2, ClipPolyPtr++, CLIP_BOTTOM,
                                  ShadeFlags, FaceFlags );
                    NewNumPoints++;
                  } 
               } 
            else 
              {
                if (P2->x<=y2)  
                  {
                    
                    XYIntersect ( P2, P1, ClipPolyPtr++, CLIP_BOTTOM,
                                  ShadeFlags, FaceFlags );
                    NewNumPoints++;
                    *(ClipPolyPtr++) = *P2;  
                    NewNumPoints++;
                  } 
              } 
            P1 = P2;
          } while (P2++<LastPoint); 
        SrcPoly = BottomClipPoly;
        NumPoints = NewNumPoints;
      } 

    if (NumPoints<3)
      return NumPoints;

    
    if (Code&CLIP_RIGHT)
      {
        NewNumPoints = 0;
        ClipPolyPtr = RightClipPoly;
        P1 = LastPoint = &(SrcPoly[NumPoints-1]);  
        P2 = SrcPoly;

        do
          {
            if (P1->x<=x2) 
              {
                if (P2->x<=x2)  
                  {
                    *(ClipPolyPtr++) = *P2;  
                    NewNumPoints++;
                  } 
                else
                  {
                    
                    XYIntersect ( P1, P2, ClipPolyPtr++, CLIP_RIGHT,
                                  ShadeFlags, FaceFlags );
                    NewNumPoints++;
                  } 
               } 
            else 
              {
                if (P2->x<=x2)  
                  {
                    
                    XYIntersect ( P2, P1, ClipPolyPtr++, CLIP_RIGHT,
                                  ShadeFlags, FaceFlags );
                    NewNumPoints++;
                    *(ClipPolyPtr++) = *P2;  
                    NewNumPoints++;
                  } 
              } 
            P1 = P2;
          } while (P2++<LastPoint); 
        SrcPoly = RightClipPoly;
        NumPoints = NewNumPoints;
      } 

    ClipPoly = SrcPoly;
    return NumPoints;
  } 

ScanEdgeType ScanEdgeFunction;
DrawPolyType DrawPolyFunction;

BOOLEAN G3DDRAW::CreateFXPEdgeTable ( FXPPOINT2D *Points, LONG NumPoints,
                                      LONG ShadeFlags, LONG FaceFlags )
  {
    LONG i;
    LONG TopY,BottomY;
    LONG LeftX,RightX;
    FXPPOINT2D *PointPtr;
    FXPPOINT2D *TopPointPtr;

    PointPtr = Points;

    FXPPOINT2D *FirstPtr,*LastPtr;
    FirstPtr = TopPointPtr = Points;
    LastPtr = &(Points[NumPoints-1]);

    TopY = BottomY = TopPointPtr->y;
    LeftX = RightX = TopPointPtr->x;

    PointPtr++;

    LONG x,y;
    do
      {
        x = PointPtr->x;
        y = PointPtr->y;

        if (x<LeftX)
          {
            LeftX = x;
          } 
        else if (x>RightX)
          {
            RightX = x;
          } 

        if (y<TopY)
          {
            TopPointPtr = PointPtr;
            TopY = y;
          } 
        else if (y>BottomY)
          {
            BottomY = y;
          } 
      } while (PointPtr++<LastPtr); 

    LONG ClipCode=0;

    LeftX >>= 16;
    RightX >>= 16;

    if (RightX<ViewPort.x1)
      return FALSE;
    if (LeftX>ViewPort.x2)
      return FALSE;
    if (BottomY<ViewPort.y1)
      return FALSE;
    if (TopY>ViewPort.y2)
      return FALSE;

    if (LeftX<ViewPort.x1)
      ClipCode |= CLIP_LEFT;
    if (RightX>ViewPort.x2)
      ClipCode |= CLIP_RIGHT;
    if (TopY<ViewPort.y1)
      ClipCode |= CLIP_TOP;
    if (BottomY>ViewPort.y2)
      ClipCode |= CLIP_BOTTOM;

    NumPoints = ClipFXPPolygon ( Points, NumPoints, ClipCode,
                                 ShadeFlags, FaceFlags );
    if (NumPoints<3)
      return FALSE;
    if (ClipPoly!=Points)  
      {
        PointPtr = ClipPoly;
        FirstPtr = TopPointPtr = ClipPoly;
        LastPtr = &(ClipPoly[NumPoints-1]);

        TopY = BottomY = PointPtr->y;
        PointPtr++;
        do
          {
            y = PointPtr->y;
            if (y<TopY)
              {
                TopPointPtr = PointPtr;
                TopY = y;
              } 
            else if (y>BottomY)
              {
                BottomY = y;
              } 
          } while (PointPtr++<LastPtr); 
      } 

    if (BottomY==TopY)
      return FALSE;

    FXPEdgeTable.Start = TopY;
    FXPEdgeTable.Length = (LONG)(BottomY-TopY)+1;

    FXPPOINT2D *NextPtr;
    FXPPOINT2D **PointPtrPtr;

    
    NumEdge1 = 0;
    NextPtr = TopPointPtr;
    y = TopY;
    PointPtrPtr = FXPEdge1Index;

    while (y<BottomY)
      {
        PointPtr = NextPtr;
        if (NextPtr<LastPtr)
          NextPtr++;
        else
          NextPtr = FirstPtr;
        if ((y!=TopY)||(NextPtr->y!=TopY))
          {
            *(PointPtrPtr++) = PointPtr;
            y = PointPtr->y;
            NumEdge1++;
          } 
      } 

    
    NumEdge2 = 0;
    NextPtr = TopPointPtr;
    y = TopY;
    PointPtrPtr = FXPEdge2Index;

    while (y<BottomY)
      {
        PointPtr = NextPtr;
        if (NextPtr>FirstPtr)
          NextPtr--;
        else
          NextPtr = LastPtr;
        if ((y!=TopY)||(NextPtr->y!=TopY))
          {
            *(PointPtrPtr++) = PointPtr;
            y = PointPtr->y;
            NumEdge2++;
          } 
      } 

    FXPEDGE *EdgePtr;

    FXPPOINT2D **IndexPtr1;
    FXPPOINT2D **IndexPtr2;

    
    EdgePtr = FXPEdge1;
    IndexPtr1 = FXPEdge1Index;
    IndexPtr2 = IndexPtr1;
    IndexPtr2++;
    i = NumEdge1-2;
    while (i--)
      {
        EdgePtr = ScanEdgeFunction ( *(IndexPtr1++), *(IndexPtr2++),
                                     EdgePtr, 0 );
      } 
    ScanEdgeFunction ( *IndexPtr1, *IndexPtr2,
                       EdgePtr, 1 );

    
    EdgePtr = FXPEdge2;
    IndexPtr1 = FXPEdge2Index;
    IndexPtr2 = IndexPtr1;
    IndexPtr2++;
    i = NumEdge2-2;
    while (i--)
      {
        EdgePtr = ScanEdgeFunction ( *(IndexPtr1++), *(IndexPtr2++),
                                     EdgePtr, 0 );
      } 
    ScanEdgeFunction ( *IndexPtr1, *IndexPtr2,
                       EdgePtr, 1 );

    
    FXPEDGE *Edge1Ptr,*Edge2Ptr;
    FXPEdgeTable.Edge1 = Edge1Ptr = FXPEdge1;
    FXPEdgeTable.Edge2 = Edge2Ptr = FXPEdge2;

    LONG x1,x2;

    i = FXPEdgeTable.Length;
    do
      {
        x1 = Edge1Ptr->x;
        x2 = Edge2Ptr->x;
        Edge1Ptr++;
        Edge2Ptr++;
        if (x1<x2)
          {
            break;
          } 
        else if (x1>x2)
          {
            FXPEdgeTable.Edge1 = FXPEdge2;
            FXPEdgeTable.Edge2 = FXPEdge1;
            break;
          } 
      } while (i--); 

    return TRUE;
  } 

VOID G3DDRAW::CreateFXPGradient ( FXPGRADIENT *FXPGradient, FXPPOINT2D *Points, LONG ShadeFlags,
                                  LONG FaceFlags )
  {
    if ((ShadeFlags==0)&&(FaceFlags==0))
      return;

    float x0,x1,x2;
    float y0,y1,y2;

    x0 = (float)(Points[0].x>>16);
    y0 = (float)(Points[0].y);
    x1 = (float)(Points[1].x>>16);
    y1 = (float)(Points[1].y);
    x2 = (float)(Points[2].x>>16);
    y2 = (float)(Points[2].y);

    float dX,OneOverdX,OneOverdY;
    dX = (x1-x2)*(y0-y2) - (x0-x2)*(y1-y2);
    if (dX==0)
      {
        FXPGradient->OneOverZdX = 0; FXPGradient->OneOverZdY = 0;
        FXPGradient->UOverZdX = 0; FXPGradient->UOverZdY = 0;
        FXPGradient->VOverZdX = 0; FXPGradient->VOverZdY = 0;
        FXPGradient->IOverZdX = 0; FXPGradient->IOverZdY = 0;
        FXPGradient->HOverZdX = 0; FXPGradient->HOverZdY = 0;
      } 

    OneOverdX = (float)1 / dX;
    OneOverdY = -OneOverdX;

    if (ShadeFlags==SHADE_GOURAUD)
      {
        float I0,I1,I2;
        I0 = (float)(Points[0].I);  
        I1 = (float)(Points[1].I);
        I2 = (float)(Points[2].I);
        float IOverZdX,IOverZdY;

        IOverZdX = ((I1-I2)*(y0-y2)-(I0-I2)*(y1-y2)) * OneOverdX;
        IOverZdY = ((I1-I2)*(x0-x2)-(I0-I2)*(x1-x2)) * OneOverdY;

        FXPGradient->IOverZdX = (LONG)IOverZdX;
        FXPGradient->IOverZdY = (LONG)IOverZdY;
      } 
    if (FaceFlags==FACE_TEXTURE)
      {
        float U0,U1,U2;
        float V0,V1,V2;
        U0 = (float)(Points[0].u);  
        V0 = (float)(Points[0].v);  
        U1 = (float)(Points[1].u);  
        V1 = (float)(Points[1].v);  
        U2 = (float)(Points[2].u);  
        V2 = (float)(Points[2].v);  
        float UOverZdX,UOverZdY;
        float VOverZdX,VOverZdY;

        UOverZdX = ((U1-U2)*(y0-y2)-(U0-U2)*(y1-y2)) * OneOverdX;
        UOverZdY = ((U1-U2)*(x0-x2)-(U0-U2)*(x1-x2)) * OneOverdY;
        VOverZdX = ((V1-V2)*(y0-y2)-(V0-V2)*(y1-y2)) * OneOverdX;
        VOverZdY = ((V1-V2)*(x0-x2)-(V0-V2)*(x1-x2)) * OneOverdY;

        FXPGradient->UOverZdX = (LONG)UOverZdX;
        FXPGradient->UOverZdY = (LONG)UOverZdY;
        FXPGradient->VOverZdX = (LONG)VOverZdX;
        FXPGradient->VOverZdY = (LONG)VOverZdY;
      } 

    #if defined (__FORWATCOM__)
      _SetGradientData ( FXPGradient );
    #endif
  } 

VOID G3DDRAW::DrawPolySlow ( FXPPOINT2D *Points, ANIMIMAGE *Texture,
                             LONG Intensity, LONG Color, LONG DrawShadeFlags,
                             LONG DrawFaceFlags )
  {
    if (Texture)
      {}
    if (Intensity)
      {}

    Vx1 = ViewPort.x1;
    Vy1 = ViewPort.y1;
    Vx2 = ViewPort.x2;
    Vy2 = ViewPort.y2;

    ScanEdgeFunction = DrawRoutine[DrawShadeFlags][DrawFaceFlags][FALSE][FALSE].ScanEdgePtr;
    DrawPolyFunction = DrawRoutine[DrawShadeFlags][DrawFaceFlags][FALSE][FALSE].DrawPolyPtr;

    FXPGRADIENT FXPGradient;
    CreateFXPGradient ( &FXPGradient, Points, DrawShadeFlags, DrawFaceFlags );

    if (CreateFXPEdgeTable ( Points, 3, DrawShadeFlags, DrawFaceFlags )==FALSE)
      return;

    FXPEDGE *LeftEdge = FXPEdgeTable.Edge1;
    FXPEDGE *RightEdge = FXPEdgeTable.Edge2;

    BYTE *DestBuffer = Dest->SetOffset ( 0, FXPEdgeTable.Start );
    LONG AddDest = Dest->GetRowInc ();

    #if defined (__FORWATCOM__)
      _SetDestData ( DestBuffer, AddDest );
      _SetEdgeData ( LeftEdge, RightEdge, sizeof(FXPEDGE) );
    #else
      if (LeftEdge)
        {}
      if (RightEdge)
        {}
    #endif
    DrawPolyFunction ( Color, 0, 0, FXPEdgeTable.Length );
  } 

VOID G3DDRAW::TestDrawTriangle2 ( FXPPOINT2D *Points, ANIMIMAGE *Texture,
                                  LONG Intensity, LONG Color, LONG DrawShadeFlags,
                                  LONG DrawFaceFlags, COLORTABLE *BlendTBL, COLORTABLE *ShadeTBL,
                                  COLORTABLE *HazeTBL )
  {
    ShadeTable = ShadeTBL;
    HazeTable = HazeTBL;
    if (BlendTBL!=NULL)
      _BlendBuffer = BlendTBL->GetTable ();
    else
      _BlendBuffer = NULL;

    #if defined (__FORWATCOM__)
      _SetShadeTable ( ShadeTable->GetTable() );
      _SetTextureBuffer ( Texture->GetBuffer () );
    #endif
    DrawPolySlow ( Points, Texture, Intensity, Color, DrawShadeFlags, DrawFaceFlags );
  } 
  
