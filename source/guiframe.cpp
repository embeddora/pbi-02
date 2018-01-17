




#include "stdgfx.h"
#include "guiframe.h"
 
APIRESULT APIPROC GUIFrameProc ( HWINDOW hWindow, MESSAGE iMessage,
                                 PARAM1 Param1, PARAM2 Param2 );

#if defined (__FORUNIX__)
  #if defined (__FORMOTIF__)
    VOID GUIFrameXWinProc ( Widget W, XtPointer ClientData, 
                          XEvent *EventPtr ); 
  #endif
#endif







GUIFRAME *NewFrame=NULL;

GUIFRAME::GUIFRAME () : GUIWINDOW ()
  {
    strcpy ( ClassName, "GUIFRAMEWINDOW" );
  } 

GUIFRAME::~GUIFRAME ()
  {
  } 

VOID GUIFRAME::RegisterWindow ()
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
      WndClass.hInstance = (HINSTANCE)hInstance;
      WndClass.hIcon = LoadIcon ( NULL, "END" );
      WndClass.lpfnWndProc = (WNDPROC)GUIFrameProc;
      WndClass.lpszClassName = ClassName;
      WndClass.lpszMenuName = NULL;
      WndClass.style = (UINT)ClassStyle;

      RegisterClass ( &WndClass );

    #elif defined (__FOROS2__)
      WinRegisterClass ( hAB, ClassName, GUIFrameProc, CS_SIZEREDRAW, 0 );
    #endif
  } 

BOOLEAN GUIFRAME::Create ( STRING Title, LONG x, LONG y, LONG Wd, LONG Ht,
                           GUIWINDOW *Parent )
  {
    PreRegister (); 
    RegisterWindow ();
    NewFrame = this;

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
          hWindow = CreateWindow ( ClassName, Title, WinStyle, (short)x, (short)y, (short)Wd,
                                   (short)Ht, hParent, NULL,
                                   (HINSTANCE)hInstance, NULL );
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
      
      CreateClientWindow ();
      hNew = hWindow;

    #elif defined (__FOROS2__)  
      if (Parent==NULL)
        hParent = HDESKTOP;
      else
        hParent = Parent->GetClient ();
        

      ULONG CreateFlag = FCF_TITLEBAR | FCF_SYSMENU | FCF_SIZEBORDER |
                              FCF_MINMAX | FCF_TASKLIST | FCF_SHELLPOSITION;
      hWindow = WinCreateStdWindow ( hParent, WS_VISIBLE, &CreateFlag, ClassName,
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
    #elif defined (__FORUNIX__)
      if (Title)
        {}
      if (x&y&Wd&Ht)
        {}
      if (Parent)
        {}
      if (hParent)
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

          XtAddCallback ( WinWidget, XmNactivateCallback, GUIFrameXWinProc,
                          NULL );
                          
          XtRealizeWidget ( WinWidget );
           
          MainXDisplay = XtDisplay ( WinWidget );
          hWindow = XtWindow ( WinWidget );
          hClient = hWindow;
          hNew = hClient;     
       #endif   
    #endif
    AddWindow ( this, hNew );
    NewFrame = NULL;
    return SUCCESS;
  } 

BOOLEAN GUIFRAME::AddChild ( GUIWINDOW *Child, STRING Title, LONG x, LONG y,
                             LONG Wd, LONG Ht )
  {
    Child->Create ( Title, x, y, Wd, Ht, this );
    return SUCCESS;
  } 

LONG GUIFRAME::OnCreate ()
  {
    return 0;
  } 

LONG GUIFRAME::OnClose ()
  {
    return 0;
  } 

LONG GUIFRAME::OnDestroy ()
  {
    ExitApplication (0);
    return 0;
  } 







APIRESULT APIPROC GUIFrameProc ( HWINDOW hWindow, MESSAGE iMessage,
                                 PARAM1 Param1, PARAM2 Param2 )
  {
    LONG Result;
    GUIWINDOW *WinPtr;

    Result = 0;
    WinPtr = FindWindow ( hWindow );
    if (WinPtr==NULL)
      {
        if (NewFrame!=NULL)
          Result = NewFrame->WndProc ( hWindow, iMessage, Param1, Param2, FALSE );
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
      if (hWindow&iMessage&Param1&Param1)
        {}
      return 0;
    #endif
  } 

#if defined (__FORUNIX__)
  #if defined (__FORMOTIF__)
VOID GUIFrameXWinProc ( Widget W, XtPointer ClientData, 
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
