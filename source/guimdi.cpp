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
#include "guimdi.h"

APIRESULT APIPROC GUIMDIChildProc ( HWINDOW hWindow, MESSAGE iMessage,
                                    PARAM1 Param1, PARAM2 Param2 );
APIRESULT APIPROC GUIMDIFrameProc ( HWINDOW hWindow, MESSAGE iMessage,
                                    PARAM1 Param1, PARAM2 Param2 );







GUIMDICHILD *NewMDIChild=NULL;

GUIMDICHILD::GUIMDICHILD () : GUICHILD ()
  {
    strcpy ( ClassName, "GUIMDICHILD" );
    #if defined (__FORWINDOWS__)
      MDICreate.szClass = new CHAR [128];
      MDICreate.szTitle = new CHAR [128];
    #endif
  } 

GUIMDICHILD::~GUIMDICHILD ()
  {
    #if defined (__FORWINDOWS__)
      delete (char*)(MDICreate.szClass);
      delete (char*)(MDICreate.szTitle);
    #endif
  } 

VOID GUIMDICHILD::RegisterWindow ()
  {
    #if defined (__FORWINDOWS__)
      WNDCLASS WndClass;
      WndClass.cbClsExtra = 0;
      WndClass.cbWndExtra = 0;
      #if defined (__FORWIN386__)
        WndClass.hbrBackground = GetStockObject ( (short)WinBG );
      #else
        WndClass.hbrBackground = GetStockObject ( WinBG );
      #endif
      WndClass.hCursor = LoadCursor ( NULL, IDC_ARROW );
      WndClass.hIcon = LoadIcon ( NULL, "END" );
      WndClass.hInstance = (HINSTANCE)hInstance;
      WndClass.lpfnWndProc = (WNDPROC)GUIMDIChildProc;
      WndClass.lpszClassName = ClassName;
      WndClass.lpszMenuName = NULL;
      WndClass.style = (UINT)ClassStyle;

      RegisterClass ( &WndClass );

    #elif defined (__FOROS2__)
      WinRegisterClass ( hAB, ClassName, GUIMDIChildProc, ClassStyle, 0 );
    #endif
  } 

BOOLEAN GUIMDICHILD::Create ( STRING Title, LONG x, LONG y, LONG Wd, LONG Ht,
                              GUIWINDOW *Parent )
  {
    PreRegister (); 
    RegisterWindow ();
    NewMDIChild = this;

    ParentWindow = Parent;
    HWINDOW hParent;
    HWINDOW hNew;

    #if defined (__FORWINDOWS__)

      strcpy ( (char*)MDICreate.szClass, ClassName );
      strcpy ( (char*)MDICreate.szTitle, Title );
      MDICreate.hOwner = hInstance;
      #if defined (__FORWIN386__)
        MDICreate.x = (short)x;
        MDICreate.y = (short)y;
        MDICreate.cx = (short)Wd;
        MDICreate.cy = (short)Ht;
        MDICreate.style = (UINT)WinStyle;
      #else
        MDICreate.x = x;
        MDICreate.y = y;
        MDICreate.cx = Wd;
        MDICreate.cy = Ht;
        MDICreate.style = WinStyle;
      #endif
      
      MDICreate.lParam = NULL;

      if (Parent==NULL)
        hParent = HDESKTOP;
      else
        hParent = Parent->GetClient ();

      hWindow = (HWND)SendMessage ( hParent, WM_MDICREATE, 0,
                                    (LONG)(LPMDICREATESTRUCT)(&MDICreate) );
      hNew = hWindow;
      CreateClientWindow ();

    #elif defined (__FOROS2__)  
      if (Parent==NULL)
        hParent = HDESKTOP;
      else
        hParent = Parent->GetClient ();
        
      ULONG CreateFlag = WinExStyle;
      hWindow = WinCreateStdWindow ( hParent, WinStyle, &CreateFlag, ClassName,
                                     Title, 0, 0, 0, &hClient );
      hNew = hClient;
      if (x&y&Wd&Ht)
        {}
                                     
    #elif defined (__FORDOS__)
      if (Title)
        {}
      if (x&y&Wd&Ht)
        {}
      if (Parent)
        {}
      if (hParent)
        {}
    #endif
    AddWindow ( this, hNew );
    NewMDIChild = NULL;
    return SUCCESS;
  } 








