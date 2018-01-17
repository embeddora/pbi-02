


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
