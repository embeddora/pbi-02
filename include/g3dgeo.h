




#ifndef __G3DGEO_H__
#define __G3DGEO_H__

#include "stdgfx.h"
#include "g3dfile.h"

class G3DGEOTOOL : public G3DFILETOOL
  {
    protected :
      BOOLEAN LoadPoints ( FILEHANDLE f, G3DPOLYHEDRON *Object,
                           LONG NumPoints, double Scale );
      BOOLEAN LoadFaces ( FILEHANDLE f, G3DPOLYHEDRON *Object );
      
      BOOLEAN SavePoints ( FILEHANDLE f, G3DPOLYHEDRON *Object,
                           LONG NumPoints, double Scale );
      BOOLEAN SaveFaces ( FILEHANDLE f, G3DPOLYHEDRON *Object );
    
    public :
      G3DGEOTOOL ();
      virtual ~G3DGEOTOOL ();

      virtual BOOLEAN Load ( FILEHANDLE f, LINKEDLIST<G3DOBJECT*> *ObjectList,
                             BOOLEAN ClockWise, double Scale );
      virtual BOOLEAN Save ( FILEHANDLE f, LINKEDLIST<G3DOBJECT*> *ObjectList,
                             BOOLEAN ClockWise, double Scale );
  }; 

#endif


