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



#include "stdgfx.h"
#include "gfxsound.h"

SOUND Sound;

SOUND::SOUND ()
  {
  } 

SOUND::~SOUND ()
  {
  } 

VOID SOUND::ReverseWORD ( SHORT *Word )
  {
    BYTE Hi,Lo;
    Hi = (BYTE)((*Word)>>8);
    Lo = (BYTE)((*Word)&0xFF);
    *Word = Lo;
    (*Word) <<= 8;
    *Word = Hi;
  } 

VOID SOUND::ReverseDWORD ( DWORD *DWord )
  {
         SHORT Hi,Lo;
         Hi = (SHORT)((*DWord)>>16);
         Lo = (SHORT)((*DWord)&0xFFFF);
         ReverseWORD ( &Hi );
         ReverseWORD ( &Lo );
         *DWord = Lo;
         (*DWord) <<= 16;
         *DWord = Hi;
  } 

BOOLEAN SOUND::ReadMIDIChunk ( FILEHANDLE f, MIDICHUNK *Chunk )
  {
         if (File.Read ( f, Chunk, sizeof ( MIDICHUNK ) )==FAILURE)
                return FAILURE;
         ReverseDWORD ( (DWORD*)&(Chunk->Length ) );

         return TRUE;
  } 

BOOLEAN SOUND::ReadMIDIHeader ( FILEHANDLE f, MIDIHEADER *Header )
  {
         if (ReadMIDIChunk ( f, &(Header->Chunk) )==FAILURE)
                return FAILURE;

         if (strncmp ( Header->Chunk.Name, "MThd", 4 )!=0)
                return FAILURE;

         
         SHORT Type;
         File.Read ( f, &Type, sizeof (SHORT ) );
         ReverseWORD ( &Type );
         Header->Type = Type;

         
         SHORT NumTracks;
         File.Read ( f, &NumTracks, sizeof (SHORT ) );
         ReverseWORD ( &NumTracks );
         Header->NumTracks = NumTracks;

         SHORT Time;
         File.Read ( f, &Time, sizeof (SHORT ) );
         ReverseWORD ( &Time );
         Header->Time = Time;

         return TRUE;
  } 

BOOLEAN SOUND::ReadMIDITrack ( FILEHANDLE f, MIDITRACK *Track )
  {
         if (ReadMIDIChunk ( f, &(Track->Chunk) )==FAILURE)
                return FAILURE;

         if (strncmp ( Track->Chunk.Name, "MTrk", 4 )!=0)
                return FAILURE;

         if (File.Read ( f, &(Track->NumEvents), sizeof(SHORT) )==FAILURE)
                return FAILURE;
         ReverseWORD ( &(Track->NumEvents) );

         LONG i;
         Track->Events = new MIDIEVENTTYPE [Track->NumEvents];
         if (Track->Events==NULL)
                return FAILURE;

         for (i=0;i<Track->NumEvents;i++)
                {
                  if (ReadMIDIEvent ( f, &(Track->Events[i]) )==FAILURE)
          return FAILURE;
                } 
         return TRUE;
  } 

BOOLEAN SOUND::ReadMIDIEvent ( FILEHANDLE f, MIDIEVENTTYPE *Event )
  {
    Event->DeltaTime = ReadMIDIDeltaTime ( f );
    return TRUE;
  } 

DWORD SOUND::ReadMIDIDeltaTime ( FILEHANDLE f )
  {
    BYTE Byte1,Byte2,Byte3,Byte4;

    Byte1 = Byte2 = Byte3 = Byte4 = 0;

    File.Read ( f, &Byte1, sizeof(BYTE) );
    if (Byte1&0x80)
                {
                  File.Read ( f, &Byte2, sizeof(BYTE) );
                  if (Byte2&0x80)
                         {
                                File.Read ( f, &Byte3, sizeof(BYTE) );
                                if (Byte3&0x80)
                                  {
                                         File.Read ( f, &Byte4, sizeof(BYTE) );
                                  } 
                         } 
                } 

    DWORD Time;
         Time = ((LONG)Byte1<<24)+((LONG)Byte2<<16)+((LONG)Byte3<<8)
                                +(LONG)Byte4;

         ReverseDWORD ( &Time );
    return Time;
  } 

BOOLEAN SOUND::LoadMIDI ( STRING FileName )
  {
         FILEHANDLE f;

         f = File.Open ( FileName, OPEN_READ | OPEN_BINARY );
         if (f==NULL)
                return FAILURE;

         MIDIHEADER Header;

         if (ReadMIDIHeader ( f, &Header )==FAILURE)
                {
                  File.Close ( f );
                  return FAILURE;
                } 

         LONG i;
         MIDITRACK *Tracks;
         Tracks = new MIDITRACK [Header.NumTracks];
         if (Tracks==NULL)
                {
                  File.Close ( f );
                  return FAILURE;
                } 

         for (i=0;i<Header.NumTracks;i++)
                {
                  if (ReadMIDITrack ( f, &Tracks[i] )==FAILURE)
                         {
                                File.Close ( f );
            return FAILURE;
                         } 
                } 

         File.Close ( f );
         return TRUE;
  } 

