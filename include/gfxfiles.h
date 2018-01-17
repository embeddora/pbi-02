




#ifndef __GFXFILES_H__
#define __GFXFILES_H__

#include "stdgfx.h"
#include "gfxdev.h"
#include "gfxerror.h"
#include <fcntl.h>

#if defined (__FORWINDOWS__)
  #define FILEHANDLE FILE*
  
  #define FROM_BEGIN   SEEK_SET
  #define FROM_CURRENT SEEK_CUR
  #define FROM_END     SEEK_END
  
  #define OPEN_READ    O_RDONLY
  #define OPEN_WRITE   O_WRONLY
  #define OPEN_RDWR    O_RDWR
  #define OPEN_APPEND  O_APPEND
  #define OPEN_BINARY  O_BINARY
  #define OPEN_TEXT    O_TEXT
  #define OPEN_CREATE  O_CREAT

#elif defined (__FOROS2__)
  #define FILEHANDLE FILE*
  
  #define FROM_BEGIN   SEEK_SET
  #define FROM_CURRENT SEEK_CUR
  #define FROM_END     SEEK_END
  
  #define OPEN_READ    O_RDONLY
  #define OPEN_WRITE   O_WRONLY
  #define OPEN_RDWR    O_RDWR
  #define OPEN_APPEND  O_APPEND
  #define OPEN_BINARY  O_BINARY
  #define OPEN_TEXT    O_TEXT
  #define OPEN_CREATE  O_CREAT

#elif defined (__FORDOS__)
  #define FILEHANDLE FILE*
  
  #define FROM_BEGIN   SEEK_SET
  #define FROM_CURRENT SEEK_CUR
  #define FROM_END     SEEK_END
  
  #define OPEN_READ    O_RDONLY
  #define OPEN_WRITE   O_WRONLY
  #define OPEN_RDWR    O_RDWR
  #define OPEN_APPEND  O_APPEND
  #define OPEN_BINARY  O_BINARY
  #define OPEN_TEXT    O_TEXT
  #define OPEN_CREATE  O_CREAT

#elif defined (__FORUNIX__)
  #define FILEHANDLE FILE*
  
  #define FROM_BEGIN   0
  #define FROM_CURRENT 1
  #define FROM_END     2
  
  #define O_BINARY  0
  #define O_TEXT    0

  #define OPEN_READ    O_RDONLY
  #define OPEN_WRITE   O_WRONLY
  #define OPEN_RDWR    O_RDWR
  #define OPEN_APPEND  O_APPEND
  #define OPEN_BINARY  O_BINARY
  #define OPEN_TEXT    O_TEXT
  #define OPEN_CREATE  O_CREAT
#endif

class FILESYSTEM : public DCIClass
  {
    public :
      FILESYSTEM ();
      virtual ~FILESYSTEM ();

      virtual BOOLEAN Init ();
      virtual VOID DeInit ();

      FILEHANDLE Open ( STRING FileName, LONG Mode );
      VOID Close ( FILEHANDLE f );
      LONG Seek ( FILEHANDLE f, LONG Offset, LONG From );
      BOOLEAN Read ( FILEHANDLE f, VOID* Buffer, LONG Size );
      BOOLEAN Write ( FILEHANDLE f, VOID* Buffer, LONG Size );
      INT  GetCh ( FILEHANDLE f );
      VOID PutCh ( FILEHANDLE f, SHORT Ch );
      LONG GetPos ( FILEHANDLE f );
      LONG GetSize ( FILEHANDLE f );
  }; 

extern FILESYSTEM File;

#endif



