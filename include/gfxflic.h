




#ifndef __GFXFLICPLAY_H__
#define __GFXFLICPLAY_H__

#include "stdgfx.h"
#include "gfximage.h"
#include "gfxfiles.h"
#include "gfxcltbl.h"

#define ID_FLI 0xAF11
#define ID_FLC 0xAF12

struct FLIHEADER
  {
    DWORD Size;
    SHORT ID;
    SHORT NumFrames;
    SHORT Width;
    SHORT Height;
    SHORT Reserved1;
    SHORT Flag;
    SHORT Speed;
    LONG  Next;
    LONG  Frit;
    BYTE  Resrved2[102];
  }; 

struct FLCHEADER
  {
    DWORD Size;
    SHORT ID;
    SHORT NumFrames;
    SHORT Width;
    SHORT Height;
    SHORT Bits;
    SHORT Flag;
    LONG  Speed;
    SHORT Unused;
    LONG  Created;
    LONG  Creator;
    LONG  Updated;
    LONG  Updator;
    SHORT AspectX;
    SHORT AspectY;
    BYTE  Reserved1[38];
    DWORD Offset1;
    DWORD Offset2;
    BYTE  Reserved[40];
  }; 

struct FRAMEHEADER
  {
    DWORD Size;
    SHORT ID;
    SHORT NumChunks;
    BYTE  Reserved[8];
  }; 

struct CHUNKHEADER
  {
    DWORD Size;
    SHORT Type;
  }; 


#define FLI_COLOR_256  4  
#define FLI_WORD_LC    7  
#define FLI_COLOR     11
#define FLI_LC        12
#define FLI_BLACK     13
#define FLI_BRUN      15
#define FLI_COPY      16
#define FLI_PREVIEW   18  

struct CHUNK_DATA
  {
    CHUNKHEADER Header;
    BYTE *Data;
  }; 

struct FRAME_DATA
  {
    SHORT NumChunks;
    CHUNK_DATA *Chunks;
  }; 

union FLICHEADER
  {
    FLIHEADER FLI;
    FLCHEADER FLC;
  }; 

#define FLIC_LOOP_INFINITE -1
#define FLIC_SPEED_DEFAULT -1
   
class FLICFILE : public MYOBJECT
  {
    protected :    
      FLICHEADER FLICHeader;
      BOOLEAN    IsFLC; 
      FRAME_DATA *FrameData;
      FILEHANDLE f;
      LONG CurrentFrame;
      IMAGE *Image;
      RGBPALETTE *Pal;
      BOOLEAN PalChange;
      HDISPLAY hDisplay;
      LONG DefaultSpeed;
      
      BOOLEAN ReadHeader ();
      BOOLEAN ReadChunk ( CHUNK_DATA *Chunk );
      BOOLEAN ReadFrame ( LONG WhichFrame );
      VOID DestroyFrames ();
      
      BOOLEAN DoColor ( RGBPALETTE *Pal, CHUNK_DATA* Chunk, BOOLEAN Is256 );
      BOOLEAN DoLine ( IMAGE *Image, CHUNK_DATA* Chunk );
      BOOLEAN DoLineWord ( IMAGE *Image, CHUNK_DATA* Chunk );
      BOOLEAN DoBRun ( IMAGE *Image, CHUNK_DATA* Chunk );
      BOOLEAN DoCopy ( IMAGE *Image, CHUNK_DATA* Chunk );
      BOOLEAN DoBlack ( IMAGE *Image, CHUNK_DATA* Chunk );
      BOOLEAN DoPreview ( IMAGE *Image, CHUNK_DATA* Chunk );
      
      VOID ConvertColor ( CHUNK_DATA* Chunk, BOOLEAN Is256, COLORTABLE *Table );
      VOID ConvertLine ( CHUNK_DATA* Chunk, COLORTABLE *Table );
      VOID ConvertLineWord ( CHUNK_DATA* Chunk, COLORTABLE *Table );
      VOID ConvertBRun ( CHUNK_DATA* Chunk, COLORTABLE *Table );
      VOID ConvertCopy ( CHUNK_DATA* Chunk, COLORTABLE *Table );
      VOID ConvertBlack ( CHUNK_DATA* Chunk, COLORTABLE *Table );
      VOID ConvertPreview ( CHUNK_DATA* Chunk, COLORTABLE *Table );
      
    public :
      LONG Loop;
      LONG Speed;
      
      FLICFILE ();
      virtual ~FLICFILE ();

      BOOLEAN Load ( STRING FileName );
      
      IMAGE* GetImage () { return Image; };
      RGBPALETTE *GetPalette () { return Pal; };
      VOID GetFirstPalette ();
      HDISPLAY GetDisplay () { return hDisplay; };
      BOOLEAN  SetDisplay ( HDISPLAY hDisplay );
      BOOLEAN CreateImage ();
      
      VOID SetFrame ( LONG FrameNumber );
      LONG GetFrame () { return CurrentFrame; };

      LONG GetNumFrames () { return FLICHeader.FLI.NumFrames; };
      LONG GetDefaultSpeed () { return DefaultSpeed; };
      
      BOOLEAN IsPalChanged () { return PalChange; };
      VOID PalProcessed () { PalChange = FALSE; };
      LONG GetImageWidth () { return FLICHeader.FLI.Width; };
      LONG GetImageHeight () { return FLICHeader.FLI.Height; };
      BOOLEAN PlayFrame ( BOOLEAN Advance );
      BOOLEAN IsFLCFile () { return IsFLC; };

      VOID Convert ( RGBPALETTE *NewPal );
  }; 

#endif