APIRESULT APIPROC GUIMDIChildProc ( HWINDOW hWindow, MESSAGE iMessage,
                                    PARAM1 Param1, PARAM2 Param2 )
  {
    LONG Result;
    GUIWINDOW *WinPtr;

    Result = 0;
    WinPtr = FindWindow ( hWindow );
    if (WinPtr==NULL)
      {
        if (NewMDIChild!=NULL)
          Result = NewMDIChild->WndProc ( hWindow, iMessage, Param1, Param2, FALSE );
      } 
    else
      {
        Result = WinPtr->WndProc ( hWindow, iMessage, Param1, Param2, FALSE );
      } 

    if (Result!=0)
      return 0;

    #if defined (__FORWINDOWS__)
      return DefMDIChildProc ( hWindow, iMessage, Param1, Param2 );

    #elif defined (__FOROS2__)
      return WinDefWindowProc ( hWindow, iMessage, Param1, Param2 );

    #elif defined (__FORDOS__)
      if (hWindow&iMessage&Param1&Param2)
        {}
      return 0;
    #endif
  } 








GUIMDIFRAME *NewMDIFrame=NULL;

GUIMDIFRAME::GUIMDIFRAME ()
  {
    strcpy ( ClassName, "GUIMDIFRAME" );
  } 

GUIMDIFRAME::~GUIMDIFRAME ()
  {
  } 

VOID GUIMDIFRAME::RegisterWindow ()
  {
    #if defined (__FORWINDOWS__)
      WNDCLASS WndClass;
      WndClass.cbClsExtra = 0;
      WndClass.cbWndExtra = 0;
      #if defined (__FORWIN386__)
        WndClass.hbrBackground = GetStockObject ( (short)WinBG );
      #else
        WndClass.hbrBackground = GetStockObject ( WinBG );
      #endif
      WndClass.hCursor = LoadCursor ( NULL, IDC_ARROW );
      WndClass.hIcon = LoadIcon ( NULL, "END" );
      WndClass.hInstance = (HINSTANCE)hInstance;
      WndClass.lpfnWndProc = (WNDPROC)GUIMDIFrameProc;
      WndClass.lpszClassName = ClassName;
      WndClass.lpszMenuName = NULL;
      WndClass.style = (UINT)ClassStyle;

      RegisterClass ( &WndClass );

    #elif defined (__FOROS2__)
      WinRegisterClass ( hAB, ClassName, GUIMDIFrameProc, ClassStyle, 0 );
    #endif
  } 

BOOLEAN GUIMDIFRAME::Create ( STRING Title, LONG x, LONG y, LONG Wd, LONG Ht,
                              GUIWINDOW *Parent )
  {
    PreRegister (); 
    RegisterWindow ();
    NewMDIFrame = this;

    ParentWindow = Parent;
    HWINDOW hParent;
    HWINDOW hNew;

    #if defined (__FORWINDOWS__)

      if (Parent==NULL)
        hParent = HDESKTOP;
      else
        hParent = Parent->GetClient ();

      #if defined (__FORWIN386__)
        if (WinExStyle==0)
          hWindow = CreateWindow ( ClassName, Title, WinStyle, (short)x, (short)y,
                                   (short)Wd, (short)Ht, hParent, NULL,
                                   hInstance, NULL );
        else
          hWindow = CreateWindowEx ( WinExStyle, ClassName, Title, WinStyle, (short)x, (short)y,
                                     (short)Wd, (short)Ht,
                                     hParent, NULL, hInstance, NULL );
      #else
        if (WinExStyle==0)
          hWindow = CreateWindow ( ClassName, Title, WinStyle, x, y, Wd, Ht, hParent, NULL,
                                   hInstance, NULL );
        else
          hWindow = CreateWindowEx ( WinExStyle, ClassName, Title, WinStyle, x, y, Wd, Ht,
                                     hParent, NULL, hInstance, NULL );
      #endif

      hNew = hWindow;
      AddWindow ( this, hNew );
      NewMDIFrame = NULL;

      CLIENTCREATESTRUCT CliCreate;
      CliCreate.hWindowMenu = NULL;
      CliCreate.idFirstChild = 100;
      hClient = CreateWindow ( "MDICLIENT", NULL, GUI_WIN_CHILD | GUI_WIN_VISIBLE | WS_CLIPCHILDREN,
                               0, 0, 0, 0, hWindow, 0, hInstance, (LPSTR)(&CliCreate) );
                               
    #elif defined (__FOROS2__)  
      if (Parent==NULL)
        hParent = HDESKTOP;
      else
        hParent = Parent->GetClient ();
        
      ULONG CreateFlag = WinExStyle;
      hWindow = WinCreateStdWindow ( hParent, WinStyle, &CreateFlag, ClassName,
                                     Title, 0, 0, 0, &hClient );
      hNew = hClient;
      AddWindow ( this, hNew );
      NewMDIFrame = NULL;
      if (x&y&Wd&Ht)
        {}
                                     
    #elif defined (__FORDOS__)
      if (Title)
        {}
      if (x&y&Wd&Ht)
        {}
      if (Parent)
        {}
      if (hParent)
        {}
      if (hNew)
        {}  
    #elif defined (__FORUNIX__)
      if (Title)
        {}
      if (x&y&Wd&Ht)
        {}
      if (Parent)
        {}
      if (hParent)
        {}
      if (hNew)
        {}  
       #if defined (__FORMOTIF__)
          INT NumArgs;
          Arg MainAttrList[4];
          Widget WinWidget;

          XtSetArg ( MainAttrList[0], XmNwidth, Wd );
          XtSetArg ( MainAttrList[1], XmNheight, Ht );
          XtSetArg ( MainAttrList[2], XmNx, x );
          XtSetArg ( MainAttrList[3], XmNy, y );
          WinWidget = XtAppInitialize ( &XtApp, Title, 
                                      NULL, 0, &NumArgs, NULL, NULL, 
                                      MainAttrList, 2 );  
          XtRealizeWidget ( WinWidget );

          MainXDisplay = XtDisplay ( WinWidget );

          hWindow = XtWindow ( WinWidget );
          hClient = hWindow;
          hNew = hClient;     
          AddWindow ( this, hNew );
          NewMDIFrame = NULL;
       #endif   
    #endif
    return SUCCESS;
  } 

