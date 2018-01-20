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
