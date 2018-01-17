




#ifndef __G3DDRAW_H__
#define __G3DDRAW_H__

#include "stdgfx.h"
#include "g3dpobj.h"
#include "g3dlight.h"

#define DRAW_FLAT_TOP     1
#define DRAW_FLAT_BOTTOM  2
#define DRAW_FLAT         3
#define DRAW_GENERAL      4

struct FXPDRAWSTRUCT
  {
    FXPPOINT2D *Top,*Bottom,*Middle;
    FXPPOINT2D *Left,*Right;
    LONG Case;
    LONG Length;
    LONG Reserved;
  }; 
  
struct FLPEDGE
  {
    float x,y;
    float u,v;
    float I;
    float Haze;
    float z;
    float Reserved1;
  }; 

struct FLPEDGETABLE
  {
    LONG Start;
    LONG Length;
    FLPEDGE *Edge1,*Edge2;  
  }; 

struct FXPEDGE
  {
    LONG x,y;
    LONG u,v;
    LONG I;
    LONG Haze;
    LONG z;
    LONG Reserved1;
  }; 

struct FXPEDGETABLE
  {
    LONG Start;
    LONG Length;
    FXPEDGE *Edge1,*Edge2;  
  }; 

struct FXPGRADIENT
  { 
    LONG OneOverZdX, OneOverZdY;  
    LONG UOverZdX, UOverZdY;      
    LONG VOverZdX, VOverZdY;      
    LONG IOverZdX, IOverZdY;      
    LONG HOverZdX, HOverZdY;      
  }; 

struct CLIPPOINT3D
  {
    float x,y,z;
    float u,v,I;
    float Haze;
  }; 

class G3DDRAW
  {
    protected :
      LONG CenterX,CenterY;
      float ViewDistance;
      IMAGE *Dest;
      RECTANGLE ViewPort;
      COLORTABLE *ShadeTable;
      float MIN_Z_DISTANCE;

      BOOLEAN SortPolyHorz  ( FXPPOINT2D *Points, FXPDRAWSTRUCT *DrawStruct );     
      
      VOID DrawPolyFast ( FXPPOINT2D *Points, ANIMIMAGE *Texture,
                          LONG Intensity, LONG Color, LONG DrawShadeFlags,
                          LONG DrawFaceFlags );

      VOID CreateFLPEdgeTable ( FLPPOINT2D *Points, LONG NumPoints );

      VOID XYIntersect ( FXPPOINT2D *In, FXPPOINT2D *Out,
                         FXPPOINT2D *Intersect, LONG Code, LONG ShadeFlags,
                         LONG FaceFlags );
      LONG ClipFXPPolygon ( FXPPOINT2D *Points, LONG NumPoints, LONG Code,
                            LONG ShadeFlags, LONG FaceFlags );
      BOOLEAN CreateFXPEdgeTable ( FXPPOINT2D *Points, LONG NumPoints,
                                   LONG ShadeFlags, LONG FaceFlags );
      VOID CreateFXPGradient ( FXPGRADIENT *FxpGradient, FXPPOINT2D *Points,
                               LONG ShadeFlags, LONG FaceFlags );
      VOID DrawPolySlow ( FXPPOINT2D *Points, ANIMIMAGE *Texture,
                          LONG Intensity, LONG Color, LONG DrawShadeFlags,
                          LONG DrawFaceFlags );

      VOID ZIntersect ( CLIPPOINT3D *In, CLIPPOINT3D *Out, CLIPPOINT3D *Intersect,
                        LONG DrawShadeFlags, LONG DrawFaceFlags );
      LONG ZClip ( CLIPPOINT3D *Points, CLIPPOINT3D *ClippedPoints, LONG Code, 
                   LONG DrawShadeFlags, LONG DrawFaceFlags );

      VOID DrawPoly ( G3DPOLYHEDRON *Object, G3DPOLY *Poly );

      VOID InitFunctionTable ();

    public :
      LONG ShadeFlags;
      LONG FaceFlags;
      COLORTABLE *BlendTable;
      BOOLEAN DoHaze;
      COLORTABLE *HazeTable;
    
      G3DDRAW ();
      virtual ~G3DDRAW ();
      
      LONG GetCenterX () { return CenterX; };
      LONG GetCenterY () { return CenterY; };
      float GetViewDistance () { return ViewDistance; };
      RECTANGLE GetViewPort ();
      float GetNearClipZ ();

      VOID SetDestination ( IMAGE *Destination );
      VOID SetViewDistance ( float VDistance ); 
      VOID SetScreenCenter ( LONG Cx, LONG Cy ); 
      VOID SetViewPort ( LONG x1, LONG y1, LONG x2, LONG y2 );
      VOID SetNearClipZ ( float Z );
      
      VOID DrawShape ( G3DSHAPE *Shape );

      
      VOID TestDrawTriangle ( FXPPOINT2D *Points, ANIMIMAGE *Texture,
                              LONG Intensity, LONG Color, LONG DrawShadeFlags,
                              LONG DrawFaceFlags, COLORTABLE *BlendTBL, COLORTABLE *ShadeTBL,
                              COLORTABLE *HazeTBL ); 
      VOID TestDrawTriangle2 ( FXPPOINT2D *Points, ANIMIMAGE *Texture,
                               LONG Intensity, LONG Color, LONG DrawShadeFlags,
                               LONG DrawFaceFlags, COLORTABLE *BlendTBL, COLORTABLE *ShadeTBL,
                               COLORTABLE *HazeTBL ); 
  }; 

#endif


