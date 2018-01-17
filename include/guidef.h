




#ifndef __GUIDEF_H__
#define __GUIDEF_H__

#include "stdgfx.h"
#include "mpgfx.h"


#if defined (__FORWINDOWS__)
  #define MESSAGE  UINT 
  #define PARAM1   WPARAM
  #define PARAM2   LPARAM
  #define HDESKTOP NULL
  #define APIRESULT LONG
  #define APIPROC  FAR PASCAL
  #define DLGPROC  CALLBACK
  
  #define WPOS_NOSIZE SWP_NOSIZE
  #define WPOS_NOMOVE SWP_NOMOVE
  #define SBAR_HORZ   SB_HORZ
  #define SBAR_VERT   SB_VERT
  #define SBAR_BOTTOM SB_BOTTOM   
  #define SBAR_ENDSCROLL SB_ENDSCROLL
  #define SBAR_LINEDOWN  SB_LINEDOWN
  #define SBAR_LINEUP    SB_LINEUP
  #define SBAR_PAGEDOWN  SB_PAGEDOWN
  #define SBAR_PAGEUP    SB_PAGEUP
  #define SBAR_THUMBPOSITION SB_THUMBPOSITION
  #define SBAR_THUMBTRACK SB_THUMBTRACK
  #define SBAR_TOP       SB_TOP
  #define MF_LDOWN       MK_LBUTTON
  #define MF_MDOWN       MK_MBUTTON
  #define MF_RDOWN       MK_RBUTTON
  
  #define WINPOS_DEFAULT CW_USEDEFAULT
  
#elif defined (__FOROS2__)
  #define MESSAGE  ULONG
  #define PARAM1   MPARAM
  #define PARAM2   MPARAM
  #define HDESKTOP HWND_DESKTOP
  #define APIRESULT MRESULT
  #define APIPROC   EXPENTRY
  #define DLGPROC   APIPROC

  #define WPOS_NOSIZE 0x0001
  #define WPOS_NOMOVE 0x0002
  #define SBAR_HORZ  0
  #define SBAR_VERT  1
  #define SBAR_BOTTOM    0
  #define SBAR_ENDSCROLL 1
  #define SBAR_LINEDOWN  2
  #define SBAR_LINEUP    3
  #define SBAR_PAGEDOWN  4
  #define SBAR_PAGEUP    5
  #define SBAR_THUMBPOSITION 6 
  #define SBAR_THUMBTRACK 7 
  #define SBAR_TOP       8       
  #define MF_LDOWN       0
  #define MF_MDOWN       1
  #define MF_RDOWN       2
  #define WINPOS_DEFAULT 0

  #define LOWORD(l) ((WORD)(l))
  #define HIWORD(l) ((WORD)((DWORD)(l) >> 16))
  #define RGB(r,g,b) ((((LONG)b)<<16)+(((LONG)g)<<8)+r)
  
#elif defined (__FORDOS__)
  #define BOOL     BOOLEAN
  #define MESSAGE  ULONG
  #define PARAM1   WORD
  #define PARAM2   LONG
  #define HDESKTOP NULL
  #define FAR far
  #define PASCAL    pascal
  #define APIRESULT LONG
  #define APIPROC   PASCAL
  #define DLGPROC   APIPROC

  #define WPOS_NOSIZE 0x0001
  #define WPOS_NOMOVE 0x0002
  #define SBAR_HORZ  0
  #define SBAR_VERT  1
  #define SBAR_BOTTOM    0
  #define SBAR_ENDSCROLL 1
  #define SBAR_LINEDOWN  2
  #define SBAR_LINEUP    3
  #define SBAR_PAGEDOWN  4
  #define SBAR_PAGEUP    5
  #define SBAR_THUMBPOSITION 6 
  #define SBAR_THUMBTRACK 7 
  #define SBAR_TOP       8       
  #define MF_LDOWN       0
  #define MF_MDOWN       1
  #define MF_RDOWN       2 
  #define WINPOS_DEFAULT 0
  
  #define LOWORD(l) ((WORD)(l))
  #define HIWORD(l) ((WORD)((DWORD)(l) >> 16))
  #define RGB(r,g,b) ((((LONG)b)<<16)+(((LONG)g)<<8)+r)

