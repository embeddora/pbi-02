




#include "stdgfx.h"
#include "gfxgdi.h"

GDIDRIVER::GDIDRIVER ()
  {
    #if defined (__FORWINDOWS__)
      Info->bmiHeader.biSize = sizeof ( BITMAPINFOHEADER );
      Info->bmiHeader.biWidth = 0;            
      Info->bmiHeader.biHeight = 0;           
      Info->bmiHeader.biPlanes = 1;
      Info->bmiHeader.biBitCount = 0;         
      Info->bmiHeader.biCompression = BI_RGB;
      Info->bmiHeader.biSizeImage = 0;        
      Info->bmiHeader.biXPelsPerMeter = 0;
      Info->bmiHeader.biYPelsPerMeter = 0;
      Info->bmiHeader.biClrUsed = 0;
      Info->bmiHeader.biClrImportant = 0;

      LogPal = (LOGPALETTE*)malloc((sizeof(LOGPALETTE)+sizeof(PALETTEENTRY)*256));;
      LogPal->palVersion = 0x300;
      LogPal->palNumEntries = 256;

      
      WORD *Pal;
      Pal = (WORD*)&(Info->bmiColors[0]);
      INT i;
      for (i=0;i<256;i++)
        {
          *Pal++ = (WORD)i;
        } 
    #endif
  } 

GDIDRIVER::~GDIDRIVER ()
  {
    #if defined (__FORWINDOWS__)
      if (LogPal!=NULL)
        free (LogPal);
      LogPal = NULL;  
    #endif  
  } 

BOOLEAN GDIDRIVER::SetUp ( DISPLAYDATA *Data )
  {
    if (Data)
      {}  
    return SUCCESS;
  } 

VOID GDIDRIVER::Reset ()
  {
  } 

VOID GDIDRIVER::Clear ( HDISPLAY hDisplay, LONG Color )
  {
    if (hDisplay)
      {}
    if (Color)
      {}
    #if defined (__FORWINDOWS__)
      LONG Wd,Ht;
      Wd = GetWidth ( hDisplay );
      Ht = GetHeight ( hDisplay );
      FillRect ( hDisplay, 0, 0, Wd, Ht, Color );
    #else
    #endif
  } 

LONG GDIDRIVER::GetWidth ( HDISPLAY hDisplay )
  {
    #if defined (__FORWINDOWS__)
      RECT Rect;
      GetClipBox ( hDisplay, &Rect );
      return Rect.right;
    #elif defined (__FOROS2__)
      if (hDisplay)
        {}
      return 0;
    #elif defined (__FORDOS__)
      if (hDisplay)
        {}
      return 0;
    #endif
  } 

LONG GDIDRIVER::GetHeight ( HDISPLAY hDisplay )
  {
    #if defined (__FORWINDOWS__)
      RECT Rect;
      GetClipBox ( hDisplay, &Rect );
      return Rect.bottom;
    #elif defined (__FOROS2__)
      if (hDisplay)
        {}
      return 0;
    #elif defined (__FORDOS__)
      if (hDisplay)
        {}
      return 0;
    #endif
  } 

