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

