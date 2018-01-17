




#ifndef __GFXERROR_H__
#define __GFXERROR_H__

#include "stdgfx.h"
#include "gfxsys.h"

#define ERR_TYPE LONG


#define ERR_NOERROR   0
#define ERR_UNKNOWN   1
#define ERR_NOMEMORY  2   
#define ERR_NOEXIST   3   
#define ERR_NOVALID   4   
#define ERR_FILEIO    5   
#define ERR_USER      6   

class ERRORHANDLER : public SYSTEMTOOL
  {
    protected :
      ERR_TYPE LastError;

    public :
      ERRORHANDLER ();
      virtual ~ERRORHANDLER ();

      void SetError ( ERR_TYPE Err );
      ERR_TYPE GetError ();
  }; 

extern ERRORHANDLER Error;

#endif


