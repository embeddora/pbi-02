




#include "stdgfx.h"
#include "gfxflic.h"

#define ID_PREFIX 0xF100
#define ID_FRAME  0xF1FA
#define ID_FRAME_NODATA  0x00A1

FLICFILE::FLICFILE () : MYOBJECT ()
  {
    FLICHeader.FLI.NumFrames = 0;
    FLICHeader.FLI.Width = 0;
    FLICHeader.FLI.Height = 0;
    FrameData = NULL;
    f = NULL;
    CurrentFrame = 0;
    Image = NULL;
    Pal = new RGBPALETTE ();
    PalChange = FALSE;
    hDisplay = NULL;
    Speed = FLIC_SPEED_DEFAULT;
    Loop = FLIC_LOOP_INFINITE;
    DefaultSpeed = 0;
  } 

FLICFILE::~FLICFILE ()
  {
    DestroyFrames ();
    
    if (Image!=NULL)
      delete Image;
    Image = NULL;
    
    if (Pal!=NULL)
      delete Pal;
    Pal = NULL;  
  } 

BOOLEAN FLICFILE::ReadHeader ()
  {
    if (File.Read ( f, &FLICHeader, 128 )==FAILURE)
      return FAILURE;

    #if defined (__MSBFIRST__)
      SwapDWord ( (DWORD*)&FLICHeader.FLI.Size );
      SwapWord ( (WORD*)&FLICHeader.FLI.ID );
      SwapWord ( (WORD*)&FLICHeader.FLI.NumFrames );
      SwapWord ( (WORD*)&FLICHeader.FLI.Width );
      SwapWord ( (WORD*)&FLICHeader.FLI.Height );
      SwapWord ( (WORD*)&FLICHeader.FLI.Reserved1 );
      SwapWord ( (WORD*)&FLICHeader.FLI.Flag );
      SwapDWord ( (DWORD*)&FLICHeader.FLC.Speed );
      SwapWord ( (WORD*)&FLICHeader.FLC.Unused );
      SwapDWord ( (DWORD*)&FLICHeader.FLC.Created );
      SwapDWord ( (DWORD*)&FLICHeader.FLC.Creator );
      SwapDWord ( (DWORD*)&FLICHeader.FLC.Updated );
      SwapDWord ( (DWORD*)&FLICHeader.FLC.Updator );
      SwapWord ( (WORD*)&FLICHeader.FLC.AspectX );
      SwapWord ( (WORD*)&FLICHeader.FLC.AspectY );
      SwapDWord ( (DWORD*)&FLICHeader.FLC.Offset1 );
      SwapDWord ( (DWORD*)&FLICHeader.FLC.Offset2 );
    #endif

    if (FLICHeader.FLI.ID==(SHORT)ID_FLI)
      IsFLC = FALSE;
    else if (FLICHeader.FLI.ID==(SHORT)ID_FLC)
      IsFLC = TRUE;
    else
      return FAILURE;
    return SUCCESS;
  } 

BOOLEAN FLICFILE::ReadChunk ( CHUNK_DATA *Chunk )
  {
    BOOLEAN Ok;
    DWORD Size;

    if (File.Read ( f, &(Chunk->Header), 6 )==FAILURE)
      return FAILURE;

    #if defined (__MSBFIRST__)
      SwapDWord ( (DWORD*)&(Chunk->Header.Size) );
      SwapWord ( (DWORD*)&(Chunk->Header.Type) );
    #endif

    Size = Chunk->Header.Size - 6;

    Chunk->Data = new BYTE [Size];
    if (Chunk->Data==NULL)
      return FAILURE;

    if (File.Read ( f, Chunk->Data, Size )==FAILURE)
      return FAILURE;

    return Ok;
  } 

