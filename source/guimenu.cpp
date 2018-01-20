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

  
