




#ifndef __GFXTIMER_H__
#define __GFXTIMER_H__

#include "stdgfx.h"
#include "gfxdev.h"
#include <time.h>

struct DATETIMETYPE
  {
    SHORT Year;
    BYTE  Month;
    BYTE  Day;
    BYTE  Hour;
    BYTE  Minutes;      
  }; 

class MYTIMER : public DCIClass
  {
    protected :
      LONG StartClock,EndClock;

    public :
      MYTIMER ();
      virtual ~MYTIMER ();

      virtual BOOLEAN Init ();
      virtual VOID DeInit ();

      VOID StartTimer ();
      VOID EndTimer ();
      double GetFrameRate ( LONG Count );

      VOID GetDateTime ( DATETIMETYPE *DateTime );
  }; 

#endif
