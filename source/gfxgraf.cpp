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
#include "gfxgraf.h"
#include "lgfxdib.h"
#include <math.h>

#include "stdlib.h"

POLYGON::POLYGON ()
  {
    NumPoints = 0;
    Points = NULL;  
  }; 

POLYGON::~POLYGON ()
  {
    DestroyPoints ();  
  }; 

VOID POLYGON::CreatePoints ( LONG Num )
  {
    DestroyPoints ();
    Points = new XYPOINT [Num];
    NumPoints = Num;  
  }; 

VOID POLYGON::DestroyPoints ()
  {
    if (Points!=NULL)
      delete Points;
    Points = NULL;
    NumPoints = 0;    
  }; 

EDGETABLE::EDGETABLE ()
  {
  } 

EDGETABLE::~EDGETABLE ()
  {
  } 







POLYGON *Poly1 = NULL;
POLYGON *Poly2 = NULL;
XYPOINT *Left = NULL;
XYPOINT *Right = NULL;

GRAFIX Grafix;

GRAFIX::GRAFIX () : DCIClass ()
  {
    Init ();
    MagH = MagV = DivH = DivV = 1;
    Poly1->CreatePoints ( 50 );
    Poly2->CreatePoints ( 50 );
  } 

GRAFIX::~GRAFIX ()
  {
    DeInit ();
  } 

BOOLEAN GRAFIX::Init ()
  {
    DisplayDriver = NULL;
    Poly1 = new POLYGON ();
    Poly2 = new POLYGON ();
    Left = new XYPOINT [768];
    Right = new XYPOINT [768];

    return TRUE;
  } 

VOID GRAFIX::DeInit ()
  {
    if (DisplayDriver!=NULL)
      delete DisplayDriver;
    DisplayDriver = NULL;

    if (Poly1!=NULL)
      delete Poly1;
    Poly1 = NULL;

    if (Poly2!=NULL)
      delete Poly2;
    Poly2 = NULL;

    if (Left!=NULL)
      delete Left;
    Left = NULL;

    if (Right!=NULL)
      delete Right;
    Right = NULL;
  } 

BOOLEAN GRAFIX::SetDisplay ( DISPLAYDATA *Data )
  {
    BOOLEAN Result;

    if (Data==NULL)
      {

        return FAILURE;  
      } 

    if (DisplayDriver!=NULL)
      {

        delete DisplayDriver;
        DisplayDriver = NULL;  

      } 
      
    #if defined (__FORWINDOWS__)
f off
    #elif defined (__FORUNIX__)


      DisplayDriver = new XWINDRIVER ();


      if (DisplayDriver==NULL)
        {

          Error.SetError ( ERR_NOMEMORY );

          return FAILURE;
        } 
printf("   GRAFIX::SetDisplay   5-e\n");
      Result = DisplayDriver->SetUp ( Data );
printf("   GRAFIX::SetDisplay   5-f\n");
      return Result;
    #endif    
  } 

VOID GRAFIX::ResetDisplay ()
  {
    if (DisplayDriver==NULL)
      return;

    DisplayDriver->Reset ();
  } 

VOID GRAFIX::SetScaleFactor ( LONG MH, LONG DH, LONG MV, LONG DV )
  {
    if (MH>=0)  
      MagH = MH;
    else
      MagH = 1;      
    if (DH>=0)  
      DivH = DH;
    else
      DivH = 1;

    if (MV>=0)  
      MagV = MV;
    else
      MagV = 1;      
    if (DV>=0)  
      DivV = DV;
    else
      DivV = 1;  
      
    if (DisplayDriver!=NULL)
      DisplayDriver->SetScaleFactor ( MagH, DivH, MagV, DivV );
  } 
  
VOID GRAFIX::ClearDisplay ( HDISPLAY hDisplay, LONG Color )
  {
    if (DisplayDriver==NULL)
      return;

    DisplayDriver->Clear ( hDisplay, Color );
  } 

VOID GRAFIX::WaitForRetrace ( LONG Count )
  {      
    #if defined (__FORDOS__)
      WaitForRetraceMany ( Count );
    #else
      if (Count)
        {}
    #endif  
  } 

VOID GRAFIX::WaitForRetrace ()
  {
    #if defined (__FORDOS__)
      WaitForRetraceOnce ();
    #endif  
  } 

VOID GRAFIX::WaitForRetraceEnd ()
  {
    #if defined (__FORDOS__)
      WaitForRetraceTerminate ();
    #endif  
  } 

VOID GRAFIX::CopyImage ( IMAGE *Src, LONG Sx, LONG Sy, LONG Wd, LONG Ht,
                         IMAGE *Dest, LONG Cx, LONG Cy )
  {
printf("   GRAFIX::CopyImage 0 \n");
    if ((Src==NULL)||(Dest==NULL))
      return;

printf("   GRAFIX::CopyImage 1 \n");
    if ((Wd==0)||(Ht==0))
      return;
    
printf("   GRAFIX::CopyImage 2 \n");
    RECTANGLE ViewPort;
    ViewPort.x1 = 0;
    ViewPort.y1 = 0;
    ViewPort.x2 = Src->GetWidth()-1;
    ViewPort.y2 = Src->GetHeight()-1;

printf("   GRAFIX::CopyImage 3 \n");
    if (ClipRect ( &ViewPort, &Sx, &Sy, &Wd, &Ht )==FALSE)
      return;

    ViewPort = Dest->GetViewPort ();
    if (ClipDest ( &ViewPort, &Sx, &Sy, &Wd, &Ht, &Cx, &Cy )==FALSE)
      return;

    INT i;

    BYTE *SrcBuffer;
    BYTE *DestBuffer;

    SrcBuffer = Src->SetOffset ( Sx, Sy );
    DestBuffer = Dest->SetOffset ( Cx, Cy );

    if (Src->Transparent<0)
      {
printf("   GRAFIX::CopyImage no \n");
        for (i=0;i<Ht;i++)
          {
            memcpy ( DestBuffer, SrcBuffer, Wd );
            SrcBuffer = Src->GetNextRow ( ROW_DOWN );
            DestBuffer = Dest->GetNextRow ( ROW_DOWN );
          } 
      } 
    else
      {
printf("   GRAFIX::CopyImage YES \n");
        for (i=0;i<Ht;i++)
          {
            TransBlt256To256 ( DestBuffer, SrcBuffer, Wd, (BYTE)Src->Transparent );
            SrcBuffer = Src->GetNextRow ( ROW_DOWN );
            DestBuffer = Dest->GetNextRow ( ROW_DOWN );
          } 
      } 
  } 

VOID GRAFIX::ScaleImage ( IMAGE *Src, LONG Sx, LONG Sy, LONG Wd, LONG Ht,
                          IMAGE *Dest, LONG Cx, LONG Cy )
  {
    if ((Src==NULL)||(Dest==NULL))
      return;

    if ((Wd==0)||(Ht==0))
      return;
      
    RECTANGLE ViewPort;
    LONG DWd,DHt;

    ViewPort.x1 = 0;
    ViewPort.y1 = 0;
    ViewPort.x2 = Src->GetWidth()-1;
    ViewPort.y2 = Src->GetHeight()-1;

    if (ClipRect ( &ViewPort, &Sx, &Sy, &Wd, &Ht )==FALSE)
      return;

    DWd = (MagH*Wd)/DivH;
    DHt = (MagV*Ht)/DivV;
    
    ViewPort = Dest->GetViewPort ();
    if (ClipDest ( &ViewPort, &Sx, &Sy, &DWd, &DHt, &Cx, &Cy )==FALSE)
      return;

    INT i;

    BYTE *SrcBuffer;
    BYTE *DestBuffer;

    SrcBuffer = Src->SetOffset ( Sx, Sy );
    DestBuffer = Dest->SetOffset ( Cx, Cy );

    WORD Error = 0;
    DWORD AddError = (((LONG)DivH<<16)/MagH);

    if (Src->Transparent<0)
      {
        for (i=0;i<DHt;i++)
          {
            ScaleBlt256To256 ( DestBuffer, SrcBuffer, DWd, Error, AddError );
            SrcBuffer = Src->SetOffset ( Sx, Sy+(i*DivV)/MagV );
            DestBuffer = Dest->GetNextRow ( ROW_DOWN );
          } 
      } 
    else
      {
        for (i=0;i<DHt;i++)
          {
            TransBlt256To256 ( DestBuffer, SrcBuffer, Wd, (BYTE)Src->Transparent );
            SrcBuffer = Src->GetNextRow ( ROW_DOWN );
            DestBuffer = Dest->GetNextRow ( ROW_DOWN );
          } 
      } 
  } 
                            
