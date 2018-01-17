




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
