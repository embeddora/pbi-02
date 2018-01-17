




#include "stdgfx.h" 
#include "gfxvesa.h"
#include "gfxdpmi.h"
#include "lgfxvsa.h"
#include "lgfxdib.h"

#include "stdlib.h"

VESADRIVER::VESADRIVER ()
  {
    VESABlock = new VESABLOCK;
  } 

VESADRIVER::~VESADRIVER ()
  {
    delete VESABlock;
    VESABlock = NULL;
  } 

BOOLEAN VESADRIVER::DetectVESA ()
  {
    #if defined (__FORWINDOWS__)
      return FAILURE;
    #elif defined (__FOROS2__)
      return FAILURE;
    #elif defined (__FORDOS__)
      union REGS Regs;
      struct SREGS SegRegs;

      Regs16.ax = 0x4F00;
      segread ( &SegRegs );

      #if defined (__32BIT__)
        #ifndef __FORPOWERPACK__
          VESABLOCK far* BlockPtr;
          INT NumPara;
          WORD Selector,Segment;

          NumPara = (512+15)/16;

          DPMI.AllocateDOSMemory ( NumPara, &Selector, &Segment );
          Regs.x.edi = 0;
          SegRegs.es = Segment;
          BlockPtr = (VESABLOCK far*)MK_FP(Selector,0);
          DPMI.Int86x ( INT_VESA, &Regs, &Regs, &SegRegs );
          _fmemcpy ( (VESABLOCK far*)VESABlock, BlockPtr, 512 );
          DPMI.FreeDOSMemory ( Selector );
        #endif
      #elif defined (__FORPWPK16__)
        VESABLOCK far* BlockPtr;
        INT NumPara;
        WORD Selector,Segment;
 
        NumPara = (512+15)/16;

        DPMI.AllocateDOSMemory ( NumPara, &Selector, &Segment );
        Regs16.di = 0;
        SegRegs.es = Segment;
        BlockPtr = (VESABLOCK far*)MK_FP(Selector,0);
        DPMI.Int86x ( INT_VESA, &Regs, &Regs, &SegRegs );
        _fmemcpy ( (VESABLOCK far*)VESABlock, BlockPtr, 512 );
        DPMI.FreeDOSMemory ( Selector );
      #else
        strncpy ( VESABlock->ID, "VBE2", 4 );
        Regs16.di = FP_OFF(VESABlock);
        SegRegs.es = FP_SEG(VESABlock);
        INTERRUPTx ( INT_VESA, &Regs, &Regs, &SegRegs );
      #endif
      
      if (Regs8.al!=0x4F)
        return FAILURE;
      if (Regs8.ah!=0)
        return FALSE;
      if (strncmp(VESABlock->ID,"VESA",4))
        return FALSE;
       return SUCCESS;
    #endif
  } 

BOOLEAN VESADRIVER::SetVESAMode ( LONG ModeNumber )
  {
    #if defined (__FORWINDOWS__)
      if (ModeNumber)
        {}
      return SUCCESS;
    #elif defined (__FOROS2__)
      if (ModeNumber)
        {}
      return SUCCESS;
    #elif defined (__FORDOS__)
      union REGS Regs;
      Regs16.ax = 0x4F02;
      Regs16.bx = (SHORT)ModeNumber;

      INTERRUPT ( INT_VESA, &Regs, &Regs );

      if (Regs8.al!=0x4F)
        return FAILURE;
      if (Regs8.ah!=0)
        return FALSE;
      SetVESASelector ( SELECTOR_VIDEO );
      return SUCCESS;
    #endif
  } 

BOOLEAN VESADRIVER::SetVESABank ( LONG Bank )
  {
    #if defined (__FORWINDOWS__)
      if (Bank)
        {}
      return SUCCESS;
    #elif defined (__FOROS2__)
      if (Bank)
        {}
      return SUCCESS;
    #elif defined (__FORDOS__)
      union REGS Regs;
      Regs16.ax = 0x4F05;
      Regs16.bx = 0;
      Regs16.cx = 0;
      Regs16.dx = (SHORT)Bank;

      INTERRUPT ( INT_VESA, &Regs, &Regs );

      if (Regs8.al!=0x4F)
        return FAILURE;
      if (Regs8.ah!=0)
        return FALSE;
      return SUCCESS;
    #endif
  } 

BOOLEAN VESADRIVER::SetRowSize ( LONG Size )
  {
    #if defined (__FORWINDOWS__)
      if (Size)
        {}
      return SUCCESS;
    #elif defined (__FOROS2__)
      if (Size)
        {}
      return SUCCESS;
    #elif defined (__FORDOS__)
      union REGS Regs;
      Regs16.ax = 0x4F06;
      Regs16.bx = 0;
      Regs16.cx = (SHORT)Size;

      INTERRUPT ( INT_VESA, &Regs, &Regs );

      if (Regs8.al!=0x4F)
        return FAILURE;
      if (Regs8.ah!=0)
        return FALSE;
      return SUCCESS;
    #endif
  } 

