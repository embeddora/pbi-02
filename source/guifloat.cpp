




#include "stdgfx.h"
#include "guifloat.h"

GUIFLOAT::GUIFLOAT () : GUICHILD ()
  {
    strcpy ( ClassName, "GUIFLOAT" );
    SetWinStyle ( GUI_WIN_OVERLAPPEDWINDOW );
    SetWinExStyle ( GUI_WINX_TOPMOST );
  } 

GUIFLOAT::~GUIFLOAT ()
  {
  } 

