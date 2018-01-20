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
#include "lgfxvga.h"
 
LONG  SrcWd;
LONG  SrcHt;




VOID SetVGASelector ( SHORT Selector )
{
  if (Selector)
    {}
} 







VOID VGACopy256To256 ( BYTE *Src, LONG AddSrc, LONG SrcWidth, LONG SrcHeight,
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
  mov  SrcWd, EBX
  mov  SrcHt, ECX
  
  sub  EDX, EBX
  mov  EBX, EDX   

  mov  EDI, 0A0000h
  mov  EAX, DestCy
  mov  EDX, 320
  mul  EDX  
  add  EAX, DestCx
  add  EDI, EAX
  
  mov  EDX, 320
  sub  EDX, SrcWd
  cld

  Copy256OutLoop :
    mov ECX, SrcWd
    mov EAX, ECX
    and EAX, 03h
    shr ECX, 2
    rep movsd
    mov ECX, EAX
    rep movsb
    add ESI, EBX
    add EDI, EDX
    dec SrcHt
    jnz Copy256OutLoop

  popad
} 
#endif
} 

