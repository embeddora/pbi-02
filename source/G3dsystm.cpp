




#include "stdgfx.h"
#include "g3dsystm.h"

#include "g3d3ds.h"
#include "g3dasc.h"
#include "g3dgem.h"
#include "g3dgeo.h"

#include "stdlib.h"

G3DSYSTEM::G3DSYSTEM ()
  {
    G3DDraw = new G3DDRAW ();

    World = NULL;
    MaterialLib = NULL;

    VisibleShapes = NULL;
    ShadeFlags = SHADE_GOURAUD;
    FaceFlags = FACE_TEXTURE;
    DepthCueing = FALSE;
    DepthScale = (float)1.0;
    BlendTable = NULL;
    HazeScale = (float)1.0;
    HazeTable = NULL;
    DoHaze = FALSE;
  } 

G3DSYSTEM::~G3DSYSTEM ()
  {
    if (G3DDraw!=NULL)
      delete G3DDraw;
    G3DDraw = NULL;

    if (VisibleShapes!=NULL)
      delete VisibleShapes;
    VisibleShapes = NULL;  

    if (World!=NULL)
      delete World;
    World = NULL;

    if (BlendTable!=NULL)
      delete BlendTable;
    BlendTable = NULL;
      
    if (HazeTable!=NULL)
      delete HazeTable;
    HazeTable = NULL;
      
    if (MaterialLib!=NULL)
      delete MaterialLib;
    MaterialLib = NULL;
  } 

VOID G3DSYSTEM::SetShadeFlags ( LONG Flags )
  {
    ShadeFlags = Flags;  
  } 

VOID G3DSYSTEM::SetFaceFlags ( LONG Flags )
  {
    FaceFlags = Flags;  
  } 
  
VOID G3DSYSTEM::SetDepthCueing ( BOOLEAN OnOff, float Scale )
  {
    DepthCueing = OnOff;
    DepthScale = Scale;  
  } 

VOID G3DSYSTEM::SetBlendTable ( COLORTABLE *Table )
  {
    BlendTable = Table;  
  } 

VOID G3DSYSTEM::SetHazing ( BOOLEAN OnOff, float Scale, COLORTABLE *Table )
  {
    DoHaze = OnOff;
    HazeScale = Scale;
    HazeTable = Table;  
  } 

VOID G3DSYSTEM::AllocateShapeTable ( LONG Num )
  {
    if (VisibleShapes!=NULL)
      delete VisibleShapes;
    VisibleShapes = new G3DSHAPE* [Num];        
  } 

LONG G3DSYSTEM::CountNumShapes ()
  {
    if (World==NULL)
      return 0;
    return World->CountNumShapes ();    
  } 

VOID G3DSYSTEM::SetDestination ( IMAGE *Dest )
  {
    if (G3DDraw!=NULL)
      {
        G3DDraw->SetDestination ( Dest );  
      } 
  } 

VOID G3DSYSTEM::SetViewDistance ( float ViewDistance )
  {
    if (G3DDraw!=NULL)
      {
        G3DDraw->SetViewDistance ( ViewDistance );  
      } 
  } 

VOID G3DSYSTEM::SetScreenCenter ( LONG CenterX, LONG CenterY )
  {
    if (G3DDraw!=NULL)
      {
        G3DDraw->SetScreenCenter ( CenterX, CenterY );  
      } 
  } 

VOID G3DSYSTEM::SetMaterialLib ( G3DMATERIALLIB *NewLib )
  {
    MaterialLib = NewLib;  
  } 
  
G3DMATERIAL* G3DSYSTEM::FindMaterialByName ( STRING Name )
  {
    if (MaterialLib==NULL)
      return NULL;
      
    return MaterialLib->FindMaterialByName ( Name );  
  } 

G3DMATERIAL* G3DSYSTEM::FindMaterialByID ( LONG SearchID )
  {
    if (MaterialLib==NULL)
      return NULL;

    return MaterialLib->FindMaterialByID ( SearchID );  
  } 

