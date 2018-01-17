




#include "stdgfx.h"
#include "gfxpcx.h"

PCXFILETOOL::PCXFILETOOL ( FILEHANDLE f ) : IMAGEFILETOOL ( f )
  {
    DataBuffer = NULL;
  } 

PCXFILETOOL::~PCXFILETOOL ()
  {
    if (DataBuffer!=NULL)
      delete DataBuffer;
    DataBuffer = NULL;
  } 

BOOLEAN PCXFILETOOL::ReadHeader ()
  {
    BOOLEAN Ok;

    File.Seek ( fp, 0, FROM_BEGIN );

    Ok = File.Read ( fp, &PCXHeader, 128 );
    if (!Ok)
      return FAILURE;

    
    if (PCXHeader.ID!=0x0A)
      return FALSE;

    #if defined (__MSBFIRST__)
      SwapWord ( (WORD*)&(PCXHeader.XMin) );
      SwapWord ( (WORD*)&(PCXHeader.YMin) );
      SwapWord ( (WORD*)&(PCXHeader.XMax) );
      SwapWord ( (WORD*)&(PCXHeader.YMax) );
      SwapWord ( (WORD*)&(PCXHeader.XResolution) );
      SwapWord ( (WORD*)&(PCXHeader.YResolution) );
      SwapWord ( (WORD*)&(PCXHeader.BytesPerRow) );
      SwapWord ( (WORD*)&(PCXHeader.PalType) );
      SwapWord ( (WORD*)&(PCXHeader.XScreen) );
      SwapWord ( (WORD*)&(PCXHeader.YScreen) ); 
    #endif

    return TRUE;
  } 

BOOLEAN PCXFILETOOL::ReadImageRow2 ( IMAGE *Image, INT Row )
  {
    INT Count;
    INT RepCount;
    INT Ch;
    INT i;
    BYTE *Buffer;
    INT Size;

    Size = PCXHeader.BytesPerRow;
    Count = 0;
    Buffer = Image->SetOffset ( 0, Row );

    while (Count<Size)
      {
        Ch = File.GetCh ( fp );
        if (Ch==EOF)
          return FAILURE;
        if (Ch<192)
          RepCount = 1;
        else
          {
            RepCount = Ch - 192;
            Ch = File.GetCh ( fp );
            if (Ch==EOF)
              return FAILURE;
          } 
        for (i=0;i<RepCount;i++)
          {
            DataBuffer[Count] = (BYTE)Ch;
            Count++;
          } 
      } 

    INT Bit1;
    INT Off1;
    INT Mask;
    INT Byte;

    Off1 = 0;

    Mask = (LONG)1<<7;
    for (i=0;i<Image->GetWidth();i++)
      {
        Bit1 = (DataBuffer[Off1+(i)/8]&Mask)&&1;
        Byte = (Bit1<<0);
        Buffer[i] = (BYTE)Byte;

        if (Mask==1)
          Mask = (LONG)1<<7;
        else
          Mask >>= 1;
      } 

    return SUCCESS;
  } 

BOOLEAN PCXFILETOOL::ReadImageRow4 ( IMAGE *Image, INT Row )
  {
    INT Count;
    INT RepCount;
    INT Ch;
    INT i;
    BYTE *Buffer;
    INT Size;

    Size = PCXHeader.BytesPerRow*2;
    Count = 0;
    Buffer = Image->SetOffset ( 0, Row );

    while (Count<Size)
      {
        Ch = File.GetCh ( fp );
        if (Ch==EOF)
          return FAILURE;
        if (Ch<192)
          RepCount = 1;
        else
          {
            RepCount = Ch - 192;
            Ch = File.GetCh ( fp );
            if (Ch==EOF)
              return FAILURE;
          } 
        for (i=0;i<RepCount;i++)
          {
            DataBuffer[Count] = (BYTE)Ch;
            Count++;
          } 
      } 

    INT Bit1,Bit2;
    INT Off1,Off2;
    INT Mask;
    INT Byte;

    Off1 = 0;
    Off2 = PCXHeader.BytesPerRow;

    Mask = (LONG)1<<7;
    for (i=0;i<Image->GetWidth();i++)
      {
        Bit1 = (DataBuffer[Off1+(i)/8]&Mask)&&1;
        Bit2 = (DataBuffer[Off2+(i)/8]&Mask)&&1;
        Byte = (Bit1<<0)+(Bit2<<1);
        Buffer[i] = (BYTE)Byte;

        if (Mask==1)
          Mask = (LONG)1<<7;
        else
          Mask >>= 1;
      } 

    return SUCCESS;
  } 

