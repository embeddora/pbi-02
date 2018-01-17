




#include "stdgfx.h"
#include "gfximage.h"

IMAGE::IMAGE ()
  {
    Buffer = NULL;
    Width = Height = 0;
    BytesPerRow = 0;
    RowInc = 0;
    Bits = 0;
    Transparent = NOTRANSPARENT;
    Direction = IMAGE_TOPDOWN;
    SetViewPort ( 0, 0, -1, -1 );
    Attached = FALSE;
    hMemDisplay = NULL;
    DIBSection = NULL;
    ForDebug = FALSE;
  } 

IMAGE::IMAGE ( LONG NewDirection )
  {
    Buffer = NULL;
    Width = Height = 0;
    BytesPerRow = 0;
    RowInc = 0;
    Bits = 0;
    Transparent = NOTRANSPARENT;
    Direction = NewDirection;
    SetViewPort ( 0, 0, -1, -1 );
    Attached = FALSE;
    hMemDisplay = NULL;
    DIBSection = NULL;
    ForDebug = FALSE;
  } 

IMAGE::~IMAGE ()
  {
    Destroy ();
  } 

VOID IMAGE::SetDebugFlag ( BOOLEAN IsForDebug )
  {
    ForDebug = IsForDebug;
  } 

BOOLEAN IMAGE::Create ( LONG NewFormat, LONG Wd, LONG Ht )
  {
    LONG Size;
    LONG BitSize;

    if (NewFormat==IMAGE_8BIT)
      BitSize = 8;
    else if (NewFormat==IMAGE_16BIT)
      BitSize = 16;
    else if (NewFormat==IMAGE_24BIT)
      BitSize = 24;
    else
      {
        Error.SetError ( ERR_NOVALID );
        return FALSE;
      } 
    
    if ((Wd<=0)||(Ht<=0))
      {
        Error.SetError ( ERR_NOVALID );
        return FALSE;
      } 

    Destroy ();

    Width = Wd;
    Height = Ht;
    Format = NewFormat;
    Bits = BitSize;
    BytesPerPixel = Bits/8;
    BytesPerRow = Width*BytesPerPixel;
    if (Direction==IMAGE_TOPDOWN)
      RowInc = BytesPerRow;
    else
      RowInc = -BytesPerRow;  

    Size = BytesPerRow*Ht*(Bits/8);

    #if defined (__FORDOS__)
      if (ForDebug)
        {
          #if defined (__FORWATCOM__)
            Buffer = (BYTE*)0xA0000;
          #endif
        } 
      else
        {
          Buffer = new BYTE [Size];
        } 
    #else
      Buffer = new BYTE [Size];
    #endif

    if (Buffer==NULL)
      {
        Width = Height = Bits = BytesPerRow = RowInc = 0;
        Error.SetError ( ERR_NOMEMORY );
        return FALSE;
      } 
    SetViewPort ( 0, 0, Width-1, Height-1 );
    return SUCCESS;
  } 

