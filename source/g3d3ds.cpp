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
#include "g3d3ds.h"

G3D3DSTOOL::G3D3DSTOOL () : G3DFILETOOL ()
  {
    u_list = NULL;
    v_list = NULL;  
  } 

G3D3DSTOOL::~G3D3DSTOOL ()
  {
    if (u_list!=NULL)
      delete u_list;  
    if (v_list!=NULL)
      delete v_list;

    u_list = v_list;  
  } 

VOID G3D3DSTOOL::SkipToNextChunk ( FILEHANDLE f, CHUNK3DS *Chunk )
  {
    File.Seek ( f, Chunk->NextOffset-6, FROM_CURRENT );  
  } 

BOOLEAN G3D3DSTOOL::Read3DSChunk ( FILEHANDLE f, CHUNK3DS *Chunk )
  {
    BOOLEAN Result;
    Result = File.Read ( f, &(Chunk->ID), 2  );  
    if (Result==FAILURE)
      return FAILURE;

    Result = File.Read ( f, &(Chunk->NextOffset), 4  );      
    #if defined (__MSBFIRST__)
      SwapWord ( ((WORD*)&(Chunk->ID)) );
      SwapDWord ( ((WORD*)&(Chunk->NextOffset)) );
    #endif

    return Result;  
  } 

BOOLEAN G3D3DSTOOL::Read3DSTriVertices ( FILEHANDLE f, G3DPOLYHEDRON* Object,
                                         LONG Fp )
  {
    SHORT NumVertices;

    File.Read ( f, &NumVertices, 2 );
    #if defined (__MSBFIRST__)
       SwapWord ( ((WORD*)&NumVertices) );
    #endif

    Object->CreateGeometryPoints ( NumVertices );
    GEOMETRYDATA *GeometryData;
    GeometryData = Object->GetGeometryData ();

    LONG i;
    float x,y,z;
    
    for (i=0;i<NumVertices;i++)
      {
        File.Read ( f, &x, 4 );  
        File.Read ( f, &y, 4 );  
        File.Read ( f, &z, 4 );

        #if defined (__MSBFIRST__)
          SwapDWord ( (DWORD*)(&x) );
          SwapDWord ( (DWORD*)(&y) );
          SwapDWord ( (DWORD*)(&z) );
        #endif

        GeometryData->FLPLocalPoints[i].x = (float)(x*AdjustScale);
        GeometryData->FLPLocalPoints[i].y = (float)(z*AdjustScale);
        GeometryData->FLPLocalPoints[i].z = (float)(y*AdjustScale);
      } 
    
    LONG CurP = File.GetPos ( f );
    if (CurP!=Fp)
      File.Seek ( f, Fp, FROM_BEGIN );
      
    return SUCCESS;  
  } 
                                        
BOOLEAN G3D3DSTOOL::Read3DSTriFaceList ( FILEHANDLE f, G3DPOLYHEDRON* Object,
                                         LONG Fp )
  {
    SHORT NumFaces;

    File.Read ( f, &NumFaces, 2 );

    #if defined (__MSBFIRST__)
      SwapWord ( ((WORD*)&NumFaces) );
    #endif

    Object->CreateGeometryPolys ( NumFaces );
    GEOMETRYDATA *GeometryData;
    GeometryData = Object->GetGeometryData ();

    LONG i;
    SHORT List0,List1,List2,Flags;
    
    for (i=0;i<NumFaces;i++)
      {
        File.Read ( f, &List0, 2 );  
        File.Read ( f, &List1, 2 );  
        File.Read ( f, &List2, 2 );
        File.Read ( f, &Flags, 2 );

        #if defined (__MSBFIRST__)
          SwapWord ( ((WORD*)&List0) );
          SwapWord ( ((WORD*)&List1) );
          SwapWord ( ((WORD*)&List2) );
          SwapWord ( ((WORD*)&Flags) );
        #endif

        
        GeometryData->Polys[i].Points[0] = List0;
        GeometryData->Polys[i].Points[1] = List1;
        GeometryData->Polys[i].Points[2] = List2;
        if (Flags)
          {}
      } 
    
    LONG CurP = File.GetPos ( f );
    if (CurP!=Fp)
      File.Seek ( f, Fp, FROM_BEGIN );
      
    return SUCCESS;  
  } 

