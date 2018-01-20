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
#include "g3dworld.h"

G3DWORLD::G3DWORLD ()
  {
    ObjectList = new LINKEDLIST<G3DOBJECT*> (); 
  } 

G3DWORLD::~G3DWORLD ()
  {
    if (ObjectList!=NULL)
                        { 
                                ObjectList->ClearAllNode ();
        delete ObjectList;
      } 
    ObjectList = NULL;  
  } 

BOOLEAN G3DWORLD::AddObject ( G3DOBJECT * NewObject )
  {
    ObjectList->AddObject ( NewObject );  
    return TRUE;  
  } 

VOID G3DWORLD::TransformWorldToCamera ( G3DMATRIX CameraMatrix,
                                        CAMERADATA *CameraData )
  {
    LISTOBJECT<G3DOBJECT*> *ObjectNode;
    G3DOBJECT* Object;

    ObjectNode = ObjectList->GetHead ();

    while (ObjectNode!=NULL)
      {
        Object = ObjectNode->Data;
        Object->TransformWorldToCamera ( CameraMatrix, CameraData );
        ObjectNode= ObjectNode->NextObject;  
      } 
  } 

BOOLEAN G3DWORLD::Init ()
  {
    LISTOBJECT<G3DOBJECT*> *ObjectNode;
    G3DOBJECT* Object;
    G3DMATRIX Matrix;

    ObjectNode = ObjectList->GetHead ();

    InitMatrix ( Matrix );
    
    while (ObjectNode!=NULL)
      {
        Object = ObjectNode->Data;
        Object->Init ();
        Object->TransformLocalToWorld ( Matrix );
        Object->ComputeRadius ();
        ObjectNode= ObjectNode->NextObject;  
      } 
    return TRUE;  
  } 

G3DOBJECT* G3DWORLD::FindObjectByName ( STRING SearchName )
  {
    LISTOBJECT<G3DOBJECT*> *ObjectNode;
    G3DOBJECT* Object;

    ObjectNode = ObjectList->GetHead ();

    while (ObjectNode!=NULL)
      {
        Object = ObjectNode->Data->FindObjectByName ( SearchName );
        if (Object!=NULL)
          return Object;
        ObjectNode= ObjectNode->NextObject;  
      } 
    return NULL;        
  } 

G3DOBJECT* G3DWORLD::FindObjectByID ( LONG SearchID )
  {
    LISTOBJECT<G3DOBJECT*> *ObjectNode;
    G3DOBJECT* Object;

    ObjectNode = ObjectList->GetHead ();

    while (ObjectNode!=NULL)
      {
        Object = ObjectNode->Data->FindObjectByID ( SearchID );
        if (Object!=NULL)
          return Object;
        ObjectNode= ObjectNode->NextObject;  
      } 
    return NULL;        
  } 

LONG G3DWORLD::CountNumShapes ()
  {
    LISTOBJECT<G3DOBJECT*> *ObjectNode;
    LONG Count;

    Count = 0;
    
    ObjectNode = ObjectList->GetHead ();

    while (ObjectNode!=NULL)
      {
        ObjectNode->Data->CountNumShapes ( &Count );
        ObjectNode= ObjectNode->NextObject;  
      } 
    return Count;              
  } 

LONG G3DWORLD::CheckCollision ( FLPVECTOR3D StartPt, FLPVECTOR3D EndPt,
                                COLLIDEDATA *CollideList, LONG MaxNum,
                                float CollideDist, float Gap )
  {
    LISTOBJECT<G3DOBJECT*> *ObjectNode;
    LONG Count;

    Count = 0;
    
    ObjectNode = ObjectList->GetHead ();

    while (ObjectNode!=NULL)
      {
        ObjectNode->Data->CheckCollision ( StartPt, EndPt, CollideList, &Count,
                                           MaxNum, CollideDist, Gap );
        ObjectNode= ObjectNode->NextObject;  
      } 
    return Count;              
  } 
  
