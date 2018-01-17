




#ifndef __GUICMDLG_H__
#define __GUICMDLG_H__

#include "stdgfx.h"
#include "guidlg.h"

#if defined (__FORWINDOWS__)
  #include <commdlg.h>
#endif

#define FD_TYPE  LONG
#define FD_OPEN  1
#define FD_SAVE  2

struct EXTENSION
  {
    STRING Ext;
    STRING Description;
  }; 

struct FILENAMEATTR
  {
    STRING DefPath;
    STRING DefExt;
    STRING DefName;
    LONG   NumExt;
    EXTENSION *Extensions;
  }; 

class GUIFILEDIALOG : public GUIDIALOG
  {
    protected :
      FILENAMEATTR FileAttr;
      BOOLEAN ForOpen;
      STRING  FileName;

      virtual VOID CloseWindow ();
      virtual LONG OnCreate ();
      virtual LONG OnClose ();
      LONG ExecuteOpen ( HWINDOW hWindow );
      LONG ExecuteSave ( HWINDOW hWindow );

    public :
      GUIFILEDIALOG ( FD_TYPE ForOpen );
      virtual ~GUIFILEDIALOG ();

      virtual BOOLEAN Create ( STRING Title, LONG x, LONG y, LONG Wd, LONG Ht,
                               GUIWINDOW *Parent );
      virtual LONG Execute ( GUIWINDOW *Parent, STRING Name );
      virtual LONG WndProc ( HWINDOW hWnd, MESSAGE iMessage, PARAM1 Param1, PARAM2 Param2, BOOLEAN FromClient );
      VOID SetDefaultPath ( STRING Path );
      VOID SetDefaultName ( STRING FileName );
      VOID SetDefaultExt ( STRING ExtName );
      VOID CreateExtensions ( LONG Num );
      VOID SetExtension ( LONG Index, STRING Ext, STRING Description );
      VOID GetFileName ( STRING Name );
  }; 

#endif