BOOLEAN G3D3DSTOOL::Read3DSTriMapList ( FILEHANDLE f, G3DPOLYHEDRON* Object,
                                        LONG Fp )
  {
    SHORT NumVertices;

    File.Read ( f, &NumVertices, 2 );

    #if defined (__MSBFIRST__)
      SwapWord ( ((WORD*)&NumVertices) );
    #endif

    GEOMETRYDATA *GeometryData;
    GeometryData = Object->GetGeometryData ();
    if (GeometryData)
      {}

    LONG i;
    float u,v;

    u_list = new float[NumVertices];
    v_list = new float[NumVertices];

    double Val,IntPart;
    
    for (i=0;i<NumVertices;i++)
      {
        File.Read ( f, &u, 4 );  
        File.Read ( f, &v, 4 );  

        #if defined (__MSBFIRST__)
          SwapDWord ( (DWORD*)(&u) );
          SwapDWord ( (DWORD*)(&v) );
        #endif

        Val = u;        
        if (u<0)
          u = -u;
        if (u>1.0)
          u = (float)modf ( Val, &IntPart );
        u_list[i] = u*255*65536;
          
        Val = v;
        if (v<0)
          v = -v;
        if (v>1.0)
          v = (float)modf ( Val, &IntPart );  
        v_list[i] = v*255*65536;
      } 

    Object->TextureMapped = TRUE;
    LONG CurP = File.GetPos ( f );
    if (CurP!=Fp)
      File.Seek ( f, Fp, FROM_BEGIN );
      
    return SUCCESS;  
  } 
                                     
BOOLEAN G3D3DSTOOL::Read3DSTriObjectChunks ( FILEHANDLE f, G3DPOLYHEDRON* Object,
                                          LONG Fp )
  {
    CHUNK3DS Chunk;
    LONG CurP;

    if (u_list!=NULL)
      delete u_list;
    u_list = NULL;
    if (v_list!=NULL)
      delete v_list;
    v_list = NULL;
      
    if (Object)
      {}
    CurP = File.GetPos ( f );

    while (CurP<Fp)
      {
        if (Read3DSChunk( f, &Chunk )==FAILURE)
          return FAILURE;

        switch (Chunk.ID)
          {
            case CHUNK_VERTICES :
              if ( Read3DSTriVertices ( f, Object, CurP+Chunk.NextOffset )==FAILURE)
                return FAILURE;
              break;  
            case CHUNK_FACELIST :
              if ( Read3DSTriFaceList ( f, Object, CurP+Chunk.NextOffset )==FAILURE)
                return FAILURE;
              break;  
            case CHUNK_MAPLIST :
              if ( Read3DSTriMapList ( f, Object, CurP+Chunk.NextOffset )==FAILURE)
                return FAILURE;
              break;  
            case CHUNK_FACE_MAT :
              SkipToNextChunk ( f, &Chunk );
              break;  
            case CHUNK_SMOOLIST :
              SkipToNextChunk ( f, &Chunk );
              break;
            case CHUNK_TRMATRIX :
              SkipToNextChunk ( f, &Chunk );
              break;
            default :
              SkipToNextChunk ( f, &Chunk );
              break;  
          } 
        CurP = File.GetPos ( f );
      } 

    GEOMETRYDATA *GeometryData;
    GeometryData = Object->GetGeometryData ();

    LONG i; 
    if ((u_list!=NULL)&&(v_list!=NULL))
      {
        for (i=0;i<GeometryData->NumPolys;i++)
          {
            LONG List0,List1,List2;
            List0 = GeometryData->Polys[i].Points[0];  
            List1 = GeometryData->Polys[i].Points[1];  
            List2 = GeometryData->Polys[i].Points[2];  
            GeometryData->Polys[i].TexCoords[0].x = u_list[List0];
            GeometryData->Polys[i].TexCoords[0].y = v_list[List0];
            GeometryData->Polys[i].TexCoords[1].x = u_list[List1];
            GeometryData->Polys[i].TexCoords[1].y = v_list[List1];
            GeometryData->Polys[i].TexCoords[2].x = u_list[List2];
            GeometryData->Polys[i].TexCoords[2].y = v_list[List2];
          } 
      } 
    else
      {
        for (i=0;i<GeometryData->NumPolys;i++)
          {
            GeometryData->Polys[i].TexCoords[0].x = (float)0;
            GeometryData->Polys[i].TexCoords[0].y = (float)0;
            GeometryData->Polys[i].TexCoords[1].x = (float)0;
            GeometryData->Polys[i].TexCoords[1].y = (float)0;
            GeometryData->Polys[i].TexCoords[2].x = (float)0;
            GeometryData->Polys[i].TexCoords[2].y = (float)0;
          } 
      } 
          
    if (u_list!=NULL)
      delete u_list;
    u_list = NULL;
    if (v_list!=NULL)
      delete v_list;
    v_list = NULL;
      
    return SUCCESS;  
  } 
                                         
