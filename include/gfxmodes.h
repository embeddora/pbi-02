




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

