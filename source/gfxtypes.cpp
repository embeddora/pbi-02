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
#include "gfxtypes.h"

VOID SwapWord ( WORD *ValuePtr )
  {
    BYTE Low,Hi;
    WORD Value;

    Value = *ValuePtr;
    Low  = (BYTE)(Value&0xFF);
    Hi = (BYTE)(Value>>8);

    Value = (WORD)(((WORD)Low<<8)+(WORD)Hi);
    *ValuePtr = Value;
  } 
  
VOID SwapDWord ( DWORD *ValuePtr )
  {
    WORD Low,Hi;
    DWORD Value;

    Value = *ValuePtr;
    Low  = (WORD)(Value&0xFFFF);
    Hi = (WORD)(Value>>16);

    SwapWord ( &Low );
    SwapWord ( &Hi );

    Value = (DWORD)(((DWORD)Low<<16)+(DWORD)Hi);
    *ValuePtr = Value;
  } 
