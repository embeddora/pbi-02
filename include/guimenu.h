




#ifndef __GUIMENU_H__
#define __GUIMENU_H__

#include "stdgfx.h"
#include "guiobj.h"

#if defined (__FORWINDOWS__)
  #define MENU_STATE_ENABLED  MF_ENABLED
  #define MENU_STATE_DISABLED MF_DISABLED
  #define MENU_STATE_GRAYED   MF_GRAYED  
#elif defined (__FOROS2__)
  #define HMENU HANDLE
  #define MENU_STATE_ENABLED  1
  #define MENU_STATE_DISABLED 2
  #define MENU_STATE_GRAYED   4
#elif defined (__FORDOS__)
  #define HMENU HANDLE
  #define MENU_STATE_ENABLED  1
  #define MENU_STATE_DISABLED 2
  #define MENU_STATE_GRAYED   4
#elif defined (__FORUNIX__)
  #define HMENU HANDLE
  #define MENU_STATE_ENABLED  1
  #define MENU_STATE_DISABLED 2
  #define MENU_STATE_GRAYED   4
#endif

class GUIMENU : public GUIOBJECT
  {
    protected :
      HMENU hMenu;

    public :
      GUIMENU ();
      virtual ~GUIMENU ();

      BOOLEAN Load ( STRING Name );
      HMENU GetHandle () { return hMenu; };

      VOID SetItemState ( LONG CommandID, LONG State );
  }; 

#endif