G3DMATERIAL* G3DSYSTEM::FindMaterialByIndex ( LONG Index )
  {
    if (MaterialLib==NULL)
      return NULL;

    if ((Index<0)||(Index>=MaterialLib->GetNumMaterials()))
      return NULL;
      
    return MaterialLib->GetMaterial ( Index );  
  } 

VOID G3DSYSTEM::SetWorld ( G3DWORLD *NewWorld, BOOLEAN DeleteOld )
  {
    if ((World!=NULL)&&(DeleteOld))
	{
      		delete World;        
	}
    World = NewWorld;  
  } 

int CompareShape ( const void *Elem1, const void *Elem2 )
  {
    G3DSHAPE *Shape1 = (*(G3DSHAPE**)Elem1);  
    G3DSHAPE *Shape2 = (*(G3DSHAPE**)Elem2);

    if (Shape1->MinZ>Shape2->MaxZ)
      return -1;
    if (Shape1->MaxZ<Shape2->MinZ)
      return 1;

    if (Shape1->MinZ>Shape2->MinZ)
      return -1;
    if (Shape1->MinZ<Shape2->MinZ)
      return 1;
      
    if (Shape1->MaxZ>Shape2->MaxZ)
      return -1;
    if (Shape1->MaxZ<Shape2->MaxZ)
      return 1;
              
    return 0;  
  } 

VOID G3DSYSTEM::PainterSort ()
  {
    qsort ( VisibleShapes, NumVisibleShapes, sizeof(G3DSHAPE*),
            CompareShape ); 
  } 

BOOLEAN G3DSYSTEM::Init ()
  {
    BOOLEAN Result;  

    if (World==NULL)
      return FAILURE;

    Result = World->Init ();

    if (Result)
      {
        LONG NumShapes;

        NumShapes = CountNumShapes ();

        AllocateShapeTable ( NumShapes*2 );  
      } 

    return Result;  
  } 

G3DOBJECT* G3DSYSTEM::FindObjectByName ( STRING SearchName )
  {
    if (World==NULL)
      return NULL;
    return World->FindObjectByName ( SearchName );    
  } 

G3DOBJECT* G3DSYSTEM::FindObjectByID ( LONG SearchID )
  {
    if (World==NULL)
      return NULL;
    return World->FindObjectByID ( SearchID );    
  } 

INT G3DSYSTEM::Get3DFileType ( FILEHANDLE f )
  {
    SHORT ID;
    
    if (File.Read ( f, &ID, 2  )==FAILURE)  
      return -1;

    
    if (ID==CHUNK_PRIMARY)  
      {
        return FILE_3DS;
      } 

    CHAR TypeStr[128];

    
    File.Seek ( f, 0, FROM_BEGIN );
    if (File.Read ( f, TypeStr, 7 )==FAILURE)
      return -1;
    if (strncmp ( TypeStr, "Ambient", 7)==0)
      return FILE_ASC;    

    
    File.Seek ( f, 0, FROM_BEGIN );
    if (File.Read ( f, TypeStr, 4 )==FAILURE)
      return -1;
    if (strncmp ( TypeStr, "3DG1", 4)==0)
      return FILE_GEO;    

    return FILE_GEM;  
  } 
  
BOOLEAN G3DSYSTEM::Load3DFile ( STRING FileName, LINKEDLIST<G3DOBJECT*> *ObjectList,
                                BOOLEAN ClockWise, double Scale )
  {
    FILEHANDLE f;
    BOOLEAN Result;
    
    f = File.Open ( FileName, OPEN_READ | OPEN_BINARY );
    if (f==NULL)
      return FAILURE;

    INT Type;

    Type = Get3DFileType ( f );
    File.Seek ( f, 0, FROM_BEGIN );

    G3DFILETOOL *G3DFileTool=NULL;
    switch (Type)
      {
        case FILE_ASC :
          G3DFileTool = new G3DASCTOOL ();
          break; 
        case FILE_3DS :
          G3DFileTool = new G3D3DSTOOL ();
          break;
        case FILE_GEM :
          G3DFileTool = new G3DGEMTOOL ();
          break;
        case FILE_GEO :
          G3DFileTool = new G3DGEOTOOL ();
          break;
        default :
          G3DFileTool = NULL;
          break;
      } 

    if (G3DFileTool!=NULL)
      {
        Result = G3DFileTool->Load ( f, ObjectList, ClockWise, Scale );
        delete G3DFileTool;
      } 
    else
      Result = FAILURE;
        
    File.Close ( f );
    return Result;
  } 

