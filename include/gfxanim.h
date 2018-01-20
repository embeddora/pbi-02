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



#ifndef __GFXANIM_H__
#define __GFXANIM_H__

#include "stdgfx.h"
#include "gfximage.h"
#include "gfxllist.h"

#define SKIP_PIXEL 0x8000

struct ANIMFRAME
  {
    LONG NumChunks;
    LONG Size;
    BYTE *Chunks;
  }; 

class ANIMIMAGE : public IMAGE
  {
    protected :
      LONG NumFrames;
      ANIMFRAME *Frames;
      BYTE *BaseImage;
 
      VOID DestroyFrame ( ANIMFRAME *Frame );      
      VOID SetBaseImage ( IMAGE *Image, LONG Sx, LONG Sy );
      
    public :
      ANIMIMAGE ();
      virtual ~ANIMIMAGE ();
      
      virtual VOID Destroy ();

      BOOLEAN CreateBaseImage ();
      VOID DestroyBaseImage ();      

      BOOLEAN CreateFrames ( LONG Num );
      VOID DestroyFrames ();
      
      VOID RecordFrame ( LONG FrameNumber, IMAGE *Image, LONG Sx, LONG Sy );
      LONG GetNumFrames () { return NumFrames; };

      VOID SetFrame ( LONG FrameNumber );
      BOOLEAN Save ( STRING FileName );
      BOOLEAN Load ( STRING FileName );
  }; 

#endif