#elif defined (__FORUNIX__)
  #define BOOL     BOOLEAN
  #define MESSAGE  ULONG

  #if defined (__FORMOTIF__)
    #define PARAM1   XEvent*
    #define PARAM2   XtPointer
  #else
    #define PARAM1   WORD
    #define PARAM2   LONG
  #endif

  #define HDESKTOP NULL
  #define FAR far
  #define PASCAL    
  #define APIRESULT LONG
  #define APIPROC   
  #define DLGPROC   APIPROC

  #define WPOS_NOSIZE 0x0001
  #define WPOS_NOMOVE 0x0002
  #define SBAR_HORZ  0
  #define SBAR_VERT  1
  #define SBAR_BOTTOM    0
  #define SBAR_ENDSCROLL 1
  #define SBAR_LINEDOWN  2
  #define SBAR_LINEUP    3
  #define SBAR_PAGEDOWN  4
  #define SBAR_PAGEUP    5
  #define SBAR_THUMBPOSITION 6 
  #define SBAR_THUMBTRACK 7 
  #define SBAR_TOP       8       
  #define MF_LDOWN       0
  #define MF_MDOWN       1
  #define MF_RDOWN       2 
  #define WINPOS_DEFAULT 0
  
  #define LOWORD(l) ((WORD)(l))
  #define HIWORD(l) ((WORD)((DWORD)(l) >> 16))
  #define RGB(r,g,b) ((((LONG)b)<<16)+(((LONG)g)<<8)+r)

  extern BOOLEAN ExitApp;
#endif

#define GUI_DESKTOP NULL

#define HD_DESKTOP 0
#define HD_WINDOW  1
#define HD_CLIENT  2 


#if defined (__FORWINDOWS__)
        
  #define GUI_WM_ACTIVATE      WM_ACTIVATE

  
  #define GUI_WM_COMMAND       WM_COMMAND 
  
  
  #define GUI_WM_CREATE        WM_CREATE
  
  
  #define GUI_WM_CLOSE         WM_CLOSE
  #define GUI_WM_DESTROY       WM_DESTROY
  
  
  #define GUI_WM_PAINT         WM_PAINT
  
  
  #define GUI_WM_INITDIALOG    WM_INITDIALOG
  
  
  #define GUI_WM_MOUSEMOVE     WM_MOUSEMOVE
  #define GUI_WM_LBUTTONDOWN   WM_LBUTTONDOWN
  #define GUI_WM_LBUTTONUP     WM_LBUTTONUP
  #define GUI_WM_LBUTTONDBLCLK WM_LBUTTONDBLCLK
  #define GUI_WM_MBUTTONDOWN   WM_MBUTTONDOWN
  #define GUI_WM_MBUTTONUP     WM_MBUTTONUP
  #define GUI_WM_MBUTTONDBLCLK WM_MBUTTONDBLCLK
  #define GUI_WM_RBUTTONDOWN   WM_RBUTTONDOWN
  #define GUI_WM_RBUTTONUP     WM_RBUTTONUP
  #define GUI_WM_RBUTTONDBLCLK WM_RBUTTONDBLCLK
  
  
  #define GUI_WM_KEYUP         WM_KEYUP
  #define GUI_WM_KEYDOWN       WM_KEYDOWN
  #define GUI_WM_CHAR          WM_CHAR 
  
  
  #define GUI_WM_MOVE          WM_MOVE
  #define GUI_WM_POSCHANGING   WM_WINDOWPOSCHANGING
  #define GUI_WM_SIZE          WM_SIZE
  
  
  #define GUI_WM_TIMER         WM_TIMER
  
  
  #define GUI_WM_HSCROLL       WM_HSCROLL 
  #define GUI_WM_VSCROLL       WM_VSCROLL 
  
#elif defined (__FOROS2__)
        
  #define GUI_WM_ACTIVATE      WM_ACTIVATE

  
  #define GUI_WM_COMMAND       WM_COMMAND 
  
  
  #define GUI_WM_CREATE        WM_CREATE
  
  
  #define GUI_WM_CLOSE         WM_CLOSE
  #define GUI_WM_DESTROY       WM_DESTROY
  
  
  #define GUI_WM_PAINT         WM_PAINT
  
  
  #define GUI_WM_INITDIALOG    25
  
  
  #define GUI_WM_MOUSEMOVE     WM_MOUSEMOVE
  #define GUI_WM_LBUTTONDOWN   WM_LBUTTONDOWN
  #define GUI_WM_LBUTTONUP     WM_LBUTTONUP
  #define GUI_WM_LBUTTONDBLCLK WM_LBUTTONDBLCLK
  #define GUI_WM_MBUTTONDOWN   WM_MBUTTONDOWN
  #define GUI_WM_MBUTTONUP     WM_MBUTTONUP
  #define GUI_WM_MBUTTONDBLCLK WM_MBUTTONDBLCLK
  #define GUI_WM_RBUTTONDOWN   WM_RBUTTONDOWN
  #define GUI_WM_RBUTTONUP     WM_RBUTTONUP
  #define GUI_WM_RBUTTONDBLCLK WM_RBUTTONDBLCLK
  
  
  #define GUI_WM_KEYUP         WM_KEYUP
  #define GUI_WM_KEYDOWN       WM_KEYDOWN
  #define GUI_WM_CHAR          WM_CHAR 
  
  
  #define GUI_WM_MOVE          WM_MOVE
  #define GUI_WM_POSCHANGING   WM_WINDOWPOSCHANGING
  #define GUI_WM_SIZE          WM_SIZE
  
  
  #define GUI_WM_TIMER         WM_TIMER
  
  
  #define GUI_WM_HSCROLL       WM_HSCROLL 
  #define GUI_WM_VSCROLL       WM_VSCROLL 
  