BOOLEAN PCXFILETOOL::ReadImageRow16 ( IMAGE *Image, INT Row )
  {
    INT Count;
    INT RepCount;
    INT Ch;
    INT i;
    BYTE *Buffer;
    INT Size;

    Size = PCXHeader.BytesPerRow*4;
    Count = 0;
    Buffer = Image->SetOffset ( 0, Row );

    while (Count<Size)
      {
        Ch = File.GetCh ( fp );
        if (Ch==EOF)
          return FAILURE;
        if (Ch<192)
          RepCount = 1;
        else
          {
            RepCount = Ch - 192;
            Ch = File.GetCh ( fp );
            if (Ch==EOF)
              return FAILURE;
          } 
        for (i=0;i<RepCount;i++)
          {
            DataBuffer[Count] = (BYTE)Ch;
            Count++;
          } 
      } 

    INT Bit1,Bit2,Bit3,Bit4;
    INT Off1,Off2,Off3,Off4;
    INT Mask;
    INT Byte;

    Off1 = 0;
    Off2 = PCXHeader.BytesPerRow;
    Off3 = PCXHeader.BytesPerRow*2;
    Off4 = PCXHeader.BytesPerRow*3;

    Mask = (LONG)1<<7;
    for (i=0;i<Image->GetWidth();i++)
      {
        Bit1 = (DataBuffer[Off1+(i)/8]&Mask)&&1;
        Bit2 = (DataBuffer[Off2+(i)/8]&Mask)&&1;
        Bit3 = (DataBuffer[Off3+(i)/8]&Mask)&&1;
        Bit4 = (DataBuffer[Off4+(i)/8]&Mask)&&1;
        Byte = (Bit1<<0)+(Bit2<<1)+(Bit3<<2)+(Bit4<<3);
        Buffer[i] = (BYTE)Byte;

        if (Mask==1)
          Mask = (LONG)1<<7;
        else
          Mask >>= 1;
      } 

    return SUCCESS;
  } 

BOOLEAN PCXFILETOOL::ReadImageRow256 ( IMAGE *Image, INT Row )
  {
    INT Count;
    INT RepCount;
    INT Ch;
    INT i;
    BYTE *Buffer;

    Count = 0;
    Buffer = Image->SetOffset ( 0, Row );

    while (Count<PCXHeader.BytesPerRow)
      {
        Ch = File.GetCh ( fp );
        if (Ch==EOF)
          return FAILURE;
        if (Ch<192)
          RepCount = 1;
        else
          {
            RepCount = Ch - 192;
            Ch = File.GetCh ( fp );
            if (Ch==EOF)
              return FAILURE;
          } 
        if (Count<Image->GetWidth())
          {
            for (i=0;i<RepCount;i++)
              {
                Buffer[Count] = (BYTE)Ch;
                Count++;
              } 
          } 
        else
          {
            for (i=0;i<RepCount;i++)
              {
                Count++;
              } 
          } 
      } 

    return SUCCESS;
  } 

BOOLEAN PCXFILETOOL::ReadImage ( IMAGE *Image)
  {
    INT i;
    BOOLEAN Ok;

    File.Seek ( fp, 128, FROM_BEGIN );

    DataBuffer = NULL;
    DataBuffer = new BYTE[(PCXHeader.BytesPerRow+1)*PCXHeader.NumPlanes];

    for (i=0;i<Image->GetHeight();i++)
      {
        if (PCXHeader.BitsPerPixel==8)
          Ok = ReadImageRow256 ( Image, i );
        else if (PCXHeader.NumPlanes==4)
          Ok = ReadImageRow16 ( Image, i );
        else if (PCXHeader.NumPlanes==2)
          Ok = ReadImageRow4 ( Image, i );
        else if (PCXHeader.NumPlanes==1)
          Ok = ReadImageRow2 ( Image, i );
        if (!Ok)
          return FAILURE;
      } 

    if (DataBuffer!=NULL)  
      delete DataBuffer;
    DataBuffer = NULL;  

    return SUCCESS;
  } 

BOOLEAN PCXFILETOOL::ReadPalette ( RGBPALETTE *Pal )
  {
    INT i;
    if (Pal==NULL)
      return SUCCESS;

    RGBCOLOR *Entry;

    Entry = Pal->GetEntry ();

    if (PCXHeader.BitsPerPixel>=8)
      {
        File.Seek ( fp, -768, FROM_END );
        for (i=0;i<256;i++)
          {
            Entry[i].Red = (BYTE)File.GetCh ( fp );
            Entry[i].Green = (BYTE)File.GetCh ( fp );
            Entry[i].Blue = (BYTE)File.GetCh ( fp );
          } 
      } 
    else
      {
        for (i=0;i<16;i++)
          {
            Entry[i].Red = (BYTE)PCXHeader.Palette[i].Red;
            Entry[i].Red = (BYTE)PCXHeader.Palette[i].Green;
            Entry[i].Red = (BYTE)PCXHeader.Palette[i].Blue;
          } 
      } 
      
    return SUCCESS;
  } 

BOOLEAN PCXFILETOOL::LoadImage ( IMAGE *Image, RGBPALETTE *Pal  )
  {
    BOOLEAN Ok;

    if (Image==NULL)
      return FAILURE;

    Ok = ReadHeader ();
    if (!Ok)
      return FAILURE;

    INT Wd,Ht;

    Wd = PCXHeader.XMax - PCXHeader.XMin + 1;
    Ht = PCXHeader.YMax - PCXHeader.YMin + 1;

    Ok = Image->Create ( IMAGE_8BIT, Wd, Ht );
    if (!Ok)
      return FAILURE;

    Ok = ReadImage ( Image );
    if (!Ok)
      return FAILURE;

    Ok = ReadPalette ( Pal );
    if (!Ok)
      return FAILURE;

    return SUCCESS;
  } 

