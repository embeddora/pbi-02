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