VOID GRAFIX::AdjustImageSize ( IMAGE *Image, LONG Wd, LONG Ht )
  {
    IMAGE *TempImage;

    if ((Image->GetWidth()==Wd)&&(Image->GetHeight()==Ht))
      return;
      
    SetScaleFactor ( 1, 1, 1, 1 );
    TempImage = new IMAGE ( Image->GetDirection () );
    if (TempImage->Create ( Image->GetFormat (), Image->GetWidth(), Image->GetHeight() )==FAILURE)
      {
        delete TempImage;  
        return;
      } 

    CopyImage ( Image, 0, 0, Image->GetWidth(), Image->GetHeight(),
                TempImage, 0, 0 );

    if (Image->Create ( TempImage->GetFormat(), Wd, Ht )==FAILURE)
      {
        delete TempImage;
        return;  
      } 
      
    SetScaleFactor ( Wd, TempImage->GetWidth(), Ht, TempImage->GetHeight() );
    ScaleImage ( TempImage, 0, 0, TempImage->GetWidth(), TempImage->GetHeight(),
                 Image, 0, 0 ); 
    SetScaleFactor ( 1, 1, 1, 1 );
    delete TempImage;
  } 

VOID GRAFIX::DisplayImage ( HDISPLAY hDisplay, IMAGE *Image, LONG Sx, LONG Sy,
                            LONG Wd, LONG Ht, LONG Cx, LONG Cy )
  {
    if (DisplayDriver==NULL)
      return;

    if ((Wd==0)||(Ht==0))
      return;
      
    RECTANGLE ViewPort;
    LONG DWd,DHt;

    ViewPort.x1 = 0;
    ViewPort.y1 = 0;
    ViewPort.x2 = Image->GetWidth()-1;
    ViewPort.y2 = Image->GetHeight()-1;
    if (ClipRect ( &ViewPort, &Sx, &Sy, &Wd, &Ht )==FALSE)
      return;

    DWd = (MagH*Wd)/DivH;
    DHt = (MagV*Ht)/DivV;

    ViewPort.x1 = 0;
    ViewPort.y1 = 0;
    ViewPort.x2 = DisplayDriver->GetWidth ( hDisplay );
    ViewPort.y2 = DisplayDriver->GetHeight ( hDisplay );

    if (ClipDest ( &ViewPort, &Sx, &Sy, &DWd, &DHt, &Cx, &Cy )==FALSE)
      return;

    DisplayDriver->DisplayImage ( Image, Sx, Sy, Wd, Ht, hDisplay, Cx, Cy, DWd, DHt );
  } 

INT GRAFIX::GetImageFileType ( FILEHANDLE f )
  {
    CHAR Ch;
    CHAR ID[8];

    
    File.Seek ( f, 0, FROM_BEGIN );
    Ch = (CHAR)File.GetCh ( f );
    if (Ch==0x0A)
      {
        return PCXFILE;
      } 

    
    File.Seek ( f, 0, FROM_BEGIN );
    File.Read ( f, ID, 2 );
    if (strncmp(ID,"BM",2)==0)
      {
        return BMPFILE;
      } 

    
    File.Seek ( f, 0, FROM_BEGIN );
    File.Read ( f, ID, 3 );
    if (strncmp(ID,"GIF",3)==0)
      {
        File.Read ( f, ID, 3 );
        if ((strncmp(ID,"87a",3)==0)||(strncmp(ID,"89a",3)==0))
          {
            return GIFFILE;
          } 
      } 

    return UNKNOWN;
  } 

BOOLEAN GRAFIX::ClipRect ( RECTANGLE *ViewPort, LONG *Sx, LONG *Sy, LONG *Wd, LONG *Ht )
  {
    LONG StartX,StartY,EndX,EndY;

    StartX = *Sx;
    StartY = *Sy;
    EndX = *Sx+*Wd-1;
    EndY = *Sy+*Ht-1;

    if (StartX<ViewPort->x1)
      StartX = ViewPort->x1;
    else if (StartX>ViewPort->x2)
      return FAILURE;

    if (StartY<ViewPort->y1)
      StartY = ViewPort->y1;
    else if (StartY>ViewPort->y2)
      return FAILURE;

    if (EndX<ViewPort->x1)
      return FAILURE;
    else if (EndX>ViewPort->x2)
      EndX = ViewPort->x2;

    if (EndY<ViewPort->y1)
      return FAILURE;
    else if (EndY>ViewPort->y2)
      EndY = ViewPort->y2;

    *Wd = EndX-StartX+1;
    *Ht = EndY-StartY+1;
    *Sx = StartX;
    *Sy = StartY;

    return SUCCESS;
  } 

BOOLEAN GRAFIX::ClipDest ( RECTANGLE *ViewPort, LONG *Sx, LONG *Sy, LONG *Wd, LONG *Ht,
                           LONG *Cx, LONG *Cy )
  {
    LONG StartX,StartY,EndX,EndY;

    StartX = *Cx;
    StartY = *Cy;
    EndX = *Cx+*Wd-1;
    EndY = *Cy+*Ht-1;

    if (StartX<ViewPort->x1)
      StartX = ViewPort->x1;
    else if (StartX>ViewPort->x2)
      return FAILURE;

    if (StartY<ViewPort->y1)
      StartY = ViewPort->y1;
    else if (StartY>ViewPort->y2)
      return FAILURE;

    if (EndX<ViewPort->x1)
      return FAILURE;
    else if (EndX>ViewPort->x2)
      EndX = ViewPort->x2;

    if (EndY<ViewPort->y1)
      return FAILURE;
    else if (EndY>ViewPort->y2)
      EndY = ViewPort->y2;

    *Wd = EndX-StartX+1;
    *Ht = EndY-StartY+1;
    *Sx += StartX-*Cx;
    *Sy += StartY-*Cy;
    *Cx = StartX;
    *Cy = StartY;

    return SUCCESS;
  } 

VOID GRAFIX::FindIntersect ( XYPOINT *In, XYPOINT *Out, XYPOINT *I,
                             RECTANGLE ViewPort, LONG Code )
  {
    double Ratio;
    switch (Code)
      {
        case 0 : 
          Ratio = (double)(ViewPort.y1-Out->y)/(In->y-Out->y);
          I->x = (LONG)((In->x-Out->x)*Ratio + Out->x);
          I->y = ViewPort.y1;
          break;
        case 1 : 
          Ratio = (double)(ViewPort.x1-Out->x)/(In->x-Out->x);
          I->x = ViewPort.x1;
          I->y = (LONG)((In->y-Out->y)*Ratio + Out->y);
          break;
        case 2 : 
          Ratio = (double)(ViewPort.y2-Out->y)/(In->y-Out->y);
          I->x = (LONG)((In->x-Out->x)*Ratio + Out->x);
          I->y = ViewPort.y2;
          break;
        case 3 : 
          Ratio = (double)(ViewPort.x2-Out->x)/(In->x-Out->x);
          I->x = ViewPort.x2;
          I->y = (LONG)((In->y-Out->y)*Ratio + Out->y);
          break;
      } 
    I->u = (LONG)((In->u-Out->u)*Ratio + Out->u);  
    I->v = (LONG)((In->v-Out->v)*Ratio + Out->v);  
  } 
                               