BOOLEAN VESADRIVER::SetUp ( DISPLAYDATA *Data )
  {
    if (Data)
      {}  
    #if defined (__FORWINDOWS__)
      return SUCCESS;
    #elif defined (__FOROS2__)
      return SUCCESS;
    #elif defined (__FORDOS__)
      
      

      LONG ModeNumber;

      ModeNumber = ModeTable[Data->Mode].ModeNumber;

      if (SetVESAMode(ModeNumber)==FAILURE)
        return FAILURE;

      if (SetVESABank ( 0 )==FAILURE)
        return FAILURE;

      if (SetRowSize(1024)==FAILURE)
        return FAILURE;

      CurMode = Data->Mode;

      Width = ModeTable[Data->Mode].Width;
      Height = ModeTable[Data->Mode].Height;
      BitSize = ModeTable[Data->Mode].BitSize;

      BytesPerRow = 1024;

      SetVESAData ( Width, Height, 1024 );

      Clear ( HVGA, 0 );

      return SUCCESS;
    #endif
  } 

VOID VESADRIVER::Reset ()
  {
    #if defined (__FORWINDOWS__)
    #elif defined (__FOROS2__)
    #elif defined (__FORDOS__)
      union REGS Regs;
      Regs8.ah = 0;
      Regs8.al = 3;
      INTERRUPT ( INT_VIDEO, &Regs, &Regs );

      Width = ModeTable[M80x25x16].Width;
      Height = ModeTable[M80x25x16].Height;
      BitSize = ModeTable[M80x25x16].BitSize;
    #endif
  } 

VOID VESADRIVER::Clear ( HDISPLAY hDisplay, LONG Color )
  {
    if (hDisplay)
      {}
    if (BitSize==8)
      VESAClear256 ( (BYTE)Color );
  } 

VOID VESADRIVER::DisplayImage ( IMAGE *Image, LONG Sx, LONG Sy,
                                LONG Wd, LONG Ht, HDISPLAY Dest,
                                LONG Cx, LONG Cy, LONG DestWd, LONG DestHt )
  {
    BYTE *SrcBuffer;
    LONG Format;

    if (Dest)
      {}
    Format = Image->GetFormat();
    SrcBuffer = Image->SetOffset ( Sx, Sy );
    if (BitSize==8)
      {
        if ((MagH==DivH)&&(MagV==DivV))
          {
            if (Format==IMAGE_8BIT)
              VESACopy256To256 ( SrcBuffer, Image->GetRowInc (), Wd, Ht,
                                 Cx, Cy );
          } 
        else
          {
            BYTE *DestBuffer;
            LONG Offset;
            LONG Page;
            
            #if defined (__32BIT__)
              DestBuffer = (BYTE*)0xA0000;
            #else
              DestBuffer = (BYTE*)MK_FP(SELECTOR_VIDEO,0);
            #endif

            Offset = Cy*BytesPerRow+Cx;

            Page = Offset >> 16;
            Offset &= 0xFFFF;
            
            SetVESABank ( Page ); 
    
            WORD Error = 0;
            DWORD AddError = (((LONG)DivH<<16)/MagH);

            INT i; 
            for (i=0;i<DestHt;i++)
              {
                ScaleBlt256To256 ( DestBuffer+Offset, SrcBuffer, DestWd,
                                   Error, AddError );
                SrcBuffer = Image->SetOffset ( Sx, Sy+(i*DivV)/MagV );
                Offset += BytesPerRow;
                if (Offset>(LONG)0xFFFF)
                  {
                    Offset &= 0xFFFF;
                    Page++;  
                    SetVESABank ( Page ); 
                  } 
              } 
          } 
      } 
  } 

