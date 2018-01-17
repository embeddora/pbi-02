




#include "stdgfx.h"
#include "gfxerror.h"

ERRORHANDLER::ERRORHANDLER () : SYSTEMTOOL ()
  {
    LastError = ERR_NOERROR;
  } 

ERRORHANDLER::~ERRORHANDLER ()
  {
  } 

void ERRORHANDLER::SetError ( ERR_TYPE Err )
  {
    LastError = Err;
  } 

ERR_TYPE ERRORHANDLER::GetError ()
  {
    return LastError;
  } 

ERRORHANDLER Error;


