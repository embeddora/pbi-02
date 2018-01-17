




#ifndef __GFXVESEDRV_H__
#define __GFXVESADRV_H__

#include "stdgfx.h"
#include "gfxvga.h"

struct VESABLOCK
  {
    char  ID[4];
    SHORT Version;
    DWORD OEM;
    DWORD Capabilities;
    SHORT Memory;
    DWORD ModePtr;

    
    SHORT OEMSoftwareRev;
    DWORD OEMVendorName;
    DWORD OEMProductName;
    DWORD OEMProductRev;
    CHAR  Reserved[222];
    CHAR  OEMData[256];
  }; 

struct VESAMODEINFO
  {
    SHORT Attributes;
    CHAR  WinAAttr;
    CHAR  WinBAttr;
    SHORT WinGranuality;
    SHORT WinSize;
    SHORT WinASegment;
    SHORT WinBSegment;
    DWORD WinFuncPtr;
    SHORT BytesPerScanLine;
    SHORT XResolution;
    SHORT YResolution;
    CHAR  XCharSize;
    CHAR  YCharSize;
    CHAR  NumPlanes;
    CHAR  BitsPerPixel;
    CHAR  NumBanks;
    CHAR  MemoryModel;
    CHAR  BankSize;
    CHAR  NumImages;
    CHAR  Res1;
    CHAR  RedMaskSize;
    CHAR  RedFieldPos;
    CHAR  GreenMaskSize;
    CHAR  GreenFieldPos;
    CHAR  BlueMaskSize;
    CHAR  BlueFieldPos;
    CHAR  RsvMaskSize;
    CHAR  RsvFieldPos; 
    CHAR  tDirectColor;

    
    DWORD PhysBasePtr;
    DWORD OffScrOffset;
    DWORD OffScrSize;
    CHAR  Res[206];
  }; 

class VESADRIVER : public VGADRIVER
  {
    protected :
      VESABLOCK *VESABlock;

      BOOLEAN DetectVESA ();
      BOOLEAN SetRowSize ( LONG Size );
      BOOLEAN SetVESAMode ( LONG ModeNumber );
      BOOLEAN SetVESABank ( LONG Bank );

      virtual VOID DrawHorizLine ( HDISPLAY hDisplay, LONG x1,
                                   LONG x2, LONG y, COLOR Color );

    public :
      VESADRIVER ();
      virtual ~VESADRIVER ();

      virtual BOOLEAN SetUp ( DISPLAYDATA *Data );
      virtual VOID Reset ();
      VOID Clear ( HDISPLAY hDisplay, LONG Color );

      virtual VOID DisplayImage ( IMAGE *Image, LONG Sx, LONG Sy,
                                  LONG Wd, LONG Ht, HDISPLAY Dest,
                                  LONG Cx, LONG Cy, LONG DestWd, LONG DestHt );
      virtual VOID DrawLine ( HDISPLAY hDisplay, LONG x1, LONG y1, LONG x2, LONG y2,
                              COLOR  Color );
      virtual VOID DrawRect ( HDISPLAY hDisplay, LONG x1, LONG y1, LONG x2, LONG y2,
                              COLOR Color );
      virtual VOID FillRect ( HDISPLAY hDisplay, LONG x1, LONG y1, LONG x2, LONG y2,
                              COLOR Color );
      virtual VOID DrawPixel ( HDISPLAY hDisplay, LONG x, LONG y,
                               COLOR Color );
  }; 

#endif
