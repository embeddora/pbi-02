




#ifndef __G3DASC_H__
#define __G3DASC_H__

#include "stdgfx.h"
#include "g3dfile.h"

class G3DASCTOOL : public G3DFILETOOL
  {
    protected :
      
      INT ReadASCToken ( FILEHANDLE f, STRING String );
      INT GoToASCToken ( FILEHANDLE f, STRING String );
      INT GetASCNumber ( FILEHANDLE f, double *Number );
      INT GetASCInt ( FILEHANDLE f, LONG *Number );
      INT GetASCName ( FILEHANDLE f, STRING Name );
      BOOLEAN LoadASCPolyObject ( FILEHANDLE f,
                                  G3DPOLYHEDRON *Object, 
                                  double Scale );    
    public :
      G3DASCTOOL ();
      virtual ~G3DASCTOOL ();
      
      virtual BOOLEAN Load ( FILEHANDLE f, LINKEDLIST<G3DOBJECT*> *ObjectList,
                             BOOLEAN ClockWise, double Scale );
      virtual BOOLEAN Save ( FILEHANDLE f, LINKEDLIST<G3DOBJECT*> *ObjectList,
                             BOOLEAN ClockWise, double Scale );
  }; 

#endif