VOID GRAFIX::ClipPolygon ( POLYGON *SrcPoly, POLYGON *ClipPoly,
                           RECTANGLE ViewPort )
  {
    LONG NumPoints;
    LONG Count;
    LONG i;
    XYPOINT P1,P2,I;    
    
    
    NumPoints = SrcPoly->NumPoints;
    Count = 0;
    
    P1 = SrcPoly->Points[NumPoints-1];
    for (i=0;i<NumPoints;i++)
      {
        P2 = SrcPoly->Points[i];
        if (P1.y>=ViewPort.y1)
          {
            if (P2.y>=ViewPort.y1)
              {
                Poly1->Points[Count++] = P2;  
              } 
            else
              {
                FindIntersect ( &P1, &P2, &I, ViewPort, 0 );
                Poly1->Points[Count++] = I;  
              } 
          } 
        else
          {
            if (P2.y>=ViewPort.y1)
              {
                FindIntersect ( &P2, &P1, &I, ViewPort, 0 );
                Poly1->Points[Count++] = I;  
                Poly1->Points[Count++] = P2;  
              } 
          } 
        P1 = P2;  
      } 
    
    
    NumPoints = Count;
    Count = 0;
    
    P1 = Poly1->Points[NumPoints-1];
    for (i=0;i<NumPoints;i++)
      {
        P2 = Poly1->Points[i];
        if (P1.x>=ViewPort.x1)
          {
            if (P2.x>=ViewPort.x1)
              {
                Poly2->Points[Count++] = P2;  
              } 
            else
              {
                FindIntersect ( &P1, &P2, &I, ViewPort, 1 );
                Poly2->Points[Count++] = I;  
              } 
          } 
        else
          {
            if (P2.x>=ViewPort.x1)
              {
                FindIntersect ( &P2, &P1, &I, ViewPort, 1 );
                Poly2->Points[Count++] = I;  
                Poly2->Points[Count++] = P2;  
              } 
          } 
        P1 = P2;  
      } 
      
    
    NumPoints = Count;
    Count = 0;
    
    P1 = Poly2->Points[NumPoints-1];
    for (i=0;i<NumPoints;i++)
      {
        P2 = Poly2->Points[i];
        if (P1.y<=ViewPort.y2)
          {
            if (P2.y<=ViewPort.y2)
              {
                Poly1->Points[Count++] = P2;  
              } 
            else
              {
                FindIntersect ( &P1, &P2, &I, ViewPort, 2 );
                Poly1->Points[Count++] = I;  
              } 
          } 
        else
          {
            if (P2.y<=ViewPort.y2)
              {
                FindIntersect ( &P2, &P1, &I, ViewPort, 2 );
                Poly1->Points[Count++] = I;  
                Poly1->Points[Count++] = P2;  
              } 
          } 
        P1 = P2;  
      } 
      
    
    NumPoints = Count;
    Count = 0;
    
    P1 = Poly1->Points[NumPoints-1];
    for (i=0;i<NumPoints;i++)
      {
        P2 = Poly1->Points[i];
        if (P1.x<=ViewPort.x2)
          {
            if (P2.x<=ViewPort.x2)
              {
                Poly2->Points[Count++] = P2;  
              } 
            else
              {
                FindIntersect ( &P1, &P2, &I, ViewPort, 3 );
                Poly2->Points[Count++] = I;  
              } 
          } 
        else
          {
            if (P2.x<=ViewPort.x2)
              {
                FindIntersect ( &P2, &P1, &I, ViewPort, 3 );
                Poly2->Points[Count++] = I;  
                Poly2->Points[Count++] = P2;  
              } 
          } 
        P1 = P2;  
      } 

    NumPoints = Count;
    
    ClipPoly->CreatePoints ( NumPoints );
    for (i=0;i<NumPoints;i++)
      {
        ClipPoly->Points[i] = Poly2->Points[i];  
      } 
  } 

VOID GRAFIX::DrawPolygon ( IMAGE *Dest, POLYGON *Poly )
  {
    INT i;
    for (i=0;i<Poly->NumPoints-2;i++)
      {
        DrawLine ( Dest, Poly->Points[i].x, Poly->Points[i].y,
                   Poly->Points[i+1].x, Poly->Points[i+1].y );
      } 
      
    DrawLine ( Dest, Poly->Points[i].x, Poly->Points[i].y,
               Poly->Points[0].x, Poly->Points[0].y );
  } 

VOID GRAFIX::CreateEdgeTable ( POLYGON *Poly, EDGETABLE *EdgeTable )
  {
    INT i;
    LONG Hi,HiP,Lo,LoP;
    LONG NumPoints;
    XYPOINT *Points;
    
    Points = Poly->Points;

    HiP = LoP = 0;
    Hi = Lo = Points[0].y;
    
    NumPoints = Poly->NumPoints;
    
    for (i=1;i<NumPoints;i++)
      {
        if (Hi>Points[i].y)
          {
            Hi = Points[i].y;
            HiP = i;
          } 
        else if (Lo<Points[i].y)
          {
            Lo = Points[i].y;
            LoP = i;
          } 
      } 

    LONG CurP,NextP;
    XYPOINT *Edge1,*Edge2;

    EdgeTable->Top = Hi;
    EdgeTable->Height = Lo-Hi+1;
    Edge1 = Left;
    Edge2 = Right;

    LONG Sx,Ex;
    LONG Sy,Ey;
    LONG Su,Eu;
    LONG Sv,Ev;
    LONG Row;
    double CurX;
    double CurU,CurV;
    double Slope;
    double Du,Dv;
    
    
    CurP = HiP;
    NextP = CurP-1;
    if (NextP<0)
      NextP = NumPoints-1;

    Row = 0;  
    while (Row<EdgeTable->Height)
      {
        Sx = Points[CurP].x;  
        Sy = Points[CurP].y;  
        Ex = Points[NextP].x;  
        Ey = Points[NextP].y;
        Su = Points[CurP].u;  
        Sv = Points[CurP].v;  
        Eu = Points[NextP].u;  
        Ev = Points[NextP].v;
        
        if ((Sx==Ex)&&(Sy==Ey))
          {
            CurP = NextP;
            if (CurP==LoP)
              break;
            NextP = CurP-1;
            if (NextP<0)
              NextP = NumPoints-1;          
            continue;
          } 

        double Ratio;
        Ratio = (double)1/(Ey-Sy+1);
        Slope = (double)(Ex-Sx)*Ratio;
        Du = (double)(Eu-Su)*Ratio;
        Dv = (double)(Ev-Sv)*Ratio;
        CurX = Sx;
        CurU = Su;
        CurV = Sv;
        for (i=0;i<(Ey-Sy+1);i++)
          {
            CurX += Slope;
            CurU += Du;
            CurV += Dv;
            Edge1[Row].x = (LONG)CurX;
            Edge1[Row].u = (LONG)CurU;
            Edge1[Row++].v = (LONG)CurV;
          } 
        CurP = NextP;
        NextP = CurP-1;
        if (NextP<0)
          NextP = NumPoints-1;          
      } 

    if (Row<EdgeTable->Height)
      EdgeTable->Height = Row;
    
    
    CurP = HiP;
    NextP = CurP+1;
    if (NextP>=NumPoints)
      NextP = 0;

    Row = 0;  
    while (Row<EdgeTable->Height)
      {
        Sx = Points[CurP].x;  
        Sy = Points[CurP].y;  
        Ex = Points[NextP].x;  
        Ey = Points[NextP].y;
        Su = Points[CurP].u;  
        Sv = Points[CurP].v;  
        Eu = Points[NextP].u;  
        Ev = Points[NextP].v;
        
        if ((Sx==Ex)&&(Sy==Ey))
          {
            CurP = NextP;
            if (CurP==LoP)
              break;
            NextP = CurP+1;
            if (NextP>=NumPoints)
              NextP = 0;          
            continue;
          } 

        double Ratio;
        Ratio = (double)1/(Ey-Sy+1);
        Slope = (double)(Ex-Sx)*Ratio;
        Du = (double)(Eu-Su)*Ratio;
        Dv = (double)(Ev-Sv)*Ratio;
        CurX = Sx;
        CurU = Su;
        CurV = Sv;
        
        for (i=0;i<(Ey-Sy+1);i++)
          {
            CurX += Slope;
            CurU += Du;
            CurV += Dv;
            Edge2[Row].x = (LONG)CurX;
            Edge2[Row].u = (LONG)CurU;
            Edge2[Row++].v = (LONG)CurV;
          } 
        CurP = NextP;
        NextP = CurP+1;
        if (NextP>=NumPoints)
          NextP = 0;          
      } 

    if (Row<EdgeTable->Height)
      EdgeTable->Height = Row;
    
    
    for (i=0;i<EdgeTable->Height;i++)
      {
        if (Edge1[i].x>Edge2[i].x)
          {
            SwapValue ( &Edge1, &Edge2 );
            break;  
          } 
        else if (Edge1[i].x<Edge2[i].x)
          break;
      } 

    EdgeTable->Left = Edge1;  
    EdgeTable->Right = Edge2;
  } 
  