VOID VESADRIVER::DrawLine ( HDISPLAY hDisplay, LONG x1, LONG y1, LONG x2, LONG y2,
                            COLOR  Color )
  {
    INT Dx, Dy;
    INT AddX,AddY;
    INT Len;

    if (hDisplay)
      {}

    if (x2<x1)
      {
        AddX = -1;
      } 
    else
      AddX = 1;

    Dx = abs(x2-x1);
    Dy = abs(y2-y1);

    Len = MaxValue ( Dx, Dy ) + 1;

    BYTE *Buffer;
    DWORD Offset;

    #if defined (__32BIT__)
      #if defined (__FORDOS4GW__)
        Buffer = (BYTE*)0xA0000;
      #endif
    #else
      Buffer = (BYTE*)MK_FP(0xA000,0);
    #endif

    AddY = BytesPerRow;
    Offset = ((y1*AddY)&0xFFFF);

    LONG BankNumber = y1>>6;
    SetVESABank ( BankNumber );

    INT i;
    DWORD Error=0;

    DWORD x;

    if (Dy==0)
      {
        x = MinValue ( x1, x2 );
        int i;
        Buffer+=x;
        for (i=0;i<Len;i++)
          Buffer[Offset+i] = (BYTE)Color;
        
      } 
    else
      {
        x = x1;
        if (Dx>Dy)
          {
            for (i=0;i<Len;i++)
              {
                Buffer[Offset+x] = (BYTE)Color;
                Error += (DWORD)Dy;
                if (Error>=(DWORD)Dx)
                  {
                    Offset += (DWORD)AddY;
                    if (Offset>(DWORD)0xFFFF)
                      {
                        Offset &= (DWORD)0xFFFF;
                        BankNumber ++;
                        SetVESABank ( BankNumber );
                      } 
                    Error -= Dx;
                  } 
                x += AddX;
              } 
          } 
        else
          {
            for (i=0;i<Len;i++)
              {
                Buffer[Offset+x] = (BYTE)Color;
                Error += (DWORD)Dx;
                if (Error>=(DWORD)Dy)
                  {
                    x += AddX;
                    Error -= (DWORD)Dy;
                  } 
                Offset += (DWORD)AddY;
                if (Offset>(DWORD)0xFFFF)
                  {
                    Offset &= (DWORD)0xFFFF;
                    BankNumber ++;
                    SetVESABank ( BankNumber );
                  } 
              } 
          } 
      } 
  } 

VOID VESADRIVER::DrawRect ( HDISPLAY hDisplay, LONG x1, LONG y1, LONG x2, LONG y2,
                            COLOR Color )
  {
    DrawLine ( hDisplay, x1, y2, x2, y2, Color );
    DrawLine ( hDisplay, x1, y1, x2, y1, Color );
    DrawLine ( hDisplay, x1, y1, x1, y2, Color );
    DrawLine ( hDisplay, x2, y1, x2, y2, Color );
  } 

VOID VESADRIVER::FillRect ( HDISPLAY hDisplay, LONG x1, LONG y1, LONG x2, LONG y2,
                           COLOR Color )
  {
    BYTE *Buffer;

    if (hDisplay)
      {}

    #if defined (__32BIT__)
      #if defined (__FORDOS4GW__)
        Buffer = (BYTE*)0xA0000;
      #endif
    #else
      Buffer = (BYTE*)MK_FP(0xA000,0);
    #endif

    LONG AddY = BytesPerRow;
    LONG Offset;
    Offset = y1*AddY+x1;
    Offset &= (DWORD)0xFFFF;

    LONG BankNumber;
    BankNumber = y1>>6;
    SetVESABank ( BankNumber );

    INT i;
    LONG Wd, Ht;

    Wd = x2-x1+1;
    Ht = y2-y1+1;

    for (i=0;i<Ht;i++)
      {
        memset ( Buffer+Offset, Color, Wd );
        Offset += AddY;
        if (Offset>(DWORD)0xFFFF)
          {
            Offset &= (DWORD)0xFFFF;
            BankNumber++;
            SetVESABank ( BankNumber );
          } 
      } 
  } 

VOID VESADRIVER::DrawPixel ( HDISPLAY hDisplay, LONG x, LONG y,
                            COLOR Color )
  {
    if (hDisplay)
      {}

    if ((x<0)||(x>=Width))
      return;
    if ((y<0)||(y>=Height))
      return;

    #if defined (__32BIT__)
      #if defined (__FORWATCOM__)
        Buffer = (BYTE*)0xA0000;
      #endif
    #else
      Buffer = (BYTE*)MK_FP(0xA000,0);
    #endif

    LONG Offset;
    Offset = y*BytesPerRow+x;
    Offset &= (DWORD)0xFFFF;

    LONG BankNumber;
    BankNumber = y>>6;
    SetVESABank ( BankNumber );

    *(Buffer+Offset) = (BYTE)Color;
  } 

VOID VESADRIVER::DrawHorizLine ( HDISPLAY hDisplay, LONG x1, LONG x2, LONG y, COLOR Color )
  {
    if (hDisplay)
      {}

    if (x1>x2)
      SwapValue ( &x1, &x2 );

    if (x1<0)
      x1 = 0;
    else if (x1>Width)
      return;

    if (x2<0)
      return;
    else if (x2>Width)
      x2 = Width;

    if (y<0)
      y = 0;
    else if (y>Height)
      return;

    INT Wd;

    Wd = x2 - x1 + 1;

    BYTE *Buffer;

    #if defined (__32BIT__)
      #if defined (__FORWATCOM__)
        Buffer = (BYTE*)0xA0000;
      #endif
    #else
      Buffer = (BYTE*)MK_FP(0xA000,0);
    #endif

    LONG Offset = y*BytesPerRow+x1;
    Offset &= (DWORD)0xFFFF;

    SetVESABank ( y>>6 );

    memset ( Buffer+Offset, Color, Wd );
  } 

