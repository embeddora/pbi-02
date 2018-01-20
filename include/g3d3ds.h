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




#ifndef __G3D3DS_H__
#define __G3D3DS_H__

#include "stdgfx.h"
#include "g3dfile.h"


#define CHUNK_PRIMARY          0x4D4D   
  #define CHUNK_RGBF           0x0010   
  #define CHUNK_RGBB           0x0011   
  
  #define CHUNK_OBJECT_MESH    0x3D3D   
    
    #define CHUNK_BGCOLOR      0x1200   
    #define CHUNK_AMBIENTCOLOR 0x2100   
    #define CHUNK_OBJECTBLOCK  0x4000   
      
      #define CHUNK_TRIANGLE   0x4100   
        
        #define CHUNK_VERTICES 0x4110   
        #define CHUNK_FACELIST 0x4120   
        #define CHUNK_FACE_MAT 0x4130   
        #define CHUNK_MAPLIST  0x4140   
        #define CHUNK_SMOOLIST 0x4150   
        #define CHUNK_TRMATRIX 0x4160   
        
      #define CHUNK_LIGHT      0x4600   
        
        #define CHUNK_SPOTLIGHT 0x4610  
              
      #define CHUNK_CAMERA     0x4700   
      
  #define CHUNK_MATERIAL       0xAFFF   
    
    #define CHUNK_MATNAME      0xA000   
    #define CHUNK_AMBIENT      0xA010   
    #define CHUNK_DIFFUSE      0xA020   
    #define CHUNK_SPECULAR     0xA030   
    #define CHUNK_TEXTURE      0xA200   
    #define CHUNK_BUMPMAP      0xA230   
    #define CHUNK_MAPFILE      0xA300   
     
  #define CHUNK_KEYFRAMER      0xB000   
    
    #define CHUNK_FRAMES       0xB008   

struct CHUNK3DS
  {
    SHORT ID;
    LONG  NextOffset;  
  }; 

class G3D3DSTOOL : public G3DFILETOOL
  {
    protected :
      float *u_list;
      float *v_list;
    
      
      VOID SkipToNextChunk ( FILEHANDLE f, CHUNK3DS *Chunk );
      BOOLEAN Read3DSChunk ( FILEHANDLE f, CHUNK3DS *Chunk );
      BOOLEAN Read3DSTriVertices ( FILEHANDLE f, G3DPOLYHEDRON* Object,
                                   LONG Fp );
      BOOLEAN Read3DSTriFaceList ( FILEHANDLE f, G3DPOLYHEDRON* Object,
                                   LONG Fp );
      BOOLEAN Read3DSTriMapList ( FILEHANDLE f, G3DPOLYHEDRON* Object,
                                  LONG Fp );
      BOOLEAN Read3DSTriObjectChunks ( FILEHANDLE f, G3DPOLYHEDRON* Object,
                                       LONG Fp );
      BOOLEAN Read3DSObjectChunks ( FILEHANDLE f, LINKEDLIST<G3DOBJECT*> *ObjectList,
                                    LONG Fp );
      BOOLEAN Read3DSMeshChunks ( FILEHANDLE f, LINKEDLIST<G3DOBJECT*> *ObjectList, LONG Fp );
      BOOLEAN Read3DSMainChunks ( FILEHANDLE f, LINKEDLIST<G3DOBJECT*> *ObjectList, LONG Fp );
      
    public :
      G3D3DSTOOL ();
      virtual ~G3D3DSTOOL ();      
      
      virtual BOOLEAN Load ( FILEHANDLE f, LINKEDLIST<G3DOBJECT*> *ObjectList,
                             BOOLEAN ClockWise, double Scale );     
      virtual BOOLEAN Save ( FILEHANDLE f, LINKEDLIST<G3DOBJECT*> *ObjectList,
                             BOOLEAN ClockWise, double Scale );
  }; 

#endif


