/*
 * Copyright (C) 2018 ARNERI arneri@ukr.net All rights reserved
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.

 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * Abstract: 
 */



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
