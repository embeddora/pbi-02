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



#ifndef __GFXTYPES_H__
#define __GFXTYPES_H__

  #include "stdgfx.h"
  #include <stdio.h>
  #include <math.h>

  #ifdef __FORx86__


  #endif

  #ifndef __FORMOTIF__
    #include <malloc.h>
    #include <string.h>
    #include <stdlib.h>
  #endif
  
  #if defined (__32BIT__)
    #define INTERRUPT int386
    #define INTERRUPTx int386x
    #define Regs32 Regs.x
    #define Regs16 Regs.w
    #define Regs8  Regs.h
  #elif defined (__16BIT__)
    #define INTERRUPT int86
    #define INTERRUPTx int86x
    #define Regs16 Regs.x
    #define Regs8  Regs.h
  #endif
  
  
  #define INT_TIMER   0x08
  #define INT_KBD     0x09
  #define INT_VIDEO   0x10
  #define INT_VESA    0x10
  #define INT_DOS     0x21
  #define INT_DPMI    0x31
  #define INT_MOUSE   0x33
    
  #if defined (__FORDOS__)
    #define CHAR     char
    #define UCHAR    unsigned char
    #define BYTE     unsigned char
    #define INT      int
    #define UINT     unsigned int
    #define SHORT    short
    #define USHORT   unsigned short
    #define WORD     unsigned short
    #define LONG     long
    #define ULONG    unsigned long
    #define DWORD    unsigned long
    #define FLOAT    float
    #define DOUBLE   double 
    #define VOID     void
    #define HANDLE   INT
    #define HDISPLAY HANDLE   
    #define HWINDOW HANDLE
    #define HMEMDISPLAY HDISPLAY
    #define TRUE  1
    #define FALSE 0
  #elif defined (__FORWINDOWS__)
    #if defined (__FORGDK__)
      #include <windows.h>
      #include <windowsx.h>
      #include <ddraw.h>
    #else  
      #include <windows.h>
    #endif
    
    #define CHAR     char
    #define BYTE     unsigned char
    #define INT      int
    #define SHORT    short
    #define USHORT   unsigned short
    #define LONG     long
    #define HDISPLAY HDC
    #define HWINDOW HANDLE
    #define HMEMDISPLAY HDISPLAY
    #define FLOAT    float
    #define DOUBLE   double 
  #elif defined (__FOROS2__)
    #define  INCL_PM
    #include <os2.h>
    #define CHAR     char
    #define BYTE     unsigned char
    #define INT      int
    #define SHORT    short
    #define USHORT   unsigned short
    #define WORD     unsigned short
    #define LONG     long
    #define DWORD    unsigned long
    #define HANDLE   LHANDLE
    #define HDISPLAY HPS
    #define HWINDOW HANDLE
    #define HMEMDISPLAY HDISPLAY
    #define FLOAT    float
    #define DOUBLE   double 
  #elif defined (__FORUNIX__)
    #define CHAR     char
    #define UCHAR    unsigned char
    #define BYTE     unsigned char
    #define INT      int
    #define UINT     unsigned int
    #define SHORT    short
    #define USHORT   unsigned short
    #define WORD     unsigned short
    #define LONG     long
    #define ULONG    unsigned long
    #define DWORD    unsigned long
    #define FLOAT    float
    #define DOUBLE   double 
    #define VOID     void
    #define HANDLE   INT
    #define HDISPLAY Drawable
    #define HWINDOW Window
    #define HMEMDISPLAY HDISPLAY
    #define TRUE  1
    #define FALSE 0
  #endif  

  #define BOOLEAN LONG
  #define SUCCESS 1
  #define FAILURE 0 

  #define STRING CHAR*
  
  struct RGBCOLOR
    {
      BYTE Red;
      BYTE Green;
      BYTE Blue;  
    }; 

  struct RECTANGLE
    {
      LONG x1;
      LONG y1;
      LONG x2;
      LONG y2;  
    }; 
  
  template <class Type> 
  Type MaxValue ( Type T1, Type T2 )
         {
                if (T1<=T2)
                  return T2;
                return T1;
         } 

  template <class Type>
  Type MinValue ( Type T1, Type T2 )
         {
                if (T1<=T2)
                  return T1;
                return T2;
         } 

  template <class Type>
  VOID SwapValue ( Type *T1, Type *T2 )
         {
                Type Temp;
                Temp = *T1;
                *T1 = *T2;
                *T2 = Temp;
         } 

  VOID SwapWord ( WORD *ValuePtr );  
  VOID SwapDWord ( DWORD *ValuePtr );
  
#endif