BOOLEAN FLICFILE::ReadFrame ( LONG WhichFrame )
  {
    FRAMEHEADER FrameHeader;
    
    if (File.Read ( f, &FrameHeader, 16 )==FAILURE)
      return FAILURE;

    #if defined (__MSBFIRST__)
      SwapDWord ( (DWORD*)&(FrameHeader.Size) );
      SwapWord ( (WORD*)&(FrameHeader.ID) );
      SwapWord ( (WORD*)&(FrameHeader.NumChunks) );
    #endif

    if (FrameHeader.ID==(SHORT)ID_PREFIX) 
      {
        File.Seek ( f, FrameHeader.Size-16, FROM_CURRENT );
        if (File.Read ( f, &FrameHeader, 16 )==FAILURE)
          return FAILURE;
      } 
    else if (FrameHeader.ID==(SHORT)ID_FRAME_NODATA) 
      {
        File.Seek ( f, FrameHeader.Size-16, FROM_CURRENT );
        if (File.Read ( f, &FrameHeader, 16 )==FAILURE)
          return FAILURE;
      } 
    else if (FrameHeader.ID!=(SHORT)ID_FRAME)
      return FAILURE;

    FrameData[WhichFrame].NumChunks = FrameHeader.NumChunks;
    FrameData[WhichFrame].Chunks = new CHUNK_DATA [FrameHeader.NumChunks];
    if (FrameData[WhichFrame].Chunks==NULL)
      return FAILURE;

    LONG i;
    for (i=0;i<FrameHeader.NumChunks;i++)
      {
        FrameData[WhichFrame].Chunks[i].Data = NULL;
      } 

    BOOLEAN Ok;

    for (i=0;i<FrameHeader.NumChunks;i++)
      {
        Ok = ReadChunk ( &(FrameData[WhichFrame].Chunks[i]) );
        if (!Ok)
          return FAILURE;
      } 

    return TRUE;
  } 

VOID FLICFILE::DestroyFrames ()
  {
    INT i,j;

    if (FrameData!=NULL)
      {
        for (i=0;i<FLICHeader.FLI.NumFrames;i++)
          {
            for (j=0;j<FrameData[i].NumChunks;j++)
              {
                if (FrameData[i].Chunks[j].Data!=NULL)
                  delete FrameData[i].Chunks[j].Data;
                FrameData[i].Chunks[j].Data=NULL;
              } 
            if (FrameData[i].Chunks!=NULL)
              delete FrameData[i].Chunks;
            FrameData[i].Chunks=NULL;
          } 

        delete FrameData;
      } 
    FrameData = NULL;
  } 

BOOLEAN FLICFILE::SetDisplay ( HDISPLAY Display )
  {
    if (Image!=NULL)
      delete Image;
    Image = NULL;  
    hDisplay = Display;  
    if (FLICHeader.FLI.NumFrames)
      {
        #if defined (__FORWIN32__)
          GetFirstPalette ();
          LOGPALETTE *LogPal;
          RGBCOLOR *Entry;
          HPALETTE hPal,OldPal;      

          Entry = Pal->GetEntry ();

          LogPal = (LOGPALETTE*)malloc(sizeof(LOGPALETTE)+
                                        sizeof(RGBQUAD)*256);
          if (LogPal==NULL)
            return FAILURE;

          INT i;
          LogPal->palVersion = 0x300;
          LogPal->palNumEntries = 256;
          for (i=0;i<256;i++)
            {
              LogPal->palPalEntry[i].peRed = Entry[i].Red;  
              LogPal->palPalEntry[i].peGreen = Entry[i].Green;  
              LogPal->palPalEntry[i].peBlue = Entry[i].Blue;
              LogPal->palPalEntry[i].peFlags = 0;              
            } 
            
          hPal = CreatePalette ( (LOGPALETTE*)LogPal );     
          OldPal = SelectPalette ( hDisplay, hPal, FALSE );
          RealizePalette ( hDisplay );
          Image = new IMAGE ( IMAGE_BOTTOMUP );
          if (Image->Create ( hDisplay, IMAGE_8BIT, FLICHeader.FLI.Width, FLICHeader.FLI.Height )==FAILURE)
            return FAILURE;
            
          SelectPalette ( hDisplay, OldPal, FALSE );
          RealizePalette ( hDisplay );
          DeleteObject ( hPal );        
          free ( LogPal );
        #elif defined (__FORWINDOWS__)
          Image = new IMAGE ( IMAGE_BOTTOMUP );
          if (Image->Create ( IMAGE_8BIT, FLICHeader.FLI.Width, FLICHeader.FLI.Height )==FAILURE)
            return FAILURE;
        #else  
          Image = new IMAGE ( IMAGE_TOPDOWN );
          if (Image->Create ( IMAGE_8BIT, FLICHeader.FLI.Width, FLICHeader.FLI.Height )==FAILURE)
            return FAILURE;
        #endif
        
        LONG Frame=CurrentFrame;
        CurrentFrame = 0;
        SetFrame ( Frame );
      } 
    return SUCCESS;  
  } 