VOID GRAFIX::FillPolygon ( IMAGE *Dest, POLYGON *Poly )
  {
    RECTANGLE ViewPort;
    POLYGON *ClipPoly;
    EDGETABLE *EdgeTable;

    EdgeTable = new EDGETABLE ();    
    ClipPoly = new POLYGON ();
    ViewPort = Dest->GetViewPort ();
    ClipPolygon ( Poly, ClipPoly, ViewPort );
    
    if (ClipPoly->NumPoints>2)
      {
        CreateEdgeTable ( ClipPoly, EdgeTable );
        LONG i;
        LONG x1,x2;
        LONG Wd;
        BYTE *DestOffset;
        DestOffset = Dest->SetOffset ( 0, EdgeTable->Top );
        for (i=0;i<EdgeTable->Height;i++)
          {
            x1 = EdgeTable->Left[i].x;  
            x2 = EdgeTable->Right[i].x;
            Wd = x2-x1+1;
            memset ( DestOffset+x1, BGColor, Wd );
            DestOffset = Dest->GetNextRow ( ROW_DOWN );
          } 
      } 
      
    delete EdgeTable;
    delete ClipPoly;
  } 

VOID GRAFIX::RotateImage ( IMAGE *Src, LONG Sx, LONG Sy, LONG Wd, LONG Ht,
                           LONG CenterX, LONG CenterY, float Angle,
                           IMAGE *Dest, LONG Cx, LONG Cy )
  {
    if ((Src==NULL)||(Dest==NULL))
      return;

    if ((Wd==0)||(Ht==0))
      return;
      
    RECTANGLE ViewPort;

    ViewPort.x1 = 0;
    ViewPort.y1 = 0;
    ViewPort.x2 = Src->GetWidth()-1;
    ViewPort.y2 = Src->GetHeight()-1;

    if (ClipRect ( &ViewPort, &Sx, &Sy, &Wd, &Ht )==FALSE)
      return;

    POLYGON *Poly,*ClipPoly;
    
    Poly = new POLYGON ();
    ClipPoly = new POLYGON ();

    double Sz,Cz;
    Angle = (float)((Angle*6.281) / 360);
    Sz = sin ( Angle );
    Cz = cos ( Angle );

    double ScaleX,ScaleY;

    ScaleX = (double)MagH/DivH;
    ScaleY = (double)MagV/DivV;
        
    Poly->CreatePoints ( 4 );
    Poly->Points[0].x = Sx-CenterX; Poly->Points[0].y = Sy-CenterY;
    Poly->Points[1].x = Sx-CenterX; Poly->Points[1].y = Sy-CenterY+Ht;
    Poly->Points[2].x = Sx-CenterX+Wd; Poly->Points[2].y = Sy-CenterY+Ht;
    Poly->Points[3].x = Sx-CenterX+Wd; Poly->Points[3].y = Sy-CenterY;

    Poly->Points[0].u = Sx<<16; Poly->Points[0].v = Sy<<16;    
    Poly->Points[1].u = Sx<<16; Poly->Points[1].v = (Sy+Ht-1)<<16;    
    Poly->Points[2].u = (Sx+Wd-1)<<16; Poly->Points[2].v = (Sy+Ht-1)<<16;    
    Poly->Points[3].u = (Sx+Wd-1)<<16; Poly->Points[3].v = Sy<<16;

    LONG x,y;
    INT i;
    for (i=0;i<4;i++)
      {
        x = (LONG)(Poly->Points[i].x * ScaleX);  
        y = (LONG)(Poly->Points[i].y * ScaleY);
        Poly->Points[i].x = (LONG)(x*Cz - y*Sz + Cx);
        Poly->Points[i].y = (LONG)(x*Sz + y*Cz + Cy);
      } 

    ViewPort = Dest->GetViewPort ();
    
    ClipPolygon ( Poly, ClipPoly, ViewPort );

    EDGETABLE *EdgeTable;   
    EdgeTable = new EDGETABLE ();

    LONG SrcWd = Src->GetBytesPerRow ();
    BYTE *SrcBuffer = Src->GetBuffer ();
    
    if (ClipPoly->NumPoints>2)
      {
        CreateEdgeTable ( ClipPoly, EdgeTable );  
        LONG x1,x2;
        LONG U1,U2,Du;
        LONG V1,V2,Dv;
        LONG DWd;
        BYTE *DestBuffer;
        
        DestBuffer = Dest->SetOffset ( 0, EdgeTable->Top );
        for (i=0;i<EdgeTable->Height;i++)
          {
            x1 = EdgeTable->Left[i].x;  
            x2 = EdgeTable->Right[i].x;
            U1 = EdgeTable->Left[i].u;  
            U2 = EdgeTable->Right[i].u;
            V1 = EdgeTable->Left[i].v;  
            V2 = EdgeTable->Right[i].v;
            DWd = x2-x1+1;
            if (DWd<=0)
              {
                DestBuffer = Dest->GetNextRow ( ROW_DOWN );
                continue;
              } 

            Du = (U2-U1)/DWd;
            Dv = (V2-V1)/DWd;
            INT j;
            DestBuffer += x1;
            for (j=0;j<DWd;j++)
              {
                LONG SrcOffset;
                SrcOffset = (V1>>16)*SrcWd+(U1>>16);
                *DestBuffer = *(SrcBuffer+SrcOffset);
                DestBuffer++;
                U1 += Du;
                V1 += Dv;
              } 
            DestBuffer = Dest->GetNextRow ( ROW_DOWN );
          } 
      } 
    
    delete EdgeTable;
    delete Poly;
    delete ClipPoly;
  } 
                           
VOID GRAFIX::ConvertImage ( IMAGE *Image, COLORTABLE *MatchTable )
  {
    LONG i,j;
    BYTE *Buffer;
    BYTE *Table;

    Table = MatchTable->GetTable ();
    if (Table==NULL)
      return;

    for (i=0;i<Image->GetHeight();i++)
      {
        Buffer = Image->SetOffset ( 0, i );
        for (j=0;j<Image->GetWidth();j++)
          {
            LONG Color;
            Color = Buffer[j];
            Buffer[j] = Table[Color];  
          } 
      } 
  } 

