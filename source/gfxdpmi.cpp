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
#include "gfxdpmi.h"

DPMITOOL DPMI;

DPMITOOL::DPMITOOL () : SYSTEMTOOL ()
  {
    RmRegs = new RMREGS;  
  } 

DPMITOOL::~DPMITOOL ()
  {
    delete RmRegs;  
        RmRegs = NULL;
  } 

VOID DPMITOOL::InitRmRegs ()
  {
    memset ( RmRegs, 0, 50 );
  } 

VOID DPMITOOL::SetRmRegs ( union REGS *InRegs )
  {
    struct SREGS SRegs;

    #if defined (__FORWINDOWS__)
      if (SRegs.es)
        {}
      if (InRegs)
        {}
    #elif defined (__FOROS2__)
      if (SRegs.es)
        {}
      if (InRegs)
        {}
    #elif defined (__FORDOS__)
      segread ( &SRegs );
      InitRmRegs ();
      SetRmRegs ( InRegs, &SRegs );
    #endif
  } 

VOID DPMITOOL::SetRmRegs ( union REGS *InRegs, struct SREGS *SRegs )
  {
    InitRmRegs ();

    RmRegs->ES = SRegs->es;
    RmRegs->DS = SRegs->ds;
    RmRegs->CS = SRegs->cs;
    RmRegs->SS = SRegs->ss;

    if (InRegs)
     {}
    if (SRegs)
     {}
            
    #if defined (__FORWINDOWS__)
    #elif defined (__FOROS2__)
    #elif defined (__FORDOS__)
      #ifdef __32BIT__
        RmRegs->EDI = InRegs->x.edi;
        RmRegs->ESI = InRegs->x.esi;
        RmRegs->EBX = InRegs->x.ebx;
        RmRegs->EDX = InRegs->x.edx;
        RmRegs->ECX = InRegs->x.ecx;
        RmRegs->EAX = InRegs->x.eax;
        segread ( &SegRegs );
        #if defined (__FORDOSPWPK32__)
           Regs.x.edi = (DWORD)RmRegs;
        #else
           SegRegs.es = FP_SEG ( RmRegs );
           Regs.x.edi = FP_OFF ( RmRegs );
        #endif
      #else
        RmRegs->EDI = InRegs->x.di;
        RmRegs->ESI = InRegs->x.si;
        RmRegs->EBX = InRegs->x.bx;
        RmRegs->EDX = InRegs->x.dx;
        RmRegs->ECX = InRegs->x.cx;
        RmRegs->EAX = InRegs->x.ax;
        SegRegs.es = (WORD)FP_SEG ( RmRegs );
        Regs.x.di = (WORD)FP_OFF ( RmRegs );
      #endif
    #endif
  } 

VOID DPMITOOL::GetRmRegs ( union REGS *OutRegs )
  {
    GetRmRegs ( OutRegs, NULL );
  } 

VOID DPMITOOL::GetRmRegs ( union REGS *OutRegs, struct SREGS *SRegs )
  {
    if (SRegs!=NULL)
      {
        SRegs->es = RmRegs->ES;
        SRegs->ds = RmRegs->DS;
        SRegs->cs = RmRegs->CS;
        SRegs->ss = RmRegs->SS;
      } 

    if (OutRegs)
      {}
    #if defined (__FORWINDOWS__)
    #elif defined (__FOROS2__)
    #elif defined (__FORDOS__)
      #ifdef __32BIT__
        OutRegs->x.edi = RmRegs->EDI;
        OutRegs->x.esi = RmRegs->ESI;
        OutRegs->x.ebx = RmRegs->EBX;
        OutRegs->x.edx = RmRegs->EDX;
        OutRegs->x.ecx = RmRegs->ECX;
        OutRegs->x.eax = RmRegs->EAX;
      #else
        OutRegs->x.di = RmRegs->EDI;
        OutRegs->x.si = RmRegs->ESI;
        OutRegs->x.bx = RmRegs->EBX;
        OutRegs->x.dx = RmRegs->EDX;
        OutRegs->x.cx = RmRegs->ECX;
        OutRegs->x.ax = RmRegs->EAX;
      #endif  
    #endif
  } 

VOID DPMITOOL::Int86 ( INT IntNo, union REGS *InRegs, union REGS *OutRegs )
  {
    SetRmRegs ( InRegs );
    if (InRegs)
      {}
    if (IntNo)
      {}
    #if defined (__FORWINDOWS__)
    #elif defined (__FOROS2__)
    #elif defined (__FORDOS__)
      Regs16.ax = 0x0300;
      Regs16.bx = (WORD)IntNo;
      Regs.h.bh = 0;
      Regs16.cx = 25;
      Regs16.dx = 0;
      INTERRUPTx ( INT_DPMI, &Regs, &Regs, &SegRegs );
    #endif
            
    GetRmRegs ( OutRegs );
  } 

