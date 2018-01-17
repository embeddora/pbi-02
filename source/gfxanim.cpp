




#include "stdgfx.h"
#include "gfxanim.h"
#include "gfxfiles.h"

ANIMIMAGE::ANIMIMAGE () : IMAGE ( IMAGE_TOPDOWN )
  {
    NumFrames = 0;
    Frames = NULL;
    BaseImage = NULL;
  } 

ANIMIMAGE::~ANIMIMAGE ()
  {
    Destroy ();
  } 

BOOLEAN ANIMIMAGE::CreateFrames ( LONG Num )
  {
    DestroyFrames ();
    Frames = new ANIMFRAME [Num];
    if (Frames==NULL)
      return FAILURE;
      
    NumFrames = Num;
    
    LONG i;
    for (i=0;i<NumFrames;i++)  
      {
        Frames[i].NumChunks = 0;
        Frames[i].Size = 0;
        Frames[i].Chunks = NULL;
      } 
    return SUCCESS;
  } 

VOID ANIMIMAGE::DestroyFrame ( ANIMFRAME *Frame )
  {
    if (Frame->Chunks!=NULL)
      {
        delete Frame->Chunks;  
      } 
    Frame->Chunks = NULL;
  } 
  
VOID ANIMIMAGE::DestroyFrames ()
  {
    if (Frames!=NULL)
      {
        LONG i;
        for (i=0;i<NumFrames;i++)
          {
            DestroyFrame ( &(Frames[i]) );
          } 
        delete Frames;  
      } 
         Frames = NULL;
  } 

VOID ANIMIMAGE::Destroy ()
  {
    IMAGE::Destroy ();
    DestroyBaseImage ();    
    DestroyFrames ();
  } 

BOOLEAN ANIMIMAGE::CreateBaseImage ()
  {
    LONG Size;
    Size = BytesPerRow*Height;
    if (Size<=0)
      return FAILURE;
      
    BaseImage = new BYTE [(LONG)Size];
    if (BaseImage==NULL)
      return FAILURE;
    memset ( BaseImage, 0, Size );
    return SUCCESS;  
  } 

VOID ANIMIMAGE::DestroyBaseImage ()
  {
    if (BaseImage!=NULL)
      delete BaseImage;
    BaseImage = NULL;    
  } 

VOID ANIMIMAGE::SetBaseImage ( IMAGE *Image, LONG Sx, LONG Sy )
  {
    if (BaseImage==NULL)
      return;

    LONG i;
    BYTE *SrcBuffer;

    for (i=0;i<Height;i++)
      {
        SetOffset ( 0, i );
        SrcBuffer = Image->SetOffset ( Sx, Sy+i );
        memcpy ( BaseImage+Offset, SrcBuffer, BytesPerRow );
      } 
  } 
  
