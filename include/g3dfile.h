




#ifndef __G3DFILES_H__
#define __G3DFILES_H__

#include "stdgfx.h"
#include "g3ddraw.h"

#define FILE_ASC    1
#define FILE_3DS    2
#define FILE_GEO    3
#define FILE_GEM    4

class G3DFILETOOL
  {
    protected :
      BOOLEAN Clockwise;
      double AdjustScale;
                
    public :
      G3DFILETOOL ();
      virtual ~G3DFILETOOL ();      
      
      virtual BOOLEAN Load ( FILEHANDLE f, LINKEDLIST<G3DOBJECT*> *ObjectList,
                             BOOLEAN ClockWise, double Scale ) = 0;
      virtual BOOLEAN Save ( FILEHANDLE f, LINKEDLIST<G3DOBJECT*> *ObjectList,
                             BOOLEAN ClockWise, double Scale ) = 0;
  }; 

#endif