VOID GDIDRIVER::DisplayImage ( IMAGE *Image, LONG Sx, LONG Sy,
                               LONG Wd, LONG Ht, HDISPLAY Dest,
                               LONG Cx, LONG Cy, LONG DestWd, LONG DestHt )
  {
    #if defined (__FORWINDOWS__)
      #if defined (__FORWIN32__)
        HDISPLAY hMemDisplay;
        if (Image->IsAttached())
          {
            hMemDisplay = Image->GetMemDisplay ();
            if ((MagH==DivH)&&(MagV==DivH))
              {
                BitBlt ( Dest, Cx, Cy, Wd, Ht, hMemDisplay,
                         Sx, Sy, SRCCOPY );
              } 
            else
              {
                StretchBlt ( Dest, Cx, Cy, DestWd, DestHt, hMemDisplay,
                             Sx, Sy, Wd, Ht, SRCCOPY );    
              } 
            return;  
          } 
      #endif
        
      BYTE *SrcBuffer;
      Info->bmiHeader.biWidth = Image->GetWidth();
      Info->bmiHeader.biBitCount = (SHORT)Image->GetBits();
      Info->bmiHeader.biSizeImage = Image->GetBytesPerRow()*Image->GetHeight();
     
      SrcBuffer = Image->GetBuffer ();
      if (MagV==DivV)
        DestWd = Wd;
      if (MagH==DivH)
        DestHt = Ht;
        
      if (Image->GetDirection()==IMAGE_TOPDOWN)
        {
          Info->bmiHeader.biHeight = -Image->GetHeight();
          #if defined (__FORWIN386__)
            StretchDIBits ( Dest, (short)Cx, (short)Cy, (short)DestWd, (short)DestHt,
                            (short)Sx, (short)(Image->GetHeight()-Sy-Ht),
                            (short)Wd, (short)Ht, SrcBuffer,
                            Info, DIB_PAL_COLORS, SRCCOPY );
          #else
            StretchDIBits ( Dest, Cx, Cy, DestWd, DestHt, Sx, Image->GetHeight()-Sy-Ht,
                            Wd, Ht, SrcBuffer,
                            Info, DIB_PAL_COLORS, SRCCOPY );
          #endif                
        } 
      else
        {
          Info->bmiHeader.biHeight = Image->GetHeight();
          #if defined (__FORWIN386__)
            StretchDIBits ( Dest, (short)Cx, (short)Cy, (short)DestWd, (short)DestHt,
                            (short)Sx, (short)(Image->GetHeight()-Sy-Ht),
                            (short)Wd, (short)Ht, SrcBuffer,
                            Info, DIB_PAL_COLORS, SRCCOPY );
          #else
            StretchDIBits ( Dest, Cx, Cy, DestWd, DestHt, Sx, Image->GetHeight()-Sy-Ht,
                            Wd, Ht, SrcBuffer,
                            Info, DIB_PAL_COLORS, SRCCOPY );
          #endif
        } 
    #else
      if (Image)
        {}
      if (Dest)
        {}
      if (Sx&Sy&Cx&Cy&Wd&Ht)
        {}
      if (DestWd&DestHt)
        {}  
    #endif
  } 

VOID GDIDRIVER::SetPalette ( HDISPLAY hDisplay, RGBPALETTE *Pal )
  {
    if (Pal==NULL)
      return;

    #if defined (__FORWINDOWS__)
      RGBCOLOR *Entry;
      HPALETTE hPal,OldPal;      

      if (LogPal==NULL)
        return;
        
      Entry = Pal->GetEntry ();

      INT i;
      for (i=0;i<256;i++)
        {
          LogPal->palPalEntry[i].peRed = Entry[i].Red;  
          LogPal->palPalEntry[i].peGreen = Entry[i].Green;  
          LogPal->palPalEntry[i].peBlue = Entry[i].Blue;  
          LogPal->palPalEntry[i].peFlags = 0;  
        } 
        
      hPal = CreatePalette ( (LOGPALETTE*)LogPal );     
      OldPal = SelectPalette ( hDisplay, hPal, TRUE );
      RealizePalette ( hDisplay );
      DeleteObject ( OldPal );
      DeleteObject ( hPal );
    #else
      if (hDisplay)
        {}
      if (Pal)
        {}
    #endif
  } 

VOID GDIDRIVER::GetPalette ( HDISPLAY hDisplay, RGBPALETTE *Pal )
  {
    #if defined (__FORWINDOWS__)
      RGBCOLOR *Entry;

      Entry = Pal->GetEntry ();
      if (Entry==NULL)
        return;

      GetSystemPaletteEntries ( hDisplay, 0, 256, LogPal->palPalEntry );

      INT i;
      for (i=0;i<256;i++)
        {
          Entry[i].Red = LogPal->palPalEntry[i].peRed;
          Entry[i].Green = LogPal->palPalEntry[i].peGreen;
          Entry[i].Blue = LogPal->palPalEntry[i].peBlue;            
        } 
    #else
      if (hDisplay)
        {}
      if (Pal)
        {}
    #endif
  } 