BOOLEAN GRAFIX::LoadImage ( STRING FileName, IMAGE *Image, RGBPALETTE *Pal )
  {
    IMAGEFILETOOL *ImageTool;
    INT FileType;
    FILEHANDLE f;
    BOOLEAN Result;

    f = File.Open ( FileName, OPEN_READ | OPEN_BINARY );
    if (f==NULL)
      {
        Error.SetError ( ERR_FILEIO );
        return FAILURE;
      } 

    FileType = GetImageFileType ( f );
    switch (FileType)
      {
        case PCXFILE :
          ImageTool = new PCXFILETOOL ( f );
          break;
        case BMPFILE :
          ImageTool = new BMPFILETOOL ( f );
          break;
        case GIFFILE :
          ImageTool = new GIFFILETOOL ( f );
          break;
        default :
          ImageTool = NULL;
          break;
      } 

    if (ImageTool==NULL)
      {
        Error.SetError ( ERR_NOMEMORY );
        File.Close ( f );
        return FAILURE;
      } 

    Result = ImageTool->LoadImage ( Image, Pal );
    if (Result==FAILURE)
      {
        Error.SetError ( ERR_NOVALID );
      } 

    File.Close ( f );
    if (Result)
      {}
    return Result;
  } 

BOOLEAN GRAFIX::SaveImage ( STRING FileName, SHORT ImageType, IMAGE *Image,
                            LONG Sx, LONG Sy, LONG Wd, LONG Ht, RGBPALETTE *Pal )
  {
    IMAGEFILETOOL *ImageTool;
    FILEHANDLE f;

    f = File.Open ( FileName, OPEN_WRITE | OPEN_BINARY );
    if (f==NULL)
      return FAILURE;

    switch (ImageType)
      {
        case PCXFILE :
          ImageTool = new PCXFILETOOL ( f );
          break;
        case BMPFILE :
          ImageTool = new BMPFILETOOL ( f );
          break;
        case GIFFILE :
          ImageTool = new GIFFILETOOL ( f );
          break;
        default :
          ImageTool = NULL;
          break;
      } 

    if (ImageTool==NULL)
      {
        File.Close ( f );
        return FAILURE;
      } 

    BOOLEAN Ok;
    RECTANGLE ViewPort;

    ViewPort = Image->GetViewPort ();

    Ok = ClipRect ( &ViewPort, &Sx, &Sy, &Wd, &Ht );
    if (!Ok)
      {
        File.Close ( f );
        return FAILURE;
      } 

    Ok = ImageTool->SaveImage ( Image, Sx, Sy, Wd, Ht, Pal );
    if (!Ok)
      {
        File.Close ( f );
        return FAILURE;
      } 
    File.Close ( f );
    return SUCCESS;
  } 

VOID GRAFIX::PlayFLIC ( FLICFILE *Flic, RECTANGLE Region, LONG Cx, LONG Cy,
                        LONG StartFrame, LONG EndFrame, LONG Flags, FLICCallback Callback )
  {
    if (DisplayDriver==NULL)
      return;

    if (Flic==NULL)
      return;

    LONG Sx,Sy,Wd,Ht;
    LONG NumFrames;
    BOOLEAN PalChange;
    IMAGE *Image;

    Sx = Region.x1;
    Sy = Region.y1;
    Wd = Region.x2-Region.x1+1;
    Ht = Region.y2-Region.y1+1;
    
    Image = Flic->GetImage ();
    HDISPLAY hDisplay = Flic->GetDisplay ();
    
    if (Flags&FLIC_NEXT)
      {
        Flic->PlayFrame ( TRUE );
        PalChange = Flic->IsPalChanged ();
        if (PalChange)
          {
            SetPalette ( hDisplay, Flic->GetPalette() );
            Flic->PalProcessed ();
          } 
        DisplayImage ( hDisplay, Image, Sx, Sy, Wd, Ht, Cx, Cy );
        return;  
      } 
      
    if (Flags&FLIC_NOADVANCE)
      {
        Flic->PlayFrame ( FALSE );
        PalChange = Flic->IsPalChanged ();
        if (PalChange)
          {
            SetPalette ( hDisplay, Flic->GetPalette() );
            Flic->PalProcessed ();
          } 
        DisplayImage ( hDisplay, Image, Sx, Sy, Wd, Ht, Cx, Cy );
        return;  
      } 
      
    NumFrames = Flic->GetNumFrames ();
    if (NumFrames==0)
      return;
     
    if (Flags&FLIC_ALL)
      {
        StartFrame = 0;
        EndFrame = NumFrames-1;  
      } 
    else
      {
        if (StartFrame>=NumFrames)
          return;  
        if (EndFrame>=NumFrames)
          EndFrame = EndFrame%NumFrames;
      } 
      
    LONG LoopCount=0;
    LONG Speed;

    Speed = Flic->Speed;
    if (Speed==FLIC_SPEED_DEFAULT)
      {
        if (Flic->IsFLCFile()!=TRUE)
          Speed = Flic->GetDefaultSpeed ();
        else
          Speed = 1;  
      } 

         if (Speed)
           {} 
    BOOLEAN Done=FALSE;

    Flic->SetFrame ( StartFrame );
    
    while (!Done)
      {
        Flic->PlayFrame ( TRUE );
        PalChange = Flic->IsPalChanged ();
        if (PalChange)
          {
            SetPalette ( hDisplay, Flic->GetPalette() );
            Flic->PalProcessed ();
          } 
              
        #if defined (__FORDOS__)
          WaitForRetrace ( Speed );
        #endif
          
        DisplayImage ( hDisplay, Image, Sx, Sy, Wd, Ht, Cx, Cy );
        if (Callback!=NULL)
          Done = Callback ( Flic );
            
        if (Flic->GetFrame ()>EndFrame)
          {
            if (Flags&FLIC_LOOP)
              {  
                Flic->SetFrame ( StartFrame );
                LoopCount++;
                if (Flic->Loop>=0)
                  {
                    if (LoopCount>=Flic->Loop)
                      Done = TRUE;  
                  } 
              } 
            else
              Done = TRUE;      
          } 
      } 
  } 

VOID GRAFIX::SetPalette ( HDISPLAY hDisplay, RGBPALETTE *Pal )
  {
    if (DisplayDriver==NULL)
      return;
    DisplayDriver->SetPalette ( hDisplay, Pal );
  } 

VOID GRAFIX::GetPalette ( HDISPLAY hDisplay, RGBPALETTE *Pal )
  {
    if (DisplayDriver==NULL)
      return;
    DisplayDriver->GetPalette ( hDisplay, Pal );
  } 

VOID GRAFIX::ClearPalette ( HDISPLAY hDisplay, BYTE R, BYTE G, BYTE B )
  {
    RGBPALETTE *Pal;
    RGBCOLOR *Entry;
    
    Pal = new RGBPALETTE ();
    if (Pal==NULL)
      return;
    Entry = Pal->GetEntry ();

    INT i;
    for (i=0;i<256;i++)
      {
        Entry[i].Red = R;  
        Entry[i].Green = G;  
        Entry[i].Blue = B;  
      } 
    SetPalette ( hDisplay, Pal );  
    delete Pal;  
  } 

