




#ifndef __GFXINPUT_H__
#define __GFXINPUT_H__

#include "stdgfx.h"
#include "gfxdev.h"
#include "gfxevent.h"

class INPUT : public DCIClass
  {
    protected :
      BOOLEAN KBDInstalled;
      void InstallKBDHandler ();
      void UnInstallKBDHandler ();

    public :
      INPUT ();
      virtual ~INPUT ();

      virtual BOOLEAN Init ();
      virtual VOID DeInit ();
      BOOLEAN IsKeyDown ( INT WhichKey );
      VOID WaitForKey ( INT WhichKey );
  }; 

extern INPUT Input;

#endif
