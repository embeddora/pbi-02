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
#include "gfxcltbl.h"
#include "gfxfiles.h"

COLORTABLE::COLORTABLE () : MYOBJECT ()
  {
    NumColors = NumLevels = 0;
    TableType = 0;
    Table = NULL;
    ProcessIndicator = NULL;
    Pal = new RGBPALETTE ();
  } 

COLORTABLE::~COLORTABLE ()
  {
    Destroy ();
    if (Pal!=NULL)
      delete Pal;
    Pal = NULL;  
  } 

BOOLEAN COLORTABLE::Create ( LONG NumL, LONG NumCol )
  {
    LONG Size;
    Destroy ();
    Size = NumL*NumCol;
    Table = new BYTE [Size];
    if (Table==NULL)
      return FAILURE;
    memset ( Table, 0, Size);
    NumLevels = NumL;
    NumColors = NumCol;  
    return SUCCESS;
  } 

VOID COLORTABLE::Destroy ()
  {
    if (Table!=NULL)
      delete Table;
    Table = NULL;
    NumColors = NumLevels = 0;
    TableType = 0;
  } 

VOID COLORTABLE::CopyPalette ( RGBPALETTE *Pal1 )
  {
    INT i;
    if (Pal1==NULL)
      return;

    RGBCOLOR *Entry1,*Entry;
    Entry1 = Pal1->GetEntry ();
    Entry = Pal->GetEntry ();
    for (i=0;i<256;i++)
      {
        Entry[i] = Entry1[i];  
      } 
  } 

BOOLEAN COLORTABLE::CreateMatchTable ( RGBPALETTE *Pal2 )
  {
    RGBCOLOR *Entry;
    RGBPALETTE *Pal1;

    Pal1 = Pal;
    Entry = Pal2->GetEntry ();
    
    if (Create ( 1, 256 )==FAILURE)
      {
        Error.SetError ( ERR_NOMEMORY );
        return FAILURE;
      } 
    
    LONG i;
    for (i=0;i<256;i++)
      {
        Table[i] = (BYTE)Pal1->GetClosestColor ( Entry[i] );
      } 

    TableType = TABLE_MATCH;    
    return SUCCESS;  
  } 

BOOLEAN COLORTABLE::CreateShadeTable ( LONG NumLevel, LONG Highest, LONG Lowest )
  {
    INT i,j;
    
    if (NumLevel==0)
      {
        Error.SetError ( ERR_NOVALID );
        return FAILURE;
      } 
      
    if (Highest<Lowest)
      SwapValue ( &Highest, &Lowest );
      
    if (Highest<0)
      Highest = 0; 
    if (Lowest<0)
      Lowest = 0;     

    if (Create ( NumLevel, 256 )==FAILURE)
      {
        Error.SetError ( ERR_NOMEMORY );
        return FAILURE;
      } 
      
    RGBCOLOR *Entry;    

    Entry = Pal->GetEntry ();

    INT R,G,B;
    INT BaseR,BaseG,BaseB;

    double HighRatio,LowRatio,StepRatio;
    double CurIntense;
   
    HighRatio = (double)Highest/100.0;
    LowRatio = (double)Lowest/100.0;

    StepRatio = (HighRatio-LowRatio)/NumLevels;
    
    float Percentage = (float)0.0;    

    for (i=0;i<256;i++)
      {
        BaseR = (Entry[i].Red);
        BaseG = (Entry[i].Green);
        BaseB = (Entry[i].Blue);
        
        CurIntense = LowRatio;

        j = 0;
        while ((CurIntense<=1.0)&&(j<NumLevels))
          {
            RGBCOLOR Color;
            R = (INT)(CurIntense*BaseR);
            G = (INT)(CurIntense*BaseG);
            B = (INT)(CurIntense*BaseB);

            if (R>255)
              Color.Red = 255;
            else
              Color.Red = (BYTE)R;  
              
            if (G>255)
              Color.Green = 255;
            else
              Color.Green = (BYTE)G;  
              
            if (B>255)
              Color.Blue = 255;
            else
              Color.Blue = (BYTE)B;  
              
            Table[j*256+i] = (BYTE)Pal->GetClosestColor ( Color );
            j++;
            CurIntense += StepRatio;
          } 
          
        while (j<NumLevels)
          {
            RGBCOLOR Color;

            if (BaseR==0)
              R = (INT)((CurIntense-1.0)*255);
            else  
              R = (INT)(CurIntense*BaseR);

            if (BaseG==0)
              G = (INT)((CurIntense-1.0)*255);
            else  
              G = (INT)(CurIntense*BaseG);

            if (BaseB==0)
              B = (INT)((CurIntense-1.0)*255);
            else  
              B = (INT)(CurIntense*BaseB);

            if (R>255)
              Color.Red = 255;
            else
              Color.Red = (BYTE)R;  
              
            if (G>255)
              Color.Green = 255;
            else
              Color.Green = (BYTE)G;  
              
            if (B>255)
              Color.Blue = 255;
            else
              Color.Blue = (BYTE)B;  
              
            Table[j*256+i] = (BYTE)Pal->GetClosestColor ( Color );
            j++;
            CurIntense += StepRatio;
          } 
          
        if (ProcessIndicator!=NULL)
          {
            Percentage += ((float)100/256);
            if (ProcessIndicator ( Percentage ))
              return FAILURE;
          } 
      } 
    TableType = TABLE_SHADE;    
    return SUCCESS;
  } 

