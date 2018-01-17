




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