BOOLEAN FLICFILE::CreateImage ()
  {
    if (FLICHeader.FLI.NumFrames==0)
      return FAILURE;
      
    Image = new IMAGE ( IMAGE_TOPDOWN );
    if (Image->Create ( IMAGE_8BIT, FLICHeader.FLI.Width, FLICHeader.FLI.Height )==FAILURE)
      return FAILURE;
      
    SetFrame ( 0 );
    return SUCCESS;
  } 

BOOLEAN FLICFILE::Load ( STRING FileName )
  {
    BOOLEAN Ok;

    if (Image!=NULL)
      delete Image;
    Image = NULL;
      
    if (f!=NULL)
      File.Close ( f );
      
    f = File.Open ( FileName, OPEN_READ | OPEN_BINARY );
    if (f==NULL)
      return FAILURE;
      
    Ok = ReadHeader ();
    if (!Ok)
      {
        File.Close ( f );
        return FAILURE;
      } 

    if (FrameData!=NULL)
      delete FrameData;

    FrameData = new FRAME_DATA [FLICHeader.FLI.NumFrames];
    if (FrameData==NULL)
      {
        File.Close ( f );
        return FAILURE;
      } 

    LONG i,NumFrames;

    NumFrames = FLICHeader.FLI.NumFrames;

    for (i=0;i<NumFrames;i++)
      {
        FrameData[i].Chunks = NULL;
      } 

    for (i=0;i<NumFrames;i++)
      {
        Ok = ReadFrame ( i );
        if (!Ok)
          {
            File.Close ( f );
            return FAILURE;
          } 
      } 

    DefaultSpeed = FLICHeader.FLI.Speed;
    File.Close ( f );

    CreateImage ();    
    return SUCCESS;
  } 

BOOLEAN FLICFILE::DoColor ( RGBPALETTE *Pal, CHUNK_DATA* Chunk, BOOLEAN Is256 )
  {
    INT i,j,Index;
    SHORT NumChanges;
    BYTE *Data;
    DWORD Offset;
    RGBCOLOR *Entry;

    if (Pal==NULL)
      return FAILURE;

    Entry = Pal->GetEntry ();
      
    Index = 0;
    Offset = 0;
    Data = Chunk->Data;
    NumChanges = (*(SHORT*)(Data+Offset));

    #if defined (__MSBFIRST__)
      SwapWord ( (WORD*)&NumChanges );
    #endif

    Offset+=2;

    INT Count;

    for (i=0;i<NumChanges;i++)
      {
        Index += Data[Offset++];
        Count = Data[Offset++];

        if (Count==0)
          Count = 256;

        for (j=0;j<Count;j++)
          {
            if (Is256)
              {
                Entry[Index].Red = Data[Offset++];
                Entry[Index].Green = Data[Offset++];
                Entry[Index].Blue = Data[Offset++];
              } 
            else
              {
                Entry[Index].Red = (BYTE)(Data[Offset++]<<2);
                Entry[Index].Green = (BYTE)(Data[Offset++]<<2);
                Entry[Index].Blue = (BYTE)(Data[Offset++]<<2);
              } 

            Index++;
          } 
        if (Count&0x01)
          Offset++; 
      } 

    PalChange = TRUE;
    return SUCCESS;
  } 

BOOLEAN FLICFILE::DoLine ( IMAGE *Image, CHUNK_DATA* Chunk )
  {
    SHORT SkipLines,ChangeLines;
    DWORD Offset;
    BYTE *Data;

    Data = Chunk->Data;
    Offset = 0;

    SkipLines = (*(SHORT*)(Data+Offset));
    Offset += 2;
    ChangeLines = (*(SHORT*)(Data+Offset));
    Offset += 2;

    #if defined (__MSBFIRST__)
      SwapWord ( (WORD*)&SkipLines );
      SwapWord ( (WORD*)&ChangeLines );
    #endif

    INT i,j,k;
    BYTE *Buffer;
    LONG BuffOffset;
    INT Count;
    INT SkipPixel;

    Buffer = Image->GetBuffer ();
    
    for (i=0;i<SkipLines;i++)
      Buffer = Image->GetNextRow ( ROW_DOWN );

    for (i=0;i<ChangeLines;i++)
      {
        BuffOffset = 0;
        INT NumPackets;

        NumPackets = Data[Offset++];


        for (j=0;j<NumPackets;j++)
          {
            SkipPixel = Data[Offset++];
            BuffOffset += SkipPixel;

            Count = Data[Offset++];
            if (Count<128)
              {
                for (k=0;k<Count;k++)
                  Buffer[BuffOffset++] = Data[Offset++];
              } 
            else
              {
                Count = 256 - Count;
                BYTE Color;
                Color = Data[Offset++];
                for (k=0;k<Count;k++)
                  Buffer[BuffOffset++] = Color;
              } 
          } 

        Buffer = Image->GetNextRow ( ROW_DOWN );
      } 

    return SUCCESS;
  } 

