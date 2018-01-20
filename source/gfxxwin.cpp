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
#include "gfxxwin.h"
  
XWINDRIVER::XWINDRIVER ()
  {
    Width = Height = 0;
    #if defined (__FORUNIX__)
      MainXDisplay = NULL;
      XWinGC = NULL;
      XWinImage = NULL;
      XWinColormap = NULL;
    #endif
  } 

XWINDRIVER::~XWINDRIVER ()
  {
    #if defined (__FORUNIX__)
      if (XWinGC!=NULL)      
        XFreeGC ( MainXDisplay, XWinGC );
      XWinGC = NULL;

      if (XWinImage!=NULL)
        XDestroyImage ( XWinImage );
      XWinImage = NULL;

      if (XWinColormap!=NULL)
        XFreeColormap ( MainXDisplay, XWinColormap );
    #endif
  } 

BOOLEAN XWINDRIVER::SetUp ( DISPLAYDATA *Data )
  {
    if (Data)
      {}  
    #if defined (__FORWINDOWS__)
      return FAILURE;
    #elif defined (__FOROS2__)
      return FAILURE;
    #elif defined (__FORDOS__)
      return FAILURE;
    #elif defined (__FORUNIX__)

#if 0
      MainXDisplay = Data->MainXDisplay;
      Window Root;
printf("   XWINDRIVER::SetUp   1\n");
      MainXScreen = DefaultScreenOfDisplay ( MainXDisplay );
      Root = RootWindowOfScreen ( MainXScreen );

printf("   XWINDRIVER::SetUp   3\n");      
      if (XWinGC!=NULL)
        XFreeGC ( MainXDisplay, XWinGC );
      XWinGC = XCreateGC ( MainXDisplay, Root, NULL, NULL );

      Visual *XWinVisual;
      XWinVisual = DefaultVisual ( MainXDisplay, DefaultScreen(MainXDisplay) );
      INT Format,Offset,BitmapPad,BytesPerLine;
      UINT Width,Height,Depth;

      Format = ZPixmap;
      Offset = 0;
      BitmapPad = 8;
      BytesPerLine = 0;
      Depth = 8;
printf("   XWINDRIVER::SetUp   9\n");      
      
#else


      Window Root;

      XWinGC = NULL;

      XWinImage = NULL;

      XWinColormap = NULL;

printf("   XWINDRIVER::SetUp   1\n");
      
      MainXDisplay = Data->MainXDisplay = XOpenDisplay ( NULL );
printf("   XWINDRIVER::SetUp   2\n");
      if (Data->MainXDisplay==NULL)
        {
          printf ("Can not open X Display.\n\n");
          exit (0);
        } 
printf("   XWINDRIVER::SetUp   3\n");





  int screen_num = DefaultScreen(MainXDisplay);
printf("   XWINDRIVER::SetUp   4      screen_num = %d \n", screen_num );
  int win_border_width = 2;
  Window win;

  
  
  
  
  
  win = XCreateSimpleWindow(MainXDisplay, RootWindow(MainXDisplay, screen_num),
                            0,  0, 600,  600, win_border_width ,
                            BlackPixel(MainXDisplay, screen_num),
                            WhitePixel(MainXDisplay, screen_num)
	);
printf("   XWINDRIVER::SetUp   5      win = <%p> \n", win);

  
  XMapWindow(MainXDisplay, win);
printf("   XWINDRIVER::SetUp   6\n");

  
  XFlush(MainXDisplay);
printf("   XWINDRIVER::SetUp   7\n");


  
  

  unsigned long valuemask = 0;		
					
  XGCValues values;			
  unsigned int line_width = 2;		
  int line_style = LineSolid;		
  int cap_style = CapButt;		
  int join_style = JoinBevel;		

printf("   XWINDRIVER::SetUp   8\n");

  XWinGC = XCreateGC(MainXDisplay, win, valuemask, &values);
  if (XWinGC < 0)
  {
	printf("ERROR>>>>>>>>>>>>>>>>>>>>>>>XCreateGC: \n");
  }
printf("   XWINDRIVER::SetUp   9\n");

  
  {
printf("   XWINDRIVER::SetUp   9 DIRECT VIDEO \n");
    XSetForeground(MainXDisplay, XWinGC, BlackPixel(MainXDisplay, screen_num));
    XSetBackground(MainXDisplay, XWinGC, WhitePixel(MainXDisplay, screen_num));
  }
printf("   XWINDRIVER::SetUp   10\n");

  
  XSetLineAttributes(MainXDisplay, XWinGC,
                     line_width, line_style, cap_style, join_style);

printf("   XWINDRIVER::SetUp   11\n");

  
  XSetFillStyle(MainXDisplay, XWinGC, FillSolid);

printf("   XWINDRIVER::SetUp   12\n");
  XSync(MainXDisplay, False);

printf("   XWINDRIVER::SetUp   13\n");


      Visual *XWinVisual;
      XWinVisual = DefaultVisual ( MainXDisplay, DefaultScreen(MainXDisplay) );
printf("   XWINDRIVER::SetUp   14\n");

      INT Format,Offset,BitmapPad,BytesPerLine;
      UINT Width = 321 ,Height = 241 ,Depth = 8;

      Format = ZPixmap;
      Offset = 0;
      BitmapPad = 8;
      BytesPerLine = 0;
      Depth = 8;

#endif 

      return SUCCESS;
    #endif
  } 

