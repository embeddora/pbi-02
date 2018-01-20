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
#include "guitbar.h"

APIRESULT APIPROC ToolBarProc ( HWINDOW hWindow, MESSAGE iMessage,
                                PARAM1 Param1, PARAM2 Param2 );







GUITOOLBAR::GUITOOLBAR () : GUICHILD ()
  {
    
    ImgWd = 16;
    ImgHt = 15;
    BarHt = ImgHt+5;

    hMemDisplay = NULL;
    Selected = -1;
    Buttons = NULL;
    hBitmap = NULL;
    NumButtons = 0;
    strcpy ( ClassName, "GUITOOLBAR" );
  } 

GUITOOLBAR::~GUITOOLBAR ()
  {
    DestroyBar ();
    if (Buttons!=NULL)
      delete Buttons;
    Buttons = NULL;
  } 

VOID GUITOOLBAR::SetButtonSize ( LONG Wd, LONG Ht )
  {
    ImgWd = Wd;
    ImgHt = Ht;
  } 

VOID GUITOOLBAR::SetBarHeight ( LONG BarHeight )
  {
    BarHt = BarHeight;  
  } 

BOOLEAN GUITOOLBAR::Load ( STRING Name, LONG Num, BUTTONDATA *Data )
  {
    if (Name==NULL)
      return FAILURE;  
    #if defined (__FORWINDOWS__)
      hBitmap = LoadBitmap ( hInstance, Name );
    #else
    #endif  
    NumButtons = Num;
    if (Buttons!=NULL)
      delete Buttons;
    Buttons = new BUTTONDATA [NumButtons];
      
    INT i;

    for (i=0;i<NumButtons;i++)
      {
        Buttons[i].x = Data[i].x;
        Buttons[i].y = Data[i].y;
        Buttons[i].Command = Data[i].Command;
        Buttons[i].State = Data[i].State;
      } 
    return SUCCESS;
  } 

VOID GUITOOLBAR::SetButtonAttr ( LONG Index, LONG Command )
  {
    if ((Index<0)||(Index>=NumButtons))
      return;
    Buttons[Index].Command = Command;  
  } 

VOID GUITOOLBAR::RegisterWindow ()
  {
    #if defined (__FORWINDOWS__)
      WNDCLASS WndClass;
      WndClass.cbClsExtra = 0;
      WndClass.cbWndExtra = 4;
      WndClass.hbrBackground = GetStockObject ( LTGRAY_BRUSH );
      WndClass.hCursor = LoadCursor ( NULL, IDC_ARROW );
      WndClass.hIcon = LoadIcon ( NULL, "END" );
      WndClass.hInstance = hInstance;
      WndClass.lpfnWndProc = (WNDPROC)ToolBarProc;
      WndClass.lpszClassName = "GUITOOLBAR";
      WndClass.lpszMenuName = NULL;      
      WndClass.style = CS_HREDRAW | CS_VREDRAW;

      RegisterClass ( &WndClass );
    #else
    #endif
  } 

BOOLEAN GUITOOLBAR::Create ( STRING Title, LONG x, LONG y, LONG Wd, LONG Ht,
                             GUIWINDOW *Parent )
  {
    if (Title)
      {}
    if (x&y&Wd&Ht)
      {}
    if (Parent)
      {}
    return FAILURE;
  } 

VOID GUITOOLBAR::CreateBar ()
  {
    DestroyBar ();
    #if defined (__FORWINDOWS__)
      HDISPLAY hDisplay = GetDisplay ( hClient );
      hMemDisplay = CreateCompatibleDC ( hDisplay );
      OldBitmap = SelectObject ( hMemDisplay, hBitmap );
      ReleaseDisplay ( hClient, hDisplay );
    #else
    #endif
  } 

VOID GUITOOLBAR::DestroyBar ()
  {
    #if defined (__FORWINDOWS__)
      if (hMemDisplay!=NULL)
        {
          SelectObject ( hMemDisplay, OldBitmap );
          DeleteObject ( hBitmap );
          DeleteDC ( hMemDisplay );
          hMemDisplay = NULL;
          hBitmap = NULL;
        } 
    #else
    #endif
  } 

