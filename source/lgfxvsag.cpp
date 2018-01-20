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
#include "lgfxvsa.h"

LONG ScreenWd;
LONG ScreenHt;
LONG ScreenLen;
LONG BankNumber;

LONG vSrcWd;
LONG vSrcHt;

#ifdef __FORx86__

#define SetBank __asm \
{ \
  __asm push AX \
  __asm push BX \
  __asm push EDX \
  __asm xor  BX, BX \
  __asm mov  EDX, BankNumber \
  __asm mov  AX, 0x4F05 \
  __asm int  0x10 \
  __asm pop  EDX \
  __asm pop  BX \
  __asm pop  AX \
}
#endif




VOID SetVESASelector ( SHORT Selector )
{
  if (Selector)
    {}    
} 





VOID SetVESAData ( LONG SrcWidth, LONG SrcHeight, LONG SrcLen )
{
#ifdef __FORx86__
__asm {
  pushad
  
  mov  EAX, SrcWidth
  mov  EDX, SrcHeight
  mov  EBX, SrcLen
  mov  ScreenWd, EAX
  mov  ScreenHt, EDX
  mov  ScreenLen, EBX

  popad
} 
#endif
} 






VOID VESACopy256To256 ( BYTE *Src, LONG AddSrc, LONG SrcWidth, LONG SrcHeight,
                        LONG DestCx, LONG DestCy )
{
if (Src)
  {}
if (AddSrc)
  {}
if (SrcWidth)
  {}
if (SrcHeight)
  {}
if (DestCx)
  {}
if (DestCy)
  {}
#ifdef __FORx86__
__asm {
  pushad

  mov  EAX, Src
  mov  EDX, AddSrc
  mov  EBX, SrcWidth
  mov  ECX, SrcHeight
    
  mov  ESI, EAX   
  mov  vSrcWd, EBX
  mov  vSrcHt, ECX
  
  sub  EDX, EBX
  mov  EBX, EDX   

  mov  EDI, 0A0000h
  mov  EAX, DestCy
  mov  EDX, ScreenLen
  mul  DX  
  add  EAX, DestCx
  adc  EDX, 0        
  and  EAX, 0FFFFh
  mov  BankNumber, EDX
  
  SetBank 
  
  add  EDI, EAX
  
  mov  EDX, ScreenLen
  sub  EDX, vSrcWd
  cld

  Copy256OutLoop :
    mov ECX, vSrcWd
    mov EAX, ECX
    and EAX, 03h
    shr ECX, 2
    rep movsd
    mov ECX, EAX
    rep movsb
    add ESI, EBX
    add DI, DX
    jnc DoNext
      inc BankNumber
      SetBank 
    
    DoNext :
      dec vSrcHt
    jnz Copy256OutLoop

  popad
} 
#endif
} 






VOID VESAClear256 ( BYTE Color )
{
if (Color)
  {}
#ifdef __FORx86__
__asm {
  pushad
  
  mov  AL, Color
  mov  AH, AL
  mov  CX, AX
  shl  EAX, 16  
  mov  AX, CX
  
  mov  EDI, 0A0000h
  mov  BankNumber, 0
  
  SetBank 
  
  mov  EDX, ScreenLen
  sub  EDX, ScreenWd
  mov  ESI, ScreenHt
  cld

  Clear256OutLoop :
    mov ECX, ScreenWd
    mov EBX, ECX
    and EBX, 03h
    shr ECX, 2
    rep stosd
    mov ECX, EBX
    rep stosb
    add DI, DX
    jnc DoNextClear
      inc BankNumber
      SetBank 
    
    DoNextClear :
      dec ESI
    jnz Clear256OutLoop

  popad
} 
#endif
} 