BOOLEAN G3DSYSTEM::Save3DFile ( STRING FileName, LINKEDLIST<G3DOBJECT*> *ObjectList,
                                BOOLEAN ClockWise, double Scale, INT Type )
  {
    FILEHANDLE f;
    BOOLEAN Result;
    
    f = File.Open ( FileName, OPEN_WRITE | OPEN_BINARY );
    if (f==NULL)
      return FAILURE;

    File.Seek ( f, 0, FROM_BEGIN );

    G3DFILETOOL *G3DFileTool=NULL;
    switch (Type)
      {
        case FILE_ASC :
          G3DFileTool = new G3DASCTOOL ();
          break; 
        case FILE_3DS :
          G3DFileTool = new G3D3DSTOOL ();
          break;
        case FILE_GEM :
          G3DFileTool = new G3DGEMTOOL ();
          break;
        case FILE_GEO :
          G3DFileTool = new G3DGEOTOOL ();
          break;
        default :
          G3DFileTool = NULL;
          break;
      } 

    if (G3DFileTool!=NULL)
      {
        Result = G3DFileTool->Save ( f, ObjectList, ClockWise, Scale );
        delete G3DFileTool;
      } 
    else
      Result = FAILURE;
        
    File.Close ( f );
    return Result;
  } 
                                
VOID G3DSYSTEM::SetViewPort ( LONG x1, LONG y1, LONG x2, LONG y2 )
  {
    if (G3DDraw!=NULL)
      G3DDraw->SetViewPort ( x1, y1, x2, y2 );  
  } 
  
VOID G3DSYSTEM::SetNearClipZ ( float Z )
  {
    if (G3DDraw!=NULL)
      G3DDraw->SetNearClipZ ( Z );  
  } 

FLPVECTOR3D G3DSYSTEM::ComputeNextPos ( FLPVECTOR3D StartP, FLPVECTOR3D Angle, 
                                        FLPVECTOR3D Vector )
  {
    G3DMATRIX Matrix;
    
    InitMatrix ( Matrix );
    RotateXYZ ( Matrix, Angle.x, Angle.y, Angle.z );

    FLPVECTOR3D EndP;
    float x,y,z;
    
    x = Vector.x*Matrix[0][0]+
         Vector.y*Matrix[1][0]+    
          Vector.z*Matrix[2][0];    
    y = Vector.x*Matrix[0][1]+
         Vector.y*Matrix[1][1]+    
          Vector.z*Matrix[2][1];    
    z = Vector.x*Matrix[0][2]+
         Vector.y*Matrix[1][2]+    
          Vector.z*Matrix[2][2];

    EndP.x = StartP.x + x;
    EndP.y = StartP.y + y; 
    EndP.z = StartP.z + z;
    
    return EndP;
  } 
  
LONG G3DSYSTEM::CheckCollision ( FLPVECTOR3D StartPt, FLPVECTOR3D EndPt,
                                 COLLIDEDATA *CollideList, LONG MaxNum,
                                 float CollideDist, float Gap )
  {
    if (World==NULL)
      return 0;

    LONG Count;

    Count = World->CheckCollision ( StartPt, EndPt, CollideList, MaxNum, CollideDist, Gap );
    return Count;   
  } 
                                 
