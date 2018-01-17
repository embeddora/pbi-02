




#ifndef __GUITBAR_H__
#define __GUITBAR_H__

#include "stdgfx.h"
#include "guichild.h"
#include "guidlg.h"

struct BUTTONDATA
  {
    LONG x, y;  
    LONG Command;
    LONG State;
  }; 

class GUITOOLBAR : public GUICHILD
  {
    protected :
      LONG ImgWd,ImgHt;
      LONG BarHt;

      HMEMDISPLAY hMemDisplay;
      HBITMAP hBitmap;
      HBITMAP OldBitmap;
      LONG NumButtons;
      BUTTONDATA *Buttons;
      LONG Selected;

      virtual VOID RegisterWindow ();
      virtual LONG OnPaint ( HDISPLAY hDisplay );
      virtual LONG OnMouse ( LONG Event, LONG Flags, LONG Mx, LONG My );
      
      VOID CreateBar ();
      VOID DestroyBar ();

      VOID DrawButton ( HDISPLAY hDisplay, LONG Index );  
      VOID SetButtonAttr ( LONG Index, LONG Command );
      LONG FindButton ( LONG Mx, LONG My );

    public :
      GUITOOLBAR ();
      virtual ~GUITOOLBAR ();

      BOOLEAN Load ( STRING Name, LONG Num, BUTTONDATA *Data );
      virtual LONG WndProc ( HWINDOW hWnd, MESSAGE iMessage, PARAM1 Param1, PARAM2 Param2,
                             BOOLEAN FromClient );

      VOID SetButtonSize ( LONG Wd, LONG Ht );
      VOID SetBarHeight ( LONG BarHeight );
      virtual BOOLEAN Create ( STRING Title, LONG x, LONG y, LONG Wd, LONG Ht,
                               GUIWINDOW *Parent );
      virtual BOOLEAN Create ( GUIWINDOW *Parent );
  }; 

#endif
