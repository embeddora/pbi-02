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
#include "dlgs.h"





#define FOG_ONOFF  101
#define FOG_SCALE  102
#define FOG_OK     IDOK
#define FOG_CANCEL IDCANCEL
#define FOG_HELP   998

static BOOLEAN FogOnOff = FALSE;
static float FogScale = (float)100; 

FOGDLG::FOGDLG () : GUIDIALOG ()
  {
  } 
  
FOGDLG::~FOGDLG ()
  {
  } 
  
LONG FOGDLG::OnInitDialog ()
  {
    SetButtonState ( FOG_SCALE, FogOnOff );
    
    CHAR Str[128];
    sprintf ( Str, "%4.2f", FogScale );
    SetItemText ( FOG_SCALE, Str );
    return 0;
  } 

LONG FOGDLG::OnCommand ( LONG Command )
  {
    CHAR Str[128];
     
    switch (Command)
      {        
        case FOG_OK :
          FogOnOff = GetButtonState ( FOG_ONOFF );
          GetItemText ( FOG_SCALE, Str );
          FogScale = (float)atof ( Str );
          ReturnValue = TRUE;
          CloseWindow ();
          break;
        case FOG_CANCEL :
          ReturnValue = FALSE;
          CloseWindow ();
          break;  
      } 
    return 0; 
  } 
  
BOOLEAN FOGDLG::IsFogOn ()
  {
    return FogOnOff;
  } 

float FOGDLG::GetFogScale ()
  {
    return FogScale;
  } 
  






#define DEPTH_ONOFF  101
#define DEPTH_SCALE  102
#define DEPTH_OK     IDOK
#define DEPTH_CANCEL IDCANCEL
#define DEPTH_HELP   998

static BOOLEAN DepthCueOnOff = FALSE;
static float DepthCueScale = (float)100; 

DEPTHDLG::DEPTHDLG () : GUIDIALOG ()
  {
  } 
  
DEPTHDLG::~DEPTHDLG ()
  {
  } 
  
LONG DEPTHDLG::OnInitDialog ()
  {
    SetButtonState ( FOG_SCALE, FogOnOff );
    
    CHAR Str[128];
    sprintf ( Str, "%4.2f", DepthCueScale );
    SetItemText ( DEPTH_SCALE, Str );
    return 0;
  } 

LONG DEPTHDLG::OnCommand ( LONG Command )
  {
    CHAR Str[128];
     
    switch (Command)
      {        
        case DEPTH_OK :
          DepthCueOnOff = GetButtonState ( DEPTH_ONOFF );
          GetItemText ( DEPTH_SCALE, Str );
          DepthCueScale = (float)atof ( Str );
          ReturnValue = TRUE;
          CloseWindow ();
          break;
        case DEPTH_CANCEL :
          ReturnValue = FALSE;
          CloseWindow ();
          break;  
      } 
    return 0; 
  } 
  
BOOLEAN DEPTHDLG::IsDepthCueOn ()
  {
    return DepthCueOnOff;
  } 

float DEPTHDLG::GetDepthCueScale ()
  {
    return DepthCueScale;
  } 
  







#define BG_RED    101
#define BG_GREEN  102
#define BG_BLUE   103
#define BG_OK     IDOK
#define BG_CANCEL IDCANCEL
#define BG_HELP   998

INT BGRed=0;
INT BGGreen=0;
INT BGBlue=0;

BGCOLORDLG::BGCOLORDLG () : GUIDIALOG ()
  {
  } 

BGCOLORDLG::~BGCOLORDLG ()
  {
  } 

LONG BGCOLORDLG::OnInitDialog ()
  {
    return 0;  
  } 

LONG BGCOLORDLG::OnCommand ( LONG Command )
  {
    switch (Command)
      {
        case BG_OK :
          BGRed = GetItemNumber ( BG_RED );
          BGGreen = GetItemNumber ( BG_GREEN );
          BGBlue = GetItemNumber ( BG_BLUE );
          ReturnValue = TRUE;
          CloseWindow ();
          break;  
        case BG_CANCEL :
          ReturnValue = FALSE;
          CloseWindow ();
          break;  
      } 
    return 0;  
  } 

VOID BGCOLORDLG::GetRGB ( RGBCOLOR *Col )
  {
    Col->Red = (BYTE)BGRed;  
    Col->Green = (BYTE)BGGreen;  
    Col->Blue = (BYTE)BGBlue;  
  } 








#define AMBIENT_RED    101
#define AMBIENT_GREEN  102
#define AMBIENT_BLUE   103
#define AMBIENT_OK     IDOK
#define AMBIENT_CANCEL IDCANCEL
#define AMBIENT_HELP   998

INT AMBIENTRed=0;
INT AMBIENTGreen=0;
INT AMBIENTBlue=0;

AMBIENTDLG::AMBIENTDLG () : GUIDIALOG ()
  {
  } 

AMBIENTDLG::~AMBIENTDLG ()
  {
  } 

LONG AMBIENTDLG::OnInitDialog ()
  {
    return 0;  
  } 

LONG AMBIENTDLG::OnCommand ( LONG Command )
  {
    switch (Command)
      {
        case AMBIENT_OK :
          AMBIENTRed = GetItemNumber ( AMBIENT_RED );
          AMBIENTGreen = GetItemNumber ( AMBIENT_GREEN );
          AMBIENTBlue = GetItemNumber ( AMBIENT_BLUE );
          ReturnValue = TRUE;
          CloseWindow ();
          break;  
        case AMBIENT_CANCEL :
          ReturnValue = FALSE;
          CloseWindow ();
          break;  
      } 
    return 0;  
  } 

VOID AMBIENTDLG::GetRGB ( RGBCOLOR *Col )
  {
    Col->Red = (BYTE)AMBIENTRed;  
    Col->Green = (BYTE)AMBIENTGreen;  
    Col->Blue = (BYTE)AMBIENTBlue;  
  } 







#define ABOUT_OK IDOK

LONG ABOUTDLG::OnCommand ( LONG Command ) 
  {
    switch (Command)
      {
        case ABOUT_OK :
          CloseWindow ();
          break;  
      } 
    return 0;  
  } 

