




#include "stdgfx.h"
#include "gfxgif.h"

#include "stdlib.h"

#define HASH_SIZE 5011
#define HASH_BITS 4
#define CAPACITY  ((long)1<<12)
#define MAXBITS 12

struct SEQ_INTERLACED
  {
    INT Start;
    INT Skip;
  }; 

SEQ_INTERLACED InterlacedData[4] =
  {
    { 0, 8 },
    { 4, 8 },
    { 2, 4 },
    { 1, 2 }
  }; 

GIFFILETOOL::GIFFILETOOL ( FILEHANDLE f ) : IMAGEFILETOOL ( f )
  {
    CodeTable = NULL;
    CodeStack = NULL;
    Prefix = NULL;
    Suffix = NULL;
    DataBlock = NULL;
  } 

GIFFILETOOL::~GIFFILETOOL ()
  {
    DeInit ();
  } 

BOOLEAN GIFFILETOOL::ReadHeader ()
  {
    BOOLEAN Ok;

    File.Seek ( fp, 0, FROM_BEGIN );
    Ok = File.Read ( fp, &GIFHeader, sizeof(GIFHEADER) );
    if (!Ok)
      return FAILURE;

    if (strncmp(GIFHeader.ID,"GIF",3)!=0)
      return FAILURE;
    if ((strncmp(GIFHeader.Version,"87a",3)!=0)&&
          (strncmp(GIFHeader.Version,"89a",3)!=0))
      return FAILURE;

    return SUCCESS;
  } 

BOOLEAN GIFFILETOOL::ReadScrDSC ()
  {
    BOOLEAN Ok;

    Ok = File.Read ( fp, &ScrDSC, 7 );
    if (!Ok)
      return FAILURE;

    #if defined (__MSBFIRST__)
      SwapWord ( (WORD*)&ScrDSC.Width );
      SwapWord ( (WORD*)&ScrDSC.Height );
    #endif
    return SUCCESS;
  } 

BOOLEAN GIFFILETOOL::ReadGCT ( RGBPALETTE *Pal )
  {
    INT NumColors;
    RGBCOLOR *Entry;

    NumColors = ScrDSC.NumColors ();
    if (Pal==NULL)
      {
        RGBCOLOR Table[256];
        File.Read ( fp, Table, sizeof(RGBCOLOR)*NumColors );
        return SUCCESS;
      } 

    Entry = Pal->GetEntry ();

    INT i;
    for (i=0;i<NumColors;i++)
      {
        Entry[i].Red = (BYTE)File.GetCh ( fp );
        Entry[i].Green = (BYTE)File.GetCh ( fp );
        Entry[i].Blue = (BYTE)File.GetCh ( fp );
      } 
    return SUCCESS;
  } 

BOOLEAN GIFFILETOOL::ReadImgDSC ()
  {
    BOOLEAN Ok;

    File.Seek ( fp, -1, FROM_CURRENT );
    Ok = File.Read ( fp, &(ImgDSC.ID), sizeof(BYTE) );
    if (!Ok)
      return FAILURE;
    Ok = File.Read ( fp, &(ImgDSC.xLeft), sizeof(SHORT) );
    if (!Ok)
      return FAILURE;
    Ok = File.Read ( fp, &(ImgDSC.yLeft), sizeof(SHORT) );
    if (!Ok)
      return FAILURE;
    Ok = File.Read ( fp, &(ImgDSC.Width), sizeof(SHORT) );
    if (!Ok)
      return FAILURE;
    Ok = File.Read ( fp, &(ImgDSC.Height), sizeof(SHORT) );
    if (!Ok)
      return FAILURE;
    Ok = File.Read ( fp, &(ImgDSC.Flags), sizeof(BYTE) );
    if (!Ok)
      return FAILURE;

    #if defined (__MSBFIRST__)
      SwapWord ( (WORD*)&ImgDSC.xLeft );
      SwapWord ( (WORD*)&ImgDSC.yLeft );
      SwapWord ( (WORD*)&ImgDSC.Width );
      SwapWord ( (WORD*)&ImgDSC.Height );
    #endif

    return SUCCESS;
  } 