LONG GUIMDIFRAME::WndProc ( HWINDOW hWnd, MESSAGE iMessage, PARAM1 Param1, PARAM2 Param2, BOOLEAN FromClient )
  {
    LONG Result;
    hWindow = hWnd;
    if (Param1)
      {}
    if (Param2)
      {}
    if (FromClient)
      {}
        
    Result = 0;
    switch (iMessage)
      {
        case GUI_WM_CREATE :
          Result = OnCreate ();
          break;

        case GUI_WM_CLOSE :
          Result = OnClose ();
          break;
          
        case GUI_WM_DESTROY :
          Result = OnDestroy ();
          DiscardWindow ( hWindow );
          break;

        case GUI_WM_PAINT :
          ResetClientArea ();
          #if defined (__FORWINDOWS__)
            PAINTSTRUCT Ps;
            HDISPLAY hDisplay;
            hDisplay = BeginPaint ( hWindow, &Ps );
            Result = OnPaint ( hDisplay );
            EndPaint ( hWindow, &Ps );
          #elif defined (__FOROS2__)
            HDISPLAY hDisplay;
            hDisplay = WinBeginPaint ( hWindow, NULLHANDLE, NULL );
            Result = OnPaint ( hDisplay );
            WinEndPaint ( hDisplay );
          #elif defined (__FORDOS__)
            Result = OnPaint ( 0 );
          #endif
          break;

        case GUI_WM_COMMAND :
          #if defined (__FORWINDOWS__)
            Result = OnCommand ( Param1 );
            if (Result==0)
              {
                HWINDOW hChild;
                hChild = (HWINDOW)LOWORD ( SendMessage ( hClient, WM_MDIGETACTIVE,
                                                         0, 0 ) );
                if (IsWindow(hChild))
                  Result = SendMessage ( hChild, GUI_WM_COMMAND, Param1, Param2 );
              } 
          #elif defined (__FOROS2__)
            Result = OnCommand ( COMMANDMSG(&iMessage)->cmd );
          #endif
          break;

                          case GUI_WM_ACTIVATE :
                                        LONG ActiveFlags;
                                        BOOLEAN Minimized;
                                        ActiveFlags = LOWORD((LONG)Param1);
                                        Minimized = HIWORD((LONG)Param1);
                                        Result = OnActivate ( ActiveFlags, Minimized );
                                        break;

        case GUI_WM_KEYDOWN :
          Result = OnKeyDown ( (LONG)Param1 );
          break;

        case GUI_WM_KEYUP :
          Result = OnKeyUp ( (LONG)Param1 );
          break;

        case GUI_WM_MOUSEMOVE :
        case GUI_WM_LBUTTONDOWN :
        case GUI_WM_LBUTTONUP :
        case GUI_WM_LBUTTONDBLCLK :
        case GUI_WM_MBUTTONDOWN :
        case GUI_WM_MBUTTONUP :
        case GUI_WM_MBUTTONDBLCLK :
        case GUI_WM_RBUTTONDOWN :
        case GUI_WM_RBUTTONUP :
        case GUI_WM_RBUTTONDBLCLK :
          LONG Mx, My;
          LONG Flags;

          #if defined (__FORWINDOWS__)
            Flags = (LONG)Param1;
            Mx = LOWORD ( Param2 );
            My = HIWORD ( Param2 );
            if (FromClient)
              Result = OnMouse ( iMessage, Flags, Mx, My );
          #elif defined (__FOROS2__)
            if (Mx&My&Flags)
              {}
          #elif defined (__FORDOS__)
            if (Mx&My&Flags)
              {}
          #endif
          break;

        case GUI_WM_SIZE :
          ResetClientArea ();
#if (0)
          Result = OnSize ( LOWORD (Param2), HIWORD (Param2) );
#else

		Result = OnSize (  (Param2) && 0x0000FFFF, HIWORD (Param2) && 0xFFFF0000 );
#endif 
          break;

        case GUI_WM_TIMER :
          Result = OnTimer ();
          break;

        case GUI_WM_VSCROLL :
          #if defined (__FORWINDOWS__)
            Result = OnScroll ( SBAR_VERT, (LONG)Param1, LOWORD((LONG)Param2));
          #endif
          break;
        case GUI_WM_HSCROLL :
          #if defined (__FORWINDOWS__)
            Result = OnScroll ( SBAR_HORZ, Param1, LOWORD(Param2));
          #endif
          break;

        case GUI_WM_POSCHANGING :
          #if defined (__FORWINDOWS__)
            WINPOSCHANGE Wpc;
            WINDOWPOS FAR *lpwp;
            #if defined (__FORWIN386__)
              lpwp = (WINDOWPOS FAR*)MK_FP(Param2>>16,Param2&0xFFFF);
            #else
              lpwp = (WINDOWPOS FAR*)Param2;
            #endif
            Wpc.xPos = lpwp->x;
            Wpc.yPos = lpwp->y;
            Wpc.Width = lpwp->cx;
            Wpc.Height = lpwp->cy;
            Wpc.Flags = lpwp->flags;

            Result = OnPosChanging ( &Wpc );

            #if defined (__FORWIN386__)
              lpwp->x = (short)Wpc.xPos;
              lpwp->y = (short)Wpc.yPos;
              lpwp->cx = (short)Wpc.Width;
              lpwp->cy = (short)Wpc.Height;
              lpwp->flags = Wpc.Flags;
            #else
              lpwp->x = Wpc.xPos;
              lpwp->y = Wpc.yPos;
              lpwp->cx = Wpc.Width;
              lpwp->cy = Wpc.Height;
              lpwp->flags = Wpc.Flags;
            #endif
          #endif
          break;

        default :
          Result = UserProc ( iMessage, Param1, Param2 );
          break;
      } 
    return Result;
  } 







