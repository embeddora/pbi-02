




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