BOOLEAN GIFFILETOOL::ReadLCT ( RGBPALETTE *Pal )
  {
    INT NumColors;
    RGBCOLOR *Entry;

    NumColors = ImgDSC.NumColors ();

    if (Pal==NULL)
      {
        RGBCOLOR Table[256];
        File.Read ( fp, Table, sizeof(RGBCOLOR)*NumColors );
        return SUCCESS;
      } 

    Entry = Pal->GetEntry ();
    INT i;
    for (i=0;i<NumColors;i++)
      {
        Entry[i].Red = (BYTE)File.GetCh ( fp );
        Entry[i].Green = (BYTE)File.GetCh ( fp );
        Entry[i].Blue = (BYTE)File.GetCh ( fp );
      } 
    return SUCCESS;
  } 

BOOLEAN GIFFILETOOL::ReadImage ( IMAGE *Image, RGBPALETTE *Pal )
  {
    BOOLEAN Ok;

    Ok = ReadImgDSC ();
    if (!Ok)
      return FAILURE;

    if (ImgDSC.IsLCT())
      {
        Ok = ReadLCT ( Pal );
        if (!Ok)
          return FAILURE;
      } 

    Ok = Image->Create ( IMAGE_8BIT, ImgDSC.Width, ImgDSC.Height );
    if (!Ok)
      return FAILURE;

    Ok = Decode ( Image );
    if (!Ok)
      return FAILURE;

    return SUCCESS;
  } 

BOOLEAN GIFFILETOOL::Decode ( IMAGE *Image )
  {
    INT i;
    BOOLEAN Ok;

    GetCodeSize ();
    InitDecoding ();

    StackPtr = 0;
    NumBytes = 0;
    ByteCount = 0;
    CurByteMask = 256;

    CurCode = GetCode ();
    while (CurCode==ClearCode)
      CurCode = GetCode ();

    PreCode = CurCode;
    FirstCh = PreCode;
    PushCode ( PreCode );

    INT Row;
    INT Pass = 0;

    if (ImgDSC.IsInterlaced())
      Row = InterlacedData[Pass].Start;
    else
      Row = 0;

    for (i=0;i<Image->GetHeight();i++)
      {
        Ok = ReadRow ( Image, Row );
        if (!Ok)
          return FAILURE;

        if (ImgDSC.IsInterlaced())
          {
            Row += InterlacedData[Pass].Skip;
            if (Row>=Image->GetHeight())
              {
                Pass++;
                Row = InterlacedData[Pass].Start;
              } 
          } 
        else
          Row++;
      } 

    return SUCCESS;
  } 

BOOLEAN GIFFILETOOL::ReadRow ( IMAGE *Image, INT Row )
  {
    DWORD Offset;
    DWORD Wd;
    BYTE *Buffer;

    Buffer = Image->SetOffset ( 0, Row );

    Wd = Image->GetWidth();
    Offset = 0;

    
    while ((Offset<Wd)&&(StackPtr>0))
      {
        Buffer[Offset] = (BYTE)PopCode ();
        Offset++;
      } 

    SHORT Index;

    while (Offset<Wd)
      {
        CurCode = GetCode ();
        if (CurCode==ClearCode)
          {
            ClearTable ();
            if (CurBits>=MAXBITS)
              {
                CurBits = MinBits;
                MaxCode = (SHORT)((1<<CurBits));
              } 
            PreCode = GetCode ();
            PushCode ( PreCode );
          } 
        else if (CurCode==EndCode)
          {
            break;
          } 
        else
          {
            if (FindStr(CurCode))
              {
                Index = CurCode;
                PushCode ( Suffix[Index] );
                Index = Prefix[Index];

                while (Index!=NULLCODE)
                  {
                    PushCode ( Suffix[Index] );
                    Index = Prefix[Index];
                  } 
                FirstCh = GetFirstCh ();
              } 
            else
              {
                PushCode ( FirstCh );
                Index = PreCode;
                PushCode ( Suffix[Index] );
                Index = Prefix[Index];

                while (Index!=NULLCODE)
                  {
                    PushCode ( Suffix[Index] );
                    Index = Prefix[Index];
                  } 

                FirstCh = GetFirstCh ();
              } 

            AddNewString ( PreCode, FirstCh );
            PreCode = CurCode;

            if (NextAvailCode>=MaxCode)
              {
                if (CurBits<MAXBITS)
                  {
                    CurBits++;
                    MaxCode = (SHORT)(1<<CurBits);
                  } 
              } 
          } 

        while ((Offset<Wd)&&(StackPtr>0))
          {
            Buffer[Offset] = (BYTE)PopCode ();
            Offset++;
          } 
      } 

    return SUCCESS;
  } 

