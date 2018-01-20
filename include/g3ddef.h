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

