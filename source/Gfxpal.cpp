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
#include "gfxpal.h"
#include <math.h>







RGBPALETTE::RGBPALETTE () : MYOBJECT ()
  {
  } 

RGBPALETTE::~RGBPALETTE ()
  {
  } 

double RGBPALETTE::GetColorDistance ( RGBCOLOR Col1, LONG Index )
  {
    double D1,D2,D3;
    double Distance;

    D1 = (double)(Col1.Red-Entry[Index].Red);  
    D2 = (double)(Col1.Green-Entry[Index].Green);
    D3 = (double)(Col1.Blue-Entry[Index].Blue);

    Distance = D1*D1 + D2*D2 + D3*D3;
    Distance = sqrt ( Distance );
    return Distance;
  } 

LONG RGBPALETTE::GetClosestColor ( RGBCOLOR Color )
  {
    INT i;
    LONG Closest;
    double Min;
    double Distance;

    Closest = 0;
    Min = Distance = GetColorDistance ( Color, 0 );
    for (i=1;i<256;i++)
      {
        Distance = GetColorDistance ( Color, i );
        if (Distance<Min)
          {
            Min = Distance;
            Closest = i;  
          } 
      } 
    return Closest;  
  } 

VOID RGBPALETTE::SetColorScale ( RGBCOLOR Color, INT Start, INT End,
                                 float StartInt, float EndInt )
  {
    Start = MaxValue ( Start, 0 );    
    End = MinValue ( End, 255 );
    
    if (End<Start)
      return;

    if (End==Start)
      {
        Entry[Start] = Color;
      } 
        
    StartInt /= 100;  
    EndInt /= 100;

    INT NumLevels = End - Start;
    float Step = (EndInt - StartInt) / (NumLevels);

    INT R,G,B;
    INT BaseR,BaseG,BaseB;
    float Intensity;
    INT i;

    Intensity = StartInt;
    i = Start;
    R = G = B = 0;
    BaseR = Color.Red;
    BaseG = Color.Green;
    BaseB = Color.Blue;
    while ((Intensity<1.00)&&(i<=End))
      {
        R = (INT)(Intensity*BaseR);
        G = (INT)(Intensity*BaseG);
        B = (INT)(Intensity*BaseB);
        R = MaxValue ( 0, MinValue ( R, 255 ) );
        G = MaxValue ( 0, MinValue ( G, 255 ) );
        B = MaxValue ( 0, MinValue ( B, 255 ) );
          
        Entry[i].Red = (BYTE)R;  
        Entry[i].Green = (BYTE)G;  
        Entry[i].Blue = (BYTE)B;
        Intensity += Step;
        i++;  
      } 

    while (i<=End)
      {
        if (BaseR==0)
          R = (INT)((Intensity-1.00)*255);
        else  
          R = (INT)(Intensity*BaseR);
          
        if (BaseG==0)
          G = (INT)((Intensity-1.00)*255);
        else  
          G = (INT)(Intensity*BaseG);
          
        if (BaseB==0)
          B = (INT)((Intensity-1.00)*255);
        else  
          B = (INT)(Intensity*BaseB);
        
        R = MaxValue ( 0, MinValue ( R, 255 ) );
        G = MaxValue ( 0, MinValue ( G, 255 ) );
        B = MaxValue ( 0, MinValue ( B, 255 ) );
          
        Entry[i].Red = (BYTE)R;  
        Entry[i].Green = (BYTE)G;  
        Entry[i].Blue = (BYTE)B;
        Intensity += Step;
        i++;  
      } 
  } 

COLOR RGBPALETTE::GetRGB ( BYTE R, BYTE G, BYTE B )
  {
    #if defined (__FORWINDOWS__)
      return RGB ( R, G, B );
    #else
      RGBCOLOR RGBCol;
      RGBCol.Red = R; 
      RGBCol.Red = G; 
      RGBCol.Red = B; 
      return GetClosestColor ( RGBCol );  
    #endif    
  } 

COLOR RGBPALETTE::GetIndex ( LONG Index )
  {
    #if defined (__FORWINDOWS__)
      return PALETTEINDEX ( Index );
    #else
      return Index;  
    #endif    
  } 

VOID RGBPALETTE::SetWindowsPalette ()
  {
    Entry[0].Red = 0; Entry[0].Green = 0; Entry[0].Blue = 0;  
    Entry[1].Red = 128; Entry[1].Green = 0; Entry[1].Blue = 0;  
    Entry[2].Red = 0; Entry[2].Green = 128; Entry[2].Blue = 0;  
    Entry[3].Red = 128; Entry[3].Green = 128; Entry[3].Blue = 0;  
    Entry[4].Red = 0; Entry[4].Green = 0; Entry[4].Blue = 128;  
    Entry[5].Red = 128; Entry[5].Green = 0; Entry[5].Blue = 128;  
    Entry[6].Red = 0; Entry[6].Green = 128; Entry[6].Blue = 128;  
    Entry[7].Red = 192; Entry[7].Green = 192; Entry[7].Blue = 192;  
    Entry[8].Red = 192; Entry[8].Green = 220; Entry[8].Blue = 192;  
    Entry[9].Red = 166; Entry[9].Green = 202; Entry[9].Blue = 240;
    
    Entry[246].Red = 255; Entry[246].Green = 251; Entry[246].Blue = 240;  
    Entry[247].Red = 160; Entry[247].Green = 160; Entry[247].Blue = 164;  
    Entry[248].Red = 128; Entry[248].Green = 128; Entry[248].Blue = 128;  
    Entry[249].Red = 255; Entry[249].Green = 0; Entry[249].Blue = 0;
    Entry[250].Red = 0; Entry[250].Green = 255; Entry[250].Blue = 0;  
    Entry[251].Red = 255; Entry[251].Green = 255; Entry[251].Blue = 0;  
    Entry[252].Red = 0; Entry[252].Green = 0; Entry[252].Blue = 255;  
    Entry[253].Red = 255; Entry[253].Green = 0; Entry[253].Blue = 255;  
    Entry[254].Red = 0; Entry[254].Green = 255; Entry[254].Blue = 255;  
    Entry[255].Red = 255; Entry[255].Green = 255; Entry[255].Blue = 255;    
  } 

BOOLEAN RGBPALETTE::IsIdentical ( RGBPALETTE *Pal )
  {
    RGBCOLOR *Entry1;
    Entry1 = Pal->GetEntry ();

    INT i;
    for (i=0;i<256;i++)
      {
        if (Entry[i].Red!=Entry1[i].Red)
          return FALSE;  
        if (Entry[i].Green!=Entry1[i].Green)
          return FALSE;  
        if (Entry[i].Blue!=Entry1[i].Blue)
          return FALSE;  
      } 
    return TRUE;  
  } 

