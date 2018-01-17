




#include "stdgfx.h"
#include "g3dcam.h"

G3DCAMERA::G3DCAMERA () : G3DOBJECT ()
  {
  } 

G3DCAMERA::~G3DCAMERA ()
  {
  } 
  
VOID G3DCAMERA::SetPosition ( float x, float y, float z )
  {
    G3DOBJECT::SetPosition ( (float)0, (float)0, (float)0 );
    Move ( x, y, z );
  } 



