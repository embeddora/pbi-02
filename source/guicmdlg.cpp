




#include "stdgfx.h"
#include "guicmdlg.h"

UINT APIPROC FileDlgProc ( HWINDOW hWindow, UINT iMessage, PARAM1 Param1, 
                           PARAM2 Param2 );







GUIFILEDIALOG::GUIFILEDIALOG ( FD_TYPE forOpen ) : GUIDIALOG ()
  {
    ForOpen = forOpen;
    FileAttr.DefPath = NULL;
    FileAttr.DefExt = NULL;
    FileAttr.DefName = NULL;
    FileAttr.NumExt = 0;
    FileAttr.Extensions = NULL;
    FileName = NULL;
  } 

GUIFILEDIALOG::~GUIFILEDIALOG ()
  {
    if (FileAttr.DefPath!=NULL)
      delete FileAttr.DefPath;
    if (FileAttr.DefExt!=NULL)
      delete FileAttr.DefExt;
    if (FileAttr.DefName!=NULL)
      delete FileAttr.DefName;

    INT i;
    if (FileAttr.Extensions!=NULL)
      {
        for (i=0;i<FileAttr.NumExt;i++)
          {
            if (FileAttr.Extensions[i].Ext!=NULL)
              delete FileAttr.Extensions[i].Ext;
            FileAttr.Extensions[i].Ext = NULL;
            if (FileAttr.Extensions[i].Description!=NULL)
              delete FileAttr.Extensions[i].Description;
            FileAttr.Extensions[i].Description = NULL;
          } 
        delete FileAttr.Extensions;
      } 
    FileAttr.DefPath = NULL;
    FileAttr.DefExt = NULL;
    FileAttr.DefName = NULL;
    FileAttr.Extensions = NULL;
    FileAttr.NumExt = 0;
    if (FileName!=NULL)
      delete FileName;
    FileName = NULL;
  } 

VOID GUIFILEDIALOG::CloseWindow ()
  {
  } 

LONG GUIFILEDIALOG::OnCreate ()
  {
    return 0;
  } 

LONG GUIFILEDIALOG::OnClose ()
  {
    return 0;
  } 

