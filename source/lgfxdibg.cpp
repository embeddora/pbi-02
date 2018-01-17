




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



