




#ifndef __GFXDEVICE_H__
#define __GFXDEVICE_H__

#include "stdgfx.h"
#include "gfxerror.h"


class DCIClass : public MYOBJECT
  {
    public :
      DCIClass ();
      virtual ~DCIClass ();

      virtual BOOLEAN Init () = 0;
      virtual VOID DeInit () = 0;
  }; 

#endif

