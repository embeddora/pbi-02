




#ifndef __GUIAPP_H__
#define __GUIAPP_H__

#include "stdgfx.h"
#include "guimdi.h"
#include "guifloat.h"
#include "guitbar.h"
#include "guicmdlg.h"

class GUIAPP : public GUIOBJECT
  {
    protected :
      VOID GoDOS ( GUIFRAME *MainWindow, DISPLAYDATA *Data );
      VOID GoWinSDI ( GUIFRAME *MainWindow, DISPLAYDATA *Data );
      VOID GoWinMDI ( GUIFRAME *MainWindow, DISPLAYDATA *Data );
      VOID GoOS2 ( GUIFRAME *MainWindow, DISPLAYDATA *Data );
      VOID GoXWindow ( GUIFRAME *MainWindow, DISPLAYDATA *Data );
      VOID GoMotif ( GUIFRAME *MainWindow, DISPLAYDATA *Data );   

    public :
      GUIAPP ();
      virtual ~GUIAPP ();

      VOID Run ( DISPLAYDATA *Data, GUIFRAME *MainWindow );
  }; 


#endif