BOOLEAN GUITOOLBAR::Create ( GUIWINDOW *Parent )
  {
    if (hBitmap==NULL)
      return FAILURE;
    if (Parent==NULL)
     return FAILURE;

    HWINDOW hParent;
    HWINDOW hNew;

    ParentWindow = Parent;
    hParent = Parent->GetHandle ();
    if (hParent==NULL)
      return FAILURE;

    RegisterWindow ();

    if (BarHt<ImgHt+5)
      BarHt = ImgHt+5;
      
    #if defined (__FORWINDOWS__)    
      RECT r;
      GetClientRect ( ParentWindow->GetHandle (), &r );
      #if defined (__FORWIN386__)
        hWindow = CreateWindow ( "GUITOOLBAR", "", WS_CHILD | WS_VISIBLE,
                                 0, 0, r.right, (short)BarHt,
                                 hParent, NULL, hInstance, NULL );
      #else
        hWindow = CreateWindow ( "GUITOOLBAR", "", WS_CHILD | WS_VISIBLE,
                                 0, 0, r.right, BarHt,
                                 hParent, NULL, hInstance, NULL );
      #endif
      Parent->SetToolBar ( hWindow, BarHt );                         
      hClient = hWindow;
      hNew = hWindow;
      CreateBar ();
    #else
      if (hNew)
        {}
    #endif
    AddWindow ( this, hNew );
    return SUCCESS;
  } 

LONG GUITOOLBAR::FindButton ( LONG Mx, LONG My )
  {
    INT i;
    if (Buttons==NULL)
      return -1;
    if ((My<0)||(My>BarHt))
      return -1;
        
    for (i=0;i<NumButtons;i++)
      {
        if ((Mx>=Buttons[i].x)&&(My>=Buttons[i].y)&&
             (Mx<=Buttons[i].x+ImgWd+5)&&(My<=Buttons[i].y+ImgHt+5))
          return i; 
      } 
    return -1;  
  } 

LONG GUITOOLBAR::OnMouse ( LONG Event, LONG Flags, LONG Mx, LONG My )
  {
    LONG Index;
    if (Flags)
      {}  
    switch (Event)
      {
        case GUI_WM_LBUTTONDOWN :
        case GUI_WM_RBUTTONDOWN :
          Index = FindButton ( Mx, My );
          if (Index!=-1)
            {
              Buttons[Index].State = BUTTON_SELECT;
              HDISPLAY hDisplay;
              hDisplay = GetDisplay ( hClient );
              DrawButton ( hDisplay, Index );
              ReleaseDisplay ( hClient, hDisplay );
              Selected = Index;
            } 
          break;  
        case GUI_WM_LBUTTONUP :
        case GUI_WM_RBUTTONUP :
          if (Selected!=-1)
            {
              Buttons[Selected].State = BUTTON_UNSELECT;
              HDISPLAY hDisplay;
              hDisplay = GetDisplay ( hClient );
              DrawButton ( hDisplay, Selected );
              ReleaseDisplay ( hClient, hDisplay );
              #if defined (__FORWINDOWS__)
                #if defined (__FORWIN386__)
                  SendMessage ( ParentWindow->GetHandle (), GUI_WM_COMMAND,
                                (short)Buttons[Selected].Command, 0 );
                #else
                  SendMessage ( ParentWindow->GetHandle (), GUI_WM_COMMAND,
                                Buttons[Selected].Command, 0 );
                #endif
              #endif
              Selected = -1;
            } 
          break;
        case GUI_WM_MOUSEMOVE :
          if (Selected!=-1)
            {
              Index = FindButton ( Mx, My );
              if (Index!=Selected)
                { 
                  Buttons[Selected].State = BUTTON_UNSELECT;
                  HDISPLAY hDisplay;
                  hDisplay = GetDisplay ( hClient );
                  DrawButton ( hDisplay, Selected );
                  ReleaseDisplay ( hClient, hDisplay );
                  #if defined (__FORWINDOWS__)
                    #if defined (__FORWIN386__)
                      SendMessage ( ParentWindow->GetHandle (), GUI_WM_COMMAND,
                                    (short)Buttons[Selected].Command, 0 );
                    #else
                      SendMessage ( ParentWindow->GetHandle (), GUI_WM_COMMAND,
                                    Buttons[Selected].Command, 0 );
                    #endif             
                  #endif
                  Selected = -1;
                } 
            } 
          break;  
      } 
    return 0;  
  } 