BOOLEAN FLICFILE::DoLineWord ( IMAGE *Image, CHUNK_DATA* Chunk )
  {
    SHORT LineCount,Content;
    DWORD Offset;
    BYTE *Data;

    Data = Chunk->Data;
    Offset = 0;

    LineCount = (*(SHORT*)(Data+Offset));

    #if defined (__MSBFIRST__)
      SwapWord ( (WORD*)&LineCount );
    #endif

    Offset += 2;

    INT i,j,k;
    BYTE *Buffer;
    LONG BuffOffset;
    INT Count;
    INT SkipPixel;
    SHORT SkipLine;

    Buffer =Image->GetBuffer ();

    BOOLEAN Done;
    INT NumPackets;

    for (i=0;i<LineCount;i++)
      {
        BuffOffset = 0;

        Content = (*(SHORT*)(Data+Offset));
        #if defined (__MSBFIRST__)
          SwapWord ( (WORD*)&Content );
        #endif

        Offset += 2;

        Done = FALSE;
        while (!Done)
          {
            if ((Content&0xC000)==0xC000) 
              {
                SkipLine = (SHORT)(-Content);
                for (j=0;j<SkipLine;j++)
                  Buffer = Image->GetNextRow ( ROW_DOWN );

                Content = (*(SHORT*)(Data+Offset));
                #if defined (__MSBFIRST__)
                  SwapWord ( (WORD*)&Content );
                #endif

                Offset += 2;
              } 
            else if ((Content&0xC000)==0x00) 
              {
                NumPackets = Content;
                Done = TRUE;
              } 
            else if ((Content&0x8000)==0x8000) 
              {
                Content &= 0xFFFF;
                Done = TRUE;
                NumPackets = 0;
              } 
            else 
              {
                return FAILURE;
              } 
          } 

        for (j=0;j<NumPackets;j++)
          {
            SkipPixel = Data[Offset++];
            BuffOffset += SkipPixel;

            Count = Data[Offset++];
            if (Count<128)
              {
                for (k=0;k<Count;k++)
                  {
                    Buffer[BuffOffset++] = Data[Offset++];
                    Buffer[BuffOffset++] = Data[Offset++];
                  } 
              } 
            else
              {
                Count = 256 - Count;
                BYTE Color1,Color2;
                Color1 = Data[Offset++];
                Color2 = Data[Offset++];
                for (k=0;k<Count;k++)
                  {
                    Buffer[BuffOffset++] = Color1;
                    Buffer[BuffOffset++] = Color2;
                  } 
              } 
          } 

        Buffer = Image->GetNextRow ( ROW_DOWN );
      } 
    return SUCCESS;
  } 

BOOLEAN FLICFILE::DoBRun ( IMAGE *Image, CHUNK_DATA* Chunk )
  {
    DWORD Offset;
    BYTE *Data;

    Data = Chunk->Data;
    Offset = 0;

    INT i,j,k;
    BYTE *Buffer;
    LONG BuffOffset;
    INT Count;

    Buffer = Image->SetOffset ( 0, 0 );

    for (i=0;i<Image->GetHeight();i++)
      {
        INT NumPackets;

        BuffOffset = 0;

        NumPackets = Data[Offset++];

        for (j=0;j<NumPackets;j++)
          {
            Count = Data[Offset++];
            BYTE Color;
            if (Count<128)
              {
                Color = Data[Offset++];
                for (k=0;k<Count;k++)
                  Buffer[BuffOffset++] = Color;
              } 
            else
              {
                Count = 256 - Count;
                for (k=0;k<Count;k++)
                  Buffer[BuffOffset++] = Data[Offset++];
              } 
          } 

        Buffer = Image->GetNextRow ( ROW_DOWN );
      } 

    return SUCCESS;
  } 