BOOLEAN GUIFILEDIALOG::Create ( STRING Title, LONG x, LONG y, LONG Wd, LONG Ht,
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

VOID GUIFILEDIALOG::SetDefaultPath ( STRING Path )
  {
    if (FileAttr.DefPath!=NULL)
      delete FileAttr.DefPath;
    FileAttr.DefPath = new CHAR [strlen(Path)+1];
    strcpy ( FileAttr.DefPath, Path );
  } 

VOID GUIFILEDIALOG::SetDefaultName ( STRING FileName )
  {
    if (FileAttr.DefName!=NULL)
      delete FileAttr.DefName;
    FileAttr.DefName = new CHAR [strlen(FileName)+1];
    strcpy ( FileAttr.DefName, FileName );
  } 

VOID GUIFILEDIALOG::SetDefaultExt ( STRING ExtName )
  {
    if (FileAttr.DefExt!=NULL)
      delete FileAttr.DefExt;
    FileAttr.DefExt = new CHAR [strlen(ExtName)+1];
    strcpy ( FileAttr.DefExt, ExtName );
  } 

VOID GUIFILEDIALOG::CreateExtensions ( LONG Num )
  {
    if (Num<=0)
      return;
    LONG i;
    if (FileAttr.Extensions!=NULL)
      {
        for (i=0;i<Num;i++)
          {
            if (FileAttr.Extensions[i].Ext!=NULL)
              delete FileAttr.Extensions[i].Ext;
            FileAttr.Extensions[i].Ext = NULL;
            if (FileAttr.Extensions[i].Description!=NULL)
              delete FileAttr.Extensions[i].Description;
            FileAttr.Extensions[i].Description = NULL;
          } 
        delete FileAttr.Extensions;
      } 

    FileAttr.Extensions = new EXTENSION [Num];
    if (FileAttr.Extensions==NULL)
      {
        return;
      } 

    FileAttr.NumExt = Num;
    for (i=0;i<Num;i++)
      {
        FileAttr.Extensions[i].Ext = NULL;
        FileAttr.Extensions[i].Description = NULL;
      } 
  } 

VOID GUIFILEDIALOG::SetExtension ( LONG Index, STRING Ext, STRING Description )
  {
    if ((Index<0)||(Index>=FileAttr.NumExt))
      return;

    if (FileAttr.Extensions[Index].Ext!=NULL)
      delete FileAttr.Extensions[Index].Ext;
    FileAttr.Extensions[Index].Ext = new CHAR [strlen(Ext)+1];
    strcpy ( FileAttr.Extensions[Index].Ext, Ext );

    if (FileAttr.Extensions[Index].Description!=NULL)
      delete FileAttr.Extensions[Index].Description;
    FileAttr.Extensions[Index].Description = new CHAR [strlen(Description)+1];
    strcpy ( FileAttr.Extensions[Index].Description, Description );
  } 

LONG GUIFILEDIALOG::ExecuteOpen ( HWINDOW hWindow )
  {
    LONG Result;

    if (FileName!=NULL)
      delete FileName;
    FileName = NULL;

    if (hWindow)
      {}
    #if defined (__FORWINDOWS__)
      OPENFILENAME ofn;
      ofn.lStructSize = sizeof(OPENFILENAME);
      ofn.hwndOwner = hWindow;
      ofn.hInstance = hInstance;
      ofn.lpstrCustomFilter = new CHAR [10];
        memset ( ofn.lpstrCustomFilter, 0, 10 );
      ofn.nMaxFileTitle = 256;
      ofn.nMaxCustFilter = 20;
      ofn.nFilterIndex = 0;
      ofn.lpstrFileTitle = new CHAR [256];
        strcpy ( ofn.lpstrFileTitle, "" );
      ofn.nMaxFile = 256;
      ofn.lpstrFile = new CHAR [256];
      if (FileAttr.DefName==NULL)
        {
          strcpy ( ofn.lpstrFile, "" );
        } 
      else
        {
          strcpy ( ofn.lpstrFile, FileAttr.DefName );
        } 
      if (FileAttr.DefPath==NULL)
        {
          ofn.lpstrInitialDir = new CHAR [4];
          strcpy ( (CHAR*)ofn.lpstrInitialDir, "" );
        } 
      else
        {
          ofn.lpstrInitialDir = new CHAR [strlen(FileAttr.DefPath)+1];
                         strcpy ( (CHAR*)ofn.lpstrInitialDir, FileAttr.DefPath );
        } 
      ofn.lpstrTitle = new CHAR [10];
        strcpy ( (CHAR*)ofn.lpstrTitle, "" );
      ofn.Flags = OFN_FILEMUSTEXIST | OFN_ENABLEHOOK;
      ofn.nFileOffset = 0;
      ofn.nFileExtension = 0;
      if (FileAttr.DefExt==NULL)
        {
          ofn.lpstrDefExt = new CHAR [10];
          strcpy ( (CHAR*)ofn.lpstrDefExt, "*.*" );
        } 
                else
        {
          ofn.lpstrDefExt = new CHAR [strlen(FileAttr.DefExt)+1];
          strcpy ( (CHAR*)ofn.lpstrDefExt, FileAttr.DefExt );
        } 
        
      ofn.lCustData = 0; 
      ofn.lpfnHook = FileDlgProc;
      ofn.lpTemplateName = NULL;

      LONG i;

      ofn.lpstrFilter = new CHAR [256]; 
      memset ( (CHAR*)ofn.lpstrFilter, 0, 256 );
      LONG j,Count=0,NumStr;
      for (i=0;i<FileAttr.NumExt;i++)
        {
          if (FileAttr.Extensions[i].Description)
            {
              NumStr = strlen (FileAttr.Extensions[i].Description);  
              for (j=0;j<NumStr;j++)
                {
                  ((CHAR*)ofn.lpstrFilter)[Count++] =
                    FileAttr.Extensions[i].Description[j];
                } 
            } 
          ((CHAR*)ofn.lpstrFilter)[Count++] = '\0';
          if (FileAttr.Extensions[i].Ext)
            {
              NumStr = strlen (FileAttr.Extensions[i].Ext);  
              for (j=0;j<NumStr;j++)
                {
                  ((CHAR*)ofn.lpstrFilter)[Count++] =
                    FileAttr.Extensions[i].Ext[j];
                } 
            } 
                         ((CHAR*)ofn.lpstrFilter)[Count++] = '\0';
        } 

      Result = GetOpenFileName ( &ofn );
      if (Result)
        {
          FileName = new CHAR [strlen(ofn.lpstrFileTitle)+1];
          strcpy ( FileName, ofn.lpstrFileTitle );
        } 

      
      delete (CHAR*)(ofn.lpstrCustomFilter);
                
      delete (CHAR*)(ofn.lpstrFileTitle);
      delete (CHAR*)(ofn.lpstrInitialDir);
      delete (CHAR*)(ofn.lpstrTitle);
      delete (CHAR*)(ofn.lpstrDefExt);
      delete (CHAR*)(ofn.lpstrFilter);
    #endif
    return Result;
  } 

LONG GUIFILEDIALOG::ExecuteSave ( HWINDOW hWindow )
  {
         LONG Result;

    if (FileName!=NULL)
      delete FileName;
    FileName = NULL;

    if (hWindow)
      {}
    #if defined (__FORWINDOWS__)
      OPENFILENAME ofn;
      ofn.lStructSize = sizeof(OPENFILENAME);
      ofn.hwndOwner = hWindow;
                ofn.hInstance = hInstance;
      ofn.lpstrCustomFilter = new CHAR [10];
        memset ( ofn.lpstrCustomFilter, 0, 10 );
      ofn.nMaxFileTitle = 256;
      ofn.nMaxCustFilter = 20;
      ofn.nFilterIndex = 0;
      ofn.lpstrFileTitle = new CHAR [256];
        strcpy ( ofn.lpstrFileTitle, "" );
      ofn.nMaxFile = 256;
      ofn.lpstrFile = new CHAR [256];
      if (FileAttr.DefName==NULL)
        {
          strcpy ( ofn.lpstrFile, "" );
        } 
      else
        {
          strcpy ( ofn.lpstrFile, FileAttr.DefName );
        } 
      if (FileAttr.DefPath==NULL)
        {
          ofn.lpstrInitialDir = new CHAR [4];
          strcpy ( (CHAR*)ofn.lpstrInitialDir, "" );
        } 
      else
        {
          ofn.lpstrInitialDir = new CHAR [strlen(FileAttr.DefPath)+1];
          strcpy ( (CHAR*)ofn.lpstrInitialDir, FileAttr.DefPath );
        } 
      ofn.lpstrTitle = new CHAR [10];
        strcpy ( (CHAR*)ofn.lpstrTitle, "" );
      ofn.Flags = OFN_OVERWRITEPROMPT | OFN_ENABLEHOOK;
      ofn.nFileOffset = 0;
      ofn.nFileExtension = 0;
      if (FileAttr.DefExt==NULL)
        {
          ofn.lpstrDefExt = new CHAR [10];
          strcpy ( (CHAR*)ofn.lpstrDefExt, "*.*" );
        } 
      else
        {
          ofn.lpstrDefExt = new CHAR [strlen(FileAttr.DefExt)+1];
          strcpy ( (CHAR*)ofn.lpstrDefExt, FileAttr.DefExt );
        } 
        
      ofn.lCustData = 0;
      ofn.lpfnHook = FileDlgProc;
      ofn.lpTemplateName = NULL;

      LONG i;

      ofn.lpstrFilter = new CHAR [256]; 
      memset ( (CHAR*)ofn.lpstrFilter, 0, 256 );
      LONG j,Count=0,NumStr;
      for (i=0;i<FileAttr.NumExt;i++)
        {
          if (FileAttr.Extensions[i].Description)
            {
              NumStr = strlen (FileAttr.Extensions[i].Description);  
              for (j=0;j<NumStr;j++)
                {
                  ((CHAR*)ofn.lpstrFilter)[Count++] =
                    FileAttr.Extensions[i].Description[j];
                } 
            } 
          ((CHAR*)ofn.lpstrFilter)[Count++] = '\0';
          if (FileAttr.Extensions[i].Ext)
            {
              NumStr = strlen (FileAttr.Extensions[i].Ext);  
              for (j=0;j<NumStr;j++)
                {
                  ((CHAR*)ofn.lpstrFilter)[Count++] =
                    FileAttr.Extensions[i].Ext[j];
                } 
            } 
          ((CHAR*)ofn.lpstrFilter)[Count++] = '\0';
        } 

      Result = GetSaveFileName ( &ofn );
      if (Result)
        {
          FileName = new CHAR [strlen(ofn.lpstrFileTitle)+1];
          strcpy ( FileName, ofn.lpstrFileTitle );
        } 

      
      delete (CHAR*)ofn.lpstrCustomFilter;
      delete (CHAR*)ofn.lpstrFile;
      delete (CHAR*)ofn.lpstrFileTitle;
      delete (CHAR*)ofn.lpstrInitialDir;
      delete (CHAR*)ofn.lpstrTitle;
      delete (CHAR*)ofn.lpstrDefExt;
      delete (CHAR*)ofn.lpstrFilter;
    #endif
    return Result;
  } 

LONG GUIFILEDIALOG::Execute ( GUIWINDOW *Parent, STRING Name )
  {
    HWINDOW hWindow;

    if (Name)
      {}
    if (Parent==NULL)
      hWindow = NULL;
    else
      hWindow = Parent->GetHandle ();

    LONG Result;
    if (ForOpen==FD_OPEN)
      Result = ExecuteOpen ( hWindow );
    else if (ForOpen==FD_SAVE)
      Result = ExecuteSave ( hWindow );
    return Result;
  } 

VOID GUIFILEDIALOG::GetFileName ( STRING Name )
  {
    if (FileName!=NULL)
      strcpy ( Name, FileName );
  } 

LONG GUIFILEDIALOG::WndProc ( HWINDOW hWnd, MESSAGE iMessage, PARAM1 Param1, PARAM2 Param2, BOOLEAN FromClient )
  {
    if (hWnd)
      {}
    if (iMessage)
      {}
    if (Param1)
      {}
    if (Param2)
      {}
    if (FromClient)
      {}
    return 0;
  } 








UINT APIPROC FileDlgProc ( HWINDOW hWindow, UINT iMessage, PARAM1 Param1, PARAM2 Param2 )
  {
    if (hWindow)
      {}
    if (iMessage)
      {}
    if (Param1)
      {}      
    if (Param2)
      {}      
    switch (iMessage)
      {
        case GUI_WM_INITDIALOG :
          #if defined (__FORWINDOWS__)
            SetWindowPos ( hWindow, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE );            
          #endif
          break;   
      } 
        
    return 0;  
  } 

