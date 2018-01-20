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


#ifndef __GFXMODES_H__
#define __GFXMODES_H__

#include "stdgfx.h"
#include "gfxtypes.h"

#define M80x25x16     0
#define M320x200x256  1
#define M640x400x256  2
#define M640x480x256  3
#define M800x600x256  4
#define M1024x768x256 5

#define SUPPORTED_MODE 6

struct MODEINFO
  {
    LONG ModeNumber;
    LONG Width;
    LONG Height;
    LONG BitSize;
    BOOLEAN IsSVGA;
  }; 

extern MODEINFO ModeTable[SUPPORTED_MODE];

#endif

