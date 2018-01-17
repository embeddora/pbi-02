




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

