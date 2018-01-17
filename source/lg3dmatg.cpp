




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