BOOLEAN FLICFILE::DoCopy ( IMAGE *Image, CHUNK_DATA* Chunk )
  {
    BYTE *Buffer;

    Buffer = Image->SetOffset ( 0, 0 );
    memcpy ( Buffer, Chunk->Data, Image->GetWidth()*Image->GetHeight() );
    
    return SUCCESS;
  } 

BOOLEAN FLICFILE::DoBlack ( IMAGE *Image, CHUNK_DATA* Chunk )
  {
    Image->Clear ( 0 );
    
    if (Chunk)
      {}
    return SUCCESS;
  } 

BOOLEAN FLICFILE::DoPreview ( IMAGE *Image, CHUNK_DATA* Chunk )
  {
    if ((Image)&&(Chunk))
      {}
    return SUCCESS;
  } 

VOID FLICFILE::GetFirstPalette ()
  {
    LONG i;  
    for (i=0;i<FrameData[0].NumChunks;i++)
      {
        LONG ChunkType = FrameData[0].Chunks[i].Header.Type;
        switch (ChunkType)
          {
            case FLI_COLOR :
              DoColor ( Pal, &(FrameData[0].Chunks[i]), FALSE );
              break;

            case FLI_COLOR_256 :
              DoColor ( Pal, &(FrameData[0].Chunks[i]), TRUE );
              break;
          } 
      } 
  } 

VOID FLICFILE::SetFrame ( LONG FrameNumber )
  {
    if (FrameNumber>=FLICHeader.FLI.NumFrames)
      FrameNumber=FLICHeader.FLI.NumFrames-1;
    if (FrameNumber<0)
      FrameNumber = 0;

    if (FrameNumber>CurrentFrame)
      {
        while (CurrentFrame<FrameNumber)
          PlayFrame ( TRUE );
      } 
    else if (FrameNumber<CurrentFrame)
      {
        CurrentFrame = 0;  
        while (CurrentFrame<FrameNumber)  
          PlayFrame ( TRUE );
      } 
  } 

BOOLEAN FLICFILE::PlayFrame ( BOOLEAN Advance )
  {
    LONG i;
    BOOLEAN Ok;
    SHORT ChunkType;

    if (FLICHeader.FLI.NumFrames==0)
      return FAILURE;

    if (Image==NULL)
      {
        if (SetDisplay ( hDisplay )==FAILURE)
          return FAILURE;
      } 
        
    if (CurrentFrame>=FLICHeader.FLI.NumFrames)
      CurrentFrame = 0;
      
    Image->SetOffset ( 0, 0 );

    for (i=0;i<FrameData[CurrentFrame].NumChunks;i++)
      {
        ChunkType = FrameData[CurrentFrame].Chunks[i].Header.Type;
        switch (ChunkType)
          {
            case FLI_COLOR :
              Ok = DoColor ( Pal, &(FrameData[CurrentFrame].Chunks[i]), FALSE );
              break;

            case FLI_COLOR_256 :
              Ok = DoColor ( Pal, &(FrameData[CurrentFrame].Chunks[i]), TRUE );
              break;
              
            case FLI_LC :
              Ok = DoLine ( Image, &(FrameData[CurrentFrame].Chunks[i]) );
              break;

            case FLI_WORD_LC :
              Ok = DoLineWord ( Image, &(FrameData[CurrentFrame].Chunks[i]) );
              break;

            case FLI_BRUN :
              Ok = DoBRun ( Image, &(FrameData[CurrentFrame].Chunks[i]) );
              break;

            case FLI_COPY :
              Ok = DoCopy ( Image, &(FrameData[CurrentFrame].Chunks[i]) );
              break;

            case FLI_BLACK :
              Ok = DoBlack ( Image, &(FrameData[CurrentFrame].Chunks[i]) );
              break;

            case FLI_PREVIEW :
              Ok = DoPreview ( Image, &(FrameData[CurrentFrame].Chunks[i]) );
              break;

            default :
              Ok = FAILURE;
              break;
          } 
        if (!Ok)
          return FAILURE;
      } 

    if (Advance)   
      CurrentFrame++;
    return SUCCESS;
  } 

