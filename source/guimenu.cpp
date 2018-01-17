




#include "stdgfx.h"
#include "guimenu.h"

GUIMENU::GUIMENU () : GUIOBJECT ()
  {
    hMenu = NULL;
  } 

GUIMENU::~GUIMENU ()
  {
    #if defined (__FORWINDOWS__)
      if (hMenu!=NULL)
        DestroyMenu ( hMenu );
    #endif
  } 

BOOLEAN GUIMENU::Load ( STRING Name )
  {
    #if defined (__FORWINDOWS__)
      if (hMenu!=NULL)
        DestroyMenu ( hMenu );

      hMenu = LoadMenu ( hInstance, Name );
      if (hMenu==NULL)
        return FAILURE;
      return SUCCESS;
    #else
      if (Name)
        {}
      return SUCCESS;
    #endif
  } 

VOID GUIMENU::SetItemState ( LONG CommandID, LONG State )
  {
    if (CommandID&State)
      {}  
    #if defined (__FORWINDOWS__)
      #if defined (__FORWIN386__)
        EnableMenuItem ( hMenu, (short)CommandID, (short)(State|MF_BYCOMMAND) );
      #else
        EnableMenuItem ( hMenu, CommandID, State | MF_BYCOMMAND );
      #endif
    #elif defined (__FOROS2__)
    #elif defined (__FORDOS__)
    #endif  
  } 

  
