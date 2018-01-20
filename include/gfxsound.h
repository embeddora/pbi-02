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