VOID ANIMIMAGE::RecordFrame ( LONG FrameNumber, IMAGE *Image, LONG Sx, LONG Sy )
  {
    if ((FrameNumber<0)||(FrameNumber>=NumFrames))
      return;
    if (BaseImage==NULL)
      return;  

    if (FrameNumber==0)
      {
        SetBaseImage ( Image, Sx, Sy );  
        return;
      } 
        
    ANIMFRAME *Frame;
    Frame = &(Frames[FrameNumber]);  
    DestroyFrame ( Frame );
        
    BYTE *TempSrcBuffer;
    BYTE *ChunkBuffer;
    BYTE *DataBuffer;

    LONG Size;
    Size = BytesPerRow*Height;

    TempSrcBuffer = new BYTE [Size];
    DataBuffer = new BYTE [Size*2];
    ChunkBuffer = new BYTE [(LONG)0xFFFF];

    
    LONG i;
    LONG Offset;
    Offset = 0;
    for (i=0;i<Height;i++)
      {
        memcpy ( TempSrcBuffer+Offset, Image->SetOffset ( Sx, Sy+i ), BytesPerRow );
        Offset += BytesPerRow;
      } 
    
    LONG Count;
    LONG cCount;    
    LONG SkipCount;
    LONG DumpCount;
    LONG NumChunks;
    BYTE CurPix,SrcPix;    

    Count=0;
    CurPix = BaseImage[Count];
    SrcPix = TempSrcBuffer[Count];
    Count++;

    cCount = 0;
    NumChunks = 0;
    
    while (Count<Size)
      {
        if (CurPix==SrcPix)
          {
            SkipCount=0;  
            while ((CurPix==SrcPix)&&(Count<Size)&(SkipCount<(LONG)0x7FFF))
              {
                SkipCount++;  
                CurPix = BaseImage[Count];
                SrcPix = TempSrcBuffer[Count];
                Count++;
              } 
            *(SHORT*)(ChunkBuffer+cCount) = (SHORT)(-SkipCount);
            cCount += sizeof(SHORT);
            NumChunks++;
          } 
        else
          {
            DumpCount=0;  
            while ((CurPix!=SrcPix)&&(Count<Size)&(DumpCount<(LONG)0x7FFF))
              {
                DataBuffer[DumpCount++] = SrcPix;
                CurPix = BaseImage[Count];
                SrcPix = TempSrcBuffer[Count];
                Count++;
              } 
            *(SHORT*)(ChunkBuffer+cCount) = (SHORT)DumpCount;
            cCount += sizeof(SHORT);

            for (i=0;i<DumpCount;i++)
              {
                *(ChunkBuffer+cCount) = DataBuffer[i];
                cCount++;
              } 
              
            NumChunks++;
          } 
      } 

    
    if (cCount>Size)
      {
        Frame->NumChunks = -1;
        Frame->Size = Size;
        Frame->Chunks = new BYTE [Size];
        memcpy ( Frame->Chunks, TempSrcBuffer, Size );  
      } 
    else
      {
        
        Frame->NumChunks = NumChunks;
        Frame->Size = cCount;
        if (cCount==0)
          Frame->Chunks = NULL;
        else  
          Frame->Chunks = new BYTE [cCount];
        memcpy ( Frame->Chunks, ChunkBuffer, cCount );          
      } 
          
    delete TempSrcBuffer;
    delete ChunkBuffer;    
    delete DataBuffer;    
  } 

VOID ANIMIMAGE::SetFrame ( LONG FrameNumber )
  {
    if (BaseImage==NULL)
      return;
    if (Buffer==NULL)
      return;
    if ((FrameNumber<0)||(FrameNumber>=NumFrames))
      return;
    
    LONG Size;
    Size = BytesPerRow*Height;      
    
    memcpy ( Buffer, BaseImage, Size );

    if (FrameNumber==0)
      return;
       
    LONG i;
    LONG RepCount;
    BYTE* ChunkBuffer;
    BYTE* DestBuffer;
    ANIMFRAME *Frame;
    Frame = &(Frames[FrameNumber]);
    ChunkBuffer = Frame->Chunks;

    if (ChunkBuffer==NULL)
      return;
      
    DestBuffer = Buffer;
    
    
    for (i=0;i<Frame->NumChunks;i++)
      {
        RepCount = *((SHORT*)ChunkBuffer);
        ChunkBuffer += sizeof(SHORT);
        if (RepCount<0)
          DestBuffer += -RepCount;
        else
          {
            memcpy ( DestBuffer, ChunkBuffer, RepCount );
            ChunkBuffer += RepCount;
            DestBuffer += RepCount;
          } 
      } 
  } 