void GIFFILETOOL::AddNewString ( SHORT Pre, SHORT New )
  {
    Prefix[NextAvailCode] = Pre;
    Suffix[NextAvailCode] = New;
    NextAvailCode++;
  } 

SHORT GIFFILETOOL::GetFirstCh ()
  {
    if (StackPtr>0)
      return CodeStack[StackPtr-1];
    return -1;
  } 

SHORT GIFFILETOOL::GetCode ()
  {
    INT Code;
    INT BitMask;
    INT i;

    BitMask = 1;
    Code = 0;

    for (i=0;i<CurBits;i++)
      {
        if (CurByteMask>128)
          CurByte = GetByte ();
        if (CurByte&CurByteMask)
          Code |= BitMask;

        BitMask <<= 1;
        CurByteMask <<= 1;
      } 
    return (SHORT)Code;
  } 

BYTE GIFFILETOOL::GetByte ()
  {
    ByteCount++;
    if (ByteCount>=NumBytes)
      {
        BYTE Byte;
        File.Read ( fp, &Byte, sizeof(BYTE) );
        NumBytes = Byte;
        if (NumBytes!=0)
          File.Read ( fp, DataBlock, sizeof(BYTE)*NumBytes );
        ByteCount = 0;
      } 

    CurByteMask = 1;
    return DataBlock[ByteCount];
  } 

void GIFFILETOOL::PushCode ( SHORT Code )
  {
    if (StackPtr==HASHSIZE)
      exit(-1);
    CodeStack[StackPtr] = Code;
    StackPtr++;
  } 

SHORT GIFFILETOOL::PopCode ( )
  {
    StackPtr--;
    return CodeStack[StackPtr];
  } 

BOOLEAN GIFFILETOOL::InitDecoding ()
  {
    DeInit ();
    CodeTable = new SHORT [HASHSIZE];
    CodeStack = new SHORT [HASHSIZE];
    Prefix = new SHORT [HASHSIZE];
    Suffix = new SHORT [HASHSIZE];
    DataBlock = new BYTE [256];

    if ((CodeTable==NULL)||
         (CodeStack==NULL)||
          (Prefix==NULL)||
           (Suffix==NULL)||
            (DataBlock==NULL))
      return FAILURE;

    ClearTable ();

    return SUCCESS;
  } 

void GIFFILETOOL::DeInit ()
  {
    if (CodeTable!=NULL)
      delete CodeTable;

    if (CodeStack!=NULL)
      delete CodeStack;

    if (Prefix!=NULL)
      delete Prefix;

    if (Suffix!=NULL)
      delete Suffix;

    if (DataBlock!=NULL)
      delete DataBlock;

    CodeTable = NULL;
    CodeStack = NULL;
    Prefix = NULL;
    Suffix = NULL;
    DataBlock = NULL;
  } 

BOOLEAN GIFFILETOOL::FindStr ( SHORT Code )
  {
    if ((Code>=0)&&(Code<NextAvailCode))
      return TRUE;
    return FALSE;
  } 