VOID G3DSYSTEM::ShowView ( G3DCAMERA *Camera )
  {
printf(" >> G3DSYSTEM::ShowView 0  \n");
    if (Camera==NULL)
      return;
printf(" >> G3DSYSTEM::ShowView 1  \n");
    if (World==NULL)
      return;
          
printf(" >> G3DSYSTEM::ShowView 2  \n");
    InitMatrix ( ViewMatrix );

printf(" >> G3DSYSTEM::ShowView 3  \n");
    FLPVECTOR3D CamAngle,CamPos;
    CamAngle = Camera->GetAngle ();
printf(" >> G3DSYSTEM::ShowView 4  \n");
    CamPos = Camera->GetWorldPosition ();
    Translate ( ViewMatrix, -CamPos.x, -CamPos.y, -CamPos.z );
printf(" >> G3DSYSTEM::ShowView 5  \n");
    RotateYXZ ( ViewMatrix, -CamAngle.x, -CamAngle.y, -CamAngle.z );

    CAMERADATA CameraData;
    CameraData.CameraPoint = CamPos;
    CameraData.NumVisibleShapes = 0;
    CameraData.VisibleShapes = VisibleShapes;
    CameraData.CenterX = (float)G3DDraw->GetCenterX();
    CameraData.CenterY = (float)G3DDraw->GetCenterY();
    CameraData.ViewDistance = G3DDraw->GetViewDistance();
    CameraData.ViewPort = G3DDraw->GetViewPort ();
    CameraData.NearClipZ = G3DDraw->GetNearClipZ ();
    CameraData.ShadeFlags = ShadeFlags;
    CameraData.FaceFlags = FaceFlags;
    CameraData.DepthCueing = DepthCueing;
    CameraData.DepthScale = DepthScale;
    
printf(" >> G3DSYSTEM::ShowView 16  \n");
    if (HazeTable!=NULL)
      {
printf(" >> G3DSYSTEM::ShowView 16  a\n");
        CameraData.DoHaze = DoHaze;
        CameraData.HazeScale = HazeScale;
        CameraData.HazeLevels = HazeTable->GetNumLevels();
      } 
    else  
      {
printf(" >> G3DSYSTEM::ShowView 16  b\n");
        CameraData.DoHaze = FALSE;
        CameraData.HazeScale = HazeScale;
        CameraData.HazeLevels = 0;
      } 

printf(" >> G3DSYSTEM::ShowView 17\n");
    World->TransformWorldToCamera ( ViewMatrix, &CameraData );

    NumVisibleShapes = CameraData.NumVisibleShapes;
printf(" >> G3DSYSTEM::ShowView 18\n");
    PainterSort ();

    LONG i;
    G3DDraw->ShadeFlags = ShadeFlags;
    G3DDraw->FaceFlags = FaceFlags;
    G3DDraw->BlendTable = BlendTable;
    G3DDraw->DoHaze = DoHaze;
    G3DDraw->HazeTable = HazeTable;
printf(" >> G3DSYSTEM::ShowView 19, NumVisibleShapes = %d \n", NumVisibleShapes );
    for (i=0;i<NumVisibleShapes;i++)
      {
        G3DDraw->DrawShape ( VisibleShapes[i] );
      } 
  } 


VOID G3DSYSTEM::TestDrawTriangle ( FXPPOINT2D *Points, ANIMIMAGE *Texture,
                                   LONG Intensity, LONG Color, LONG DrawShadeFlags,
                                   LONG DrawFaceFlags, COLORTABLE *BlendTBL, COLORTABLE *ShadeTBL,
                                   COLORTABLE *HazeTBL )
  {
    G3DDraw->TestDrawTriangle ( Points, Texture, Intensity, Color, DrawShadeFlags,
                                DrawFaceFlags,  BlendTBL, ShadeTBL, HazeTBL );
  } 

VOID G3DSYSTEM::TestDrawTriangle2 ( FXPPOINT2D *Points, ANIMIMAGE *Texture,
                                    LONG Intensity, LONG Color, LONG DrawShadeFlags,
                                    LONG DrawFaceFlags, COLORTABLE *BlendTBL, COLORTABLE *ShadeTBL,
                                    COLORTABLE *HazeTBL )
  {
    G3DDraw->TestDrawTriangle2 ( Points, Texture, Intensity, Color, DrawShadeFlags,
                                 DrawFaceFlags,  BlendTBL, ShadeTBL, HazeTBL );
  } 

