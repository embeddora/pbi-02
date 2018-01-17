




#include "stdgfx.h"
#include "gfxddraw.h"
#include "lgfxdib.h"

DDRAWDRIVER::DDRAWDRIVER ()
  {
    #if defined (__FORGDK__)
      lpDirectDraw = NULL;
      lpPrimarySurface = NULL;
      lpSecondarySurface = NULL;
			lpDirectDrawPalette = NULL;
      hMainWindow = NULL;
    #endif  
  } 

DDRAWDRIVER::~DDRAWDRIVER ()
  {
    Reset ();  
  } 

BOOLEAN DDRAWDRIVER::SetUp ( DISPLAYDATA *Data )
  {
     if (Data)
       {} 
     #if defined (__FORGDK__)
       HRESULT Result;
       
       Result = DirectDrawCreate ( NULL, &lpDirectDraw, NULL );
       if (Result!=DD_OK)
         return FAILURE;
       Result = lpDirectDraw->SetCooperativeLevel ( Data->hMainWindow,
                                                    DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN );
       if (Result!=DD_OK)
         {
           Reset ();  
           return FAILURE;                                              
         } 

       Result = lpDirectDraw->SetDisplayMode ( ModeTable[Data->Mode].Width,
                                               ModeTable[Data->Mode].Height,   
                                               ModeTable[Data->Mode].BitSize );
       if (Result!=DD_OK)
         {
           Reset ();  
           return FAILURE;                                              
         } 

       Width = ModeTable[Data->Mode].Width;
       Height = ModeTable[Data->Mode].Height;
       BitSize = ModeTable[Data->Mode].BitSize;
       BytesPerRow = Width*8 / BitSize;
       
       DDSURFACEDESC ddsdDesc;
       ddsdDesc.dwSize = sizeof(DDSURFACEDESC);
       ddsdDesc.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
       ddsdDesc.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP | DDSCAPS_COMPLEX;
       ddsdDesc.dwBackBufferCount = 1;
       Result = lpDirectDraw->CreateSurface ( &ddsdDesc, &lpPrimarySurface, NULL );
       if (Result!=DD_OK)
         {
           Reset ();  
           return FAILURE;                                              
         } 

       DDSCAPS ddscCaps;
       ddscCaps.dwCaps = DDSCAPS_BACKBUFFER;
       Result = lpPrimarySurface->GetAttachedSurface ( &ddscCaps, &lpSecondarySurface );                
       if (Result!=DD_OK)
         {
           Reset ();  
           return FAILURE;                                              
         } 

 			 PALETTEENTRY pePalette[256]; 
			 memset ( pePalette, 0, sizeof(PALETTEENTRY)*256 );
			 Result = lpDirectDraw->CreatePalette ( DDPCAPS_8BIT, pePalette, &lpDirectDrawPalette, NULL );
       if (Result!=DD_OK)
         {
           Reset ();  
           return FAILURE;                                              
         } 

       if (lpDirectDrawPalette==NULL)
         {
           Reset ();  
           return FAILURE;                                              
         } 

			 Result = lpPrimarySurface->SetPalette ( lpDirectDrawPalette );
       if (Result!=DD_OK)
         {
           Reset ();  
           return FAILURE;                                              
         } 

     #endif  
     return SUCCESS;
  } 

VOID DDRAWDRIVER::Reset ()
  {
     #if defined (__FORGDK__)
       if (lpDirectDraw!=NULL)
         {
           lpDirectDraw->RestoreDisplayMode ();
           lpDirectDraw->Release ();
           lpDirectDraw = NULL;  
         } 
     #endif 
  } 

VOID DDRAWDRIVER::Clear ( HDISPLAY hDisplay, LONG Color )
  {
    if (hDisplay)
      {}
    if (Color)
      {}    
    #if defined (__FORGDK__)
      INT i;
      for (i=0;i<2;i++)
        {
          DDBLTFX ddbltfx;
          ddbltfx.dwSize = sizeof (DDBLTFX);
          ddbltfx.dwFillColor = Color;
          if (lpSecondarySurface->Blt ( NULL, NULL, NULL, DDBLT_WAIT | DDBLT_COLORFILL,
                                        &ddbltfx )!=DD_OK)
            return;                           
          if (lpPrimarySurface->Flip ( NULL, DDFLIP_WAIT )!=DD_OK)
            return;
        } 
    #else
      if (hDisplay)
        {}
      if (Color)
        {}
    #endif
  } 

LONG DDRAWDRIVER::GetWidth ( HDISPLAY hDisplay )
  {
    if (hDisplay)
      {}
    #if defined (__FORGDK__)
      return Width;
    #else
      return NULL;
    #endif
  } 

LONG DDRAWDRIVER::GetHeight ( HDISPLAY hDisplay )
  {
    if (hDisplay)
      {}
    #if defined (__FORGDK__)
      return Height;
    #else
      return NULL;
    #endif
  } 

