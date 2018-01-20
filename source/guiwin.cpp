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
#include "guiwin.h"

BOOLEAN ExitApp = FALSE;

APIRESULT APIPROC GUIWinProc ( HWINDOW hWindow, MESSAGE iMessage,
                               PARAM1 Param1, PARAM2 Param2 );
APIRESULT APIPROC ClientProc ( HWINDOW hWindow, MESSAGE iMessage,
                               PARAM1 Param1, PARAM2 Param2 );
#if defined (__FORUNIX__)
  #if defined (__FORMOTIF__)
    VOID GUIWinXWinProc ( Widget W, XtPointer ClientData, 
                          XEvent *EventPtr ); 
  #endif
#endif

struct WINDOWNODE
  {
    GUIWINDOW *WinPtr;
    HWINDOW Handle;
    WINDOWNODE *Next;

    WINDOWNODE () { WinPtr = NULL; Next = NULL; };
    ~WINDOWNODE ()
      {
        if (WinPtr!=NULL)
          delete WinPtr;
        if (Next!=NULL)
          delete Next;
        WinPtr = NULL;
        Next = NULL;
      } 
  }; 

WINDOWNODE *WinHashTable[256];
WINDOWNODE *RemovedList;

VOID AddWindow ( GUIWINDOW *NewWin, HWINDOW Handle )
  {
    LONG Index;
    WINDOWNODE *NodePtr;
    WINDOWNODE *NewNode;

    NewNode = new WINDOWNODE ();
    NewNode->WinPtr = NewWin;
    NewNode->Handle = Handle;

    Index = (LONG)Handle & 255;
    NodePtr = WinHashTable[Index];
    if (NodePtr==NULL)
      WinHashTable[Index] = NewNode;
    else
      {
        while (NodePtr->Next!=NULL)
          NodePtr = NodePtr->Next;
        NodePtr->Next = NewNode;
      } 
  } 

VOID DiscardWindow ( HWINDOW hWindow )
  {
    LONG Index;
    WINDOWNODE *NodePtr;
    WINDOWNODE *PrevPtr;
    WINDOWNODE *NextPtr;
    WINDOWNODE *RemovePtr;
    GUIWINDOW  *WinPtr;

    Index = (LONG)hWindow & 255;
    NodePtr = WinHashTable[Index];
    PrevPtr = NULL;
    while (NodePtr!=NULL)
      {
        NextPtr = NodePtr->Next;
        WinPtr = NodePtr->WinPtr;
        if ((WinPtr!=NULL)&&(NodePtr->Handle==hWindow))
          {
            if (PrevPtr!=NULL)
              {
                PrevPtr->Next = NextPtr;
              } 
            else
              WinHashTable[Index] = NextPtr;

            NodePtr->Next = NULL;
            RemovePtr = RemovedList;
            if (RemovePtr==NULL)
              RemovedList = NodePtr;
            else
              {
                while (RemovePtr->Next!=NULL)
                  RemovePtr = RemovePtr->Next;
                RemovePtr->Next = NodePtr;
              } 
            return;
          } 
        PrevPtr = NodePtr;
        NodePtr = NextPtr;
      } 
  } 

VOID ClearRemovedList ()
  {
    WINDOWNODE *Ptr;

    Ptr = RemovedList;
    if (Ptr!=NULL)
      {
        delete Ptr;
      } 
    RemovedList = NULL;
  } 

GUIWINDOW *FindWindow ( HWINDOW hWindow )
  {
    LONG Index;
    WINDOWNODE *NodePtr;
    GUIWINDOW *WinPtr;

    Index = (LONG)hWindow & 255;
    NodePtr = WinHashTable[Index];
    while (NodePtr!=NULL)
      {
        WinPtr = NodePtr->WinPtr;
        if ((WinPtr!=NULL)&&(NodePtr->Handle==hWindow))
          return NodePtr->WinPtr;
      } 
    return NULL;
  } 

