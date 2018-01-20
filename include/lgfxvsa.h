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


#ifndef __LGFXVESA_H__
#define __LGFXVESA_H__

#include "stdgfx.h"
#include "gfxtypes.h"

extern "C"
  {
    VOID SetVESASelector ( SHORT Selector );
    VOID SetVESAData ( LONG Width, LONG Height, LONG RowSize );
    VOID VESAClear256 ( BYTE Color );
    VOID VESACopy256To256 ( BYTE *Src, LONG AddSrc, LONG Wd, LONG Ht,
                            LONG Cx, LONG Cy );
  } 

#endif
