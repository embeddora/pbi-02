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


#ifndef __GUICMDLG_H__
#define __GUICMDLG_H__

#include "stdgfx.h"
#include "guidlg.h"

#if defined (__FORWINDOWS__)
  #include <commdlg.h>
#endif

#define FD_TYPE  LONG
#define FD_OPEN  1
#define FD_SAVE  2

struct EXTENSION
  {
    STRING Ext;
    STRING Description;
  }; 

struct FILENAMEATTR
  {
    STRING DefPath;
    STRING DefExt;
    STRING DefName;
    LONG   NumExt;
    EXTENSION *Extensions;
  }; 

class GUIFILEDIALOG : public GUIDIALOG
  {
    protected :
      FILENAMEATTR FileAttr;
      BOOLEAN ForOpen;
      STRING  FileName;

      virtual VOID CloseWindow ();
      virtual LONG OnCreate ();
      virtual LONG OnClose ();
      LONG ExecuteOpen ( HWINDOW hWindow );
      LONG ExecuteSave ( HWINDOW hWindow );

    public :
      GUIFILEDIALOG ( FD_TYPE ForOpen );
      virtual ~GUIFILEDIALOG ();

      virtual BOOLEAN Create ( STRING Title, LONG x, LONG y, LONG Wd, LONG Ht,
                               GUIWINDOW *Parent );
      virtual LONG Execute ( GUIWINDOW *Parent, STRING Name );
      virtual LONG WndProc ( HWINDOW hWnd, MESSAGE iMessage, PARAM1 Param1, PARAM2 Param2, BOOLEAN FromClient );
      VOID SetDefaultPath ( STRING Path );
      VOID SetDefaultName ( STRING FileName );
      VOID SetDefaultExt ( STRING ExtName );
      VOID CreateExtensions ( LONG Num );
      VOID SetExtension ( LONG Index, STRING Ext, STRING Description );
      VOID GetFileName ( STRING Name );
  }; 

#endif