VOID InitWinHashTable ()
  {
    INT i;
    for (i=0;i<256;i++)
      {
        WinHashTable[i] = NULL;
      } 
    RemovedList = NULL;
  } 

VOID DeInitWinHashTable ()
  {
    INT i;
    for (i=0;i<256;i++)
      {
        if (WinHashTable[i]!=NULL)
        delete WinHashTable[i];
        WinHashTable[i] = NULL;
      } 
    if (RemovedList!=NULL)
      delete RemovedList;
    RemovedList = NULL;
  } 








GUIWINDOW *NewWindow=NULL;

GUIWINDOW::GUIWINDOW () : GUIOBJECT ()
  {
    ParentWindow = NULL;
    ClientXPos = ClientYPos = 0;
    hToolBar = NULL;
    hWindow = hClient = NULL;
    #if defined (__FORWINDOWS__)
      WinBG = WHITE_BRUSH;
      ClassStyle = GUI_CLASS_VREDRAW | GUI_CLASS_HREDRAW;
      WinStyle = GUI_WIN_OVERLAPPEDWINDOW | GUI_WIN_CLIPSIBLINGS;
      WinExStyle = 0;
      WinBG = WHITE_BRUSH;
      ClientFlag = GUI_WIN_CHILD | GUI_WIN_VISIBLE | GUI_WIN_CLIPSIBLINGS;
      ClientClass = 0;
      strcpy ( ClassName, "GUIWINDOW" );
    #endif
  } 

GUIWINDOW::~GUIWINDOW ()
  {
  } 

VOID GUIWINDOW::PreRegister ()
  {
    #if defined (__FORWINDOWS__)  
      if (WinStyle&GUI_WIN_VSCROLL)
        ClientFlag |= GUI_WIN_VSCROLL;  
      if (WinStyle&GUI_WIN_HSCROLL)
        ClientFlag |= GUI_WIN_HSCROLL;

      RemoveWinStyle ( GUI_WIN_VSCROLL );
      RemoveWinStyle ( GUI_WIN_HSCROLL );
      
      if (ClassStyle|GUI_CLASS_OWNDISPLAY)
        ClientClass |= GUI_CLASS_OWNDISPLAY;
      
      RemoveClassStyle ( GUI_CLASS_OWNDISPLAY );
    #endif  
  } 

VOID GUIWINDOW::RegisterWindow ()
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
      WndClass.lpfnWndProc = (WNDPROC)GUIWinProc;
      WndClass.lpszClassName = ClassName;
      WndClass.lpszMenuName = NULL;
      WndClass.style = (UINT)ClassStyle;

      RegisterClass ( &WndClass );

    #elif defined (__FOROS2__)
      WinRegisterClass ( hAB, ClassName, GUIWinProc, ClassStyle, 0 );
    #endif
  } 

VOID GUIWINDOW::CreateClientWindow ()
  {
    #if defined (__FORGDK__)
      hClient = hWindow;
    #elif defined (__FORWINDOWS__)
      WNDCLASS WndClass;
      WndClass.cbClsExtra = 0;
      WndClass.cbWndExtra = 4;
      #if defined (__FORWIN386__)
        WndClass.hbrBackground = GetStockObject ( (short)WinBG );
      #else
        WndClass.hbrBackground = GetStockObject ( WinBG );
      #endif
      WndClass.hCursor = LoadCursor ( NULL, IDC_ARROW );
      WndClass.hIcon = LoadIcon ( NULL, "END" );
      WndClass.hInstance = (HINSTANCE)hInstance;
      WndClass.lpfnWndProc = (WNDPROC)ClientProc;
      WndClass.lpszClassName = "GUICLIENT";
      WndClass.lpszMenuName = NULL;      
      WndClass.style = (UINT)ClientClass;

      RegisterClass ( &WndClass );

      RECT r;
      GetClientRect ( hWindow, &r );

      #if defined (__FORWIN386__)
        hClient = CreateWindow ( "GUICLIENT", "", ClientFlag,
                                 (short)ClientXPos, (short)ClientYPos,
                                 (short)(r.right-ClientXPos),
                                 (short)(r.bottom-ClientYPos),
                                 hWindow, NULL, hInstance, NULL );
      #else
        hClient = CreateWindow ( "GUICLIENT", "", ClientFlag,
                                 ClientXPos, ClientYPos, r.right-ClientXPos,
                                 r.bottom-ClientYPos,
                                 hWindow, NULL, hInstance, NULL );
      #endif  
      SetWindowLong ( hClient, 0, (LONG)this );
      ::ShowWindow ( hClient, SW_SHOW );
    #else
    #endif
  } 

