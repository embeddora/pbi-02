




#ifndef __G3DSYSTEM_H__
#define __G3DSYSTEM_H__

#include "stdgfx.h"
#include "g3dcam.h"
#include "g3ddraw.h"
#include "g3dfile.h"
#include "g3dworld.h"

class G3DSYSTEM 
  {
    protected :
      G3DDRAW *G3DDraw;
      G3DWORLD *World;
      G3DMATERIALLIB *MaterialLib;
      G3DMATRIX ViewMatrix;
      LONG NumVisibleShapes;
      G3DSHAPE **VisibleShapes;
      LONG ShadeFlags;
      LONG FaceFlags;
      BOOLEAN DepthCueing;
      float DepthScale;
      COLORTABLE *BlendTable;
      BOOLEAN DoHaze;
      COLORTABLE *HazeTable;
      float HazeScale;

      VOID AllocateShapeTable ( LONG Num );
      LONG CountNumShapes ();
      VOID PainterSort ();
      INT Get3DFileType ( FILEHANDLE f );
      
    public :      
      G3DSYSTEM ();
      virtual ~G3DSYSTEM ();
      
      VOID SetDestination ( IMAGE *Dest );
      VOID SetViewDistance ( float ViewDistance );
      VOID SetScreenCenter ( LONG CenterX, LONG CenterY );

      VOID SetMaterialLib ( G3DMATERIALLIB *NewLib );
      VOID SetWorld ( G3DWORLD *NewWorld, BOOLEAN DeleteOld );

      VOID SetShadeFlags ( LONG Flags );
      VOID SetFaceFlags ( LONG Flags );
      VOID SetDepthCueing ( BOOLEAN OnOff, float Scale );
      VOID SetBlendTable ( COLORTABLE *Table );
      VOID SetHazing ( BOOLEAN OnOff, float Scale, COLORTABLE *Table );

      G3DMATERIALLIB* GetMaterialLib () { return MaterialLib; };
      G3DMATERIAL *FindMaterialByName ( STRING Name );
      G3DMATERIAL *FindMaterialByID ( LONG SearchID );
      G3DMATERIAL *FindMaterialByIndex ( LONG Index );
      
      G3DWORLD* GetWorld () { return World; };
      
      G3DOBJECT* FindObjectByName ( STRING SearchName );
      G3DOBJECT* FindObjectByID ( LONG SearchID );
      
      BOOLEAN Load3DFile ( STRING FileName, LINKEDLIST<G3DOBJECT*> *ObjectList,
                           BOOLEAN ClockWise, double Scale );
      BOOLEAN Save3DFile ( STRING FileName, LINKEDLIST<G3DOBJECT*> *ObjectList,
                           BOOLEAN ClockWise, double Scale, INT Type );
                        
      BOOLEAN Init ();
      VOID ShowView ( G3DCAMERA *Camera );
      VOID SetViewPort ( LONG x1, LONG y1, LONG x2, LONG y2 );
      VOID SetNearClipZ ( float Z );
      FLPVECTOR3D ComputeNextPos ( FLPVECTOR3D StartP, FLPVECTOR3D Angle, 
                                   FLPVECTOR3D Vector );
      LONG CheckCollision ( FLPVECTOR3D StartPt, FLPVECTOR3D EndPt,
                            COLLIDEDATA *CollideList, LONG MaxNum,
                            float CollideDist, float Gap );

      
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




