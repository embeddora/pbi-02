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




#ifndef __STDGFX_H__
#define __STDGFX_H__

  #if defined (__FORMFC__)
    #include <afxwin.h>
  #elif defined (__FOROWL__)
    #include <owl/owlall.h>
  #endif
 
  
  #if defined (__FORDOS16__)
    #define __FORDOS__
    #define __16BIT__
    #define __FORx86__
  #elif defined (__FORDOS4GW__)
    #define __FORDOS__  
    #define __32BIT__
    #define __FORx86__
  #elif defined (__FORDOSPWPK32__)  
    #define __FORDOS__
    #define __FORPOWERPACK__
    #define __32BIT__
    #define __FORx86__
  #elif defined (__FORDOSPWPK16__)  
    #define __FORDOS__  
    #define __FORPOWERPACK__
    #define __16BIT__
    #define __FORx86__
  #elif defined (__FORDOSTNT__)
    #define __FORDOS__  
    #define __32BIT__
    #define __FORx86__
  #elif defined (__FORWIN16__)   
    #define __FORWINDOWS__
    #define __16BIT__
    #define __FORx86__
    #ifndef _WIN32
      #define _WIN32
    #endif  
  #elif defined (__FORWIN32s__)  
    #define __FORWINDOWS__
    #define __32BIT__
    #define __FORx86__
    #ifndef _WIN32
      #define _WIN32
    #endif  
  #elif defined (__FORWIN32__)   
    #define __FORWINDOWS__
    #define __32BIT__
    #define __FORx86__
  #elif defined (__FORWIN386__)  
    #define __FORWINDOWS__
    #define __32BIT__
    #define __FORx86__
  #elif defined (__FOROS2__)
    #define __32BIT__
    #define __FORx86__
  #elif defined (__FORUNIX__)
    #define __32BIT__
    #if defined (__FORMOTIF__)
      #include <Xm/Xm.h> 
    #else 
      #include <X11/Xlib.h> 
      #include <X11/Xutil.h> 
      #include <X11/Xresource.h>
    #endif 
  #endif

  
  #if defined (__FORWATCOM__)
    #include <i86.h>
    #include <mem.h>
    #if defined (__FORWINDOWS__)
      #define INCLUDE_COMMDLG_H
      #define INCLUDE_CUSTCNTL_H
      #define INCLUDE_DDE_H
      #define INCLUDE_DDEML_H
      #define INCLUDE_DRIVINIT_H
      #define INCLUDE_LZEXPAND_H
      #define INCLUDE_OLE_H
      #define INCLUDE_PENWOEN_H
      #define INCLUDE_PRINT_H
      #define INCLUDE_SHELLAPI_H
      #define INCLUDE_STRESS_H
      #define INCLUDE_TOOLHELP_H
      #define INCLUDE_MMSYSTEM_H
    #endif   
  #elif defined (__FORVISUAL__)
    #include <memory.h>
  #elif defined (__FORBORLAND__)
    #include <mem.h>
  #elif defined (__FORSYMANTEC__)
    #include <mem.h>
  #elif defined (__FORGCC__)
    #define __FORSUNSPARC__ 
    
  #endif  

  

    #define __LSBFIRST__


#endif