BOOLEAN GUIWINDOW::Create ( STRING Title, LONG x, LONG y, LONG Wd, LONG Ht,
                            GUIWINDOW *Parent )
  {
    PreRegister (); 
    RegisterWindow ();
    NewWindow = this;

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
    NewWindow = NULL;
    return SUCCESS;
  } 

VOID GUIWINDOW::InvalidateClient ()
  {
    #if defined (__FORWINDOWS__)
      InvalidateRect ( hWindow, NULL, TRUE );  
    #else
    #endif  
  } 

VOID GUIWINDOW::CloseWindow ()
  {
    #if defined (__FORWINDOWS__)
      SendMessage ( hWindow, GUI_WM_CLOSE, 0, 0 );
    #endif
  } 

HDISPLAY GUIWINDOW::GetDisplay ( HWINDOW hWnd )
  {
         if (hWnd)
      {}
         #if defined (__FORWINDOWS__)
                return GetDC ( hWnd );
         #elif defined (__FOROS2__)
                return 0;
         #elif defined (__FORUNIX__)
                return hWnd;
         #elif defined (__FORDOS__)
      return HVGA;
    #endif
  } 

VOID GUIWINDOW::ReleaseDisplay ( HWINDOW hWnd, HDISPLAY hDisplay )
  {
    #if defined (__FORWINDOWS__)
      ReleaseDC ( hWnd, hDisplay );
    #elif defined (__FOROS2__)
      if (hDisplay&hWnd)
        {}
    #elif defined (__FORDOS__)
      if (hDisplay&hWnd)
        {}
    #endif
  } 

VOID GUIWINDOW::ShowWindow ( LONG CmdShow )
  {
    if (CmdShow)
      {}
    #if defined (__FORWINDOWS__)
      #if defined (__FORWIN386__)
        ::ShowWindow ( hWindow, (short)CmdShow );
      #else
        ::ShowWindow ( hWindow, CmdShow );
      #endif
    #endif
  } 

VOID GUIWINDOW::UpdateWindow ()
  {
    #if defined (__FORWINDOWS__)
      ::UpdateWindow ( hWindow );
    #endif
  } 

LONG GUIWINDOW::ShowMessageBox ( STRING Title, STRING Message, LONG Flag )
  {
    #if defined (__FORWINDOWS__)
      #if defined (__FORWIN386__)
        return MessageBox ( hWindow, Message, Title, (short)Flag );
      #else
        return MessageBox ( hWindow, Message, Title, Flag );
      #endif
    #elif defined (__FOROS2__)
      if (Title)
        {}
      if (Message)
        {}
      if (Flag)
        {}
      return SUCCESS;
    #elif defined (__FORDOS__)
      if (Title)
        {}
      if (Message)
        {}
      if (Flag)
        {}
      return SUCCESS;
    #endif
  } 

VOID GUIWINDOW::SetClassStyle ( LONG Style )
  {
    ClassStyle = Style;
  } 

VOID GUIWINDOW::AddClassStyle ( LONG Style )
  {
    ClassStyle |= Style;
  } 

VOID GUIWINDOW::RemoveClassStyle ( LONG Style )
  {
    ClassStyle &= ~Style;
  } 

VOID GUIWINDOW::SetWinStyle ( LONG Style )
  {
    WinStyle = Style;
  } 

VOID GUIWINDOW::AddWinStyle ( LONG Style )
  {
    WinStyle |= Style;
  } 

