




#include "stdgfx.h"
#include "guidlg.h"

BOOL DLGPROC GUIDialogProc ( HWINDOW hDlg, MESSAGE iMessage, PARAM1 Param1, PARAM2 Param2 );







GUIDIALOG *NewDialog=NULL;

GUIDIALOG::GUIDIALOG ()
  {
    ReturnValue = 0;
  } 

GUIDIALOG::~GUIDIALOG ()
  {
  } 

LONG GUIDIALOG::OnInitDialog ()
  {
    return 0;  
  } 

LONG GUIDIALOG::OnCreate ()
  {
    return 0;
  } 

LONG GUIDIALOG::OnClose ()
  {
    return 0;
  } 

BOOLEAN GUIDIALOG::Create ( STRING Title, LONG x, LONG y, LONG Wd, LONG Ht,
                            GUIWINDOW *Parent )
  {
    if (Parent)
      {}
    if (Title)
      {}
    if (x&y&Wd&Ht)
      {}
    return SUCCESS;
  } 

LONG GUIDIALOG::Execute ( GUIWINDOW *Parent, STRING Name )
  {
    ParentWindow = Parent;
    #if defined (__FORWINDOWS__)
      HWINDOW hParent;

      if (ParentWindow==NULL)
        hParent = HDESKTOP;
      else
        hParent = ParentWindow->GetHandle ();

      #if defined (__FORWIN386__)
        FARPROC pDlgProc;
        pDlgProc = MakeProcInstance ( (FARPROC)GUIDialogProc, hInstance );
        NewDialog = this;
        ReturnValue = DialogBox ( hInstance, Name, hParent, pDlgProc );
        NewDialog = NULL;
        FreeProcInstance ( pDlgProc );
      #else
        FARPROC pDlgProc;
        pDlgProc = MakeProcInstance ( (FARPROC)GUIDialogProc, hInstance );
        NewDialog = this;
        ReturnValue = DialogBox ( hInstance, Name, hParent, pDlgProc );
        FreeProcInstance ( pDlgProc );
        NewDialog = NULL;
      #endif
      
      return ReturnValue;
    #elif defined (__FOROS2__)
      if (Name)
        {}
      return 0;
    #elif defined (__FORDOS__)
      if (Name)
        {}
      return 0;
    #endif
  } 

VOID GUIDIALOG::CloseWindow ()
  {
    #if defined (__FORWINDOWS__)
      #if defined (__FORWIN386__)
        EndDialog ( hWindow, (short)ReturnValue );
      #else  
        EndDialog ( hWindow, (int)ReturnValue );
      #endif
    #endif
  } 

VOID GUIDIALOG::SetItemText ( LONG ID, STRING Text )
  {
    #if defined (__FORWINDOWS__)
      #if defined (__FORWIN386__)
        SetDlgItemText ( hWindow, (short)ID, Text );
      #else
        SetDlgItemText ( hWindow, ID, Text );
      #endif  
    #else
      if (ID)
        {}
      if (Text)
        {}
    #endif 
  } 

VOID GUIDIALOG::SetItemNumber ( LONG ID, LONG Number )
  {
    #if defined (__FORWINDOWS__)
      #if defined (__FORWIN386__)
        SetDlgItemInt ( hWindow, (short)ID, (short)Number, TRUE );
      #else
        SetDlgItemInt ( hWindow, ID, Number, TRUE );
      #endif
    #else
      if (ID)
        {}
      if (Number)
        {}
    #endif 
  } 

VOID GUIDIALOG::SetButtonState ( LONG ID, LONG State )
  {
    #if defined (__FORWINDOWS__)
      #if defined (__FORWIN386__)
        CheckDlgButton ( hWindow, (short)ID, (short)State );
      #else  
        CheckDlgButton ( hWindow, ID, State );
      #endif
    #else
      if (ID&State)
        {}  
    #endif  
  } 

LONG GUIDIALOG::GetButtonState ( LONG ID )
  {
    #if defined (__FORWINDOWS__)
      #if defined (__FORWIN386__)
        return IsDlgButtonChecked ( hWindow, (short)ID );
      #else
        return IsDlgButtonChecked ( hWindow, ID );
      #endif
    #else
      if (ID)
        {}
      return 0;    
    #endif  
  } 

LONG GUIDIALOG::GetItemText ( LONG ID, STRING Text )
  {
    #if defined (__FORWINDOWS__)
      LONG Number;
      #if defined (__FORWIN386__)
        Number = GetDlgItemText ( hWindow, (short)ID, Text, 128 );
      #else
        Number = GetDlgItemText ( hWindow, ID, Text, 128 );
      #endif
      return Number;        
    #else
      if (ID)
        {}
      if (Text)
        {}
      return 0;
    #endif 
  } 

LONG GUIDIALOG::GetItemNumber ( LONG ID )
  {
    #if defined (__FORWINDOWS__)
      BOOL Result;
      #if defined (__FORWIN386__)
        return GetDlgItemInt ( hWindow, (short)ID, &Result, TRUE );
      #else
        return GetDlgItemInt ( hWindow, ID, &Result, TRUE );
      #endif
    #else
      if (ID)
        {}
      return 0;
    #endif 
  } 
  
LONG GUIDIALOG::WndProc ( HWINDOW hWnd, MESSAGE iMessage, PARAM1 Param1, PARAM2 Param2, BOOLEAN FromClient )
  {
    LONG Result;
    hWindow = hWnd;
    hClient = hWindow;
    if (Param1)
      {}
    if (Param2)
      {}
    if (FromClient)
      {}
    switch (iMessage)
      {
        case GUI_WM_INITDIALOG :
          Result = OnInitDialog ();
          #if defined (__FORWINDOWS__)
            SetWindowPos ( hWindow, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );
          #endif  
          break;

        case GUI_WM_CLOSE :
          Result = OnClose ();
          if (Result==0)
            CloseWindow ();
          break;
          
        case GUI_WM_DESTROY :
          Result = OnDestroy ();
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

        default :
          Result = UserProc ( iMessage, Param1, Param2 );
          break;
      } 
    return Result;
  } 








BOOL DLGPROC GUIDialogProc ( HWINDOW hDlg, MESSAGE iMessage, PARAM1 Param1, PARAM2 Param2 )
  {
    LONG Result;
    Result = 0; 
    if (NewDialog!=NULL)
      {
        Result = NewDialog->WndProc ( hDlg, iMessage, Param1, Param2, FALSE );
      } 

    #if defined (__FORWINDOWS__)
    #elif defined (__FOROS2__)
      if (Result!=0)
        return Result;
      Result = (LONG)WinDefDlgProc ( hDlg, iMessage, Param1, Param2 );
    #elif defined (__FORDOS__)
    #endif
    if (Result)
      return 1;
    else
      return 0;  
  } 

  
  
