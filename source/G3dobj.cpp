




#include "stdgfx.h"
#include "g3dobj.h"

G3DOBJECT::G3DOBJECT ()
  {
    SubObjectList = new LINKEDLIST<G3DOBJECT*> ();
    FLPLocalPosition.x = (float)0;
    FLPLocalPosition.y = (float)0;
    FLPLocalPosition.z = (float)0;
    FLPAngle.x = (float)0;
    FLPAngle.y = (float)0;
    FLPAngle.z = (float)0;
    Type = 0;
    InitMatrix ( LocalMatrix );
    LightSource = NULL;
    Moved = Rotated = TRUE;
    Initialized = FALSE;
    MaxVisibleDistance = (float)64000;
    ScaleFactor = (float)1.0;
  } 

G3DOBJECT::~G3DOBJECT ()
  {
    if (SubObjectList!=NULL)
      {
        SubObjectList->ClearAllNode ();  
        delete SubObjectList;
      } 
    SubObjectList = NULL;    
  } 

VOID G3DOBJECT::SetName ( STRING NewName )
  {
    strcpy ( Name, NewName );  
  } 

G3DOBJECT* G3DOBJECT::FindObjectByName ( STRING SearchName )
  {
    if (strcmp ( SearchName, Name )==0)
      return this;
      
    LISTOBJECT<G3DOBJECT*> *ObjectNode;
    ObjectNode = SubObjectList->GetHead ();
    G3DOBJECT* Object;

    while (ObjectNode!=NULL)
      {
        Object = ObjectNode->Data->FindObjectByName ( SearchName );
        if (Object!=NULL)
          return Object;
        ObjectNode = ObjectNode->NextObject;
      } 
    return NULL;      
  } 

G3DOBJECT* G3DOBJECT::FindObjectByID ( LONG SearchID )
  {
    if (ID==SearchID)
      return this;
      
    LISTOBJECT<G3DOBJECT*> *ObjectNode;
    ObjectNode = SubObjectList->GetHead ();
    G3DOBJECT* Object;

    while (ObjectNode!=NULL)
      {
        Object = ObjectNode->Data->FindObjectByID ( SearchID );
        if (Object!=NULL)
          return Object;
        ObjectNode = ObjectNode->NextObject;
      } 
    return NULL;      
  } 

VOID G3DOBJECT::SetPosition ( float x, float y, float z )
  {
    FLPLocalPosition.x = x;  
    FLPLocalPosition.y = y;  
    FLPLocalPosition.z = z;  
  } 

VOID G3DOBJECT::SetAngle ( float x, float y, float z )
  {
    FLPAngle.x = x;  
    FLPAngle.y = y;  
    FLPAngle.z = z;  
  } 

VOID G3DOBJECT::SetScaleFactor ( float NewScaleFactor )
  {
    ScaleFactor = NewScaleFactor;  
  } 

VOID G3DOBJECT::SetMaxVisibleDistance ( float Distance )
  {
    MaxVisibleDistance = Distance;  
  } 

VOID G3DOBJECT::UpdateLocalToWorld ()
  {
    float x,y,z;
    float cx,cy,cz;
    
    x = (float)0;       
    y = (float)0;       
    z = (float)0;       
    cx = FLPLocalCenter.x + x;       
    cy = FLPLocalCenter.y + y;       
    cz = FLPLocalCenter.z + z;

    FLPWorldPosition.x = x*ResultMatrix[0][0]+
                          y*ResultMatrix[1][0]+
                           z*ResultMatrix[2][0]+
                              ResultMatrix[3][0];
    FLPWorldPosition.y = x*ResultMatrix[0][1]+
                          y*ResultMatrix[1][1]+
                           z*ResultMatrix[2][1]+
                              ResultMatrix[3][1];
    FLPWorldPosition.z = x*ResultMatrix[0][2]+
                          y*ResultMatrix[1][2]+
                           z*ResultMatrix[2][2]+
                              ResultMatrix[3][2];

    FLPWorldCenter.x  = cx*ResultMatrix[0][0]+
                          cy*ResultMatrix[1][0]+
                           cz*ResultMatrix[2][0]+
                               ResultMatrix[3][0];
    FLPWorldCenter.y  = cx*ResultMatrix[0][1]+
                          cy*ResultMatrix[1][1]+
                           cz*ResultMatrix[2][1]+
                               ResultMatrix[3][1];
    FLPWorldCenter.z  = cx*ResultMatrix[0][2]+
                          cy*ResultMatrix[1][2]+
                           cz*ResultMatrix[2][2]+
                               ResultMatrix[3][2];
  } 

