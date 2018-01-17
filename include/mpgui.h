




#ifndef __MPGUI_H__
#define __MPGUI_H__

#include "stdgfx.h"
#include "guiapp.h"

VOID ProcessArgs ( int argc, char **argv );
VOID InitApp ();
LONG GetNumArgs ();
VOID GetArg ( LONG Index, STRING Str );
VOID PrepareArg ( STRING args );

#if defined (__FORWINDOWS__)
  #define MAINPROGRAM int PASCAL WinMain ( HINSTANCE hIns, HINSTANCE hPrevIns, LPSTR lpCmdLine, int nCmdShow )
  #define BEGIN { hInstance = hIns; hPrevInstance = hPrevIns; PrepareArg (lpCmdLine); CmdShow = nCmdShow; InitApp ();
  #define END   }
#elif defined (__FOROS2__)
  #define MAINPROGRAM int main ( int argc, char **argv )
  #define BEGIN { ProcessArgs ( argc, argv ); InitApp ();
  #define END   }
#elif defined (__FORDOS__)
  #define MAINPROGRAM int main ( int argc, char **argv )
  #define BEGIN { ProcessArgs ( argc, argv );
  #define END   }
#elif defined (__FORUNIX__)
  #define MAINPROGRAM int main ( int argc, char **argv )
  #define BEGIN { ProcessArgs ( argc, argv );
  #define END   }
#endif

#endif