BOOLEAN COLORTABLE::CreateHazeTable ( RGBCOLOR TargetColor, LONG NumLevel )
  {
    INT i,j;
    
    if (NumLevel==0)
      {
        Error.SetError ( ERR_NOVALID );
        return FAILURE;
      } 
      
    if (Create ( NumLevel, 256 )==FAILURE)
      {
        Error.SetError ( ERR_NOMEMORY );
        return FAILURE;
      } 
      
    RGBCOLOR *Entry;
    float Percentage;
    
    Entry = Pal->GetEntry ();

    double R,G,B;
    double lR,lG,lB;  
    double dR,dG,dB;

    Percentage = (float)0.0;
        
    for (i=0;i<256;i++)
      {
        R = (double)(Entry[i].Red);
        G = (double)(Entry[i].Green);
        B = (double)(Entry[i].Blue);
        
        lR = (double)(TargetColor.Red);
        lG = (double)(TargetColor.Green);
        lB = (double)(TargetColor.Blue);
        
        dR = (R-lR) / NumLevels; 
        dG = (G-lG) / NumLevels; 
        dB = (B-lB) / NumLevels; 
        
        R = lR;
        G = lG;
        B = lB;
        
        for (j=0;j<NumLevels;j++)  
          {
            RGBCOLOR Color;
            R += dR;
            G += dG;
            B += dB;
            Color.Red = (BYTE)R;
            Color.Green = (BYTE)G;
            Color.Blue = (BYTE)B;
            Table[j*256+i] = (BYTE)Pal->GetClosestColor ( Color );
          } 
          
        if (ProcessIndicator!=NULL)
          {
            Percentage += ((float)100/256);
            if (ProcessIndicator ( Percentage ))
              return FAILURE;
          } 
      } 
    TableType = TABLE_HAZE;    
    return SUCCESS;
  } 

BOOLEAN COLORTABLE::CreateBlendTable ( LONG NumLevel, float StartRatio,
                                       float EndRatio )
  {
    INT i,j,k;
    
    if (NumLevel==0)
      {
        Error.SetError ( ERR_NOVALID );
        return FAILURE;
      } 
      
    if (Create ( NumLevel, 256*256 )==FAILURE)
      {
        Error.SetError ( ERR_NOMEMORY );
        return FAILURE;
      } 

    if (StartRatio>100)
      StartRatio = (float)100.0;
    else if (StartRatio<0)
      StartRatio = (float)0.0;
          
    if (EndRatio>100)
      EndRatio = (float)100.0;
    else if (EndRatio<0)
      EndRatio = (float)0.0;
      
    RGBCOLOR *Entry;
    float Percentage;
    
    Entry = Pal->GetEntry ();

    Percentage = (float)0.0;

    float SrcR,SrcG,SrcB;
    float TargetR,TargetG,TargetB;
    float StartR,StartG,StartB;
    float EndR,EndG,EndB;
    float dR,dG,dB;

    StartRatio /= 100;    
    EndRatio /= 100;
    
    for (i=0;i<256;i++)
      {
        SrcR = (float)Entry[i].Red;  
        SrcG = (float)Entry[i].Green;  
        SrcB = (float)Entry[i].Blue;
        
        for (j=0;j<256;j++)
          {
            TargetR = (float)Entry[j].Red;  
            TargetG = (float)Entry[j].Green;  
            TargetB = (float)Entry[j].Blue;
            
            StartR = SrcR*(1-StartRatio) + TargetR*StartRatio;
            StartG = SrcG*(1-StartRatio) + TargetG*StartRatio;
            StartB = SrcB*(1-StartRatio) + TargetB*StartRatio;
            
            if (StartR>255) StartR = (float)255.0;
            if (StartG>255) StartG = (float)255.0;
            if (StartB>255) StartB = (float)255.0;
              
            EndR = SrcR*(1-EndRatio) + TargetR*EndRatio;
            EndG = SrcG*(1-EndRatio) + TargetR*EndRatio;
            EndB = SrcB*(1-EndRatio) + TargetR*EndRatio;
            
            if (EndR>255) EndR = (float)255.0;
            if (EndG>255) EndG = (float)255.0;
            if (EndB>255) EndB = (float)255.0;

            dR = (EndR-StartR) / (float)NumLevels;
            dG = (EndG-StartG) / (float)NumLevels;
            dB = (EndB-StartB) / (float)NumLevels;

            RGBCOLOR Color;
            
            for (k=0;k<NumLevels;k++)  
              {
                Color.Red = (BYTE)StartR;  
                Color.Green = (BYTE)StartG;  
                Color.Blue = (BYTE)StartB;
                
                Table[k*256*256+i*256+j] = (BYTE)Pal->GetClosestColor ( Color );

                StartR += dR;
                StartG += dG;
                StartB += dB;
              } 
          
            if (ProcessIndicator!=NULL)
              {
                Percentage += ((float)100/(256*256));
                if (ProcessIndicator ( Percentage ))
                  return FAILURE;
              } 
          } 
      } 
      
    TableType = TABLE_BLEND;    
    return SUCCESS;  
  } 