#elif defined (__FORDOS__)
        
  #define GUI_WM_ACTIVATE      1

  
  #define GUI_WM_COMMAND       2
  
  
  #define GUI_WM_CREATE        3
  
  
  #define GUI_WM_CLOSE         4
  #define GUI_WM_DESTROY       5
  
  
  #define GUI_WM_PAINT         6
  
  
  #define GUI_WM_MOUSEMOVE     7
  #define GUI_WM_LBUTTONDOWN   8
  #define GUI_WM_LBUTTONUP     9
  #define GUI_WM_LBUTTONDBLCLK 10
  #define GUI_WM_MBUTTONDOWN   11
  #define GUI_WM_MBUTTONUP     12
  #define GUI_WM_MBUTTONDBLCLK 13
  #define GUI_WM_RBUTTONDOWN   14
  #define GUI_WM_RBUTTONUP     15
  #define GUI_WM_RBUTTONDBLCLK 16
  
  
  #define GUI_WM_KEYUP         17
  #define GUI_WM_KEYDOWN       18
  #define GUI_WM_CHAR          19
  
  
  #define GUI_WM_MOVE          20
  #define GUI_WM_POSCHANGING   21
  #define GUI_WM_SIZE          22
  
  
  #define GUI_WM_TIMER         23
  
  
  #define GUI_WM_HSCROLL       24
  #define GUI_WM_VSCROLL       25
  
  
  #define GUI_WM_INITDIALOG    26  
  
#elif defined (__FORUNIX__)
        
  #define GUI_WM_ACTIVATE      1

  
  #define GUI_WM_COMMAND       2
  
  
  #define GUI_WM_CREATE        3
  
  
  #define GUI_WM_CLOSE         4
  #define GUI_WM_DESTROY       5
  
  
  #define GUI_WM_PAINT         6
  
  
  #define GUI_WM_MOUSEMOVE     7
  #define GUI_WM_LBUTTONDOWN   8
  #define GUI_WM_LBUTTONUP     9
  #define GUI_WM_LBUTTONDBLCLK 10
  #define GUI_WM_MBUTTONDOWN   11
  #define GUI_WM_MBUTTONUP     12
  #define GUI_WM_MBUTTONDBLCLK 13
  #define GUI_WM_RBUTTONDOWN   14
  #define GUI_WM_RBUTTONUP     15
  #define GUI_WM_RBUTTONDBLCLK 16
  
  
  #define GUI_WM_KEYUP         17
  #define GUI_WM_KEYDOWN       18
  #define GUI_WM_CHAR          19
  
  
  #define GUI_WM_MOVE          20
  #define GUI_WM_POSCHANGING   21
  #define GUI_WM_SIZE          22
 
  
  #define GUI_WM_TIMER         23
  
  
  #define GUI_WM_HSCROLL       24
  #define GUI_WM_VSCROLL       25
  
  
  #define GUI_WM_INITDIALOG    26  
#endif



#if defined (__FORWINDOWS__)
  #define GUI_WIN_CHILD       WS_CHILD
  #define GUI_WIN_VISIBLE     WS_VISIBLE
  #define GUI_WIN_POPUP       WS_POPUP
  #define GUI_WIN_POPUPWINDOW       WS_POPUPWINDOW
  #define GUI_WIN_OVERLAPPED  WS_OVERLAPPED
  #define GUI_WIN_OVERLAPPEDWINDOW  WS_OVERLAPPEDWINDOW
  #define GUI_WIN_THICKFRAME  WS_THICKFRAME
  #define GUI_WIN_SYSMENU     WS_SYSMENU
  #define GUI_WIN_CAPTION     WS_CAPTION
  #define GUI_WIN_BORDER      WS_BORDER
  #define GUI_WIN_VSCROLL     WS_VSCROLL
  #define GUI_WIN_HSCROLL     WS_HSCROLL
  #define GUI_WIN_CLIPSIBLINGS WS_CLIPSIBLINGS
  #define GUI_WINX_TOPMOST     WS_EX_TOPMOST
  
