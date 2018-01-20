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
#include "lgfxdib.h"

LONG  U;
LONG  V;
LONG  Du;
LONG  Dv;
LONG  SrcAdd;





VOID WaitForRetraceMany ( LONG Count )
{
if (Count)
  {}
} 




VOID WaitForRetraceOnce ()
{
} 




VOID WaitForRetraceTerminate ()
{
} 






VOID TransBlt256To256 ( BYTE *Dest, BYTE *Src, 
                        LONG dWd, BYTE TransColor )
{
LONG i;
BYTE Color;

for (i=0;i<dWd;i++)
  {
    Color = (*Src);
    if (Color==TransColor)
      (*Dest) = Color;
    Src++;
    Dest++;    
  } 
} 






VOID ScaleBlt256To256 ( BYTE *Dest, BYTE *Src, 
                        LONG dWd, LONG Error, DWORD AddError )
{
LONG i;
BYTE Color;

for (i=0;i<dWd;i++)
  {
    Color = (*(Src+(Error>>16)));
    Error += AddError;
    (*Dest) = Color;
    Dest++;    
  } 
} 










VOID SetAffineUV ( LONG U, LONG V, LONG Du, LONG Dv )
{
  if (U&V&Du&Dv)
  {}
} 
  





VOID Affine256To256 ( BYTE *Dest, BYTE *Src, LONG dWd )
{
if (Dest)
  {}
if (Src)
  {}
if (dWd)
  {}
} 