VOID GUIWINDOW::RemoveWinStyle ( LONG Style )
  {
    WinStyle &= ~Style;
  } 

VOID GUIWINDOW::SetWinExStyle ( LONG Style )
  {
    WinExStyle = Style;
  } 

VOID GUIWINDOW::AddWinExStyle ( LONG Style )
  {
    WinExStyle |= Style;
  } 

VOID GUIWINDOW::RemoveExWinStyle ( LONG Style )
  {
    WinExStyle &= ~Style;
  } 

VOID GUIWINDOW::SetBackGround ( LONG Background )
  {
    WinBG = Background;
  } 

LONG GUIWINDOW::OnPaint ( HDISPLAY hDisplay )
  {
    if (hDisplay)
      {}
    return 0;
  } 

LONG GUIWINDOW::OnCommand ( LONG Command )
  {
    if (Command)
      {}
    return 0;
  } 

LONG GUIWINDOW::OnClose ()
  {
    return 0;
  } 

LONG GUIWINDOW::OnSize ( LONG Wd, LONG Ht )
  {
    if (Wd&Ht)
      {}
    return 0;
  } 

LONG GUIWINDOW::OnKeyDown ( LONG WhichKey )
  {
    if (WhichKey)
      {}
    return 0;
  } 

LONG GUIWINDOW::OnKeyUp ( LONG WhichKey )
  {
    if (WhichKey)
      {}
    return 0;
  } 

LONG GUIWINDOW::OnMouse ( LONG Event, LONG Flags, LONG Mx, LONG My )
  {
    if (Event)
      {}
    if (Mx&My)
      {}
    if (Flags)
      {}
    return 0;
  } 

LONG GUIWINDOW::OnTimer ()
  {
    return 0;
  } 

LONG GUIWINDOW::OnDestroy ()
  {
    return 0;
  } 

LONG GUIWINDOW::OnScroll ( LONG SBarID, LONG Code, LONG Pos )
  {
    if (SBarID)
      {}
    if (Code)
      {}
    if (Pos)
      {}
    return 0;
  } 

LONG GUIWINDOW::OnPosChanging ( WINPOSCHANGE *WinPosChange )
  {
    if (WinPosChange)
      {}
    return 0;
  } 

LONG GUIWINDOW::OnActivate ( LONG Flags, BOOLEAN IsMinimized )
  {
    if (Flags)
      {}
    if (IsMinimized)
      {}
    return 0;
  } 

VOID GUIWINDOW::SetTimer ( LONG Timing )
  {
    #if defined (__FORWINDOWS__)
      #if defined (__FORWIN386__)
        ::SetTimer ( hWindow, NULL, (short)Timing, NULL );
      #else
        ::SetTimer ( hWindow, NULL, Timing, NULL );
      #endif
    #elif defined (__FOROS2__)
      if (Timing)
        {}
    #elif defined (__FORDOS__)
      if (Timing)
        {}
    #endif
  } 

VOID GUIWINDOW::KillTimer ()
  {
    #if defined (__FORWINDOWS__)
      ::KillTimer ( hWindow, 0 );
    #elif defined (__FORDOS__)
    #endif
  } 

VOID GUIWINDOW::SetScrollRange ( LONG SBarID, LONG Min, LONG Max )
  {
    #if defined (__FORWINDOWS__)
      #if defined (__FORWIN386__)
        ::SetScrollRange ( hClient, (short)SBarID, (short)Min, (short)Max, TRUE );
      #else
        ::SetScrollRange ( hClient, SBarID, Min, Max, TRUE );
      #endif
    #else
      if (SBarID)
        {}
      if (Min&Max)
        {}
    #endif
  } 