#elif defined (__FOROS2__)
  #define GUI_WIN_CHILD       WS_CHILD
  #define GUI_WIN_VISIBLE     WS_VISIBLE
  #define GUI_WIN_POPUP       WS_POPUP
  #define GUI_WIN_POPUPWINDOW       WS_POPUPWINDOW
  #define GUI_WIN_OVERLAPPED  WS_OVERLAPPED
  #define GUI_WIN_OVERLAPPEDWINDOW  WS_OVERLAPPED
  #define GUI_WIN_THICKFRAME  WS_THICKFRAME
  #define GUI_WIN_SYSMENU     WS_SYSMENU
  #define GUI_WIN_CAPTION     WS_CAPTION
  #define GUI_WIN_BORDER      WS_BORDER
  #define GUI_WIN_VSCROLL     WS_VSCROLL
  #define GUI_WIN_HSCROLL     WS_HSCROLL
  #define GUI_WINX_TOPMOST    ((LONG)1<<0)
  
#elif defined (__FORDOS__)
  #define GUI_WIN_CHILD       ((LONG)1<<0)
  #define GUI_WIN_VISIBLE     ((LONG)1<<1)
  #define GUI_WIN_POPUP       ((LONG)1<<2)
  #define GUI_WIN_POPUPWINDOW (GUI_WIN_POPUP | GUI_WIN_BORDER | GUI_CAPTION | GUI_WIN_SYSMENU)    
  #define GUI_WIN_OVERLAPPED  ((LONG)1<<3)
  #define GUI_WIN_OVERLAPPEDWINDOW ((LONG)1<<4)
  #define GUI_WIN_THICKFRAME  ((LONG)1<<5)
  #define GUI_WIN_SYSMENU     ((LONG)1<<6)
  #define GUI_WIN_CAPTION     ((LONG)1<<7)
  #define GUI_WIN_BORDER      ((LONG)1<<8)
  #define GUI_WIN_VSCROLL     ((LONG)1<<9)
  #define GUI_WIN_HSCROLL     ((LONG)1<<10)
  #define GUI_WINX_TOPMOST    ((LONG)1<<0)
    
#elif defined (__FORUNIX__)
  #define GUI_WIN_CHILD       ((LONG)1<<0)
  #define GUI_WIN_VISIBLE     ((LONG)1<<1)
  #define GUI_WIN_POPUP       ((LONG)1<<2)
  #define GUI_WIN_POPUPWINDOW (GUI_WIN_POPUP | GUI_WIN_BORDER | GUI_CAPTION | GUI_WIN_SYSMENU)    
  #define GUI_WIN_OVERLAPPED  ((LONG)1<<3)
  #define GUI_WIN_OVERLAPPEDWINDOW ((LONG)1<<4)
  #define GUI_WIN_THICKFRAME  ((LONG)1<<5)
  #define GUI_WIN_SYSMENU     ((LONG)1<<6)
  #define GUI_WIN_CAPTION     ((LONG)1<<7)
  #define GUI_WIN_BORDER      ((LONG)1<<8)
  #define GUI_WIN_VSCROLL     ((LONG)1<<9)
  #define GUI_WIN_HSCROLL     ((LONG)1<<10)
  #define GUI_WINX_TOPMOST    ((LONG)1<<0)
    
#endif



#if defined (__FORWINDOWS__)
  #define GUI_CLASS_OWNDISPLAY   CS_OWNDC
  #define GUI_CLASS_VREDRAW   CS_VREDRAW
  #define GUI_CLASS_HREDRAW   CS_HREDRAW
  
#elif defined (__FOROS2__)
  #define GUI_CLASS_OWNDISPLAY   CS_OWNDC
  #define GUI_CLASS_VREDRAW   CS_VREDRAW
  #define GUI_CLASS_HREDRAW   CS_HREDRAW
  
#elif defined (__FORDOS__)
  #define GUI_CLASS_OWNDISPLAY   ((LONG)1<<0)
  #define GUI_CLASS_VREDRAW   ((LONG)1<<1)
  #define GUI_CLASS_HREDRAW   ((LONG)1<<2)

#elif defined (__FORUNIX__)
  #define GUI_CLASS_OWNDISPLAY   ((LONG)1<<0)
  #define GUI_CLASS_VREDRAW   ((LONG)1<<1)
  #define GUI_CLASS_HREDRAW   ((LONG)1<<2)
#endif

#endif