VOID GDIDRIVER::DrawLine ( HDISPLAY hDisplay, LONG x1, LONG y1, LONG x2, LONG y2,
                           COLOR Color )
  {
    #if defined (__FORWINDOWS__)
      HGDIOBJ OldPen;
      HPEN hPen;

      hPen = CreatePen ( PS_SOLID, 1, Color );

      OldPen = SelectObject ( hDisplay, hPen );

      #if defined (__FORWIN386__)
        MoveToEx ( hDisplay, (short)x1, (short)y1, NULL );
        LineTo ( hDisplay, (short)x2, (short)y2 );
      #else
        MoveToEx ( hDisplay, x1, y1, NULL );
        LineTo ( hDisplay, x2, y2 );
      #endif
      
      SelectObject ( hDisplay, OldPen );
      DeleteObject ( hPen );
    #else
      if (hDisplay)
        {}
      if (x1&y1&x2&y2)
        {}
      if (Color)
        {}
    #endif
  } 

VOID GDIDRIVER::DrawRect ( HDISPLAY hDisplay, LONG x1, LONG y1, LONG x2, LONG y2,
                           COLOR Color )
  {
    #if defined (__FORWINDOWS__)
      HGDIOBJ OldPen;
      HGDIOBJ OldBrush;
      HPEN hPen;

      hPen = CreatePen ( PS_SOLID, 1, Color );

      OldPen = SelectObject ( hDisplay, hPen );
      OldBrush = SelectObject ( hDisplay, GetStockObject ( NULL_BRUSH ) );

      #if defined (__FORWIN386__)
        Rectangle ( hDisplay, (short)x1, (short)y1, (short)x2, (short)y2 );
      #else
        Rectangle ( hDisplay, x1, y1, x2, y2 );
      #endif
      
      SelectObject ( hDisplay, OldPen );
      SelectObject ( hDisplay, OldBrush );
      DeleteObject ( hPen );
    #else
      if (hDisplay)
        {}
      if (x1&y1&x2&y2)
        {}
      if (Color)
        {}
    #endif
  } 

VOID GDIDRIVER::FillRect ( HDISPLAY hDisplay, LONG x1, LONG y1, LONG x2, LONG y2,
                           COLOR Color )
  {
    #if defined (__FORWINDOWS__)
      HGDIOBJ OldPen;
      HGDIOBJ OldBrush;
      HPEN hPen;
      HBRUSH hBrush;
      LOGBRUSH lgb;

      hPen = CreatePen ( PS_SOLID, 1, Color );

      lgb.lbStyle = BS_SOLID;
      lgb.lbColor = Color;
      lgb.lbHatch = 0;

      hBrush = CreateBrushIndirect ( &lgb );
      OldPen = SelectObject ( hDisplay, hPen );
      OldBrush = SelectObject ( hDisplay, hBrush );

      #if defined (__FORWIN386__)
        Rectangle ( hDisplay, (short)x1, (short)y1, (short)x2, (short)y2 );
      #else
        Rectangle ( hDisplay, x1, y1, x2, y2 );
      #endif
      
      SelectObject ( hDisplay, OldPen );
      SelectObject ( hDisplay, OldBrush );
      DeleteObject ( hPen );
      DeleteObject ( hBrush );
    #else
      if (hDisplay)
        {}
      if (x1&y1&x2&y2)
        {}
      if (Color)
        {}
    #endif
  } 