VOID GUIWINDOW::GetScrollRange ( LONG SBarID, LONG *Min, LONG *Max )
  {
    #if defined (__FORWINDOWS__)
      #if defined (__FORWIN386__)
        SHORT iMin,iMax;
        ::GetScrollRange ( hClient, (short)SBarID, &iMin, &iMax );
      #else
        INT iMin,iMax;
        ::GetScrollRange ( hClient, SBarID, &iMin, &iMax );
      #endif      
      *Min = iMin;
      *Max = iMax;
    #else
      if (SBarID)
        {}
      if (Min)
        {}
      if (Max)
        {}
    #endif
  } 

VOID GUIWINDOW::SetScrollPos ( LONG SBarID, LONG Pos )
  {
    #if defined (__FORWINDOWS__)
      #if defined (__FORWIN386__)
        ::SetScrollPos ( hClient, (short)SBarID, (short)Pos, TRUE );
      #else
        ::SetScrollPos ( hClient, SBarID, Pos, TRUE );
      #endif  
    #else
      if (SBarID)
        {}
      if (Pos)
        {}
    #endif
  } 

VOID GUIWINDOW::GetScrollPos ( LONG SBarID, LONG *Pos )
  {
    #if defined (__FORWINDOWS__)
      #if defined (__FORWIN386__)
        *Pos = ::GetScrollPos ( hClient, (short)SBarID );
      #else
        *Pos = ::GetScrollPos ( hClient, SBarID );
      #endif
    #else
      if (SBarID)
        {}
      if (Pos)
        {}
    #endif
  } 

VOID GUIWINDOW::SetClientAreaPos ( LONG x, LONG y )
  {
    if (x&y)
      {}
    if (hWindow==hClient)
      return;
        
    #if defined (__FORWINDOWS__)
      ClientXPos = x;
      ClientYPos = y;
      #if defined (__FORWIN386__)
        SetWindowPos ( hClient, NULL, (short)x, (short)y, 0, 0, SWP_NOSIZE | SWP_NOACTIVATE |
                       SWP_NOZORDER );
      #else
        SetWindowPos ( hClient, NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOACTIVATE |
                       SWP_NOZORDER );
      #endif
    #else
    #endif  
  } 

BOOLEAN GUIWINDOW::AddChildWindow ( GUIWINDOW *ChildWindow, STRING Title, LONG x, LONG y,
                                    LONG Wd, LONG Ht )
  {
    if (ChildWindow==NULL)
      return FAILURE;
    BOOLEAN Result;  
    Result = ChildWindow->Create ( Title, x, y, Wd, Ht, this );
    if (Result==FAILURE)
      return FAILURE;
    #if defined (__FORWINDOWS__)  
      ChildWindow->ShowWindow ( SW_SHOW );
    #endif  
    return Result;  
  } 
                                    
VOID GUIWINDOW::ExitApplication ( LONG ReturnValue )
  {
    if (ReturnValue)
      {}
    #if defined (__FORWINDOWS__)
      #if defined (__FORWIN386__)
        PostQuitMessage ( (short)ReturnValue );
      #else
        PostQuitMessage ( ReturnValue );
      #endif
    #elif defined (__FOROS2__)
    #elif defined (__FORDOS__)
    #elif defined (__FORUNIX__)
      ExitApp = TRUE;
    #endif
  } 

VOID GUIWINDOW::SetToolBar ( HWINDOW ToolBar, LONG Ht )
  {
    hToolBar = ToolBar;  
    SetClientAreaPos ( 0, Ht );  
  } 

VOID GUIWINDOW::ResetClientArea ()
  {
    #if defined (__FORWINDOWS__)
      RECT r;
      GetClientRect ( hWindow, &r );
      if (hClient!=hWindow)
        {
          #if defined (__FORWIN386__)
            SetWindowPos ( hClient, NULL, (short)ClientXPos, (short)ClientYPos,
                           (short)(r.right-ClientXPos),
                           (short)(r.bottom-ClientYPos),
                           SWP_NOZORDER | SWP_NOACTIVATE );
          #else
            SetWindowPos ( hClient, NULL, ClientXPos, ClientYPos, r.right-ClientXPos,
                           r.bottom-ClientYPos,
                           SWP_NOZORDER | SWP_NOACTIVATE );
          #endif  
        } 
      if (hToolBar!=NULL)
        {
          #if defined (__FORWIN386__)
            SetWindowPos ( hToolBar, NULL, 0, 0, r.right,
                           (short)ClientYPos,
                           SWP_NOZORDER | SWP_NOACTIVATE );
          #else
            SetWindowPos ( hToolBar, NULL, 0, 0, r.right,
                           ClientYPos,
                           SWP_NOZORDER | SWP_NOACTIVATE );
          #endif  
        } 
    #else
    #endif
  } 

