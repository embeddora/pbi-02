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


#ifndef __DLGS_H__
#define __DLGS_H__

#include "stdgfx.h"
#include "mpgui.h"

class FOGDLG : public GUIDIALOG 
  {
    protected :
      virtual LONG OnInitDialog ();
      virtual LONG OnCommand ( LONG Command );
     
    public :    
      FOGDLG (); 
      virtual ~FOGDLG (); 
      
      BOOLEAN IsFogOn ();
      float GetFogScale ();
  }; 

class DEPTHDLG : public GUIDIALOG 
  {
    protected :
      virtual LONG OnInitDialog ();
      virtual LONG OnCommand ( LONG Command );
     
    public :    
      DEPTHDLG (); 
      virtual ~DEPTHDLG (); 
      
      BOOLEAN IsDepthCueOn ();
      float GetDepthCueScale ();
  }; 

class BGCOLORDLG : public GUIDIALOG 
  {
    protected :
      virtual LONG OnInitDialog ();
      virtual LONG OnCommand ( LONG Command );
     
    public :    
      BGCOLORDLG (); 
      virtual ~BGCOLORDLG (); 
      
      VOID GetRGB ( RGBCOLOR *Col );
  }; 

class AMBIENTDLG : public GUIDIALOG 
  {
    protected :
      virtual LONG OnInitDialog ();
      virtual LONG OnCommand ( LONG Command );
     
    public :    
      AMBIENTDLG (); 
      virtual ~AMBIENTDLG ();

      VOID GetRGB ( RGBCOLOR *Col );
  }; 

class ABOUTDLG : public GUIDIALOG
  {
    protected :
      virtual LONG OnCommand ( LONG Command );    
  }; 

#endif