VOID XWINDRIVER::Reset ()
  {
    SetUp ( M80x25x16 );
  } 

VOID XWINDRIVER::Clear ( HDISPLAY hDisplay, LONG Color )
  {
    if (hDisplay)
      {}
    if (Color)
      {}

    #if defined (__FORUNIX__)
      XClearArea ( MainXDisplay, hDisplay, 0, 0, 0, 0, FALSE );
    #endif
  } 

LONG XWINDRIVER::GetWidth ( HDISPLAY hDisplay )
  {
    if (hDisplay)
      {}
    return 3000;
  } 

LONG XWINDRIVER::GetHeight ( HDISPLAY hDisplay )
  {
    if (hDisplay)
      {}
    return 2000;
  } 

VOID XWINDRIVER::DisplayImage ( IMAGE *Image, LONG Sx, LONG Sy,
                                LONG Wd, LONG Ht, HDISPLAY Dest,
                                LONG Cx, LONG Cy, LONG DestWd, LONG DestHt )
  {
    if (Image)
      {}
    if (Sx&Sy&Wd&Ht&Cx&Cy&DestWd&DestHt)
      {}
    if (Dest)
      {}  
    #if defined (__FORUNIX__)
      Visual *XWinVisual;
      XWinVisual = DefaultVisual ( MainXDisplay, DefaultScreen(MainXDisplay) );

      XWinImage = XCreateImage ( MainXDisplay, XWinVisual, 8, ZPixmap,
                                 0, (char*)Image->GetBuffer(), Image->GetWidth(),
                                 Image->GetHeight(), 8, 0 );

      INT Depth;

      Depth = DefaultDepth ( MainXDisplay, DefaultScreen ( MainXDisplay ) );

      if (Depth>=8)
        {
          
          XPutImage ( MainXDisplay, Dest, XWinGC, XWinImage, 
                      Sx, Sy, Cx, Cy, 
                      DestWd, DestHt );
        } 
      else
        {
          INT i,j;
          BYTE *Buffer;
          for (i=Sy;i<Sy+DestHt;i++)
            {
              Buffer = Image->SetOffset ( 0, i );
              for (j=Sx;j<Sx+DestWd;j++)
                {
                  DrawPixel ( Dest, Cx+j-Sx, Cy+i-Sy, Buffer[j] ); 
                } 
            } 
        } 

      XFree ( XWinImage );
      XWinImage = NULL;
    #endif
  } 

#if defined (__FORUNIX__)
  Visual* GetProperVisual ( Display *MainXDisplay, INT ScreenID, INT Depth, INT Class )
    {
      XVisualInfo *VisualList, TemplateVisual;
      INT NumVisuals;

      TemplateVisual.screen = ScreenID;
      
      VisualList = XGetVisualInfo ( MainXDisplay, VisualScreenMask,
                                    &TemplateVisual, &NumVisuals );
      if (NumVisuals==0)
        return NULL;

      INT i;

      for (i=0;i<NumVisuals;i++)
        {
          if ((VisualList[i].c_class==Class)&&(VisualList[i].depth>=Depth))
            {
              return VisualList[i].visual;
            } 
        } 

      return NULL;
    } 
#endif 

VOID XWINDRIVER::SetPalette ( HDISPLAY hDisplay, RGBPALETTE *Pal )
  {
    RGBCOLOR *Entry;

    if (hDisplay)
      {}
      
    if (Pal==NULL)
      return;

    Entry = Pal->GetEntry ();
    if (Entry==NULL)
      return;

    #if defined (__FORUNIX__)
      XColor XWinColors[256];
      INT i;
      for (i=0;i<256;i++)
        {
          XWinColors[i].red = Entry[i].Red*256;
          XWinColors[i].green = Entry[i].Green*256;
          XWinColors[i].blue = Entry[i].Blue*256;
          XWinColors[i].pixel = i;
          XWinColors[i].flags = DoRed | DoGreen | DoBlue;
        } 

      Visual *LocalVisual;

      LocalVisual = DefaultVisual ( MainXDisplay, DefaultScreen(MainXDisplay) );
      if (DefaultDepth ( MainXDisplay, DefaultScreen(MainXDisplay))<8)
        LocalVisual = GetProperVisual ( MainXDisplay, DefaultScreen(MainXDisplay),
                                        8,  PseudoColor );
  
      if (LocalVisual==NULL)
        return;

      if (XWinColormap==NULL)
        XWinColormap = XCreateColormap ( MainXDisplay, 
                          RootWindow(MainXDisplay,DefaultScreen(MainXDisplay)),
                          LocalVisual, AllocAll );

      XStoreColors ( MainXDisplay, XWinColormap, XWinColors, 256 );
      XSetWindowColormap ( MainXDisplay, hDisplay, XWinColormap );
    #endif
  } 