VOID GUIWINDOW::SetMenu ( GUIMENU *Menu )
  {
    #if defined (__FORWINDOWS__)
      ::SetMenu ( hWindow, Menu->GetHandle() );
      ::DrawMenuBar ( hWindow );
    #elif defined (__FOROS2__)
      if (Menu)
        {}
    #elif defined (__FORDOS__)
      if (Menu)
        {}      
    #endif
  } 

LONG GUIWINDOW::UserProc ( MESSAGE iMessage, PARAM1 Param1, PARAM2 Param2 )
  {
    if (iMessage)
      {}
    if (Param1)
      {}
    if (Param2)
      {}
    return 0;
  } 

LONG GUIWINDOW::WndProc ( HWINDOW hWnd, MESSAGE iMessage, PARAM1 Param1, PARAM2 Param2, BOOLEAN FromClient )
  {
    LONG Result;
    hWindow = hWnd;
    if (FromClient)
      {}
    if (Param1)
      {}
    if (Param2)
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
          #if defined (__FORWINDOWS__)
            PAINTSTRUCT Ps,Ps1;
            HDISPLAY hDisplay;

            if (hWindow!=hClient)
              {
                BeginPaint ( hWindow, &Ps1 );
                hDisplay = BeginPaint ( hClient, &Ps );
              } 
            else
              hDisplay = BeginPaint ( hWindow, &Ps1 );
                  
            Result = OnPaint ( hDisplay );

            if (hWindow!=hClient)
              {
                EndPaint ( hClient, &Ps );
                EndPaint ( hWindow, &Ps1 );
              } 
            else
              EndPaint ( hWindow, &Ps1 );

          #elif defined (__FOROS2__)
            HDISPLAY hDisplay;
            hDisplay = WinBeginPaint ( hWindow, NULLHANDLE, NULL );
            Result = OnPaint ( hDisplay );
            WinEndPaint ( hDisplay );

          #elif defined (__FORUNIX__)
            Result = OnPaint ( hWindow );  
 
          #elif defined (__FORDOS__)
            Result = OnPaint ( 0 );
          #endif
          break;

        case GUI_WM_COMMAND :
          #if defined (__FORWINDOWS__)
            Result = OnCommand ( Param1 );
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

        case GUI_WM_TIMER :
          Result = OnTimer ();
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
            if ((FromClient)&(hWindow!=hClient))
              Result = OnMouse ( iMessage, Flags, Mx, My );
            if ((FromClient==FALSE)&(hWindow==hClient))
              Result = OnMouse ( iMessage, Flags, Mx, My );
          #elif defined (__FOROS2__)
            if (Mx&My&Flags)
              {}
          #elfi defined (__FORUNIX__)

          #elif defined (__FORDOS__)
            if (Mx&My&Flags)
              {}
          #endif
          break;

        case GUI_WM_SIZE :
          ResetClientArea ();
          Result = OnSize ( LOWORD (Param2), HIWORD (Param2) );
          break;

        case GUI_WM_VSCROLL :
          #if defined (__FORWINDOWS__)
            Result = OnScroll ( SBAR_VERT, Param1, LOWORD(Param2));
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
            #else  
              lpwp->x = Wpc.xPos;
              lpwp->y = Wpc.yPos;
              lpwp->cx = Wpc.Width;
              lpwp->cy = Wpc.Height;
            #endif
            lpwp->flags = Wpc.Flags;
          #endif
          break;

        default :
          Result = UserProc ( iMessage, Param1, Param2 );
          break;
      } 
    return Result;
  } 








