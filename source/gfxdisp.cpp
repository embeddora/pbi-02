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

#include "gfxdisp.h"

#include "stdlib.h"



DISPLAYDRIVER::DISPLAYDRIVER () : DRIVER ()

  {

    MagH = 1;  

    DivH = 1;  

    MagV = 1;  

    DivV = 1;  

    Info = (BITMAPINFO*)malloc(sizeof(BITMAPINFOHEADER)+sizeof(RGBQUAD)*256);

  }



DISPLAYDRIVER::~DISPLAYDRIVER ()

  {

    if (Info!=NULL)

      delete Info;

    Info = NULL;

  } // End of Destructor for DISPLAYDRIVER



VOID DISPLAYDRIVER::Clear ( HDISPLAY hDisplay, LONG Color )

  {

    if (hDisplay)

      {}

    if (Color)

      {}

  } 



VOID DISPLAYDRIVER::SetScaleFactor ( LONG MH, LONG DH, LONG MV, LONG DV )

  {

    MagH = MH;  

    DivH = DH;  

    MagV = MV;  

    DivV = DV;  

  }

  

VOID DISPLAYDRIVER::DrawText ( HDISPLAY hDisplay, STRING Text, LONG x, LONG y,

                               COLOR Color )

  {

    if (hDisplay)

      {}

    if (Text)

      {}

    if (Color)

      {}

    if (x&y)

      {}

  } 