VOID GRAFIX::FadePalette ( HDISPLAY hDisplay, RGBPALETTE *Pal,
                           LONG Direction, LONG NumSteps )
  {
    RGBCOLOR OldEntry[256];
    RGBCOLOR *Entry;
    INT i,j;

    if (NumSteps==0)
      return;

    Entry = Pal->GetEntry ();
    for (i=0;i<256;i++)
      {
        OldEntry[i].Red = Entry[i].Red;
        OldEntry[i].Green = Entry[i].Green;
        OldEntry[i].Blue = Entry[i].Blue;
      } 

    double StepRatio;
    double Ratio;
    StepRatio = (double)1/NumSteps;

    if (Direction==PAL_FADE_IN)
      {
        for (i=0;i<NumSteps;i++)
          {
            Ratio = StepRatio*i;
            for (j=0;j<256;j++)
              {
                Entry[j].Red = (BYTE)(Ratio*OldEntry[j].Red);
                Entry[j].Green = (BYTE)(Ratio*OldEntry[j].Green);
                Entry[j].Blue = (BYTE)(Ratio*OldEntry[j].Blue);
              } 
            WaitForRetrace ();
            SetPalette ( hDisplay, Pal );
          } 
      } 
    else
      {
        for (i=NumSteps-1;i>=0;i--)
          {
            Ratio = StepRatio*i;
            for (j=0;j<256;j++)
              {
                Entry[j].Red = (BYTE)(Ratio*OldEntry[j].Red);
                Entry[j].Green = (BYTE)(Ratio*OldEntry[j].Green);
                Entry[j].Blue = (BYTE)(Ratio*OldEntry[j].Blue);
              } 
            WaitForRetrace ();
            SetPalette ( hDisplay, Pal );
          } 
      } 

    for (i=0;i<256;i++)
      {
        Entry[i].Red = OldEntry[i].Red;
        Entry[i].Green = OldEntry[i].Green;
        Entry[i].Blue = OldEntry[i].Blue;
      } 
  } 

BOOLEAN GRAFIX::SavePalette ( STRING FileName, RGBPALETTE *Pal, LONG PalFormat )
  {
    FILEHANDLE f;

    if (Pal==NULL)
      return FAILURE;

    if (PalFormat==PAL_MSWIN)
      {
        f = File.Open ( FileName, OPEN_WRITE | OPEN_BINARY );
        if (f==NULL)
          return FAILURE;

        CHAR Str[4];
        DWORD Size;

        strncpy ( Str, "RIFF", 4 );
        if (File.Write ( f, Str, 4 )==FALSE)
          {
            File.Close ( f );
            return FAILURE;
          } 

        Size = 1040;
        if (File.Write ( f, &Size, 4 )==FALSE)
          {
            File.Close ( f );
            return FAILURE;
          } 

        strncpy ( Str, "PAL ", 4 );
        if (File.Write ( f, Str, 4 )==FALSE)
          {
            File.Close ( f );
            return FAILURE;
          } 

        strncpy ( Str, "data", 4 );
        if (File.Write ( f, Str, 4 )==FALSE)
          {
            File.Close ( f );
            return FAILURE;
          } 

        Size = 4+4*256;
        if (File.Write ( f, &Size, 4 )==FALSE)
          {
            File.Close ( f );
            return FAILURE;
          } 

        SHORT Version;
        SHORT NumEntries;

        Version = 0x0300;
        if (File.Write ( f, &Version, 2 )==FALSE)
          {
            File.Close ( f );
            return FAILURE;
          } 

        NumEntries = 256;
        if (File.Write ( f, &NumEntries, 2 )==FALSE)
          {
            File.Close ( f );
            return FAILURE;
          } 

        RGBCOLOR *Entry;

        Entry = Pal->GetEntry ();

        INT i;
        BYTE Flags;
        for (i=0;i<256;i++)
          {
            File.Write ( f, &(Entry[i].Red), 1 );
            File.Write ( f, &(Entry[i].Green), 1 );
            File.Write ( f, &(Entry[i].Blue), 1 );
            File.Write ( f, &(Flags), 1 );
          } 

        File.Close ( f );
        return SUCCESS;
      } 
    else if (PalFormat==PAL_PSP)
      {
        f = File.Open ( FileName, OPEN_WRITE );
        if (f==NULL)
          return FAILURE;

        fprintf ( f, "JASC-PAL\n" );
        fprintf ( f, "0100\n" );
        fprintf ( f, "256\n" );

        RGBCOLOR *Entry;
        Entry = Pal->GetEntry ();

        INT i;
        for (i=0;i<256;i++)
          {
            fprintf ( f, "%d %d %d\n", Entry[i].Red,
                                       Entry[i].Green,
                                       Entry[i].Blue );
          } 

        File.Close ( f );
        return SUCCESS;
      } 
    else     
      {
        return FAILURE;
      } 
  } 

BOOLEAN GRAFIX::LoadPalette ( STRING FileName, RGBPALETTE *Pal )
  {
    FILEHANDLE f;
    if (Pal==NULL)
      return FAILURE;

    f = File.Open ( FileName, OPEN_READ | OPEN_BINARY );
    if (f==NULL)
      return FAILURE;

    CHAR Str[4];
    DWORD Size;

    
    CHAR ID[8];
    LONG Format;

    if (File.Read ( f, ID, 8 )==FALSE)
      {
        File.Close ( f );
        return FAILURE;
      } 
    if (strncmp (ID, "RIFF", 4)==0)
      {
        Format = PAL_MSWIN;
      } 
    else if (strncmp ( ID, "JASC-PAL", 8 )==0)
      {
        Format = PAL_PSP;
      } 
    else
      {
        File.Close ( f );
        return FAILURE;
      } 

    
    File.Seek ( f, 0, FROM_BEGIN );

    if (Format==PAL_MSWIN)
      {
        if (File.Read ( f, Str, 4 )==FALSE)
          {
            File.Close ( f );
            return FAILURE;
          } 
        if (strncmp (Str, "RIFF", 4)!=0)
          {
            File.Close ( f );
            return FAILURE;
          } 

        if (File.Read ( f, &Size, 4 )==FALSE)
          {
            File.Close ( f );
            return FAILURE;
          } 

        if (File.Read ( f, Str, 4 )==FALSE)
          {
            File.Close ( f );
            return FAILURE;
          } 
        if (strncmp (Str, "PAL ", 4)!=0)
          {
            File.Close ( f );
            return FAILURE;
          } 

        if (File.Read ( f, Str, 4 )==FALSE)
          {
            File.Close ( f );
            return FAILURE;
          } 
        if (strncmp (Str, "data", 4)!=0)
          {
            File.Close ( f );
            return FAILURE;
          } 

        if (File.Read ( f, &Size, 4 )==FALSE)
          {
            File.Close ( f );
            return FAILURE;
          } 

        SHORT Version;
        SHORT NumEntries;

        if (File.Read ( f, &Version, 2 )==FALSE)
          {
            File.Close ( f );
            return FAILURE;
          } 

        if (File.Read ( f, &NumEntries, 2 )==FALSE)
          {
            File.Close ( f );
            return FAILURE;
          } 

        #if defined (__MSBFIRST__)
          SwapWord ( (WORD*)NumEntries );
        #endif 

        RGBCOLOR *Entry;

        Entry = Pal->GetEntry ();

        INT i;
        BYTE Flags;

        for (i=0;i<NumEntries;i++)
          {
            File.Read ( f, &(Entry[i].Red), 1 );
            File.Read ( f, &(Entry[i].Green), 1 );
            File.Read ( f, &(Entry[i].Blue), 1 );
            File.Read ( f, &(Flags), 1 );
          } 

        File.Close ( f );
        return SUCCESS;
      } 
    else if (Format==PAL_PSP)
      {
        CHAR VerStr[8];
        CHAR PalStr[8];
        INT Num;
        INT Red,Green,Blue;

        fscanf ( f, "%s\n", PalStr );
        fscanf ( f, "%s\n", VerStr );
        fscanf ( f, "%d\n", &Num );

        RGBCOLOR *Entry;
        Entry = Pal->GetEntry ();

        if (Entry==NULL)
          {
            File.Close ( f );
            return FAILURE;
          } 
          
        INT i;
        for (i=0;i<Num;i++)
          {
            fscanf ( f, "%d %d %d\n", &Red, &Green, &Blue );
            Entry[i].Red = (BYTE)Red;
            Entry[i].Green = (BYTE)Green;
            Entry[i].Blue = (BYTE)Blue;
          } 
        File.Close ( f );
        return SUCCESS;  
      } 
    File.Close ( f );
    return FAILURE;
  } 

