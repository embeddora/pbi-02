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



#ifndef __GUIWIN_H__
#define __GUIWIN_H__

#include "stdgfx.h"
#include "guimenu.h"

struct WINPOSCHANGE
  {
    INT  xPos;
    INT  yPos;
    INT  Width;
    INT  Height;
    UINT Flags;
  }; 

#define SKIPDEFPROC -1

class GUIWINDOW : public GUIOBJECT
  {
    protected :
      HWINDOW hWindow;
      HWINDOW hClient;
      HWINDOW hToolBar;

      GUIWINDOW *ParentWindow;

      LONG ClassStyle;
      LONG WinStyle;
      LONG WinExStyle;
      LONG ClientXPos;
      LONG ClientYPos;
      LONG ClientFlag;
      LONG ClientClass;
      
      LONG WinBG;
      CHAR ClassName[128];
 
      VOID PreRegister ();      
      virtual VOID RegisterWindow ();
      virtual VOID CreateClientWindow ();

      virtual LONG OnPaint ( HDISPLAY hDisplay );
      virtual LONG OnCommand ( LONG Command );
      virtual LONG OnCreate ()=0;
      virtual LONG OnClose ()=0;
      virtual LONG OnDestroy ();
      virtual LONG OnSize ( LONG Wd, LONG Ht );
      virtual LONG OnKeyDown ( LONG WhichKey );
      virtual LONG OnKeyUp ( LONG WhichKey );
      virtual LONG OnMouse ( LONG Event, LONG Flags, LONG Mx, LONG My );
      virtual LONG OnTimer ();
      virtual LONG OnScroll ( LONG SBarID, LONG Code, LONG Pos );
      virtual LONG OnPosChanging ( WINPOSCHANGE *WinPosChange );
			virtual LONG OnActivate ( LONG Flags, BOOLEAN IsMinimized ); 
      virtual LONG UserProc ( MESSAGE iMessage, PARAM1 Param1, PARAM2 Param2 );

      VOID SetTimer ( LONG Timing );
      VOID KillTimer ();

      VOID SetScrollRange ( LONG SBarID, LONG Min, LONG Max );
      VOID GetScrollRange ( LONG SBarID, LONG *Min, LONG *Max );
      VOID SetScrollPos ( LONG SBarID, LONG Pos );
      VOID GetScrollPos ( LONG SBarID, LONG *Pos );

      VOID SetClassStyle ( LONG Style );
      VOID AddClassStyle ( LONG Style );
      VOID RemoveClassStyle ( LONG Style );
      VOID SetWinStyle ( LONG Style );
      VOID AddWinStyle ( LONG Style );
      VOID RemoveWinStyle ( LONG Style );
      VOID SetWinExStyle ( LONG Style );
      VOID AddWinExStyle ( LONG Style );
      VOID RemoveExWinStyle ( LONG Style );

      BOOLEAN AddChildWindow ( GUIWINDOW *ChildWindow, STRING Title, LONG x, LONG y,
                               LONG Wd, LONG Ht );
      
      VOID InvalidateClient ();  
      LONG ShowMessageBox ( STRING Title, STRING Message, LONG Flag );
      VOID ExitApplication ( LONG ReturnValue );
      VOID ResetClientArea ();

    public :
      GUIWINDOW ();
      virtual ~GUIWINDOW ();

      virtual BOOLEAN Create ( STRING Title, LONG x, LONG y, LONG Wd, LONG Ht,
                               GUIWINDOW *Parent );
      virtual LONG WndProc ( HWINDOW hWnd, MESSAGE iMessage, PARAM1 Param1, PARAM2 Param2,
                             BOOLEAN FromClient );

      virtual VOID CloseWindow ();
      
      HDISPLAY GetDisplay ( HWINDOW hWnd );
      VOID ReleaseDisplay ( HWINDOW hWnd, HDISPLAY hDisplay );
      VOID ShowWindow ( LONG CmdShow );
      VOID UpdateWindow ();

      HWINDOW GetHandle () { return hWindow; };
      HWINDOW GetClient () { return hClient; };

      VOID SetBackGround ( LONG Background );
      VOID SetMenu ( GUIMENU *Menu );
      VOID SetToolBar ( HWINDOW ToolBar, LONG Ht );
      VOID SetClientAreaPos ( LONG x, LONG y );

      #if defined (__FORUNIX__)
        VOID MapXWinEvent ( MESSAGE *iMessage, XEvent *EventPtr );
      #endif
  }; 

VOID AddWindow ( GUIWINDOW *NewWin, HWINDOW Handle );
VOID DiscardWindow ( HWINDOW hWindow );
GUIWINDOW *FindWindow ( HWINDOW hWindow );
VOID ClearRemovedList ();
VOID InitWinHashTable ();
VOID DeInitWinHashTable ();

#endif
