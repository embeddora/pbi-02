




#ifndef __G3DDEF_H__
#define __G3DDEF_H__

#include "stdgfx.h"
#include "mpgfx.h"

#if defined (__FORWATCOM__)
  #define __USEASM__
#elif defined (__FORVISUAL__)
#elif defined (__FORBORLAND__)
#elif defined (__FORGCC__)
#endif

#define FACE_WIREFRAME   0
#define FACE_SOLID       1
#define FACE_TEXTURE     2

#define SHADE_NONE     0
#define SHADE_FLAT     1
#define SHADE_GOURAUD  2

typedef float G3DMATRIX[4][4];  
 
#endif