LONG GRAFIX::GetWidth ( HDISPLAY hDisplay )
  {
    if (DisplayDriver==NULL)
      return 0;
    return DisplayDriver->GetWidth ( hDisplay );
  } 

LONG GRAFIX::GetHeight ( HDISPLAY hDisplay )
  {
    if (DisplayDriver==NULL)
      return 0;
    return DisplayDriver->GetHeight ( hDisplay );
  } 

LONG GRAFIX::GetCode ( RECTANGLE Rect, LONG x, LONG y )
  {
    
    

    INT Code;

    Code = 0;
    if (y<Rect.y1)
      Code |= (1<<3);
    else if (y>Rect.y2)
      Code |= (1<<2);

    if (x<Rect.x1)
      Code |= (1<<0);
    else if (x>Rect.x2)
      Code |= (1<<1);

    return Code;
  } 

BOOLEAN GRAFIX::ClipLine ( RECTANGLE Rect, LONG *x1, LONG *y1, LONG *x2, LONG *y2 )
  {
    LONG Code1,Code2;

    LONG Code;

    BOOLEAN Done=FALSE;

    Code1 = GetCode ( Rect, *x1, *y1 );
    Code2 = GetCode ( Rect, *x2, *y2 );

    INT x, y;

    while (!Done)
      {
        if ((Code1|Code2)==0)
          return TRUE;
        if (Code1&Code2)
          return FALSE;

        if (Code1)
          Code = Code1;
        else
          Code = Code2;

        if (Code&(1<<3)) 
          {
            x = *x1 + ((*x2-*x1)*(Rect.y1-*y1) / (*y2-*y1));
            y = Rect.y1;
          } 
        else if (Code&(1<<2)) 
          {
            x = *x1 + ((*x2-*x1)*(Rect.y2-*y1) / (*y2-*y1));
            y = Rect.y2;
          } 
        else if (Code&(1<<1)) 
          {
            y = *y1 + ((*y2-*y1)*(Rect.x2-*x1) / (*x2-*x1));
            x = Rect.x2;
          } 
        else if (Code&(1<<0)) 
          {
            y = *y1 + ((*y2-*y1)*(Rect.x1-*x1) / (*x2-*x1));
            x = Rect.x1;
          } 

        if (Code==Code1)
          {
            *x1 = x;
            *y1 = y;
            Code1 = GetCode ( Rect, *x1, *y1 );
          } 
        else
          {
            *x2 = x;
            *y2 = y;
            Code2 = GetCode ( Rect, *x1, *y1 );
          } 
      } 
    return FALSE;
  } 

VOID GRAFIX::DrawPixel ( IMAGE *Dest, LONG x, LONG y )
  {
    RECTANGLE ViewPort;

    ViewPort = Dest->GetViewPort ();
    if ((x<ViewPort.x1)||(y<ViewPort.y1)||(x>ViewPort.x2)||(y>ViewPort.y2))
      return;

    BYTE *Buffer;
    Buffer = Dest->SetOffset ( x, y );
    *Buffer = (BYTE)FGColor;
  } 

VOID GRAFIX::DrawPixel ( HDISPLAY hDisplay, LONG x, LONG y )
  {
    if (DisplayDriver==NULL)
      return;

    DisplayDriver->DrawPixel ( hDisplay, x, y, FGColor );
  } 

VOID GRAFIX::DrawLine ( IMAGE *Dest, LONG x1, LONG y1, LONG x2, LONG y2 )
  {
    BOOLEAN Ok;
    RECTANGLE ViewPort;

    ViewPort = Dest->GetViewPort ();

    Ok = ClipLine ( ViewPort, &x1, &y1, &x2, &y2 );
    if (!Ok)
      return;

    if (y1>y2)
      {                   
        #ifndef __FORVISUAL__
          SwapValue ( &y1, &y2 );
          SwapValue ( &x1, &x2 );
        #endif  
      } 

    INT Dx, Dy;
    INT AddX;
    INT Len;

    if (x2<x1)
      {
        AddX = -1;
      } 
    else
      AddX = 1;

    Dx = abs(x2-x1);
    Dy = abs(y2-y1);

    Len = MaxValue ( Dx, Dy ) + 1;

    BYTE *Buffer;

    Buffer = Dest->SetOffset ( 0, y1 );

    COLOR Color = FGColor;
    INT i;
    DWORD Error=0;

    DWORD x;

    if (Dy==0)
      {
        x = MinValue ( x1, x2 );
        int i;
        Buffer+=x;
        for (i=0;i<Len;i++)
          Buffer[i] = (BYTE)Color;
        
      } 
    else
      {
        x = x1;
        if (Dx>Dy)
          {
            for (i=0;i<Len;i++)
              {
                Buffer[x] = (BYTE)Color;
                Error += (DWORD)Dy;
                if (Error>=(DWORD)Dx)
                  {
                    Buffer = Dest->GetNextRow ( ROW_DOWN );
                    Error -= (DWORD)Dx;
                  } 
                x += AddX;
              } 
          } 
        else
          {
            for (i=0;i<Len;i++)
              {
                Buffer[x] = (BYTE)Color;
                Error += (DWORD)Dx;
                if (Error>=(DWORD)Dy)
                  {
                    x += AddX;
                    Error -= (DWORD)Dy;
                  } 
                Buffer = Dest->GetNextRow ( ROW_DOWN );
              } 
          } 
      } 
  } 

VOID GRAFIX::DrawLine ( HDISPLAY hDisplay, LONG x1, LONG y1, LONG x2, LONG y2 )
  {
    if (DisplayDriver==NULL)
      return;

    RECTANGLE ViewPort;
    ViewPort.x1 = 0;
    ViewPort.y1 = 0;
    ViewPort.x2 = DisplayDriver->GetWidth( hDisplay )-1;
    ViewPort.y2 = DisplayDriver->GetHeight( hDisplay )-1;

    BOOLEAN Ok = ClipLine ( ViewPort, &x1, &y1, &x2, &y2 );
    if (!Ok)
      return;

    if (y1>y2)
      {
        SwapValue ( &y1, &y2 );
        SwapValue ( &x1, &x2 );
      } 

    COLOR Color = FGColor;
    DisplayDriver->DrawLine ( hDisplay, x1, y1, x2, y2, Color );
  } 

VOID GRAFIX::DrawRect ( IMAGE *Dest, LONG x1, LONG y1, LONG x2, LONG y2 )
  {
    DrawLine ( Dest, x1, y2, x2, y2 );
    DrawLine ( Dest, x1, y1, x2, y1 );
    DrawLine ( Dest, x1, y1, x1, y2 );
    DrawLine ( Dest, x2, y1, x2, y2 );
  } 

VOID GRAFIX::DrawRect ( HDISPLAY hDisplay, LONG x1, LONG y1, LONG x2, LONG y2 )
  {
    if (DisplayDriver==NULL)
      return;

    RECTANGLE ViewPort;
    ViewPort.x1 = 0;
    ViewPort.y1 = 0;
    ViewPort.x2 = DisplayDriver->GetWidth( hDisplay )-1;
    ViewPort.y2 = DisplayDriver->GetHeight( hDisplay )-1;

    if (y1>y2)
      {
        SwapValue ( &y1, &y2 );
        SwapValue ( &x1, &x2 );
      } 

    LONG Wd,Ht;
    Wd = x2-x1+1;
    Ht = y2-y1+1;

    BOOLEAN Ok = ClipRect ( &ViewPort, &x1, &y1, &Wd, &Ht );
    if (!Ok)
      return;

    x2 = x1+Wd-1;
    y2 = y1+Ht-1;

    DisplayDriver->DrawRect ( hDisplay, x1, y1, x2, y2, FGColor );
  } 