void GIFFILETOOL::ClearTable ()
  {
    INT i;

    for (i=0;i<RootSize;i++)
      {
        Prefix[i] = NULLCODE;
        Suffix[i] = (SHORT)i;
      } 
    NextAvailCode = (SHORT)(RootSize + 2);
  } 

INT GIFFILETOOL::GetCodeSize ()
  {
    BYTE Size;

    File.Read ( fp, &Size, sizeof(BYTE) );
    MinBits = (SHORT)(Size+1);
    CurBits = MinBits;
    RootSize = (SHORT)(1<<Size);
    ClearCode = RootSize;
    EndCode = (SHORT)(RootSize+1);
    NextAvailCode = (SHORT)(RootSize+2);
    MaxCode = (SHORT)(1<<CurBits);
    return Size;
  } 

BOOLEAN GIFFILETOOL::LoadImage ( IMAGE *Image, RGBPALETTE *Pal )
  {
    BOOLEAN Ok;

    if (Image==NULL)
      return FAILURE;

    Ok = ReadHeader ();
    if (!Ok)
      return FAILURE;

    Ok = ReadScrDSC ();
    if (!Ok)
      return FAILURE;

    if (ScrDSC.IsGCT())
      {
        Ok = ReadGCT ( Pal );
        if (!Ok)
          return FAILURE;
      } 

    SHORT Type;

    Type = 1;

    while (Type>0)
      {
        Type = (SHORT)File.GetCh ( fp );
        switch (Type)
          {
            case 0x2C :  
              Ok = ReadImage ( Image, Pal );
              if (!Ok)
                break;
              return SUCCESS;
            default :
              break;
          } 
      } 

    DeInit ();
    return Ok;
  } 

BOOLEAN GIFFILETOOL::WriteHeader ()
  {
    BOOLEAN Ok;

    strcpy (GIFHeader.ID,"GIF");
    strcpy (GIFHeader.Version,"87a");

    File.Seek ( fp, 0, FROM_BEGIN );
    Ok = File.Write ( fp, &GIFHeader, sizeof(GIFHEADER) );
    if (!Ok)
      return FAILURE;

    return SUCCESS;
  } 

BOOLEAN GIFFILETOOL::WriteScrDSC ( INT Wd, INT Ht )
  {
    DWORD Size;

    ScrDSC.Width = (SHORT)Wd;
    ScrDSC.Height = (SHORT)Ht;

    #if defined (__MSBFIRST__)
      SwapWord ( (WORD*)&ScrDSC.Width );
      SwapWord ( (WORD*)&ScrDSC.Height );
    #endif

    ScrDSC.Flags = 0;
    ScrDSC.Flags |= scdGCT;     
    ScrDSC.Flags |= (8 - 1)<<4; 
    ScrDSC.Flags &= ~scdSORT;   
    ScrDSC.Flags |= 7;          

    ScrDSC.BGColor = 0;

    Size = File.Write ( fp, &ScrDSC, 7 );
    if (Size!=7)
      return FAILURE;
    return SUCCESS;
  } 

BOOLEAN GIFFILETOOL::WriteGCT ( RGBPALETTE *Pal )
  {
    RGBCOLOR *Entry;

    if (Pal==NULL)
      return FAILURE;

    Entry = Pal->GetEntry ();

    INT i;
    for (i=0;i<256;i++)
      {
        File.PutCh ( fp, Entry[i].Red );
        File.PutCh ( fp, Entry[i].Green );
        File.PutCh ( fp, Entry[i].Blue );
      } 

    return SUCCESS;
  } 

BOOLEAN GIFFILETOOL::InitEncoding ()
  {
    DeInit ();
    CodeTable = new SHORT [HASHSIZE];
    CodeStack = new SHORT [HASHSIZE];
    Prefix = new SHORT [HASHSIZE];
    Suffix = new SHORT [HASHSIZE];
    DataBlock = new BYTE [256];

    if ((CodeTable==NULL)||
         (CodeStack==NULL)||
          (Prefix==NULL)||
           (Suffix==NULL)||
            (DataBlock==NULL))
      return FAILURE;

    InitTable ();

    return SUCCESS;
  } 