VOID XWINDRIVER::GetPalette ( HDISPLAY hDisplay, RGBPALETTE *Pal )
  {
    RGBCOLOR *Entry;

    if (hDisplay)
      {}
    if (Pal==NULL)
      return;

    Entry = Pal->GetEntry ();
    if (Entry==NULL)
      return;
  } 

VOID XWINDRIVER::DrawLine ( HDISPLAY hDisplay, LONG x1, LONG y1, LONG x2, LONG y2,
                            COLOR  Color )
  {
printf("   XWINDRIVER::DrawLine 0 \n");
    if (hDisplay)
      {
printf("   XWINDRIVER::DrawLine 1 \n");
      }
    if (x1&y1&x2&y2)
      {
printf("   XWINDRIVER::DrawLine 2 \n");
      }
    if (Color)
      {
printf("   XWINDRIVER::DrawLine 3 \n");
      }
    #if defined (__FORUNIX__)      
printf("   XWINDRIVER::DrawLine 4  MainXDisplay = %p, XWinGC = %p, Color = %d \n", MainXDisplay, XWinGC, Color);
      XSetForeground ( MainXDisplay, XWinGC, Color );
printf("   XWINDRIVER::DrawLine 5 \n");
      XDrawLine ( MainXDisplay, hDisplay, XWinGC, x1, y1, x2, y2 );
printf("   XWINDRIVER::DrawLine 6 \n");
    #endif
  } 

VOID XWINDRIVER::DrawRect ( HDISPLAY hDisplay, LONG x1, LONG y1, LONG x2, LONG y2,
                           COLOR Color )
  {
    DrawLine ( hDisplay, x1, y2, x2, y2, Color );
    DrawLine ( hDisplay, x1, y1, x2, y1, Color );
    DrawLine ( hDisplay, x1, y1, x1, y2, Color );
    DrawLine ( hDisplay, x2, y1, x2, y2, Color );
  } 

VOID XWINDRIVER::FillRect ( HDISPLAY hDisplay, LONG x1, LONG y1, LONG x2, LONG y2,
                           COLOR Color )
  {
    if (hDisplay)
      {}
    if (x1&y1&x2&y2)
      {}
    if (Color)
      {}      
    #if defined (__FORUNIX__)      
      XSetForeground ( MainXDisplay, XWinGC, Color );
      XFillRectangle ( MainXDisplay, hDisplay, XWinGC, x1, y1, x2-x1+1, 
                       y2-y1+1 );
    #endif
  } 

VOID XWINDRIVER::DrawPixel ( HDISPLAY hDisplay, LONG x, LONG y,
                            COLOR Color )
  {
    if (hDisplay)
      {}
    if (x&y)
      {}
    if (Color)
      {}      
    #if defined (__FORUNIX__)      
      XSetForeground ( MainXDisplay, XWinGC, Color );
      
      XDrawPoint ( MainXDisplay, hDisplay, XWinGC, x, y );
    #endif
  } 

VOID XWINDRIVER::DrawEllipse ( HDISPLAY hDisplay, LONG Cx, LONG Cy, LONG Rx, LONG Ry,
                              COLOR Color )
  {
    if (hDisplay)
      {}
    if (Cx&Cy&Rx&Ry)
      {}
    if (Color)
      {}      
    #if defined (__FORUNIX__)      
      XSetForeground ( MainXDisplay, XWinGC, Color );      
      XDrawArc ( MainXDisplay, hDisplay, XWinGC, Cx-Rx, Cy-Ry, Rx*2, Ry*2,
                 0, 360*64 );
    #endif
  } 

VOID XWINDRIVER::FillEllipse ( HDISPLAY hDisplay, LONG Cx, LONG Cy,
                              LONG Rx, LONG Ry, COLOR Color )
  {
    if (hDisplay)
      {}
    if (Cx&Cy&Rx&Ry)
      {}
    if (Color)
      {}      
    #if defined (__FORUNIX__)      
      XSetForeground ( MainXDisplay, XWinGC, Color );      
      XDrawArc ( MainXDisplay, hDisplay, XWinGC, Cx-Rx, Cy-Ry, Rx*2, Ry*2,
                 0, 360*64 );
    #endif
  } 