VOID GRAFIX::FillRect ( IMAGE *Dest, LONG x1, LONG y1, LONG x2, LONG y2 )
  {
    if (DisplayDriver==NULL)
      return;

    RECTANGLE ViewPort;
    ViewPort = Dest->GetViewPort ();

    if (y1>y2)
      {
        SwapValue ( &y1, &y2 );
        SwapValue ( &x1, &x2 );
      } 

    LONG Wd,Ht;
    Wd = x2-x1+1;
    Ht = y2-y1+1;

    BOOLEAN Ok = ClipRect ( &ViewPort, &x1, &y1, &Wd, &Ht );
    if (!Ok)
      return;

    x2 = x1+Wd-1;
    y2 = y1+Ht-1;

    INT i;
    BYTE *Buffer;

    Buffer = Dest->SetOffset ( x1, y1 );

    for (i=0;i<Ht;i++)
      {
        memset ( Buffer, BGColor, Wd );
        Buffer = Dest->GetNextRow ( ROW_DOWN );
      } 
  } 

VOID GRAFIX::FillRect ( HDISPLAY hDisplay, LONG x1, LONG y1, LONG x2, LONG y2 )
  {
    if (DisplayDriver==NULL)
      return;

    RECTANGLE ViewPort;
    ViewPort.x1 = 0;
    ViewPort.y1 = 0;
    ViewPort.x2 = DisplayDriver->GetWidth( hDisplay )-1;
    ViewPort.y2 = DisplayDriver->GetHeight( hDisplay )-1;

    if (y1>y2)
      {
        SwapValue ( &y1, &y2 );
        SwapValue ( &x1, &x2 );
      } 

    LONG Wd,Ht;
    Wd = x2-x1+1;
    Ht = y2-y1+1;

    BOOLEAN Ok = ClipRect ( &ViewPort, &x1, &y1, &Wd, &Ht );
    if (!Ok)
      return;

    x2 = x1+Wd-1;
    y2 = y1+Ht-1;

    DisplayDriver->FillRect ( hDisplay, x1, y1, x2, y2, BGColor );
  } 

VOID GRAFIX::DrawEllipsePoint ( IMAGE *Dest, LONG Cx, LONG Cy, LONG x, LONG y )
  {
    INT Dx,Dy;

    Dx = x - Cx;
    Dy = Cy - y;

    DrawPixel ( Dest, Cx+Dx, Cy-Dy );
    DrawPixel ( Dest, Cx+Dx, Cy+Dy );
    DrawPixel ( Dest, Cx-Dx, Cy-Dy );
    DrawPixel ( Dest, Cx-Dx, Cy+Dy );
  } 

VOID GRAFIX::DrawEllipse ( IMAGE *Dest, LONG Cx, LONG Cy, LONG Rx, LONG Ry )
  {
    LONG RsqX,RsqY;
    LONG RsqXY;
    LONG X2,Y2;
    LONG x,y;
    LONG MidTest;

    if ((Rx==0)||(Ry==0))
      return;

    RsqX = Rx*Rx;
    RsqY = Ry*Ry;
    RsqXY = RsqX*RsqY;
    x = 0;
    y = Ry;

    
    DrawEllipsePoint ( Dest, Cx, Cy, x+Cx,
                       Cy-y );

    Y2 = y*y;
    while ((x*RsqY)<=(y*RsqX))
      {
        x++;
        X2 = x*x;
        MidTest = RsqY*X2+RsqX*Y2 - RsqXY;
        if (MidTest>=0)
          {
            y--;
            Y2 = y*y;
          } 
        DrawEllipsePoint ( Dest, Cx, Cy, x+Cx,
                           Cy-y );
      } 

    while (y>=0)
      {
        y--;
        Y2 = y*y;
        MidTest = RsqY*X2+RsqX*Y2 - RsqXY;
        if (MidTest<0)
          {
            x++;
            X2 = x*x;
          } 
        DrawEllipsePoint ( Dest, Cx, Cy, x+Cx,
                           Cy-y );
      } 
  } 

VOID GRAFIX::DrawEllipse ( HDISPLAY hDisplay, LONG Cx, LONG Cy, LONG Rx, LONG Ry )
  {
    if (DisplayDriver==NULL)
      return;
    DisplayDriver->DrawEllipse ( hDisplay, Cx, Cy, Rx, Ry, FGColor );
  } 

VOID GRAFIX::DrawHorizLine ( IMAGE *Dest, LONG x1, LONG x2, LONG y )
  {
    RECTANGLE ViewPort;

    ViewPort = Dest->GetViewPort ();

    if (x1>x2)
      SwapValue ( &x1, &x2 );

    if (x1<ViewPort.x1)
      x1 = ViewPort.x1;
    else if (x1>ViewPort.x2)
      return;

    if (x2<ViewPort.x1)
      return;
    else if (x2>ViewPort.x2)
      x2 = ViewPort.x2;

    if (y<ViewPort.y1)
      y = ViewPort.y1;
    else if (y>ViewPort.y2)
      return;

    INT Wd;

    Wd = x2 - x1 + 1;

    BYTE *Buffer = Dest->SetOffset ( x1, y );

    memset ( Buffer, BGColor, Wd );
  } 

VOID GRAFIX::FillEllipsePoint ( IMAGE *Dest, LONG Cx, LONG Cy, LONG x, LONG y )
  {
    INT Dx,Dy;

    Dx = x - Cx;
    Dy = Cy - y;

    DrawHorizLine ( Dest, Cx-Dx, Cx+Dx, Cy-Dy );
    DrawHorizLine ( Dest, Cx-Dx, Cx+Dx, Cy+Dy );
  } 

VOID GRAFIX::FillEllipse ( IMAGE *Dest, LONG Cx, LONG Cy, LONG Rx, LONG Ry )
  {
    LONG RsqX,RsqY;
    LONG RsqXY;
    LONG X2,Y2;
    LONG x,y;
    LONG MidTest;

    if ((Rx==0)||(Ry==0))
      return;

    RsqX = Rx*Rx;
    RsqY = Ry*Ry;
    RsqXY = RsqX*RsqY;
    x = 0;
    y = Ry;

    
    FillEllipsePoint ( Dest, Cx, Cy, x+Cx,
                       Cy-y );

    Y2 = y*y;
    while ((x*RsqY)<=(y*RsqX))
      {
        x++;
        X2 = x*x;
        MidTest = RsqY*X2+RsqX*Y2 - RsqXY;
        if (MidTest>=0)
          {
            y--;
            Y2 = y*y;
          } 
        FillEllipsePoint ( Dest, Cx, Cy, x+Cx,
                           Cy-y );
      } 

    while (y>=0)
      {
        y--;
        Y2 = y*y;
        MidTest = RsqY*X2+RsqX*Y2 - RsqXY;
        if (MidTest<0)
          {
            x++;
            X2 = x*x;
          } 
        FillEllipsePoint ( Dest, Cx, Cy, x+Cx,
                           Cy-y );
      } 
  } 

VOID GRAFIX::FillEllipse ( HDISPLAY hDisplay, LONG Cx, LONG Cy, LONG Rx, LONG Ry )
  {
    if (DisplayDriver==NULL)
      return;
    DisplayDriver->FillEllipse ( hDisplay, Cx, Cy, Rx, Ry, BGColor );
  } 

VOID GRAFIX::DrawText ( HDISPLAY hDisplay, STRING Text, LONG x, LONG y )
  {
    if (DisplayDriver==NULL)
      return;
    DisplayDriver->DrawText ( hDisplay, Text, x, y, FGColor );
  } 


