




#ifndef __GFXADST_H__
#define __GFXADST_H__

#include "stdgfx.h"
#include "gfxobj.h"

class DATAOBJECT : public MYOBJECT
  {
    public :
      DATAOBJECT ();
      virtual ~DATAOBJECT ();
  }; 

class ADST : public MYOBJECT
  {
    protected :
      LONG NumObjects;

    public :
      ADST () : MYOBJECT () { NumObjects = 0; };
      virtual ~ADST () {};

      LONG GetNumObjects () { return NumObjects; };
  }; 

#endif