VOID G3DOBJECT::TransformLocalToWorld ( G3DMATRIX ParentMatrix )
  {
    MultMatrix ( LocalMatrix, ParentMatrix, ResultMatrix );
    Scale ( ResultMatrix, ScaleFactor );
    Translate ( ResultMatrix, FLPLocalPosition.x, FLPLocalPosition.y, FLPLocalPosition.z );

    UpdateLocalToWorld ();

    LISTOBJECT<G3DOBJECT*> *ObjectNode;
    ObjectNode = SubObjectList->GetHead ();

    while (ObjectNode!=NULL)
      {
        ObjectNode->Data->TransformLocalToWorld ( ResultMatrix );
        ObjectNode = ObjectNode->NextObject;  
      } 
      
    InitMatrix ( LocalMatrix );  
  } 

VOID G3DOBJECT::UpdateWorldToCamera ( G3DMATRIX CameraMatrix,
                                      CAMERADATA *CameraData )  
  {
    float x,y,z;
    float cx,cy,cz;
    
    x = FLPWorldPosition.x;       
    y = FLPWorldPosition.y;       
    z = FLPWorldPosition.z;       
    cx = FLPWorldCenter.x;       
    cy = FLPWorldCenter.y;       
    cz = FLPWorldCenter.z;

    FLPCameraPosition.x = x*CameraMatrix[0][0]+
                           y*CameraMatrix[1][0]+
                            z*CameraMatrix[2][0]+
                               CameraMatrix[3][0];
    FLPCameraPosition.y = x*CameraMatrix[0][1]+
                           y*CameraMatrix[1][1]+
                            z*CameraMatrix[2][1]+
                               CameraMatrix[3][1];
    FLPCameraPosition.z = x*CameraMatrix[0][2]+
                           y*CameraMatrix[1][2]+
                            z*CameraMatrix[2][2]+
                               CameraMatrix[3][2];

    FLPCameraCenter.x  = cx*CameraMatrix[0][0]+
                           cy*CameraMatrix[1][0]+
                            cz*CameraMatrix[2][0]+
                                CameraMatrix[3][0];
    FLPCameraCenter.y  = cx*CameraMatrix[0][1]+
                           cy*CameraMatrix[1][1]+
                            cz*CameraMatrix[2][1]+
                                CameraMatrix[3][1];
    FLPCameraCenter.z  = cx*CameraMatrix[0][2]+
                           cy*CameraMatrix[1][2]+
                            cz*CameraMatrix[2][2]+
                                CameraMatrix[3][2];

    ClipObject ( CameraData );
  } 

VOID G3DOBJECT::TransformWorldToCamera ( G3DMATRIX CameraMatrix,
                                         CAMERADATA *CameraData )  
  {    
    UpdateWorldToCamera ( CameraMatrix, CameraData );

    if (IsObjectVisible())
      {
        LISTOBJECT<G3DOBJECT*> *ObjectNode;
        ObjectNode = SubObjectList->GetHead ();

        while (ObjectNode!=NULL)
          {
            ObjectNode->Data->TransformWorldToCamera ( CameraMatrix,
                                                       CameraData );
            ObjectNode = ObjectNode->NextObject;  
          } 
      } 
      
    Moved = Rotated = FALSE;
  } 

VOID G3DOBJECT::InitLocalMatrix ()
  {
    InitMatrix ( LocalMatrix );  
  } 