BOOLEAN ANIMIMAGE::Save ( STRING FileName )
  {
    FILEHANDLE f;

    if (BaseImage==NULL)
      return FAILURE;
      
    f = File.Open ( FileName, OPEN_BINARY | OPEN_WRITE );
    if (f==NULL)
      return FAILURE;

    CHAR Str[32];
    sprintf ( Str, "ANIMATION" );
    File.Write ( f, Str, 32 );

    LONG Wd,Ht,BytesPerPix,NumFrm;
    Wd = Width;
    Ht = Height;
    BytesPerPix = BytesPerPixel;
    NumFrm = NumFrames;

    #if defined (__MSBFIRST__)
      SwapDWord ( (DWORD*)&Wd );
      SwapDWord ( (DWORD*)&Ht );
      SwapDWord ( (DWORD*)&BytesPerPix );
      SwapDWord ( (DWORD*)&NumFrm );
    #endif
    
    File.Write ( f, &Wd, sizeof(LONG) );     
    File.Write ( f, &Ht, sizeof(LONG) );     
    File.Write ( f, &BytesPerPix, sizeof(LONG) );     
    File.Write ( f, &NumFrm, sizeof(LONG) );

    LONG Size;
    Size = BytesPerRow*Height;

    File.Write ( f, BaseImage, Size );

    INT i;
    LONG NumChunks,ChunkSize;
    for (i=1;i<NumFrames;i++)
      {
        NumChunks = Frames[i].NumChunks;  
        ChunkSize = Frames[i].Size;
        #if defined (__MSBFIRST__)
          SwapDWord ( (DWORD*)&NumChunks );
          SwapDWord ( (DWORD*)&Size );
        #endif
        File.Write ( f, &NumChunks, sizeof(LONG) );
        File.Write ( f, &Size, sizeof(LONG) );
        File.Write ( f, Frames[i].Chunks, Frames[i].Size );
      } 
      
    File.Close ( f );
    return SUCCESS;    
  } 

BOOLEAN ANIMIMAGE::Load ( STRING FileName )
  {
    FILEHANDLE f;

    f = File.Open ( FileName, OPEN_BINARY | OPEN_READ );
    if (f==NULL)
      return FAILURE;

    Destroy ();
    DestroyFrames ();
    
    CHAR Str[32];
    File.Read ( f, Str, 32 );
    if(strcmp ( Str, "ANIMATION" ))
      {
        File.Close ( f );
        return FAILURE;  
      } 
      
    File.Read ( f, &Width, sizeof(LONG) );     
    File.Read ( f, &Height, sizeof(LONG) );     
    File.Read ( f, &BytesPerPixel, sizeof(LONG) );

    #if defined (__MSBFIRST__)
      SwapDWord ( (DWORD*)&Width );
      SwapDWord ( (DWORD*)&Height );
      SwapDWord ( (DWORD*)&BytesPerPixel );
    #endif
    
    LONG NewFormat=0;
    switch (BytesPerPixel)
      {
        case 1 :
          NewFormat = IMAGE_8BIT;
          break; 
        case 2 :
          NewFormat = IMAGE_16BIT;
          break; 
        case 3 :
          NewFormat = IMAGE_24BIT;
          break; 
      } 

    if (Create ( NewFormat, Width, Height )==FAILURE)
      return FAILURE;

    if (CreateBaseImage()==FAILURE)
      return FAILURE;
      
    File.Read ( f, &NumFrames, sizeof(LONG) );
    
    #if defined (__MSBFIRST__)
      SwapDWord ( (DWORD*)&NumFrames );
    #endif

    if (CreateFrames ( NumFrames )==FAILURE)
      return FAILURE;
    
    LONG Size;
    Size = BytesPerRow*Height;

    File.Read ( f, BaseImage, Size );

    INT i;
    for (i=1;i<NumFrames;i++)
      {
        File.Read ( f, &(Frames[i].NumChunks), sizeof(LONG) );
        File.Read ( f, &(Frames[i].Size), sizeof(LONG) );
        #if defined (__MSBFIRST__)
          SwapDWord ( (DWORD*)&(Frames[i].NumChunks) );
          SwapDWord ( (DWORD*)&(Frames[i].Size) );
        #endif
        File.Read ( f, Frames[i].Chunks, Frames[i].Size );
      } 
      
    File.Close ( f );
    return SUCCESS;    
  } 