VOID DDRAWDRIVER::DisplayImage ( IMAGE *Image, LONG Sx, LONG Sy,
                                 LONG Wd, LONG Ht, HDISPLAY Dest,
                                 LONG Cx, LONG Cy, LONG DestWd, LONG DestHt )
  {
     if (Image)
       {}
     if (Sx&Sy&Wd&Ht&Cx&Cy)
       {}
     if (DestWd&DestHt)
       {}  
     if (Dest)
       {}
     #if defined (__FORGDK__)
       HRESULT Result;
       DDSURFACEDESC ddsdDesc;
       INT i;
       
       ddsdDesc.dwSize = sizeof(DDSURFACEDESC);       
       Result = lpSecondarySurface->Lock ( NULL, &ddsdDesc, 0, NULL );

       if (Result==DDERR_SURFACELOST)
         {
           if (lpPrimarySurface->Restore()!=DD_OK)
             return;               
           Clear ( hMainWindow, 0 );
           Result = lpSecondarySurface->Lock ( NULL, &ddsdDesc, 0, NULL );           
         } 
       if (Result!=DD_OK)
         return;

       BYTE *DestBuffer = (BYTE*)ddsdDesc.lpSurface;
       DestBuffer += Cy*Width+Cx;
       LONG lDestPitch = ddsdDesc.lPitch;
         
       BYTE *SrcBuffer = Image->SetOffset ( Sx, Sy );    
       
       if ((MagH==DivH)&&(MagV==DivV))
         {
           for (i=0;i<Ht;i++)
            {
              memcpy ( DestBuffer, SrcBuffer, Wd );
              SrcBuffer = Image->GetNextRow ( ROW_DOWN );
              DestBuffer += BytesPerRow;
            } 
         } 
       else
         {    
           WORD Error = 0;
           DWORD AddError = (((LONG)DivH<<16)/MagH);

           INT i; 
           for (i=0;i<DestHt;i++)
             {
               ScaleBlt256To256 ( DestBuffer, SrcBuffer, DestWd, Error, AddError );
               SrcBuffer = Image->SetOffset ( Sx, Sy+(i*DivV)/MagV );
               DestBuffer += BytesPerRow;
             } 
          } 

			  Result = lpSecondarySurface->Unlock (NULL);
        if (Result!=DD_OK)
          {
					  return;
          } 

        Result = lpPrimarySurface->Flip ( NULL, DDFLIP_WAIT );
        if (Result!=DD_OK)
          {
            if (Result==DDERR_SURFACELOST)
              {
                lpPrimarySurface->Restore ();                  
              } 
          } 
     #endif
  } 

VOID DDRAWDRIVER::SetPalette ( HDISPLAY hDisplay, RGBPALETTE *Pal )
  {
     if (hDisplay)
       {}
     if (Pal)
       {}
     #if defined (__FORGDK__)
		   PALETTEENTRY pePalette[256];
			 RGBCOLOR *Entry;

			 Entry = Pal->GetEntry ();

			 INT i;
			 for (i=0;i<256;i++)
				 {
				   pePalette[i].peRed = Entry[i].Red;
				   pePalette[i].peGreen = Entry[i].Green;
				   pePalette[i].peBlue = Entry[i].Blue;
				 } 

		   lpDirectDrawPalette->SetEntries ( DDSETPAL_IMMEDIATE, 0, 256, pePalette );		   
     #endif
  } 

VOID DDRAWDRIVER::GetPalette ( HDISPLAY hDisplay, RGBPALETTE *Pal )
  {
     if (hDisplay)
       {}
     if (Pal)
       {}
     #if defined (__FORGDK__)
		   PALETTEENTRY pePalette[256];
			 RGBCOLOR *Entry;

			 Entry = Pal->GetEntry ();

		   lpDirectDrawPalette->SetEntries ( 0, 0, 256, pePalette );		   

			 INT i;
			 for (i=0;i<256;i++)
				 {
				   Entry[i].Red = pePalette[i].peRed;
				   Entry[i].Green = pePalette[i].peGreen;
				   Entry[i].Blue = pePalette[i].peBlue;
				 } 
     #endif
  } 

VOID DDRAWDRIVER::DrawLine ( HDISPLAY hDisplay, LONG x1, LONG y1, LONG x2, LONG y2,
                                COLOR  Color )
  {
     if (hDisplay)
       {}
     if (x1&y1&x2&y2)
       {}
     if (Color)
       {}
     #if defined (__FORGDK__)
     #endif
  } 

VOID DDRAWDRIVER::DrawRect ( HDISPLAY hDisplay, LONG x1, LONG y1, LONG x2, LONG y2,
                                 COLOR Color )
  {
     if (hDisplay)
       {}
     if (x1&y1&x2&y2)
       {}
     if (Color)
       {}
     #if defined (__FORGDK__)
     #endif
  } 

VOID DDRAWDRIVER::FillRect ( HDISPLAY hDisplay, LONG x1, LONG y1, LONG x2, LONG y2,
                               COLOR Color )
  {
     if (hDisplay)
       {}
     if (x1&y1&x2&y2)
       {}
     if (Color)
       {}
  } 

VOID DDRAWDRIVER::DrawPixel ( HDISPLAY hDisplay, LONG x, LONG y,
                                  COLOR Color )
  {
     if (hDisplay)
       {}
     if (x&y)
       {}
     if (Color)
       {}
  } 

VOID DDRAWDRIVER::DrawEllipse ( HDISPLAY hDisplay, LONG Cx, LONG Cy, LONG Rx, LONG Ry,
                                    COLOR Color )
  {
     if (hDisplay)
       {}
     if (Cx&Cy&Rx&Ry)
       {}
     if (Color)
       {}
  } 

VOID DDRAWDRIVER::FillEllipse ( HDISPLAY hDisplay, LONG Cx, LONG Cy, LONG Rx, LONG Ry,
                                  COLOR Color )
  {
     if (hDisplay)
       {}
     if (Cx&Cy&Rx&Ry)
       {}
     if (Color)
       {}
  } 

VOID DDRAWDRIVER::DrawText ( HDISPLAY hDisplay, STRING Text, LONG x, LONG y,
                                 COLOR Color )
  {
     if (hDisplay)
       {}
     if (x&y)
       {}
     if (Color)
       {}
     if (Text)
       {}
  } 

