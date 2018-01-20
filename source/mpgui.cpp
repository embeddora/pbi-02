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
#include "mpgui.h"

void ProcessArgs ( int argc, char **argv )
  {
    INT i;
    INT Pos=0;
    NumArgs = argc-1;
         for (i=1;i<argc;i++)
      {
        INT j=0;
        while (argv[i][j]!=NULL)
          {
            CmdLine[Pos] = argv[i][j];
            Pos++;
            j++;
          } 
        CmdLine[Pos] = ' ';
        Pos++;
      } 
  } 

void InitApp ()
  {
    InitWinHashTable ();
    #if defined (__FOROS2__)
      hAB = WinInitialize ( 0 );
      hMQ = WinCreateMsgQueue ( hAB, 0 );
    #elif defined (__FORUNIX__)
    #endif
  } 

VOID PrepareArg ( STRING args )
  {
    INT i,Pos;

    i = 0;
    Pos = 0;
    NumArgs = 0;
    while (args[i]!=NULL)
      {
        while ((args[i]==' '))
          i++;
        if (args[i]==NULL)
          break;
          
        while ((args[i]!=' ')&&(args[i]!=NULL))
          {
            CmdLine[Pos++] = args[i++];  
          } 
        NumArgs++;  
        CmdLine[Pos++] = ' ';  
      } 
    if (Pos>0)  
      CmdLine[Pos-1] = NULL;
    else
      CmdLine[Pos] = NULL;  
  } 

LONG GetNumArgs ()
  {
    return NumArgs;    
  } 

VOID GetArg ( LONG Index, STRING Str )
  {
    if (Str==NULL)
      return;
      
    if ((Index>=NumArgs)||(Index<0))
      {
        Str[0] = NULL;  
      } 

    INT i,Count;

    i = 0;
    Count = 0;
    while (Count!=Index)
      {
        if (CmdLine[i]==' ')
          Count++;
        i++;  
      } 

    INT j;
    j = 0;
    while ((CmdLine[i]!=' ')&&(CmdLine[i]!=NULL))
      Str[j++] = CmdLine[i++];
    Str[j] = NULL;  
  } 

  








