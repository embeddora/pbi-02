




#ifndef __GUIDLG_H__
#define __GUIDLG_H__

#include "stdgfx.h"
#include "guiwin.h"

#define BUTTON_SELECT   1
#define BUTTON_UNSELECT 0
#define BUTTON_DISABLE  2 

class GUIDIALOG : public GUIWINDOW
  {
    protected :
      LONG ReturnValue;

      virtual VOID CloseWindow ();
      virtual LONG OnInitDialog ();
      virtual LONG OnCreate ();
      virtual LONG OnClose ();

      VOID SetItemText ( LONG ID, STRING Text );
      VOID SetItemNumber ( LONG ID, LONG Number );
      VOID SetButtonState ( LONG ID, LONG State );
      LONG GetItemText ( LONG ID, STRING Text );
      LONG GetItemNumber ( LONG ID );
      LONG GetButtonState ( LONG ID );

    public :
      GUIDIALOG ();
      virtual ~GUIDIALOG ();

      virtual BOOLEAN Create ( STRING Title, LONG x, LONG y, LONG Wd, LONG Ht,
                               GUIWINDOW *Parent );
      virtual LONG Execute ( GUIWINDOW *Parent, STRING Name );
      virtual LONG WndProc ( HWINDOW hWnd, MESSAGE iMessage, PARAM1 Param1, PARAM2 Param2, BOOLEAN FromClient );
  }; 

#endif
