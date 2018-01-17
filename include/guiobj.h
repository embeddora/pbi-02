




#ifndef __GUIOBJ_H__
#define __GUIOBJ_H__

#include "stdgfx.h"
#include "mpgfx.h"
#include "guidef.h"

extern HANDLE hInstance;
extern HANDLE hPrevInstance;
extern CHAR CmdLine[256];
extern LONG NumArgs;
extern LONG CmdShow;
#if defined (__FOROS2__)
  extern HAB  hAB;
  extern HMQ  hMQ;
#elif defined (__FORUNIX__)
  #if defined (__FORMOTIF__)
    extern XtAppContext XtApp;
  #endif  
#endif  

class GUIOBJECT : public MYOBJECT
  {
    public :
      GUIOBJECT ();
      virtual ~GUIOBJECT ();
  }; 

#endif


