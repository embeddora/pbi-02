




#ifndef __GFXSOUND_H__
#define __GFXSOUND_H__

#include "stdgfx.h"
#include "gfxsddrv.h"

struct MIDICHUNK
  {
    CHAR Name[4];
    LONG Length;
  }; 

struct MIDIHEADER
  {
    MIDICHUNK Chunk;
    SHORT Type;
    SHORT NumTracks;
    SHORT Time;
  }; 

struct MIDIEVENTTYPE
  {
    LONG DeltaTime;
  }; 

struct MIDITRACK
  {
    MIDICHUNK Chunk;
    SHORT NumEvents;
    MIDIEVENTTYPE *Events;
  }; 

class SOUND : public MYOBJECT
  {
    protected :
      VOID ReverseWORD ( SHORT *Word );
      VOID ReverseDWORD ( DWORD *DWord );

      DWORD ReadMIDIDeltaTime ( FILEHANDLE f );
      BOOLEAN ReadMIDIChunk ( FILEHANDLE f, MIDICHUNK *Chunk );
      BOOLEAN ReadMIDIHeader ( FILEHANDLE f, MIDIHEADER *Header );
      BOOLEAN ReadMIDIEvent ( FILEHANDLE f, MIDIEVENTTYPE *Event );
      BOOLEAN ReadMIDITrack ( FILEHANDLE f, MIDITRACK *Track );

    public :
      SOUND ();
      virtual ~SOUND ();

      BOOLEAN LoadMIDI ( STRING FileName );
  }; 

extern SOUND Sound;

#endif
