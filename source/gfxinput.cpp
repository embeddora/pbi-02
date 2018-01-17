




#include "stdgfx.h"
#include "gfxinput.h"

INPUT Input;

INPUT::INPUT ()
  {
    KBDInstalled = FALSE;
  } 

INPUT::~INPUT ()
  {
    DeInit ();
  } 

BOOLEAN INPUT::Init ()
  {
    InstallKBDHandler ();
    return SUCCESS;
  } 

VOID INPUT::DeInit ()
  {
    if (KBDInstalled)
      UnInstallKBDHandler ();
  } 

BYTE KBDState[128];
volatile LONG WhichKey;

#if defined (__FORDOS__)
  #if defined (__FORWATCOM__)
    VOID __interrupt __far MyKBDHandler ()
  #elif defined (__FORVISUAL__)
    VOID __interrupt __far MyKBDHandler ()
  #elif defined (__FORBORLAND__)
    #ifndef __FORDOSPWPK32__
      VOID interrupt MyKBDHandler (...)
    #else
      VOID MyKBDHandler (...)
    #endif
  #endif
#else
  VOID MyKBDHandler ()
#endif
  {
    #if defined (__FORDOS__)
      _disable ();
      
      #ifndef __FORDOSPWPK32__
         WhichKey = inp ( 0x60 );
      #else
        WhichKey = 0;
      #endif
      if (WhichKey>127)
        {
          WhichKey &= 127;
          KBDState[WhichKey] = 0;
        } 
      else
        {
          KBDState[WhichKey] = 1;
        } 

      INT Temp1,Temp2;
      #ifndef __FORDOSPWPK32__
        Temp1 = inp ( 0x61 );
        Temp2 = Temp1;
        Temp1 |= 0x80;
        outp ( 0x80, Temp1 );
        Temp1 = Temp2;
        outp ( 0x80, Temp1 );
        outp ( 0x20, 0x20 );
      #else
        Temp1 = 0;
        Temp2 = 0;
        if (Temp1&Temp2)
          {}
      #endif
      
      _enable ();
   #endif
  } 

#if defined (__FORDOS__)
  #if defined  (__FORWATCOM__)
    VOID (__interrupt __far *OldKBDHandler)(); 
  #elif defined (__FORBORLAND__)
    #ifndef __FORDOSPWPK32__
      VOID interrupt (*OldKBDHandler)(...); 
    #else
      VOID (*OldKBDHandler)(...); 
    #endif
  #endif
#endif

VOID INPUT::InstallKBDHandler ()
  {
    INT i;
    for (i=0;i<128;i++)
      KBDState[i] = 0;

    #if defined (__FORWINDOWS__)
    #elif defined (__FOROS2__)
    #elif defined (__FORDOS__)
      #ifdef __FORDOSPWPK32__
      #else
        
        OldKBDHandler = _dos_getvect ( INT_KBD );
        
        _dos_setvect ( INT_KBD, MyKBDHandler );
      #endif
    #endif

    KBDInstalled = TRUE;
  } 

VOID INPUT::UnInstallKBDHandler ()
  {
    #if defined (__FORWINDOWS__)
    #elif defined (__FOROS2__)
    #elif defined (__FORDOS__)
      #ifdef __FORDOSPWPK32__
      #else
        _dos_setvect ( INT_KBD, OldKBDHandler );
      #endif
    #endif
    KBDInstalled = FALSE;
  } 

BOOLEAN INPUT::IsKeyDown ( INT WhichKey )
  {
    #if defined (__FORWINDOWS__)
      LONG Result;
      #if defined (__FORWIN386__)
        Result = GetKeyState ( (short)WhichKey );
      #else
        Result = GetKeyState ( WhichKey );
      #endif
      if (Result<0)  
        return TRUE;
      return FALSE;  
    #elif defined (__FOROS2__)
      if (WhichKey)
        {}
      return FALSE;
    #elif defined (__FORDOS__)
      if (KBDState[WhichKey])
        return TRUE;
      return FALSE;
    #endif
  } 

VOID INPUT::WaitForKey ( INT WhichKey )
  {
    while (Input.IsKeyDown(WhichKey)==FALSE)
      {}  
  } 
  
