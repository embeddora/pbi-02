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