APIRESULT APIPROC GUIWinProc ( HWINDOW hWindow, MESSAGE iMessage,
                               PARAM1 Param1, PARAM2 Param2 )
  {
    LONG Result;
    GUIWINDOW *WinPtr;

    Result = 0;
    WinPtr = FindWindow ( hWindow );
    if (WinPtr==NULL)
      {
        if (NewWindow!=NULL)
          Result = NewWindow->WndProc ( hWindow, iMessage, Param1, Param2, FALSE );
      } 
    else
      {
        Result = WinPtr->WndProc ( hWindow, iMessage, Param1, Param2, FALSE );
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








BOOLEAN CallParent ( MESSAGE iMessage )
  {
    switch (iMessage)
      {
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
        case GUI_WM_CHAR :
        case GUI_WM_PAINT :
        case GUI_WM_VSCROLL :
        case GUI_WM_HSCROLL :        
          return TRUE;
      } 
    return FALSE;
  } 

APIRESULT APIPROC ClientProc ( HWINDOW hWindow, MESSAGE iMessage,
                               PARAM1 Param1, PARAM2 Param2 )
  {
    GUIWINDOW *Parent;
    LONG Result;

    #if defined (__FORWINDOWS__)
      Parent = (GUIWINDOW*)GetWindowLong( hWindow, 0 );
    #endif  

    Result = 0;
    if (Parent!=NULL)
      {
        if (CallParent(iMessage))
          Result = Parent->WndProc ( Parent->GetHandle(), iMessage,
                                     Param1, Param2, TRUE );
        else
          {
            if (iMessage==GUI_WM_PAINT)
              {
                #if defined (__FORWINDOWS__)  
                  PAINTSTRUCT Ps;  
                  BeginPaint ( hWindow, &Ps );
                  EndPaint ( hWindow, &Ps );
                  Result = 1;
                #endif  
              } 
          } 
      } 

    if (Result!=0)
      {};

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

#if defined (__FORUNIX__)
VOID GUIWINDOW::MapXWinEvent ( MESSAGE *iMessage, XEvent *EventPtr )
  {
    switch (EventPtr->type)
      {
        case Expose :
          if (EventPtr->xexpose.count==0)
              *iMessage = GUI_WM_PAINT;
              break;
        case ButtonPress :
          switch (EventPtr->xbutton.button)
            {
              case Button1 :
                *iMessage = GUI_WM_LBUTTONDOWN;
                break;
              case Button2 :
                *iMessage = GUI_WM_MBUTTONDOWN;
                break;
              case Button3 :
                *iMessage = GUI_WM_RBUTTONDOWN;
                break;
            } 
        case ButtonRelease :
          switch (EventPtr->xbutton.button)
            {
              case Button1 :
                *iMessage = GUI_WM_LBUTTONUP;
                break;
              case Button2 :
                *iMessage = GUI_WM_MBUTTONUP;
                break;
              case Button3 :
                *iMessage = GUI_WM_RBUTTONUP;
                break;
            } 
          break;
        default :
          *iMessage = EventPtr->type;
          break;
      } 
  } 
 
  #if defined (__FORMOTIF__)
VOID GUIWinXWinProc ( Widget W, XtPointer ClientData, 
                      XEvent *EventPtr )
  {
    GUIWINDOW *WinPtr;
    HWINDOW hWindow;
    PARAM1 Param1;
    PARAM2 Param2;
    MESSAGE iMessage;

    hWindow = XtWindow ( W );
    WinPtr = FindWindow ( hWindow );
    if (WinPtr==NULL)
      {
      } 
    else
      {
        Param1 = EventPtr;
        Param2 = ClientData;
        WinPtr->MapXWinEvent ( &iMessage, EventPtr );
        WinPtr->WndProc ( hWindow, iMessage, Param1, 
                          Param2, FALSE );
       } 
    ClearRemovedList ();
  } 
  #endif
#endif