VOID GUITOOLBAR::DrawButton ( HDISPLAY hDisplay, LONG Index )
  {
    LONG x,y;
    INT AddX,AddY;

    x = Buttons[Index].x;  
    y = Buttons[Index].y;

    #if defined (__FORWINDOWS__)
      Grafix.FGColor = RGB ( 0, 0, 0 );  
    #endif
    
    Grafix.DrawLine ( hDisplay, x+1, y, x+ImgWd+4, y );
    Grafix.DrawLine ( hDisplay, x+1, y+ImgHt+4, x+ImgWd+4, y+ImgHt+4 );
    Grafix.DrawLine ( hDisplay, x, y+1, x, y+ImgHt+4 );
    Grafix.DrawLine ( hDisplay, x+ImgWd+4, y+1, x+ImgWd+4, y+ImgHt+4 );
    
    if (Buttons[Index].State==BUTTON_SELECT)
      {
        AddX = 4;
        AddY = 4;
        #if defined (__FORWINDOWS__)
          Grafix.FGColor = RGB ( 128, 128, 128 );
        #endif
        Grafix.DrawLine ( hDisplay, x+1, y+1, x+ImgWd+4, y+1 );
        Grafix.DrawLine ( hDisplay, x+1, y+2, x+1, y+ImgHt+4 );
        
        Grafix.FGColor = RGB ( 192, 192, 192 );           
        Grafix.DrawLine ( hDisplay, x+2, y+2, x+ImgWd+4, y+2 );
        Grafix.DrawLine ( hDisplay, x+2, y+3, x+ImgWd+4, y+3 );
        Grafix.DrawLine ( hDisplay, x+2, y+4, x+2, y+ImgHt+4 );
        Grafix.DrawLine ( hDisplay, x+3, y+4, x+3, y+ImgHt+4 );
      } 
    else
      {
        AddX = 2;
        AddY = 2;  
        Grafix.FGColor = RGB ( 255, 255, 255 );           
        Grafix.DrawLine ( hDisplay, x+1, y+1, x+ImgWd+3, y+1 );
        Grafix.DrawLine ( hDisplay, x+1, y+1, x+1, y+ImgHt+3 );

        Grafix.FGColor = RGB ( 128, 128, 128 );
        Grafix.DrawLine ( hDisplay, x+ImgWd+3, y+1, x+ImgWd+3, y+ImgHt+3 );
        Grafix.DrawLine ( hDisplay, x+ImgWd+2, y+2, x+ImgWd+2, y+ImgHt+3 );
        Grafix.DrawLine ( hDisplay, x+2, y+ImgHt+2, x+ImgWd+3, y+ImgHt+2 );
        Grafix.DrawLine ( hDisplay, x+1, y+ImgHt+3, x+ImgWd+4, y+ImgHt+3 );          
      } 
    #if defined (__FORWINDOWS__)
      #if defined (__FORWIN386__)
        BitBlt ( hDisplay, (short)(x+AddX), (short)(y+AddY), (short)ImgWd, (short)ImgHt,
                 hMemDisplay, (short)(Index*ImgWd), 0, SRCCOPY );
      #else
        BitBlt ( hDisplay, x+AddX, y+AddY, ImgWd, ImgHt, hMemDisplay, Index*ImgWd, 0, SRCCOPY );
      #endif
    #else
         #endif
         if (AddX+AddY)
           {}  
  } 
  
LONG GUITOOLBAR::OnPaint ( HDISPLAY hDisplay )
  {
    if (hDisplay)
      {}
    #if defined (__FORWINDOWS__)
      INT i;
      for (i=0;i<NumButtons;i++)
        DrawButton ( hDisplay, i );
    #else
    #endif
    return 0;
  } 

LONG GUITOOLBAR::WndProc ( HWINDOW hWnd, MESSAGE iMessage, PARAM1 Param1, PARAM2 Param2, BOOLEAN FromClient )
  {
    LONG Result;
    hWindow = hWnd;
    if (Param1)
      {}
    if (Param2)
      {}
    if (FromClient)
      {}  
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
            Flags = Param1;
            Mx = LOWORD ( Param2 );
            My = HIWORD ( Param2 );
            Result = OnMouse ( iMessage, Flags, Mx, My );
          #elif defined (__FOROS2__)
            if (Mx&My&Flags)
              {}
          #elif defined (__FORDOS__)
            if (Mx&My&Flags)
              {}
          #endif
          break;

        default :
          Result = UserProc ( iMessage, Param1, Param2 );
          break;
      } 
    return Result;
  } 







APIRESULT APIPROC ToolBarProc ( HWINDOW hWindow, MESSAGE iMessage,
                                PARAM1 Param1, PARAM2 Param2 )
  {
    GUIWINDOW *WinPtr;
    LONG Result;

    WinPtr = FindWindow ( hWindow );

    Result = 0;
    if (WinPtr!=NULL)
      {
        Result = WinPtr->WndProc ( hWindow, iMessage,
                                   Param1, Param2, TRUE );
      } 

    if (Result!=0)
      return 0;

    #if defined (__FORWINDOWS__)
      return DefWindowProc ( hWindow, iMessage, Param1, Param2 );

    #elif defined (__FOROS2__)
      return WinDefWindowProc ( hWindow, iMessage, Param1, Param2 );

    #elif defined (__FORDOS__)
      if (hWindow&iMessage&Param1&Param2)
        {}
      return 0;
    #endif
  } 
