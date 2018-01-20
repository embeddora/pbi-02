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



#ifndef __GFXDPMI_H__
#define __GFXDPMI_H__

#include "stdgfx.h"
#include "gfxsys.h"

struct RMREGS
  {
    DWORD EDI;
    DWORD ESI;
    DWORD EBP;
    DWORD Reserved;
    DWORD EBX;
    DWORD EDX;
    DWORD ECX;
    DWORD EAX;
    WORD  Flags;
    WORD  ES;
    WORD  DS;
    WORD  FS;
    WORD  GS;
    WORD  IP;
    WORD  CS;
    WORD  SP;
    WORD  SS;
  }; 

#if defined (__FORWIN32__)
  #if defined (__FORBORLAND__)
     struct BYTEREGS
       {
         BYTE al,ah,bl,bh;
         BYTE cl,ch,dl,dh;
       };

     struct WORDREGS
       {
         USHORT ax,bx,cx,dx;
         USHORT si,di,cflag,flags;
       };

     union REGS
       {
         struct WORDREGS x;
         struct BYTEREGS h;
       };

     struct SREGS
       {
         USHORT es;
         USHORT ss;
         USHORT cs;
         USHORT ds;
       };
  #endif
#if defined (__FORVISUAL__)
     struct BYTEREGS
       {
         BYTE al,ah,bl,bh;
         BYTE cl,ch,dl,dh;
       };

     struct WORDREGS
       {
         USHORT ax,bx,cx,dx;
         USHORT si,di,cflag,flags;
       };

     union REGS
       {
         struct WORDREGS x;
         struct BYTEREGS h;
       };

     struct SREGS
       {
         USHORT es;
         USHORT ss;
         USHORT cs;
         USHORT ds;
       };
  #endif
#elif defined (__FORWIN32S__)
  #if defined (__FORBORLAND__)
     struct BYTEREGS
       {
         BYTE al,ah,bl,bh;
         BYTE cl,ch,dl,dh;
       };

     struct WORDREGS
       {
         USHORT ax,bx,cx,dx;
         USHORT si,di,cflag,flags;
       };

     union REGS
       {
         struct WORDREGS x;
         struct BYTEREGS h;
       };

     struct SREGS
       {
         USHORT es;
         USHORT ss;
         USHORT cs;
         USHORT ds;
       };
  #endif
#if defined (__FORVISUAL__)
     struct BYTEREGS
       {
         BYTE al,ah,bl,bh;
         BYTE cl,ch,dl,dh;
       };

     struct WORDREGS
       {
         USHORT ax,bx,cx,dx;
         USHORT si,di,cflag,flags;
       };

     union REGS
       {
         struct WORDREGS x;
         struct BYTEREGS h;
       };

     struct SREGS
       {
         USHORT es;
         USHORT ss;
         USHORT cs;
         USHORT ds;
       };
  #endif
#elif defined (__FORUNIX__)
  struct BYTEREGS
    {
      BYTE al,ah,bl,bh;
      BYTE cl,ch,dl,dh;
    };

  struct WORDREGS
    {
      USHORT ax,bx,cx,dx;
      USHORT si,di,cflag,flags;
    };

  union REGS
    {
      struct WORDREGS x;
      struct BYTEREGS h;
    };

  struct SREGS
    {
      USHORT es;
      USHORT ss;
      USHORT cs;
      USHORT ds;
    };
#endif

class DPMITOOL : public SYSTEMTOOL
  {
    protected :
      union  REGS  Regs;
      struct SREGS SegRegs;
      RMREGS  *RmRegs;

      void InitRmRegs ();
      void SetRmRegs ( union REGS *InRegs );
      void SetRmRegs ( union REGS *InRegs, struct SREGS *SRegs );
      void GetRmRegs ( union REGS *OutRegs, struct SREGS *SRegs );
      void GetRmRegs ( union REGS *OutRegs );

    public :
      DPMITOOL ();
      virtual ~DPMITOOL ();

      void  AllocateDOSMemory ( INT NumPara, WORD* Selector, WORD* Segment );
      void  FreeDOSMemory ( WORD Selector );
      void  Int86 ( INT IntNo, union REGS *InRegs, union REGS *OutRegs );
      void  Int86x ( INT IntNo, union REGS *InRegs, union REGS *OutRegs, struct SREGS* SRegs );
      void* MapRealToLinear ( DWORD Pointer );
      WORD  AllocateSelector ( INT NumSelector );
      void  FreeSelector ( WORD Selector );
      BOOLEAN SetSelectorAddress ( WORD Selector, DWORD Address );
      BOOLEAN SetSelectorLimit ( WORD Selector, DWORD Limit ); 
  }; 

extern DPMITOOL DPMI;

#endif

