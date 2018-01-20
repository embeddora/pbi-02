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



#ifndef __G3DMATH_H__
#define __G3DMATH_H__

#include "stdgfx.h"
#include "g3ddef.h"

struct FLPVECTOR3D 
  {
    float x,y,z,w;
  }; 
  
struct FXPVECTOR3D 
  {
    LONG x,y,z,w;
  }; 
  
struct FLPVECTOR2D 
  {
    float x,y;
  }; 
  
struct FXPVECTOR2D 
  {
    LONG x,y;
  }; 
  
struct FLPPOINT2D
  {
    float x,y;
    float u,v;
    float I;
    float Haze;
    float z;
    float Reserved1; 
  }; 
  
struct FXPPOINT2D
  {
    LONG x,y;
    LONG u,v;
    LONG I;
    LONG Haze;
    LONG z;
    LONG Reserved1; 
  }; 
  
VOID InitMatrix ( G3DMATRIX Matrix );
VOID CopyMatrix ( G3DMATRIX Src, G3DMATRIX Dest );
VOID MultMatrix ( G3DMATRIX Mat1, G3DMATRIX Mat2, G3DMATRIX Dest );
VOID Translate ( G3DMATRIX Matrix, float xt, float yt, float zt );
VOID Scale ( G3DMATRIX Matrix, float ScaleFactor );
VOID RotateXYZ ( G3DMATRIX Matrix, float xa, float ya, float za );
VOID RotateYXZ ( G3DMATRIX Matrix, float xa, float ya, float za );

float FLPVectorDot ( FLPVECTOR3D *V1, FLPVECTOR3D *V2 );
float FLPVectorDistance ( FLPVECTOR3D *P1, FLPVECTOR3D *P2 );
float FLPVectorMagnitude ( FLPVECTOR3D *V1 );
VOID FLPVectorAdd ( FLPVECTOR3D *V1, FLPVECTOR3D *V2, FLPVECTOR3D *Result ); 
VOID FLPVectorSub ( FLPVECTOR3D *V1, FLPVECTOR3D *V2, FLPVECTOR3D *Result ); 
VOID FLPVectorNormalize ( FLPVECTOR3D *V1 );
VOID FLPVectorCross ( FLPVECTOR3D *V1, FLPVECTOR3D *V2, FLPVECTOR3D *Result );

#endif


