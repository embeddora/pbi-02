




#ifndef __GUIFRAME_H__
#define __GUIFRAME_H__

#include "stdgfx.h"
#include "guiwin.h"

class GUIFRAME : public GUIWINDOW
  {
    protected :
      VOID RegisterWindow ();
      virtual LONG OnCreate ();
      virtual LONG OnClose ();
      virtual LONG OnDestroy ();

    public :
      GUIFRAME ();
      virtual ~GUIFRAME ();

      virtual BOOLEAN IsMDISupported () { return FALSE; };
      virtual BOOLEAN Create ( STRING Title, LONG x, LONG y, LONG Wd, LONG Ht,
                               GUIWINDOW *Parent );
      virtual BOOLEAN AddChild ( GUIWINDOW *Child, STRING Title, LONG x, LONG y,
                                 LONG Wd, LONG Ht );
  }; 

#endif

