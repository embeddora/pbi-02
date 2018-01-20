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
#include "guiapp.h"

#include "stdlib.h"

GUIAPP::GUIAPP ()
  {
  } 

GUIAPP::~GUIAPP ()
  {
  } 

#define DEFAULT_BGCOLOR  "white"
#define DEFAULT_FGCOLOR  "black"
#define DEFAULT_BDWIDTH   1
#define DEFAULT_FONT     "fixed"

VOID GUIAPP::GoDOS ( GUIFRAME *MainWindow, DISPLAYDATA *Data )
  {
    if (MainWindow)
      {}
    Grafix.SetDisplay ( Data );  
  } 

VOID GUIAPP::GoWinSDI ( GUIFRAME *MainWindow, DISPLAYDATA *Data )
  {
    if (MainWindow)
      {}
      
    Grafix.SetDisplay ( Data );

    #if defined (__FORWINDOWS__)
      MSG Message;

      MainWindow->ShowWindow ( CmdShow );
      while (GetMessage(&Message,0,0,0))
        {
          TranslateMessage ( &Message );
          DispatchMessage ( &Message );
          ClearRemovedList ();
        } 
     #endif
  } 

VOID GUIAPP::GoWinMDI ( GUIFRAME *MainWindow, DISPLAYDATA *Data )
  {
    if (MainWindow)
      {}
      
    Grafix.SetDisplay ( Data );

    #if defined (__FORWINDOWS__)
      HWINDOW hFrame;
      HWINDOW hClient;
      MSG Message;      

      hFrame = MainWindow->GetHandle ();
      hClient = GetWindow ( hFrame, GW_CHILD );

      MainWindow->ShowWindow ( CmdShow );
      MainWindow->UpdateWindow ();
      while (GetMessage(&Message,NULL,0,0))
        {
          if (!TranslateMDISysAccel(hClient,&Message) &&
              !TranslateAccelerator(hFrame,NULL,&Message))
            {    
              TranslateMessage ( &Message );
              DispatchMessage ( &Message );
              ClearRemovedList ();
            } 
        } 
    #endif
  } 

VOID GUIAPP::GoOS2 ( GUIFRAME *MainWindow, DISPLAYDATA *Data )
  {
    if (MainWindow)
      {}
      
    Grafix.SetDisplay ( Data );

    #if defined (__FOROS2__)
      QMSG Message;
      while (WinGetMsg(hAB,&Message,NULLHANDLE,0,0))
        {
          WinDispatchMsg ( hAB, &Message );
          ClearRemovedList ();
        } 

      WinDestroyMsgQueue ( hMQ );
      WinTerminate ( 0 );
    #endif
  } 

