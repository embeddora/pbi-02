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

