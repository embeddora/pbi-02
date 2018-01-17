




#ifndef __G3DGEM_H__
#define __G3DGEM_H__

#include "stdgfx.h"
#include "g3dfile.h"

class G3DGEMTOOL : public G3DFILETOOL
  {
    protected :
      BOOLEAN LoadPoints ( FILEHANDLE f, G3DPOLYHEDRON *Object,
                           LONG NumPoints, double Scale );
      BOOLEAN LoadFaces ( FILEHANDLE f, G3DPOLYHEDRON *Object,
                          LONG NumFaces );
    
    public :
      G3DGEMTOOL ();
      virtual ~G3DGEMTOOL ();

      virtual BOOLEAN Load ( FILEHANDLE f, LINKEDLIST<G3DOBJECT*> *ObjectList,
                             BOOLEAN ClockWise, double Scale );
      virtual BOOLEAN Save ( FILEHANDLE f, LINKEDLIST<G3DOBJECT*> *ObjectList,
                             BOOLEAN ClockWise, double Scale );
  }; 

#endif