VOID FLICFILE::ConvertColor ( CHUNK_DATA* Chunk, BOOLEAN Is256, COLORTABLE *Table )
  {
    INT i,j,Index;
    SHORT NumChanges;
    BYTE *Data;
    DWORD Offset;
    RGBCOLOR *Entry;

    if (Pal==NULL)
      return;

    Entry = (Table->GetPalette())->GetEntry();
      
    Index = 0;
    Offset = 0;
    Data = Chunk->Data;
    NumChanges = (*(SHORT*)(Data+Offset));

    #if defined (__MSBFIRST__)
      SwapWord ( (WORD*)&NumChanges );
    #endif

    Offset+=2;

    INT Count;

    for (i=0;i<NumChanges;i++)
      {
        Index += Data[Offset++];
        Count = Data[Offset++];

        if (Count==0)
          Count = 256;

        for (j=0;j<Count;j++)
          {
            if (Is256)
              {
                Data[Offset++] = Entry[Index].Red;
                Data[Offset++] = Entry[Index].Green;
                Data[Offset++] = Entry[Index].Blue;
              } 
            else
              {
                Data[Offset++] = (BYTE)((Entry[Index].Red)>>2);
                Data[Offset++] = (BYTE)((Entry[Index].Green)>>2);
                Data[Offset++] = (BYTE)((Entry[Index].Blue)>>2);
              } 

            Index++;
          } 
        if (Count&0x01)
          Offset++; 
      } 
  } 

VOID FLICFILE::ConvertLine ( CHUNK_DATA* Chunk, COLORTABLE *Table )
  {
    SHORT SkipLines,ChangeLines;
    DWORD Offset;
    BYTE *Data;

    Data = Chunk->Data;
    Offset = 0;

    SkipLines = (*(SHORT*)(Data+Offset));
    Offset += 2;
    ChangeLines = (*(SHORT*)(Data+Offset));
    Offset += 2;

    #if defined (__MSBFIRST__)
      SwapWord ( (WORD*)&SkipLines );
      SwapWord ( (WORD*)&ChangeLines );
    #endif

    INT i,j,k;
    INT Count;
    INT SkipPixel;
    BYTE Color;
    BYTE *TableEntry = Table->GetTable ();

    for (i=0;i<SkipLines;i++)
      {}

    for (i=0;i<ChangeLines;i++)
      {
        INT NumPackets;

        NumPackets = Data[Offset++];


        for (j=0;j<NumPackets;j++)
          {
            SkipPixel = Data[Offset++];

            Count = Data[Offset++];
            if (Count<128)
              {
                for (k=0;k<Count;k++)
                  {
                    Color = Data[Offset];
                    Data[Offset] = TableEntry[Color];
                    Offset++;  
                  } 
              } 
            else
              {
                Count = 256 - Count;
                Color = Data[Offset];
                Data[Offset] = TableEntry[Color];
                Offset++;  
              } 
          } 
      } 
  } 

VOID FLICFILE::ConvertLineWord ( CHUNK_DATA* Chunk, COLORTABLE *Table )
  {
    SHORT LineCount,Content;
    DWORD Offset;
    BYTE *Data;

    Data = Chunk->Data;
    Offset = 0;

    LineCount = (*(SHORT*)(Data+Offset));

    #if defined (__MSBFIRST__)
      SwapWord ( (WORD*)&LineCount );
    #endif

    Offset += 2;

    INT i,j,k;
    INT Count;
    INT SkipPixel;
    SHORT SkipLine;

    BOOLEAN Done;
    INT NumPackets;
    BYTE Color;
    BYTE *TableEntry = Table->GetTable ();

    for (i=0;i<LineCount;i++)
      {
        Content = (*(SHORT*)(Data+Offset));
        #if defined (__MSBFIRST__)
          SwapWord ( (WORD*)&Content );
        #endif

        Offset += 2;

        Done = FALSE;
        while (!Done)
          {
            if ((Content&0xC000)==0xC000) 
              {
                SkipLine = (SHORT)(-Content);
                for (j=0;j<SkipLine;j++)
                  {}

                Content = (*(SHORT*)(Data+Offset));
                #if defined (__MSBFIRST__)
                  SwapWord ( (WORD*)&Content );
                #endif

                Offset += 2;
              } 
            else if ((Content&0xC000)==0x00) 
              {
                NumPackets = Content;
                Done = TRUE;
              } 
            else if ((Content&0x8000)==0x8000) 
              {
                Content &= 0xFFFF;
                Done = TRUE;
                NumPackets = 0;
              } 
            else 
              {
                return;
              } 
          } 

        for (j=0;j<NumPackets;j++)
          {
            SkipPixel = Data[Offset++];

            Count = Data[Offset++];
            if (Count<128)
              {
                for (k=0;k<Count;k++)
                  {
                    Color = Data[Offset];
                    Data[Offset] = TableEntry[Color];
                    Offset++;
                    
                    Color = Data[Offset];
                    Data[Offset] = TableEntry[Color];
                    Offset++;
                  } 
              } 
            else
              {
                Count = 256 - Count;
                Color = Data[Offset];
                Data[Offset] = TableEntry[Color];
                Offset++;
                    
                Color = Data[Offset];
                Data[Offset] = TableEntry[Color];
                Offset++;
              } 
          } 
      } 
  } 