VOID GDIDRIVER::DrawPixel ( HDISPLAY hDisplay, LONG x, LONG y,
                            COLOR Color )
  {
    #if defined (__FORWINDOWS__)
      #if defined (__FORWIN386__)
        SetPixel ( hDisplay, (short)x, (short)y, Color );
      #else
        SetPixel ( hDisplay, x, y, Color );
      #endif
    #else
      if (hDisplay)
        {}
      if (x&y)
        {}
      if (Color)
        {}
    #endif
  } 

VOID GDIDRIVER::DrawEllipse ( HDISPLAY hDisplay, LONG Cx, LONG Cy, LONG Rx, LONG Ry,
                              COLOR Color )
  {
    #if defined (__FORWINDOWS__)
      HGDIOBJ OldPen;
      HGDIOBJ OldBrush;
      HPEN hPen;

      hPen = CreatePen ( PS_SOLID, 1, Color );

      OldPen = SelectObject ( hDisplay, hPen );
      OldBrush = SelectObject ( hDisplay, GetStockObject ( NULL_BRUSH ) );

      #if defined (__FORWIN386__)
        Ellipse ( hDisplay, (short)(Cx-Rx), (short)(Cy-Ry), (short)(Cx+Rx), (short)(Cy+Ry) );
      #else
        Ellipse ( hDisplay, Cx-Rx, Cy-Ry, Cx+Rx, Cy+Ry );
      #endif
      
      SelectObject ( hDisplay, OldPen );
      SelectObject ( hDisplay, OldBrush );
      DeleteObject ( hPen );
    #else
      if (hDisplay)
        {}
      if (Cx&Cy&Rx&Ry)
        {}
      if (Color)
        {}
    #endif
  } 

VOID GDIDRIVER::FillEllipse ( HDISPLAY hDisplay, LONG Cx, LONG Cy, LONG Rx, LONG Ry,
                              COLOR Color )
  {
    #if defined (__FORWINDOWS__)
      HGDIOBJ OldPen;
      HGDIOBJ OldBrush;
      HPEN hPen;
      HBRUSH hBrush;
      LOGBRUSH lgb;

      hPen = CreatePen ( PS_SOLID, 1, Color );

      lgb.lbStyle = BS_SOLID;
      lgb.lbColor = Color;
      lgb.lbHatch = 0;

      hBrush = CreateBrushIndirect ( &lgb );
      OldPen = SelectObject ( hDisplay, hPen );
      OldBrush = SelectObject ( hDisplay, hBrush );

      #if defined (__FORWIN386__)
        Ellipse ( hDisplay, (short)(Cx-Rx), (short)(Cy-Ry), (short)(Cx+Rx), (short)(Cy+Ry));
      #else
        Ellipse ( hDisplay, Cx-Rx, Cy-Ry, Cx+Rx, Cy+Ry );
      #endif

      SelectObject ( hDisplay, OldPen );
      SelectObject ( hDisplay, OldBrush );
      DeleteObject ( hPen );
      DeleteObject ( hBrush );
    #else
      if (hDisplay)
        {}
      if (Cx&Cy&Rx&Ry)
        {}
      if (Color)
        {}
    #endif
  } 

VOID GDIDRIVER::DrawText ( HDISPLAY hDisplay, STRING Text, LONG x, LONG y,
                           COLOR Color )
  {
    #if defined (__FORWINDOWS__)
      COLOR OldColor;

      OldColor = SetTextColor ( hDisplay, Color );
      
      #if defined (__FORWIN386__)
        MoveToEx ( hDisplay, (short)x, (short)y, NULL );
        TextOut ( hDisplay, (short)x, (short)y, Text, (short)strlen ( Text ) );
      #else
        MoveToEx ( hDisplay, x, y, NULL );
        TextOut ( hDisplay, x, y, Text, strlen ( Text ) );
      #endif
      SetTextColor ( hDisplay, OldColor );
    #else
      if (hDisplay)
        {}
      if (Text)
        {}
      if (x&y)
        {}
      if (Color)
        {}
    #endif
  } 