BOOLEAN IMAGE::Create ( HDISPLAY hDisplay, LONG NewFormat, LONG Wd, LONG Ht )
  {
    if (hDisplay)
      {}
    #if defined (__FORWIN32__)
      Destroy ();
      
      BITMAPINFO *Info;
      Info = (BITMAPINFO*)malloc(sizeof(BITMAPINFOHEADER)+sizeof(RGBQUAD)*256);
      if (Info==NULL)
        return FAILURE;

      LONG BitSize;
      
      if (NewFormat==IMAGE_8BIT)
        BitSize = 8;
      else if (NewFormat==IMAGE_16BIT)
        BitSize = 16;
      else if (NewFormat==IMAGE_24BIT)
        BitSize = 24;
      else
        {
          Error.SetError ( ERR_NOVALID );
          return FALSE;
        } 

      Info->bmiHeader.biSize = sizeof ( BITMAPINFOHEADER );

      Wd = (Wd+3) & ~3;
      
      Info->bmiHeader.biWidth = Wd;
      if (Direction==IMAGE_TOPDOWN)
        Info->bmiHeader.biHeight = -Ht;
      else  
        Info->bmiHeader.biHeight = Ht;
        
      Info->bmiHeader.biPlanes = 1;
      Info->bmiHeader.biBitCount = (SHORT)BitSize;
      Info->bmiHeader.biCompression = BI_RGB;
      Info->bmiHeader.biSizeImage = (DWORD)Wd*(DWORD)Ht*(BitSize/8);
      Info->bmiHeader.biXPelsPerMeter = 0;
      Info->bmiHeader.biYPelsPerMeter = 0;
      Info->bmiHeader.biClrUsed = 0;
      Info->bmiHeader.biClrImportant = 0;

      WORD *Pal;
      INT i;

      
      Pal = (WORD*)&(Info->bmiColors[0]);
      for (i=0;i<256;i++)
        {
          *Pal++ = (WORD)i;   
        } 
        
      DIBSection = CreateDIBSection ( hDisplay, Info, DIB_PAL_COLORS,
                                      (VOID**)&Buffer, 0, 0 );

      hMemDisplay = CreateCompatibleDC ( hDisplay );
      OldBitmap = (HBITMAP)SelectObject(hMemDisplay,DIBSection);
          
      Width = Info->bmiHeader.biWidth;
      Height = Info->bmiHeader.biHeight;
      if (Height<0)
        Height = -Height;
        
      Bits = BitSize;
      BytesPerPixel = Bits/8;
      BytesPerRow = Width*BytesPerPixel;
      if (Direction==IMAGE_TOPDOWN)
        RowInc = BytesPerRow;
      else
        RowInc = -BytesPerRow;  
      
      free ( Info );      
      if (DIBSection==NULL)
        {
          Destroy ();  
          return FAILURE;
        } 
        
      Attached = TRUE;
      SetViewPort ( 0, 0, Width-1, Height-1 );
      return SUCCESS;    
    #else
      return Create ( NewFormat, Wd, Ht );  
    #endif  
  } 

VOID IMAGE::Destroy ()
  {
    #if defined (__FORWIN32__)
      if (Attached)
        {
          if (hMemDisplay!=NULL)
            {
              SelectObject ( hMemDisplay, OldBitmap );
              DeleteDC ( hMemDisplay );  
            } 
          if (DIBSection!=NULL)
            {
              DeleteObject ( DIBSection );  
            } 
          hMemDisplay = NULL;
          DIBSection = NULL;
          Buffer = NULL;
          Attached = FALSE;  
        } 
    #endif
      
    #if defined (__FORDOS__)
      if (ForDebug)
        Buffer = NULL;
    #endif

    if (Buffer!=NULL)
      delete Buffer;
    Buffer = NULL;
    Width = Height = Bits = BytesPerRow = RowInc= 0;
    SetViewPort ( 0, 0, -1, -1 );
  } 

BYTE* IMAGE::SetOffset ( LONG x, LONG y )
  {
    if (Direction==IMAGE_TOPDOWN)
      {  
        Offset = y*BytesPerRow+x*BytesPerPixel;
      } 
    else
      {
        y = Height - y - 1;  
        Offset = y*BytesPerRow+x*BytesPerPixel;
      } 
    return Buffer+Offset;
  } 

BYTE* IMAGE::GetNextRow ( LONG RowDirection )
  {
    if (RowDirection==ROW_DOWN)
      Offset += RowInc;
    else
      Offset -= RowInc;   
    return Buffer+Offset;
  } 

VOID IMAGE::SetViewPort ( LONG x1, LONG y1, LONG x2, LONG y2 )
  {
    if (x1>x2)
      SwapValue ( &x1, &x2 );
    if (y1>y2)
      SwapValue ( &y1, &y2 );

    if (x1<0)
      x1 = 0;
    else if (x1>=Width)
      x1 = Width-1;

    if (x2<-1)
      x2 = -1;
    else if (x2>=Width)
      x2 = Width-1;

    if (y1<0)
      y1 = 0;
    else if (y1>=Height)
      y1 = Height-1;

    if (y2<0)
      y2 = 0;
    else if (y2>=Height)
      y2 = Height-1;

    ViewPort.x1 = x1;
    ViewPort.y1 = y1;
    ViewPort.x2 = x2;
    ViewPort.y2 = y2;
  } 

VOID IMAGE::Clear ( LONG Color )
  {
    if (Buffer==NULL)
      return;

    LONG Size;
    Size = Height*BytesPerRow;
    memset ( Buffer, Color, Size );
  } 

BOOLEAN IMAGE::IsCreated ()
  {
    if (Buffer==NULL)
      return FALSE; 
    return TRUE;  
  } 
