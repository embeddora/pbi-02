




#ifndef __GUIMDI_H__
#define __GUIMDI_H__

#include "stdgfx.h"
#include "guichild.h"
#include "guiframe.h"

class GUIMDICHILD : public GUICHILD
  {
    protected :
      #if defined (__FORWINDOWS__)
        MDICREATESTRUCT MDICreate;
      #endif
      VOID RegisterWindow ();

    public :
      GUIMDICHILD ();
      virtual ~GUIMDICHILD ();

      BOOLEAN Create ( STRING Title, LONG x, LONG y, LONG Wd, LONG Ht,
                       GUIWINDOW *Parent );
  }; 

class GUIMDIFRAME : public GUIFRAME
  {
    protected :
      VOID RegisterWindow ();

    public :
      GUIMDIFRAME ();
      virtual ~GUIMDIFRAME ();

      virtual BOOLEAN IsMDISupported () { return TRUE; };
      virtual LONG WndProc ( HWINDOW hWnd, MESSAGE iMessage, PARAM1 Param1, PARAM2 Param2, BOOLEAN FromClient );
      BOOLEAN Create ( STRING Title, LONG x, LONG y, LONG Wd, LONG Ht,
                       GUIWINDOW *Parent );
  }; 

#endif