BOOLEAN PCXFILETOOL::WriteHeader ( INT Wd, INT Ht )
  {
    PCXHeader.ID = 0x0A;
    PCXHeader.Version = 5;
    PCXHeader.Encoding = 1;
    PCXHeader.BitsPerPixel = 8;
    PCXHeader.XMin = 0;
    PCXHeader.YMin = 0;
    PCXHeader.XMax = (SHORT)(Wd-1);
    PCXHeader.YMax = (SHORT)(Ht-1);
    PCXHeader.XResolution = 0;
    PCXHeader.YResolution = 0;
    PCXHeader.Reserved = 0;
    PCXHeader.NumPlanes = 1;
    if (Wd&0x01)
      PCXHeader.BytesPerRow = (SHORT)(Wd+1);
    else
      PCXHeader.BytesPerRow = (SHORT)(Wd);

    PCXHeader.PalType = 1;
    PCXHeader.XScreen = 0;
    PCXHeader.YScreen = 0;

    #if defined (__MSBFIRST__)
      SwapWord ( (WORD*)&(PCXHeader.XMin) );
      SwapWord ( (WORD*)&(PCXHeader.YMin) );
      SwapWord ( (WORD*)&(PCXHeader.XMax) );
      SwapWord ( (WORD*)&(PCXHeader.YMax) );
      SwapWord ( (WORD*)&(PCXHeader.XResolution) );
      SwapWord ( (WORD*)&(PCXHeader.YResolution) );
      SwapWord ( (WORD*)&(PCXHeader.BytesPerRow) );
      SwapWord ( (WORD*)&(PCXHeader.PalType) );
      SwapWord ( (WORD*)&(PCXHeader.XScreen) );
      SwapWord ( (WORD*)&(PCXHeader.YScreen) ); 
    #endif

    BOOLEAN Ok;

    File.Seek ( fp, 0, FROM_BEGIN );
    Ok = File.Write ( fp, &PCXHeader, 128 );
    if (!Ok)
      return FAILURE;
    return SUCCESS;
  } 

BOOLEAN PCXFILETOOL::WriteImageRow256 ( IMAGE *Image, INT Sx, INT Wd, INT WhichRow )
  {
    BYTE *Buffer;
    INT Ch;
    INT Count;
    INT RepCount;
    INT Size;

    Size = Wd;
    Buffer = Image->SetOffset ( Sx, WhichRow );
    Count = 0;

    while (Count<Size)
      {
        RepCount = 1;
        Ch = Buffer[Count];
        Count++;
        while ((Ch==Buffer[Count])&&(RepCount<63)&&(Count<Size))
          {
            Count++;
            RepCount++;
          } 
        if ((Ch<192)&&(RepCount==1))
          {
            File.PutCh ( fp, (CHAR)Ch );
          } 
        else
          {
            RepCount += 192;
            File.PutCh ( fp, (CHAR)RepCount );
            File.PutCh ( fp, (CHAR)Ch );
          } 
      } 

    if (Wd<PCXHeader.BytesPerRow)
      File.PutCh ( fp, 0 );

    return SUCCESS;
  } 

BOOLEAN PCXFILETOOL::WriteImage ( IMAGE *Image, INT Sx, INT Sy, INT Wd, INT Ht  )
  {
    INT i;
    BOOLEAN Ok;

    for (i=0;i<Ht;i++)
      {
        Ok = WriteImageRow256 ( Image, Sx, Wd, Sy+i );
        if (!Ok)
          return FAILURE;
      } 
    return SUCCESS;
  } 

BOOLEAN PCXFILETOOL::WritePalette ( RGBPALETTE *Pal )
  {
    RGBCOLOR *Entry;

    File.PutCh ( fp, 0x0C );

    if (Pal==NULL)
      {
        return FAILURE;
      } 
    else
      {
        Entry = Pal->GetEntry ();

        INT i;
        for (i=0;i<256;i++)
          {
            File.PutCh ( fp, Entry[i].Red );
            File.PutCh ( fp, Entry[i].Green );
            File.PutCh ( fp, Entry[i].Blue );
          } 
      } 
    return SUCCESS;
  } 

BOOLEAN PCXFILETOOL::SaveImage ( IMAGE *Image, LONG Sx, LONG Sy,
                                 LONG Wd, LONG Ht, RGBPALETTE *Pal  )
  {
    if (Image==NULL)
      {
        return FAILURE;
      } 

    BOOLEAN Ok;

    Ok = WriteHeader ( Wd, Ht );
    if (!Ok)
      return FAILURE;

    Ok = WriteImage ( Image, Sx, Sy, Wd, Ht );
    if (!Ok)
      return FAILURE;

    Ok = WritePalette ( Pal );
    if (!Ok)
      return FAILURE;

    return SUCCESS;
  } 