APIRESULT APIPROC GUIMDIFrameProc ( HWINDOW hWindow, MESSAGE iMessage,
                                    PARAM1 Param1, PARAM2 Param2 )
  {
    LONG Result;
    HWINDOW hWndClient=0;
    GUIWINDOW *WinPtr;

    if (hWndClient)
      {}
    Result = 0;
    WinPtr = FindWindow ( hWindow );
    if (WinPtr==NULL)
      {
        if (NewMDIFrame!=NULL)
          {
            Result = NewMDIFrame->WndProc ( hWindow, iMessage, Param1, Param2, FALSE );
            hWndClient = NewMDIFrame->GetClient ();
          } 
      } 
    else
      {
        Result = WinPtr->WndProc ( hWindow, iMessage, Param1, Param2, FALSE );
        hWndClient = WinPtr->GetClient ();
      } 

    if (Result!=0)
      return 0;

    #if defined (__FORWINDOWS__)
      return DefFrameProc ( hWindow, hWndClient, iMessage, Param1, Param2 );

    #elif defined (__FOROS2__)
      return WinDefWindowProc ( hWindow, iMessage, Param1, Param2 );

    #elif defined (__FORDOS__)
      if (hWindow&iMessage&Param1&Param2&hWndClient)
        {}
      return 0;
    #endif
  } 

