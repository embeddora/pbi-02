




#include "stdgfx.h"
#include "guiobj.h"

HANDLE hInstance = 0;
HANDLE hPrevInstance = 0;
CHAR CmdLine[256];
LONG NumArgs = 0;
LONG CmdShow = 0;
#if defined (__FOROS2__)
  HAB  hAB = 0;
  HMQ  hMQ = 0;
#elif defined (__FORUNIX__)
  #if defined (__FORMOTIF__)
    XtAppContext XtApp;
  #endif
#endif

GUIOBJECT::GUIOBJECT ()
  {
  } 

GUIOBJECT::~GUIOBJECT ()
  {
  } 