VOID G3DOBJECT::Init () 
  {
    ComputeCenter ();
    
    LISTOBJECT<G3DOBJECT*> *ObjectNode;
    ObjectNode = SubObjectList->GetHead ();

    while (ObjectNode!=NULL)
      {
        ObjectNode->Data->Init ();
        ObjectNode = ObjectNode->NextObject;  
      } 
  } 

VOID G3DOBJECT::Move ( float xt, float yt, float zt )
  {
    FLPLocalPosition.x += xt;  
    FLPLocalPosition.y += yt;  
    FLPLocalPosition.z += zt;
    G3DMATRIX Matrix;
    InitMatrix ( Matrix );
    TransformLocalToWorld ( Matrix );
    Moved = TRUE;     
  } 

VOID G3DOBJECT::Rotate ( float xa, float ya, float za )
  {
    FLPAngle.x += xa;  
    FLPAngle.y += ya;  
    FLPAngle.z += za;
    
    RotateXYZ ( LocalMatrix, FLPAngle.x, FLPAngle.y,
                FLPAngle.z );
    TransformLocalToWorld ( LocalMatrix );
    Rotated = TRUE;
  } 

VOID G3DOBJECT::CountShapes ( LONG *Count )
  {
    if (Count)
      {}  
  } 

VOID G3DOBJECT::CountNumShapes ( LONG *Count )
  {
    CountShapes ( Count );
      
    LISTOBJECT<G3DOBJECT*> *ObjectNode;
    ObjectNode = SubObjectList->GetHead ();

    while (ObjectNode!=NULL)
      {
        ObjectNode->Data->CountNumShapes ( Count );
        ObjectNode = ObjectNode->NextObject;  
      } 
  } 

BOOLEAN G3DOBJECT::ClipObject ( CAMERADATA *CameraData )
  {
    float MaxZ,MinZ;
    MaxZ = FLPCameraCenter.z + Radius;
    MinZ = FLPCameraCenter.z - Radius;

    Flags &= ~OBJECT_VISIBLE;

    if (MaxZ<CameraData->NearClipZ)
      return FALSE;
    if (MinZ>MaxVisibleDistance)
      return FALSE;  

    float MinX,MinY,MaxX,MaxY;

    MinX = FLPCameraCenter.x - Radius/2;
    MaxX = FLPCameraCenter.x + Radius/2;
    MinY = FLPCameraCenter.y - Radius/2;
    MaxY = FLPCameraCenter.y + Radius/2;

    float MinScrX,MaxScrX;
    float MinScrY,MaxScrY;

    float Ratio = CameraData->ViewDistance / MaxZ;
    
    MinScrX = MinX*Ratio + CameraData->CenterX;
    MinScrY = -MaxY*Ratio + CameraData->CenterY;
    MaxScrX = MaxX*Ratio + CameraData->CenterX;
    MaxScrY = -MinY*Ratio + CameraData->CenterY;
    
    if (MinScrX>CameraData->ViewPort.x2)
      return FALSE;
    if (MinScrY>CameraData->ViewPort.y2)
      return FALSE;
    if (MaxScrX<CameraData->ViewPort.x1)
      return FALSE;
    if (MaxScrY<CameraData->ViewPort.y1)
      return FALSE;
      
    Flags |= OBJECT_VISIBLE;
    return TRUE;
  } 

VOID G3DOBJECT::ComputeCenter ()
  {
    FLPLocalCenter = FLPLocalPosition;
  } 

float G3DOBJECT::ComputeRadius ()
  {
    float Distance;
    float MaxDist;

    MaxDist = (float)0;
    
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
  
VOID G3DOBJECT::CheckCollision ( FLPVECTOR3D StartPt, FLPVECTOR3D EndPt,
                                 COLLIDEDATA* CollideList, LONG *CurNum, LONG MaxNum,
                                 float CollideDist, float Gap )
  {
    if (*CurNum<MaxNum)
      {
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