BOOLEAN G3D3DSTOOL::Read3DSObjectChunks ( FILEHANDLE f, LINKEDLIST<G3DOBJECT*> *ObjectList,
                                       LONG Fp )
  {
    CHUNK3DS Chunk;
    LONG CurP;

    CHAR Name[128];
    INT i;

    i = 0;
    
    Name[i] = (CHAR)File.GetCh ( f );
    while (Name[i++]!=NULL)
      {
        Name[i] = (CHAR)File.GetCh ( f );
      } 
    
    CurP = File.GetPos ( f );

    while (CurP<Fp)
      {
        if (Read3DSChunk( f, &Chunk )==FAILURE)
          return FAILURE;

        switch (Chunk.ID)
          {
            case CHUNK_TRIANGLE :
              G3DPOLYHEDRON *PolyObject;
              PolyObject = new G3DPOLYHEDRON ();
              PolyObject->SetName ( Name );
              PolyObject->ClockWised = Clockwise;
              if ( Read3DSTriObjectChunks ( f, PolyObject, CurP+Chunk.NextOffset )==FAILURE)
                return FAILURE;
              ObjectList->AddObject( PolyObject );  
              break;  
            case CHUNK_LIGHT :
              SkipToNextChunk ( f, &Chunk );
              break;  
            case CHUNK_CAMERA :
              SkipToNextChunk ( f, &Chunk );
              break;  
          } 
        CurP = File.GetPos ( f );
      } 
    return SUCCESS;  
  } 
                                         
BOOLEAN G3D3DSTOOL::Read3DSMeshChunks ( FILEHANDLE f, LINKEDLIST<G3DOBJECT*> *ObjectList,
                                        LONG Fp )
  {
    CHUNK3DS Chunk;
    LONG CurP;

    CurP = File.GetPos ( f );

    while (CurP<Fp)
      {
        if (Read3DSChunk( f, &Chunk )==FAILURE)
          return FAILURE;

        switch (Chunk.ID)
          {
            case CHUNK_BGCOLOR :
              SkipToNextChunk ( f, &Chunk );
              break;  
            case CHUNK_AMBIENTCOLOR :
              SkipToNextChunk ( f, &Chunk );
              break;  
            case CHUNK_OBJECTBLOCK :
              if ( Read3DSObjectChunks ( f, ObjectList, CurP+Chunk.NextOffset )==FAILURE)
                return FAILURE;
              break;
            default :  
              SkipToNextChunk ( f, &Chunk );
              break;
          } 
        if (ObjectList)
          {}
        CurP = File.GetPos ( f );
      } 
    return SUCCESS;  
  } 
                                     
BOOLEAN G3D3DSTOOL::Read3DSMainChunks ( FILEHANDLE f, LINKEDLIST<G3DOBJECT*> *ObjectList,
                                     LONG Fp )
  {
    CHUNK3DS Chunk;
    LONG CurP;

    CurP = File.GetPos ( f );

    while (CurP<Fp)
      {
        if (Read3DSChunk( f, &Chunk )==FAILURE)
          return FAILURE;

        switch (Chunk.ID)
          {
            case CHUNK_OBJECT_MESH : 
              if ( Read3DSMeshChunks ( f, ObjectList, CurP+Chunk.NextOffset )==FAILURE)
                return FAILURE;
              break;
            case CHUNK_KEYFRAMER :
              SkipToNextChunk ( f, &Chunk );
              break;
            default :
              SkipToNextChunk ( f, &Chunk );
              break;  
          } 
          
        CurP = File.GetPos ( f );
      } 
    return SUCCESS;    
  } 
                                     
BOOLEAN G3D3DSTOOL::Load ( FILEHANDLE f, LINKEDLIST<G3DOBJECT*> *ObjectList,
                           BOOLEAN ClockWise, double Scale )
  {
    CHUNK3DS Chunk;

    Clockwise = ClockWise;
    AdjustScale = Scale;

    if (Read3DSChunk(f,&Chunk)==FAILURE)
      {
        return FAILURE;  
      } 

    if (Chunk.ID!=CHUNK_PRIMARY)
      {
        return FAILURE;  
      } 

    LONG Size;

    Size = File.GetSize ( f );
    if (Read3DSMainChunks(f,ObjectList,Size)==FAILURE)
      {
        return FAILURE;  
      } 
      
    return SUCCESS;  
  } 

BOOLEAN G3D3DSTOOL::Save ( FILEHANDLE f, LINKEDLIST<G3DOBJECT*> *ObjectList,
                           BOOLEAN ClockWise, double Scale )
  {
    if (f)
      {}
    if (ObjectList)
      {}
    if (ClockWise)
      {}
    if (Scale)
      {}
    return FAILURE;          
  } 


