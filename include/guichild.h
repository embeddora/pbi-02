




#ifndef __GUICHILD_H__
#define __GUICHILD_H__

#include "stdgfx.h"
#include "guiframe.h"

class GUICHILD : public GUIWINDOW
  {
    protected :
      virtual LONG OnCreate ();
      virtual LONG OnClose ();

    public :
      GUICHILD ();
      virtual ~GUICHILD ();
  }; 

#endif