VOID DPMITOOL::Int86x ( INT IntNo, union REGS *InRegs, union REGS *OutRegs, struct SREGS* SRegs )
  {
    SetRmRegs ( InRegs, SRegs );
    if (IntNo)
      {}
    #if defined (__FORWINDOWS__)
    #elif defined (__FOROS2__)
    #elif defined (__FORDOS__)
      Regs16.ax = 0x0300;
      Regs16.bx = (WORD)IntNo;
      Regs.h.bh = 0;
      Regs16.cx = 25;
      Regs16.dx = 0;
      INTERRUPTx ( INT_DPMI, &Regs, &Regs, &SegRegs );
    #endif
    GetRmRegs ( OutRegs );
  } 

VOID DPMITOOL::AllocateDOSMemory ( INT NumPara, WORD* Selector, WORD* Segment )
  {
    #if defined (__FORWINDOWS__)
      if(NumPara)
        {}
    #elif defined (__FOROS2__)
      if(NumPara)
        {}
    #elif defined (__FORDOS__)
      Regs16.ax = 0x0100;
      Regs16.bx = (WORD)NumPara;
      INTERRUPT ( INT_DPMI, &Regs, &Regs );
      *Selector = Regs16.dx;
      *Segment = Regs16.ax;
    #endif
    if (Selector)
      {}
    if (Segment)
      {}
    if (NumPara)
      {}
  } 

VOID DPMITOOL::FreeDOSMemory ( WORD Selector )
  {
    if (Selector)
      {}
    #if defined (__FORWINDOWS__)
    #elif defined (__FOROS2__)
    #elif defined (__FORDOS__)
       Regs16.ax = 0x0100;
       Regs16.dx = Selector;
       INTERRUPT ( INT_DPMI, &Regs, &Regs );
    #endif
  } 

VOID* DPMITOOL::MapRealToLinear ( DWORD Pointer )
  {
    WORD Seg,Off;
    Seg = (WORD)(Pointer>>12);
    Off = (WORD)(Pointer&0xFFFF);
    return (VOID*)(Seg+Off);
  } 

WORD DPMITOOL::AllocateSelector ( INT NumSelector )
  {
    if (NumSelector)
      {}
    #if defined (__FORWINDOWS__)
      return 0;
    #elif defined (__FOROS2__)
      return 0;
    #elif defined (__FORDOS__)
      Regs16.ax = 0x0000;
      Regs16.cx = (WORD)NumSelector;
      INTERRUPT ( INT_DPMI, &Regs, &Regs );
      if ( Regs16.cflag )
        return 0;
      return Regs16.ax;
    #endif
  } 

VOID DPMITOOL::FreeSelector ( WORD Selector )
  {
    if (Selector)
      {}
    #if defined (__FORWINDOWS__)
    #elif defined (__FOROS2__)
    #elif defined (__FORDOS__)
      Regs16.ax = 0x0001;
      Regs16.bx = Selector;
      INTERRUPT ( INT_DPMI, &Regs, &Regs );
    #endif
  } 

BOOLEAN DPMITOOL::SetSelectorAddress ( WORD Selector, DWORD Address )
  {
    if (Selector)
      {}
    if (Address)
      {}
    #if defined (__FORDOS__)
      Regs16.ax = 0x0007;
      Regs16.bx = Selector;
      Regs16.cx = (WORD)(Address>>16);
      Regs16.dx = (WORD)(Address&0xFFFF);
      INTERRUPT ( INT_DPMI, &Regs, &Regs );
      if ( Regs16.cflag )
        return 0;
      return Regs16.ax;
    #else
      return 0;  
    #endif
  } 

BOOLEAN DPMITOOL::SetSelectorLimit ( WORD Selector, DWORD Limit )
  {
    if (Selector)
      {}
    if (Limit)
      {}
    #if defined (__FORDOS__)
      Regs16.ax = 0x0008;
      Regs16.bx = Selector;
      Regs16.cx = (WORD)(Limit>>16);
      Regs16.dx = (WORD)(Limit&0xFFFF);
      INTERRUPT ( INT_DPMI, &Regs, &Regs );
      if ( Regs16.cflag )
        return 0;
      return Regs16.ax;
    #else
      return 0;  
    #endif
  } 