INT GIFFILETOOL::FindStr ( INT Pre, INT Cur )
  {
    INT Di,Index;
    Index = (Cur<<HASH_BITS) ^ Pre;
    if (Index==0)
      Di = 1;
    else
      Di = CAPACITY - Index;

    BOOLEAN Done=FALSE;

         while (!Done)
      {
        if (CodeTable[Index]==-1) 
          {
            Done = TRUE;
            break;
          } 

        if ((Prefix[Index]==Pre)&&(Suffix[Index]==Cur)) 
          {
            Done = TRUE;
            break;
                         } 

        Index -= Di;
        if (Index<0)
          {
            while (Index<0)
              Index += CAPACITY;
            Di ++;
          } 
      } 

         if (Done)
                {}

         return Index;
  } 

void GIFFILETOOL::InitTable ()
  {
    INT i;

    for (i=0;i<HASH_SIZE;i++)
      {
        CodeTable[i] = -1;
        Suffix[i] = -1;
        Prefix[i] = -1;
      } 

    CurBits = MinBits;
    NextAvailCode = (SHORT)(RootSize+2);
    MaxCode = (SHORT)(1<<CurBits);
  } 

void GIFFILETOOL::PutCodeSize ( BYTE CodeSize )
  {
    File.Write ( fp, &CodeSize, 1 );
  } 

void GIFFILETOOL::PutCode ( SHORT Code )
  {
    INT BitMask;
    INT i;

    BitMask = 1;

    for (i=0;i<CurBits;i++)
      {
        if (Code&BitMask)
          CurByte |= CurByteMask;
        BitMask <<= 1;
        CurByteMask <<= 1;
        if (CurByteMask>128)
          {
            PutByte ();
          } 
      } 
  } 

void GIFFILETOOL::PutByte ()
  {
    BYTE Count;
    DataBlock[NumBytes++] = (BYTE)CurByte;
    CurByte = 0;
    CurByteMask = 1;

    if (NumBytes>=255)
      {
        Count = (BYTE)NumBytes;
        File.Write ( fp, &Count, 1 );
        File.Write ( fp, DataBlock, NumBytes );
        NumBytes = 0;
      } 
  } 

void GIFFILETOOL::AddNewString ( INT Pre, INT Cur, INT Index )
  {
    CodeTable[Index] = NextAvailCode;
    NextAvailCode++;
    Prefix[Index] = (SHORT)Pre;
    Suffix[Index] = (SHORT)Cur;
  } 

void GIFFILETOOL::FlushAll ()
  {
    BYTE Count;
    Count = (BYTE)NumBytes;
    File.Write ( fp, &Count, 1 );
    File.Write ( fp, DataBlock, NumBytes );
  } 

BOOLEAN GIFFILETOOL::WriteRow ( IMAGE *Image, INT Row, INT Sx, INT Wd )
  {
    BYTE *Buffer;

    Buffer = Image->SetOffset ( Sx, Row );
    if (Buffer==NULL)
      return FAILURE;

    INT Offset;
    INT Index;
    Offset = 0;

    if (Row==0)
      PreCode = Buffer[Offset++];

    while (Offset<Wd)
      {
        CurCode = Buffer[Offset];
        Offset++;
        Index = FindStr(PreCode,CurCode);
        if (CodeTable[Index]!=-1)
          {
            PreCode = CodeTable[Index];
          } 
        else
          {
            AddNewString ( PreCode, CurCode, Index );
            PutCode ( PreCode );
            PreCode = CurCode;
            if (CodeTable[Index]>MaxCode-1)
              {
                if (CurBits>=MAXBITS)
                  {
                    PutCode ( ClearCode ); 
                    InitTable ();
                  } 
                else
                  {
                    CurBits++;
                    MaxCode = (SHORT)(1<<CurBits);
                  } 
              } 
          } 
      } 
    return SUCCESS;
  } 

