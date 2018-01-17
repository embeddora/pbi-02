




#include "stdgfx.h"
#include "guichild.h"

GUICHILD::GUICHILD () : GUIWINDOW ()
  {
    strcpy ( ClassName, "GUICHILD" );
    AddWinStyle ( GUI_WIN_CHILD | GUI_WIN_VISIBLE );
  } 

GUICHILD::~GUICHILD ()
  {
  } 

LONG GUICHILD::OnCreate ()
  {
    return 0;
  } 

LONG GUICHILD::OnClose ()
  {
    return 0;
  } 
