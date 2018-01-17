




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