BOOLEAN COLORTABLE::Save ( STRING FileName )
  {
    FILEHANDLE f;
    
    if (TableType==0)
      return FAILURE;      
    if (Table==NULL)
      return FAILURE;
      
    f = File.Open ( FileName, OPEN_WRITE | OPEN_BINARY );
    if (f==NULL)
      return FAILURE;

    CHAR Str[128];

    sprintf ( Str, "coltable" );
    if (File.Write ( f, Str, 128 )==FAILURE)
      {
        File.Close ( f );  
        return FAILURE;
      } 
                
    #if defined (__MSBFIRST__)
      SwapDWord ( (DWORD*)&TableType );
      SwapDWord ( (DWORD*)&NumColors );
      SwapDWord ( (DWORD*)&NumLevels );
    #endif

    if (File.Write ( f, &TableType, sizeof(LONG) )==FAILURE)
      {
        File.Close ( f );  
        return FAILURE;
      } 
    if (File.Write ( f, &NumColors, sizeof(LONG) )==FAILURE)
      {
        File.Close ( f );  
        return FAILURE;
      } 
    if (File.Write ( f, &NumLevels, sizeof(LONG) )==FAILURE)
      {
        File.Close ( f );  
        return FAILURE;
      } 

    #if defined (__MSBFIRST__)
      SwapDWord ( (DWORD*)&TableType );
      SwapDWord ( (DWORD*)&NumColors );
      SwapDWord ( (DWORD*)&NumLevels );
    #endif

    LONG Size;
    Size = NumColors*NumLevels;  
    if (File.Write ( f, Table, Size )==FAILURE)
      {
        File.Close ( f );  
        return FAILURE;
      } 
      
    
    if (File.Write ( f, Pal->GetEntry(), (int)256*sizeof(RGBCOLOR) )==FAILURE)
      {
        File.Close ( f );  
        return FAILURE;
      } 
      
    File.Close ( f );
    return SUCCESS;  
  } 

BOOLEAN COLORTABLE::Load ( STRING FileName )
  {
    CHAR Str[128];
    FILEHANDLE f;
    
    f = File.Open ( FileName, OPEN_READ | OPEN_BINARY );
    if (f==NULL)
      return FAILURE;

    if (File.Read ( f, &(Str[0]), 128 )==FAILURE)
      {
        File.Close ( f );  
        return FAILURE;
      } 

    if (strncmp(Str,"coltable",8)!=0)
      {
        File.Close ( f );  
        return FAILURE;
      } 

    LONG Type,NumC,NumL;      
    if (File.Read ( f, &Type, sizeof(LONG) )==FAILURE)
      {
        File.Close ( f );  
        return FAILURE;
      } 

    if (File.Read ( f, &NumC, sizeof(LONG) )==FAILURE)
      {
        File.Close ( f );  
        return FAILURE;
      } 
    NumColors = NumC;
      
    if (File.Read ( f, &NumL, sizeof(LONG) )==FAILURE)
      {
        File.Close ( f );  
        return FAILURE;
      } 
    NumLevels = NumL;   

    #if defined (__MSBFIRST__)
      SwapDWord ( (DWORD*)&TableType );
      SwapDWord ( (DWORD*)&NumColors );
      SwapDWord ( (DWORD*)&NumLevels );
    #endif

    if (Create(NumLevels,NumColors)==FAILURE)
      {
        File.Close ( f );
        return FAILURE;  
      } 
      
    TableType = Type;
    
    LONG Size;
    Size = NumColors*NumLevels;

    if (File.Read ( f, Table, Size )==FAILURE)
      {
        File.Close ( f );  
        return FAILURE;
      } 
      
    
    if (File.Read ( f, Pal->GetEntry(), (int)256*sizeof(RGBCOLOR) )==FAILURE)
      {
        File.Close ( f );  
        return FAILURE;
      } 
      
    File.Close ( f );
    return SUCCESS;  
  } 

VOID COLORTABLE::SetProcessIndicator ( COLTBLPROC Proc )
  {
    ProcessIndicator = Proc;  
  } 

  