VOID GUIAPP::GoXWindow ( GUIFRAME *MainWindow, DISPLAYDATA *Data )
  {
    if (MainWindow)
      {}
    if (Data)
      {}  
    #if defined (__FORUNIX__)
      XEvent theEvent;
      GUIWINDOW *WinPtr;
      HWINDOW hWindow;
      PARAM1 Param1;
      PARAM2 Param2;
      
      
      Data->MainXDisplay = XOpenDisplay ( NULL );
      if (Data->MainXDisplay==NULL)
        {
          printf ("Can not open X Display.\n\n");
          exit (0);
        } 

      Grafix.SetDisplay ( Data );

      
      XSizeHints *p_XSizeHints;
      p_XSizeHints = XAllocSizeHints ();
      if (p_XSizeHints==NULL)
        {
          printf (" Can not allocate X Size Hint.\n\n" );
          exit (0);
        } 
      p_XSizeHints->flags = (PPosition | PSize | PMinSize);
      p_XSizeHints->width = 600;
      p_XSizeHints->min_width = p_XSizeHints->width;
      p_XSizeHints->height = 400;
      p_XSizeHints->min_height = p_XSizeHints->height;
      p_XSizeHints->x = 100;
      p_XSizeHints->y = 100;

      
      XFontStruct *p_XFont;
      p_XFont = XLoadQueryFont ( Data->MainXDisplay, DEFAULT_FONT );
      if (p_XFont==NULL)
        {
          printf ("Can not create default font.\n\n" );
          exit (0);
        } 

      
      CHAR GeomString[80];
      sprintf ( GeomString, "%dx%d+%d+%d", p_XSizeHints->width,
                p_XSizeHints->height, p_XSizeHints->x, p_XSizeHints->y );
 
      INT BitMask;     
      BitMask = XGeometry ( Data->MainXDisplay, DefaultScreen ( Data->MainXDisplay ),

                            GeomString, GeomString, DEFAULT_BDWIDTH,
                            p_XFont->max_bounds.width,         
                            p_XFont->max_bounds.ascent+p_XFont->max_bounds.descent,
                            1, 1, &(p_XSizeHints->x), &(p_XSizeHints->y),
                            &(p_XSizeHints->width), &(p_XSizeHints->height) );

      if (BitMask&(XValue|YValue))
        p_XSizeHints->flags |= USPosition;
      if (BitMask&(WidthValue|HeightValue))
        p_XSizeHints->flags |= USSize;


      
      DWORD FGPix,BGPix;
      Colormap default_cmap;
      XColor color;

      default_cmap = DefaultColormap ( Data->MainXDisplay, DefaultScreen(Data->MainXDisplay) ); 
      if (XParseColor(Data->MainXDisplay, default_cmap, DEFAULT_BGCOLOR, &color )==0 ||
           XAllocColor ( Data->MainXDisplay,  default_cmap, &color ) == 0)
        BGPix = WhitePixel ( Data->MainXDisplay, DefaultScreen(Data->MainXDisplay) );
      else
        BGPix = color.pixel;

      if (XParseColor(Data->MainXDisplay, default_cmap, DEFAULT_FGCOLOR, &color )==0 ||
           XAllocColor ( Data->MainXDisplay, default_cmap, &color ) == 0)
        FGPix = BlackPixel ( Data->MainXDisplay, DefaultScreen(Data->MainXDisplay) );
      else
        FGPix = color.pixel;
                    
      
      HWINDOW Main;
      Main = XCreateSimpleWindow ( Data->MainXDisplay, DefaultRootWindow(Data->MainXDisplay),
                                   p_XSizeHints->x, p_XSizeHints->y,
                                   p_XSizeHints->width, p_XSizeHints->height, 
                                   DEFAULT_BDWIDTH, FGPix, BGPix ); 
         
      
      XClassHint *p_XClassHint;
      p_XClassHint = XAllocClassHint ();
      if (p_XClassHint==NULL)
        {
          printf ("Can not create X Class Hints.\n\n" );
          exit (0);
        } 
                   
      p_XClassHint->res_name = "Hello";
      p_XClassHint->res_class = "XClass";
      
      
      XTextProperty wname,iname;
      CHAR *WTitle = "Hello";
      CHAR *ITitle = "Hello";

      if (XStringListToTextProperty ( &WTitle, 1, &wname )==0)
        {
          printf ( "Can not create XTextProperty\n\n");
          exit(0);
        } 

      if (XStringListToTextProperty ( &ITitle, 1, &iname )==0)
        {
          printf ( "Can not create XTextProperty\n\n");
          exit(0);
        } 

      
      XWMHints *p_XWMHints;
      p_XWMHints = XAllocWMHints();
      if (p_XWMHints==NULL)
        {
          printf ( "Can not create Window Manager Hints\n\n");
          exit(0);
        } 
      
      p_XWMHints->flags = (InputHint|StateHint);
      p_XWMHints->input = FALSE;
      p_XWMHints->initial_state = NormalState;
      XSetWMProperties ( Data->MainXDisplay, Main, &wname, &iname, NULL, 0, 
                         p_XSizeHints, p_XWMHints, p_XClassHint );

      
      XSetWindowAttributes XWinAttr;

      XWinAttr.colormap = DefaultColormap ( Data->MainXDisplay, DefaultScreen(Data->MainXDisplay) );
      XWinAttr.bit_gravity = CenterGravity;
      XChangeWindowAttributes ( Data->MainXDisplay, Main, (CWColormap|CWBitGravity), 
                                &XWinAttr );

      
      XSelectInput ( Data->MainXDisplay, Main, ExposureMask );

      
      XMapWindow ( Data->MainXDisplay, Main );

      LONG Result;
      ExitApp = FALSE;

      while (!ExitApp)
        {
          XNextEvent ( Data->MainXDisplay, &theEvent );
          hWindow = theEvent.xany.window;
          WinPtr = FindWindow ( hWindow );
          if (WinPtr!=NULL)
            Result = WinPtr->WndProc ( hWindow, theEvent.type, Param1, 
                                       Param2, FALSE );
          if (Result)
            {
            } 
          ClearRemovedList ();
        } 

      XUnloadFont ( Data->MainXDisplay, p_XFont->fid );
      XDestroyWindow ( Data->MainXDisplay, Main );
      XCloseDisplay ( Data->MainXDisplay );
    #endif
  } 

VOID GUIAPP::GoMotif ( GUIFRAME *MainWindow, DISPLAYDATA *Data )
  {
    if (MainWindow)
      {}
    if (Data)
      {}  
    #if defined (__FORMOTIF__)
      Grafix.SetDisplay ( Data );
      XtAppMainLoop ( XtApp );                   
    #endif
  } 

VOID GUIAPP::Run ( DISPLAYDATA *Data, GUIFRAME* MainWindow )
  {
    if (MainWindow==NULL)
      return;

    #if defined (__FORDOS__)
      GoDOS ( MainWindow, Data );
    #elif defined (__FORWINDOWS__)
      if (MainWindow->IsMDISupported ()==FALSE)
        {
          GoWinSDI ( MainWindow, Data );
        } 
      else
        {
          GoWinMDI ( MainWindow, Data );
        } 

    #elif defined (__FOROS2__)
      GoOS2 ( MainWindow, Data );

    #elif defined (__FORUNIX__)
      #if defined (__FORMOTIF__)
        GoMotif ( MainWindow, Data );
      #else
        GoXWindow ( MainWindow, Data );
      #endif 
    #endif

    Grafix.ResetDisplay ();
    DeInitWinHashTable ();
  } 




