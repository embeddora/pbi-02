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
#include "lg3dmat.h"





VOID _FLPMultMatrix ( G3DMATRIX Mat1, G3DMATRIX Mat2, G3DMATRIX Result )
{
  INT i,j,k;
	float Val;

	for (i=0;i<4;i++)
		{
  	  for (j=0;j<4;j++)
		    {
          Val = 0;
	        for (k=0;k<4;k++)
		        {
						  Val += Mat1[i][k]*Mat2[k][j];
		        } 
					Result[i][j] = Val;
		    } 
		} 
} 

