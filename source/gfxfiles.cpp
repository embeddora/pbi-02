




#include "stdgfx.h"
#include "gfxfiles.h"

FILESYSTEM File;

FILESYSTEM::FILESYSTEM () : DCIClass ()
  {
  } 

FILESYSTEM::~FILESYSTEM ()
  {
  } 

BOOLEAN FILESYSTEM::Init ()
  {
    return TRUE;
  } 

VOID FILESYSTEM::DeInit ()
  {
  } 

FILEHANDLE FILESYSTEM::Open ( STRING FileName, LONG Mode )
  {
    CHAR ModeStr[16];
    LONG Pos;

    memset ( ModeStr, 0, 16 );

    Pos = 0;
    LONG Open;
    Open = OPEN_READ;
    if (Open==0)
      {
        if ((Mode&0x01)==0)
          ModeStr[Pos++] = 'r';
      } 
    else
      {
        if (Mode&OPEN_READ)
          ModeStr[Pos++] = 'r';
      } 

    if (Mode&OPEN_WRITE)
      ModeStr[Pos++] = 'w';
    if (Mode&OPEN_RDWR)
      {
            ModeStr[Pos++] = 'w';
            ModeStr[Pos++] = '+';
      } 

    if (Mode&OPEN_APPEND)
      ModeStr[Pos++] = 'a';

    #ifndef __FORUNIX__
      if (Mode&OPEN_TEXT)
        ModeStr[Pos++] = 't';
      if (Mode&OPEN_BINARY)
        ModeStr[Pos++] = 'b';
    #endif 
         if (Pos)
      {}
    return fopen ( FileName, ModeStr );
  } 

VOID FILESYSTEM::Close ( FILEHANDLE f )
  {
    fclose ( f );
  } 

LONG FILESYSTEM::Seek ( FILEHANDLE f, LONG Offset, LONG From )
  {
    return fseek ( f, Offset, From );
  } 

BOOLEAN FILESYSTEM::Read ( FILEHANDLE f, VOID* Buffer, LONG Size )
  {
    if (fread(Buffer,Size,1,f)!=1)
      {
        Error.SetError ( ERR_FILEIO );
        return FAILURE;
      } 
    return SUCCESS;
  } 

BOOLEAN FILESYSTEM::Write ( FILEHANDLE f, VOID* Buffer, LONG Size )
  {
    if (fwrite(Buffer,Size,1,f)!=1)
      {
        Error.SetError ( ERR_FILEIO );
        return FAILURE;
      } 
    return SUCCESS;
  } 

INT FILESYSTEM::GetCh ( FILEHANDLE f )
  {
    INT Ch;
    Ch = fgetc ( f );
    return Ch;
  } 

VOID FILESYSTEM::PutCh ( FILEHANDLE f, SHORT Ch )
  {
    fputc ( Ch, f );
  } 

LONG FILESYSTEM::GetPos ( FILEHANDLE f )
  {
    #ifdef __FORVISUAL__
      fpos_t Pos;
    #else
      LONG Pos;
    #endif

    #ifdef __FORUNIX__
      return ftell ( f );
    #else
      fgetpos ( f, &Pos );
      return (LONG)Pos;
    #endif
  } 

LONG FILESYSTEM::GetSize ( FILEHANDLE f )
  {
    LONG Size;
    LONG CurPos;
    CurPos = GetPos ( f );
    Seek ( f, 0, FROM_END );
    Size = GetPos ( f );
    Seek ( f, CurPos, FROM_BEGIN );
    return Size;
  } 