BOOLEAN GIFFILETOOL::Encode ( IMAGE *Image, INT Sx, INT Sy, INT Wd, INT Ht )
  {
    MinBits = 8+1;
    RootSize = (1<<8);
    ClearCode = RootSize;
    EndCode = (SHORT)(RootSize+1);
    CurBits = MinBits;
    NextAvailCode = (SHORT)(RootSize+2);
    MaxCode = (SHORT)((1<<CurBits));

    PutCodeSize ( 8 );
    InitEncoding ();

    NumBytes = 0;
    CurByteMask = 1;
    CurByte = 0;

    PutCode ( ClearCode ); 

    INT i;
    BOOLEAN Ok;

    for (i=Sy;i<Sy+Ht;i++)
      {
        Ok = WriteRow ( Image, i, Sx, Wd );
        if (!Ok)
          return FAILURE;
      } 

    PutCode ( PreCode );
    PutCode ( EndCode ); 
    FlushAll ();

    return SUCCESS;
  } 

BOOLEAN GIFFILETOOL::WriteImgDSC ( INT Wd, INT Ht )
  {
    BOOLEAN Ok;

    ImgDSC.ID = 0x2C;
    ImgDSC.xLeft = 0;
    ImgDSC.yLeft = 0;
    ImgDSC.Width = (SHORT)Wd;
    ImgDSC.Height = (SHORT)Ht;
    ImgDSC.Flags = 0;
    ImgDSC.Flags |= 7;

    #if defined (__MSBFIRST__)
      SwapWord ( (WORD*)&ImgDSC.xLeft );
      SwapWord ( (WORD*)&ImgDSC.yLeft );
      SwapWord ( (WORD*)&ImgDSC.Width );
      SwapWord ( (WORD*)&ImgDSC.Height );
    #endif

    Ok = File.Write ( fp, &(ImgDSC.ID), sizeof(BYTE) );
    if (!Ok)
      return FAILURE;
    Ok = File.Write ( fp, &(ImgDSC.xLeft), sizeof(SHORT) );
    if (!Ok)
      return FAILURE;
    Ok = File.Write ( fp, &(ImgDSC.yLeft), sizeof(SHORT) );
    if (!Ok)
      return FAILURE;
    Ok = File.Write ( fp, &(ImgDSC.Width), sizeof(SHORT) );
    if (!Ok)
      return FAILURE;
    Ok = File.Write ( fp, &(ImgDSC.Height), sizeof(SHORT) );
    if (!Ok)
      return FAILURE;
    Ok = File.Write ( fp, &(ImgDSC.Flags), sizeof(BYTE) );
    if (!Ok)
      return FAILURE;
    return SUCCESS;
  } 

BOOLEAN GIFFILETOOL::WriteImage ( IMAGE *Image, INT Sx, INT Sy, INT Wd, INT Ht )
  {
    BOOLEAN Ok;

    Ok = WriteImgDSC ( Wd, Ht );
    if (!Ok)
     return FAILURE;

    Ok = Encode ( Image, Sx, Sy, Wd, Ht );
    if (!Ok)
     return FAILURE;

    BYTE End = 0;

    File.Write ( fp, &End, 1 );

    return SUCCESS;
  } 

BOOLEAN GIFFILETOOL::SaveImage ( IMAGE *Image, LONG Sx, LONG Sy,
                                 LONG Wd, LONG Ht, RGBPALETTE *Pal )
  {
    if (Image==NULL)
      return FAILURE;

    BOOLEAN Ok;

    Ok = WriteHeader ();
    if (!Ok)
      return FAILURE;

    Ok = WriteScrDSC ( Wd, Ht );
    if (!Ok)
      return FAILURE;

    Ok = WriteGCT ( Pal );
    if (!Ok)
      return FAILURE;

    Ok = WriteImage ( Image, Sx, Sy, Wd, Ht );
    if (!Ok)
      return FAILURE;

    BYTE EndTrailer = 0x3B;

    File.Write ( fp, &EndTrailer, 1 );

    return SUCCESS;
  } 