VOID FLICFILE::ConvertBRun ( CHUNK_DATA* Chunk, COLORTABLE *Table )
  {
    LONG i,j,k;
    BYTE *Data = Chunk->Data;
    LONG Offset = 0;
    INT Count;
    BYTE *TableEntry = Table->GetTable ();
      
    for (i=0;i<GetImageHeight();i++)
      {
        INT NumPackets;

        NumPackets = Data[Offset++];

        for (j=0;j<NumPackets;j++)
          {
            Count = Data[Offset++];
            BYTE Color;
            if (Count<128)
              {
                Color = Data[Offset];
                Data[Offset] = TableEntry[Color];
                Offset++;
              } 
            else
              {
                Count = 256 - Count;
                for (k=0;k<Count;k++)
                  {
                    Color = Data[Offset];  
                    Data[Offset] = TableEntry[Color];
                    Offset++;
                  } 
              } 
          } 
      } 
  } 

VOID FLICFILE::ConvertCopy ( CHUNK_DATA* Chunk, COLORTABLE *Table )
  {
    LONG i;
    BYTE *TableEntry = Table->GetTable ();
    BYTE *ChunkData;
    BYTE Color;

    ChunkData = Chunk->Data;
    
    for (i=0;i<GetImageWidth()*GetImageHeight();i++)
      {
        Color = *ChunkData;
        *ChunkData = TableEntry[Color];
        ChunkData++;
      } 
  } 

VOID FLICFILE::ConvertBlack ( CHUNK_DATA* Chunk, COLORTABLE *Table )
  {
    if (Chunk)
      {}
    if (Table)
      {}    
  } 

VOID FLICFILE::ConvertPreview ( CHUNK_DATA* Chunk, COLORTABLE *Table )
  {
    if (Chunk)
      {}
    if (Table)
      {}    
  } 

VOID FLICFILE::Convert ( RGBPALETTE *NewPal )
  {
    if (FLICHeader.FLI.NumFrames==0)
      return;

    GetFirstPalette ();
    
    COLORTABLE *MatchTable = new COLORTABLE ();
    MatchTable->CopyPalette ( NewPal );
    MatchTable->CreateMatchTable ( Pal );

    LONG i,j;
    LONG ChunkType;
    
    for (i=0;i<FLICHeader.FLI.NumFrames;i++)
      {
        for (j=0;j<FrameData[i].NumChunks;j++)
          {
            ChunkType = FrameData[i].Chunks[j].Header.Type;
            switch (ChunkType)
              {
                case FLI_COLOR :
                  ConvertColor ( &(FrameData[i].Chunks[j]), FALSE, MatchTable );
                  break;

                case FLI_COLOR_256 :
                  ConvertColor ( &(FrameData[i].Chunks[j]), TRUE, MatchTable );
                  break;
              
                case FLI_LC :
                  ConvertLine ( &(FrameData[i].Chunks[j]), MatchTable );
                  break;

                case FLI_WORD_LC :
                  ConvertLineWord ( &(FrameData[i].Chunks[j]), MatchTable );
                  break;

                case FLI_BRUN :
                  ConvertBRun ( &(FrameData[i].Chunks[j]), MatchTable );
                  break;

                case FLI_COPY :
                  ConvertCopy ( &(FrameData[i].Chunks[j]), MatchTable );
                  break;

                case FLI_BLACK :
                  ConvertBlack ( &(FrameData[i].Chunks[j]), MatchTable );
                  break;

                case FLI_PREVIEW :
                  ConvertPreview ( &(FrameData[i].Chunks[j]), MatchTable );
                  break;
                  
              } 
          } 
      } 

    GetFirstPalette ();
    delete MatchTable;  
  } 

