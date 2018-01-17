




#include "stdgfx.h"
#include "lg3dras.h"

LONG  _LeftX;
LONG  _LeftY;
LONG  _LeftU;
LONG  _LeftV;
LONG  _LeftI;
LONG  _LeftZ;
LONG  _LeftH;
  
LONG  _RightX;
LONG  _RightY;
LONG  _RightU;
LONG  _RightV;
LONG  _RightI;
LONG  _RightZ;
LONG  _RightH;
  
LONG  _LeftDx;
LONG  _LeftDy;
LONG  _LeftDu;
LONG  _LeftDv;
LONG  _LeftDI;
LONG  _LeftDz;
LONG  _LeftDh;
  
LONG  _RightDx;
LONG  _RightDy;
LONG  _RightDu;
LONG  _RightDv;
LONG  _RightDI;
LONG  _RightDz;
LONG  _RightDh;
    
BYTE*  _DestBuffer;
BYTE*  _TextureBuffer;
LONG   _AddDest;
BYTE*  _ShadeBuffer;
BYTE*  _BlendBuffer;
BYTE*  _HazeBuffer;
float* _DivTable;

LONG  Color;
LONG  LoopCount;
LONG  VMinX;
LONG  VMaxX;
LONG  VMinY;
LONG  VMaxY;
LONG  TempLen;
LONG  TempVal;
  

LONG  InDeltaDI;
LONG  InDeltaDu;
LONG  InDeltaDv;
LONG  InDeltaDh;

SHORT InLoopCount;

float DummyFloat;





VOID _SetViewPortData ( LONG MinX, LONG MinY, 
                        LONG MaxX, LONG MaxY )
{
  VMinX = MinX;
  VMinY = MinY;
  VMaxX = MaxX;
  VMaxY = MaxY;
} 






VOID _DrawSolidNoHorz ( LONG nColor, LONG nLoopCount, 
                        LONG SkipRow, LONG SideClipped )
{
  if (SkipRow!=0)
    {
      _LeftX += _LeftDx*SkipRow;
      _RightX += _RightDx*SkipRow;
      _DestBuffer += _AddDest*SkipRow;
    } 

  LONG i,j,DrawLen;
  LONG DrawLeftX,DrawRightX;
  BYTE *DrawBuffer;
  nLoopCount -= SkipRow;

  if (!SideClipped)
    {
      for (i=0;i<nLoopCount;i++)
        {
          DrawLeftX = (_LeftX>>16);
          DrawRightX = (_RightX>>16);
          DrawBuffer = _DestBuffer + DrawLeftX;
          DrawLen = DrawRightX - DrawLeftX + 1;
          for (j=0;j<DrawLen;j++)
            {
              *DrawBuffer = (BYTE)nColor;
              DrawBuffer++;
            } 
          _DestBuffer += _AddDest;
          _LeftX += _LeftDx;
          _RightX += _RightDx;
        } 
    } 
  else
    {
      for (i=0;i<nLoopCount;i++)
        {
          DrawLeftX = (_LeftX>>16);
          DrawRightX = (_RightX>>16);
          if ((DrawLeftX<=VMaxX)&&(DrawRightX>=VMinX))
            {
              if (DrawLeftX<VMinX)
                {
                  DrawLeftX = VMinX;
                } 
              if (DrawRightX>VMaxX)
                {
                  DrawRightX = VMaxX;
                } 

              DrawBuffer = _DestBuffer + DrawLeftX;
              DrawLen = DrawRightX - DrawLeftX + 1;
              for (j=0;j<DrawLen;j++)
                {
                  *DrawBuffer = (BYTE)nColor;
                  DrawBuffer++;
                } 
            } 
          _DestBuffer += _AddDest;
          _LeftX += _LeftDx;
          _RightX += _RightDx;
        } 
    } 
} 






VOID _DrawSolidGrdHorz ( LONG nColor, LONG nLoopCount, LONG SkipRow, 
                         LONG SideClipped )
{
  if (SkipRow!=0)
    {
      _LeftX += _LeftDx*SkipRow;
      _RightX += _RightDx*SkipRow;
      _LeftI += _LeftDI*SkipRow;
      _RightI += _RightDI*SkipRow;
      _DestBuffer += _AddDest*SkipRow;
    } 

  nColor &= 0xFF;

  LONG i,j,DrawLen;
  LONG DrawLeftX,DrawRightX;
  LONG DrawLeftI,DrawRightI;
  BYTE *DrawBuffer;
  nLoopCount -= SkipRow;

  float DivResult;
  LONG ShadeOffset;

  if (!SideClipped)
    {
      for (i=0;i<nLoopCount;i++)
        {
          DrawLeftX = (_LeftX>>16);
          DrawRightX = (_RightX>>16);
          DrawLeftI = _LeftI;
          DrawRightI = _RightI;

          DrawBuffer = _DestBuffer + DrawLeftX;
          DrawLen = DrawRightX - DrawLeftX + 1;
          DivResult = _DivTable[DrawLen];
          
          if (DrawLen>0)
            InDeltaDI = (DrawRightI-DrawLeftI) / DrawLen;

          for (j=0;j<DrawLen;j++)
             {
               ShadeOffset = (DrawLeftI>>8)&0xFF00;
               *DrawBuffer = _ShadeBuffer[ShadeOffset+nColor];
               DrawBuffer++;
               DrawLeftI += InDeltaDI;
             } 
          _DestBuffer += _AddDest;
          _LeftX += _LeftDx;
          _RightX += _RightDx;
          _LeftI += _LeftDI;
          _RightI += _RightDI;
        } 
    } 
  else
    {
      for (i=0;i<nLoopCount;i++)
        {
          DrawLeftX = (_LeftX>>16);
          DrawRightX = (_RightX>>16);
          DrawLeftI = _LeftI;
          DrawRightI = _RightI;

          if ((DrawLeftX<=VMaxX)&&(DrawRightX>=VMinX))
            {
              DrawLen = DrawRightX - DrawLeftX + 1;
              DivResult = _DivTable[DrawLen];
              TempLen = 0;
              if (DrawLeftX<VMinX)
                {
                  TempLen = VMinX - DrawLeftX;
                  DrawLeftX = VMinX;
                } 
              if (DrawRightX>VMaxX)
                {
                  DrawRightX = VMaxX;
                } 

              
              if (DrawLen>0)
                InDeltaDI = (DrawRightI-DrawLeftI) / DrawLen;
              DrawLeftI += InDeltaDI*TempLen;
              DrawBuffer = _DestBuffer + DrawLeftX;
              DrawLen = DrawRightX - DrawLeftX + 1;
              for (j=0;j<DrawLen;j++)
                {
                  ShadeOffset = (DrawLeftI>>8)&0xFF00;
                  *DrawBuffer = _ShadeBuffer[ShadeOffset+nColor];
                  DrawBuffer++;
                  DrawLeftI += InDeltaDI;
                } 
            } 

          _DestBuffer += _AddDest;
          _LeftX += _LeftDx;
          _RightX += _RightDx;
          _LeftI += _LeftDI;
          _RightI += _RightDI;
        } 
    } 
} 






VOID _DrawTextureNoHorz ( LONG nColor, LONG nLoopCount, 
                          LONG SkipRow, LONG SideClipped )
{
  if (SkipRow!=0)
    {
      _LeftX += _LeftDx*SkipRow;
      _RightX += _RightDx*SkipRow;
      _LeftU += _LeftDu*SkipRow;
      _RightU += _RightDu*SkipRow;
      _LeftV += _LeftDv*SkipRow;
      _RightV += _RightDv*SkipRow;
      _DestBuffer += _AddDest*SkipRow;
    } 

  nColor &= 0xFF;

  LONG i,j,DrawLen;
  LONG DrawLeftX,DrawRightX;
  LONG DrawLeftU,DrawRightU;
  LONG DrawLeftV,DrawRightV;
  BYTE *DrawBuffer;
  nLoopCount -= SkipRow;

  float DivResult;
  LONG TexOffset;

  if (!SideClipped)
    {
      for (i=0;i<nLoopCount;i++)
        {
          DrawLeftX = (_LeftX>>16);
          DrawRightX = (_RightX>>16);
          DrawLeftU = _LeftU;
          DrawRightU = _RightU;
          DrawLeftV = _LeftV;
          DrawRightV = _RightV;

          DrawBuffer = _DestBuffer + DrawLeftX;
          DrawLen = DrawRightX - DrawLeftX + 1;
          DivResult = _DivTable[DrawLen];
          InDeltaDu = DivResult*(DrawRightU-DrawLeftU);
          InDeltaDv = DivResult*(DrawRightV-DrawLeftV);

          for (j=0;j<DrawLen;j++)
            {
              TexOffset = ((DrawLeftV>>8)&0xFF00) + (DrawLeftU>>16);
              *DrawBuffer = _TextureBuffer[TexOffset];
              DrawBuffer++;
              DrawLeftU += InDeltaDu;
              DrawLeftV += InDeltaDv;
            } 
          _DestBuffer += _AddDest;
          _LeftX += _LeftDx;
          _RightX += _RightDx;
          _LeftU += _LeftDu;
          _RightU += _RightDu;
          _LeftV += _LeftDv;
          _RightV += _RightDv;
        } 
    } 
  else
    {
      for (i=0;i<nLoopCount;i++)
        {
          DrawLeftX = (_LeftX>>16);
          DrawRightX = (_RightX>>16);
          DrawLeftU = _LeftU;
          DrawRightU = _RightU;
          DrawLeftV = _LeftV;
          DrawRightV = _RightV;

          if ((DrawLeftX<=VMaxX)&&(DrawRightX>=VMinX))
            {
              TempLen = DrawRightX - DrawLeftX + 1;
              DivResult = _DivTable[TempLen];
              TempLen = 0;
              if (DrawLeftX<VMinX)
                {
                  TempLen = VMinX - DrawLeftX;
                  DrawLeftX = VMinX;
                } 
              if (DrawRightX>VMaxX)
                {
                  DrawRightX = VMaxX;
                } 

              InDeltaDu = DivResult*(DrawRightU-DrawLeftU);
              InDeltaDv = DivResult*(DrawRightV-DrawLeftV);

              DrawLeftU += InDeltaDu*TempLen;
              DrawLeftV += InDeltaDv*TempLen;

              DrawBuffer = _DestBuffer + DrawLeftX;
              DrawLen = DrawRightX - DrawLeftX + 1;
              for (j=0;j<DrawLen;j++)
                {
                  TexOffset = ((DrawLeftV>>8)&0xFF00) + (DrawLeftU>>16);
                  *DrawBuffer = _TextureBuffer[TexOffset];
                  DrawBuffer++;
                  DrawLeftU += InDeltaDu;
                  DrawLeftV += InDeltaDv;
                } 
            } 

          _DestBuffer += _AddDest;
          _LeftX += _LeftDx;
          _RightX += _RightDx;
          _LeftU += _LeftDu;
          _RightU += _RightDu;
          _LeftV += _LeftDv;
          _RightV += _RightDv;
        } 
    } 
} 






VOID _DrawTextureFlatHorz ( LONG nColor, LONG nLoopCount, 
                            LONG SkipRow, LONG SideClipped )
{
  if (SkipRow!=0)
    {
      _LeftX += _LeftDx*SkipRow;
      _RightX += _RightDx*SkipRow;
      _LeftU += _LeftDu*SkipRow;
      _RightU += _RightDu*SkipRow;
      _LeftV += _LeftDv*SkipRow;
      _RightV += _RightDv*SkipRow;
      _DestBuffer += _AddDest*SkipRow;
    } 

  nColor &= 0xFF;

  LONG i,j,DrawLen;
  LONG DrawLeftX,DrawRightX;
  LONG DrawLeftU,DrawRightU;
  LONG DrawLeftV,DrawRightV;
  BYTE *DrawBuffer;
  nLoopCount -= SkipRow;

  float DivResult;
  LONG TexOffset;
  LONG ShadeLevel;
  BYTE TexColor;

  ShadeLevel = _LeftI>>8;  

  if (!SideClipped)
    {
      for (i=0;i<nLoopCount;i++)
        {
          DrawLeftX = (_LeftX>>16);
          DrawRightX = (_RightX>>16);
          DrawLeftU = _LeftU;
          DrawRightU = _RightU;
          DrawLeftV = _LeftV;
          DrawRightV = _RightV;

          DrawBuffer = _DestBuffer + DrawLeftX;
          DrawLen = DrawRightX - DrawLeftX + 1;
          DivResult = _DivTable[DrawLen];
          InDeltaDu = DivResult*(DrawRightU-DrawLeftU);
          InDeltaDv = DivResult*(DrawRightV-DrawLeftV);

          for (j=0;j<DrawLen;j++)
            {
              TexOffset = ((DrawLeftV>>8)&0xFF00) + (DrawLeftU>>16);
              TexColor = _TextureBuffer[TexOffset];
              *DrawBuffer = _ShadeBuffer[ShadeLevel+TexColor];
              DrawBuffer++;
              DrawLeftU += InDeltaDu;
              DrawLeftV += InDeltaDv;
            } 
          _DestBuffer += _AddDest;
          _LeftX += _LeftDx;
          _RightX += _RightDx;
          _LeftU += _LeftDu;
          _RightU += _RightDu;
          _LeftV += _LeftDv;
          _RightV += _RightDv;
        } 
    } 
  else
    {
      for (i=0;i<nLoopCount;i++)
        {
          DrawLeftX = (_LeftX>>16);
          DrawRightX = (_RightX>>16);
          DrawLeftU = _LeftU;
          DrawRightU = _RightU;
          DrawLeftV = _LeftV;
          DrawRightV = _RightV;

          if ((DrawLeftX<=VMaxX)&&(DrawRightX>=VMinX))
            {
              TempLen = DrawRightX - DrawLeftX + 1;
              DivResult = _DivTable[TempLen];
              TempLen = 0;
              if (DrawLeftX<VMinX)
                {
                  TempLen = VMinX - DrawLeftX;
                  DrawLeftX = VMinX;
                } 
              if (DrawRightX>VMaxX)
                {
                  DrawRightX = VMaxX;
                } 

              InDeltaDu = DivResult*(DrawRightU-DrawLeftU);
              InDeltaDv = DivResult*(DrawRightV-DrawLeftV);

              DrawLeftU += InDeltaDu*TempLen;
              DrawLeftV += InDeltaDv*TempLen;

              DrawBuffer = _DestBuffer + DrawLeftX;
              DrawLen = DrawRightX - DrawLeftX + 1;
              for (j=0;j<DrawLen;j++)
                {
                  TexOffset = ((DrawLeftV>>8)&0xFF00) + (DrawLeftU>>16);
                  TexColor = _TextureBuffer[TexOffset];
                  *DrawBuffer = _ShadeBuffer[ShadeLevel+TexColor];
                  DrawBuffer++;
                  DrawLeftU += InDeltaDu;
                  DrawLeftV += InDeltaDv;
                } 
            } 

          _DestBuffer += _AddDest;
          _LeftX += _LeftDx;
          _RightX += _RightDx;
          _LeftU += _LeftDu;
          _RightU += _RightDu;
          _LeftV += _LeftDv;
          _RightV += _RightDv;
        } 
    } 
} 






VOID _DrawTextureGrdHorz ( LONG nColor, LONG nLoopCount, 
                           LONG SkipRow, LONG SideClipped )
{
  if (SkipRow!=0)
    {
      _LeftX += _LeftDx*SkipRow;
      _RightX += _RightDx*SkipRow;
      _LeftU += _LeftDu*SkipRow;
      _RightU += _RightDu*SkipRow;
      _LeftV += _LeftDv*SkipRow;
      _RightV += _RightDv*SkipRow;
      _LeftI += _LeftDI*SkipRow;
      _RightI += _RightDI*SkipRow;
      _DestBuffer += _AddDest*SkipRow;
    } 

  nColor &= 0xFF;

  LONG i,j,DrawLen;
  LONG DrawLeftX,DrawRightX;
  LONG DrawLeftU,DrawRightU;
  LONG DrawLeftV,DrawRightV;
  LONG DrawLeftI,DrawRightI;
  BYTE *DrawBuffer;
  nLoopCount -= SkipRow;

  float DivResult;
  LONG TexOffset;
  LONG ShadeLevel;
  BYTE TexColor;

  ShadeLevel = _LeftI>>8;  

  if (!SideClipped)
    {
      for (i=0;i<nLoopCount;i++)
        {
          DrawLeftX = (_LeftX>>16);
          DrawRightX = (_RightX>>16);
          DrawLeftU = _LeftU;
          DrawRightU = _RightU;
          DrawLeftV = _LeftV;
          DrawRightV = _RightV;
          DrawLeftI = _LeftI;
          DrawRightI = _RightI;

          DrawBuffer = _DestBuffer + DrawLeftX;
          DrawLen = DrawRightX - DrawLeftX + 1;
          DivResult = _DivTable[DrawLen];
          InDeltaDu = DivResult*(DrawRightU-DrawLeftU);
          InDeltaDv = DivResult*(DrawRightV-DrawLeftV);
          
          if (DrawLen>0)
            InDeltaDI = (DrawRightI-DrawLeftI) / DrawLen;

          for (j=0;j<DrawLen;j++)
            {
              TexOffset = ((DrawLeftV>>8)&0xFF00) + (DrawLeftU>>16);
              TexColor = _TextureBuffer[TexOffset];
              ShadeLevel = (DrawLeftI>>8)&0xFF00;
              *DrawBuffer = _ShadeBuffer[ShadeLevel+TexColor];
              DrawBuffer++;
              DrawLeftU += InDeltaDu;
              DrawLeftV += InDeltaDv;
              DrawLeftI += InDeltaDI;
            } 
          _DestBuffer += _AddDest;
          _LeftX += _LeftDx;
          _RightX += _RightDx;
          _LeftU += _LeftDu;
          _RightU += _RightDu;
          _LeftV += _LeftDv;
          _RightV += _RightDv;
          _LeftI += _LeftDI;
          _RightI += _RightDI;
        } 
    } 
  else
    {
      for (i=0;i<nLoopCount;i++)
        {
          DrawLeftX = (_LeftX>>16);
          DrawRightX = (_RightX>>16);
          DrawLeftU = _LeftU;
          DrawRightU = _RightU;
          DrawLeftV = _LeftV;
          DrawRightV = _RightV;
          DrawLeftI = _LeftI;
          DrawRightI = _RightI;

          if ((DrawLeftX<=VMaxX)&&(DrawRightX>=VMinX))
            {
              DrawLen = DrawRightX - DrawLeftX + 1;
              DivResult = _DivTable[DrawLen];
              TempLen = 0;
              if (DrawLeftX<VMinX)
                {
                  TempLen = VMinX - DrawLeftX;
                  DrawLeftX = VMinX;
                } 
              if (DrawRightX>VMaxX)
                {
                  DrawRightX = VMaxX;
                } 

              InDeltaDu = DivResult*(DrawRightU-DrawLeftU);
              InDeltaDv = DivResult*(DrawRightV-DrawLeftV);
              
              if (DrawLen>0)
                InDeltaDI = (DrawRightI-DrawLeftI) / DrawLen;

               DrawLeftU += InDeltaDu*TempLen;
               DrawLeftV += InDeltaDv*TempLen;
               DrawLeftI += InDeltaDI*TempLen;

               DrawBuffer = _DestBuffer + DrawLeftX;
               DrawLen = DrawRightX - DrawLeftX + 1;
               for (j=0;j<DrawLen;j++)
                 {
                   TexOffset = ((DrawLeftV>>8)&0xFF00) + (DrawLeftU>>16);
                   TexColor = _TextureBuffer[TexOffset];
                   ShadeLevel = (DrawLeftI>>8)&0xFF00;
                   *DrawBuffer = _ShadeBuffer[ShadeLevel+TexColor];
                   DrawBuffer++;
                   DrawLeftU += InDeltaDu;
                   DrawLeftV += InDeltaDv;
                   DrawLeftI += InDeltaDI;
                 } 
             } 

           _DestBuffer += _AddDest;
           _LeftX += _LeftDx;
           _RightX += _RightDx;
           _LeftU += _LeftDu;
           _RightU += _RightDu;
           _LeftV += _LeftDv;
           _RightV += _RightDv;
           _LeftI += _LeftDI;
           _RightI += _RightDI;
         } 
     } 
} 






VOID _DrawSolidNoTransHorz ( LONG nColor, LONG nLoopCount, LONG SkipRow,
                             LONG SideClipped )
{
  nColor = nColor & 0x00FF;
    
  if (SkipRow!=0)
    {
      _LeftX += _LeftDx*SkipRow;
      _RightX += _RightDx*SkipRow;
      _DestBuffer += _AddDest*SkipRow;
    } 

  LONG i,j,DrawLen;
  LONG DrawLeftX,DrawRightX;
  BYTE *DrawBuffer;
  nLoopCount -= SkipRow;
  LONG bColor;

  nColor <<= 8;
   
  if (!SideClipped)
    {
      for (i=0;i<nLoopCount;i++)
        {
          DrawLeftX = (_LeftX>>16);
          DrawRightX = (_RightX>>16);
          DrawBuffer = _DestBuffer + DrawLeftX;
          DrawLen = DrawRightX - DrawLeftX + 1;
          for (j=0;j<DrawLen;j++)
            {
              bColor = *DrawBuffer;
              *DrawBuffer = (BYTE)_BlendBuffer[nColor+bColor];
              DrawBuffer++;
            } 
          _DestBuffer += _AddDest;
          _LeftX += _LeftDx;
          _RightX += _RightDx;
        } 
    } 
  else
    {
      for (i=0;i<nLoopCount;i++)
        {
          DrawLeftX = (_LeftX>>16);
          DrawRightX = (_RightX>>16);
          if ((DrawLeftX<=VMaxX)&&(DrawRightX>=VMinX))
            {
              if (DrawLeftX<VMinX)
                {
                  DrawLeftX = VMinX;
                } 
              if (DrawRightX>VMaxX)
                {
                  DrawRightX = VMaxX;
                } 

              DrawBuffer = _DestBuffer + DrawLeftX;
              DrawLen = DrawRightX - DrawLeftX + 1;
              for (j=0;j<DrawLen;j++)
                {
                  bColor = *DrawBuffer;
                  *DrawBuffer = (BYTE)_BlendBuffer[nColor+bColor];
                  DrawBuffer++;
                } 
            } 
          _DestBuffer += _AddDest;
          _LeftX += _LeftDx;
          _RightX += _RightDx;
        } 
    } 
} 
                             





VOID _DrawSolidGrdTransHorz ( LONG nColor, LONG nLoopCount, LONG SkipRow,
                              LONG SideClipped )
{
  nColor = nColor & 0x00FF;
  
  if (SkipRow!=0)
    {
      _LeftX += _LeftDx*SkipRow;
      _RightX += _RightDx*SkipRow;
      _LeftI += _LeftDI*SkipRow;
      _RightI += _RightDI*SkipRow;
      _DestBuffer += _AddDest*SkipRow;
    } 

  nColor &= 0xFF;

  LONG i,j,DrawLen;
  LONG DrawLeftX,DrawRightX;
  LONG DrawLeftI,DrawRightI;
  BYTE *DrawBuffer;
  nLoopCount -= SkipRow;

  float DivResult;
  LONG ShadeOffset;
  LONG bColor;
  LONG fColor;

  if (!SideClipped)
    {
      for (i=0;i<nLoopCount;i++)
        {
          DrawLeftX = (_LeftX>>16);
          DrawRightX = (_RightX>>16);
          DrawLeftI = _LeftI;
          DrawRightI = _RightI;

          DrawBuffer = _DestBuffer + DrawLeftX;
          DrawLen = DrawRightX - DrawLeftX + 1;
          DivResult = _DivTable[DrawLen];
          
          if (DrawLen>0)
            InDeltaDI = (DrawRightI-DrawLeftI) / DrawLen;

          for (j=0;j<DrawLen;j++)
             {
               ShadeOffset = (DrawLeftI>>8)&0xFF00;
               bColor = *DrawBuffer;
               fColor = _ShadeBuffer[ShadeOffset+nColor];
               *DrawBuffer = _BlendBuffer[(fColor<<8)+bColor];
               DrawBuffer++;
               DrawLeftI += InDeltaDI;
             } 
          _DestBuffer += _AddDest;
          _LeftX += _LeftDx;
          _RightX += _RightDx;
          _LeftI += _LeftDI;
          _RightI += _RightDI;
        } 
    } 
  else
    {
      for (i=0;i<nLoopCount;i++)
        {
          DrawLeftX = (_LeftX>>16);
          DrawRightX = (_RightX>>16);
          DrawLeftI = _LeftI;
          DrawRightI = _RightI;

          if ((DrawLeftX<=VMaxX)&&(DrawRightX>=VMinX))
            {
              DrawLen = DrawRightX - DrawLeftX + 1;
              DivResult = _DivTable[DrawLen];
              TempLen = 0;
              if (DrawLeftX<VMinX)
                {
                  TempLen = VMinX - DrawLeftX;
                  DrawLeftX = VMinX;
                } 
              if (DrawRightX>VMaxX)
                {
                  DrawRightX = VMaxX;
                } 

              
              if (DrawLen>0)
                InDeltaDI = (DrawRightI-DrawLeftI) / DrawLen;
              DrawLeftI += InDeltaDI*TempLen;
              DrawBuffer = _DestBuffer + DrawLeftX;
              DrawLen = DrawRightX - DrawLeftX + 1;
              for (j=0;j<DrawLen;j++)
                {
                  ShadeOffset = (DrawLeftI>>8)&0xFF00;
                  bColor = *DrawBuffer;
                  fColor = _ShadeBuffer[ShadeOffset+nColor];
                  *DrawBuffer = _BlendBuffer[(fColor<<8)+bColor];
                  DrawBuffer++;
                  DrawLeftI += InDeltaDI;
                } 
            } 

          _DestBuffer += _AddDest;
          _LeftX += _LeftDx;
          _RightX += _RightDx;
          _LeftI += _LeftDI;
          _RightI += _RightDI;
        } 
    } 
} 
                             





VOID _DrawTextureNoTransHorz ( LONG nColor, LONG nLoopCount, LONG SkipRow,
                               LONG SideClipped )
{
  if (SkipRow!=0)
    {
      _LeftX += _LeftDx*SkipRow;
      _RightX += _RightDx*SkipRow;
      _LeftU += _LeftDu*SkipRow;
      _RightU += _RightDu*SkipRow;
      _LeftV += _LeftDv*SkipRow;
      _RightV += _RightDv*SkipRow;
      _DestBuffer += _AddDest*SkipRow;
    } 

  nColor &= 0xFF;

  LONG i,j,DrawLen;
  LONG DrawLeftX,DrawRightX;
  LONG DrawLeftU,DrawRightU;
  LONG DrawLeftV,DrawRightV;
  BYTE *DrawBuffer;
  nLoopCount -= SkipRow;

  float DivResult;
  LONG TexOffset;
  LONG bColor;
  LONG fColor;

  if (!SideClipped)
    {
      for (i=0;i<nLoopCount;i++)
        {
          DrawLeftX = (_LeftX>>16);
          DrawRightX = (_RightX>>16);
          DrawLeftU = _LeftU;
          DrawRightU = _RightU;
          DrawLeftV = _LeftV;
          DrawRightV = _RightV;

          DrawBuffer = _DestBuffer + DrawLeftX;
          DrawLen = DrawRightX - DrawLeftX + 1;
          DivResult = _DivTable[DrawLen];
          InDeltaDu = DivResult*(DrawRightU-DrawLeftU);
          InDeltaDv = DivResult*(DrawRightV-DrawLeftV);

          for (j=0;j<DrawLen;j++)
            {
              TexOffset = ((DrawLeftV>>8)&0xFF00) + (DrawLeftU>>16);
              bColor = *DrawBuffer;
              fColor = _TextureBuffer[TexOffset];
              *DrawBuffer = _BlendBuffer[(fColor<<8)+bColor];
              DrawBuffer++;
              DrawLeftU += InDeltaDu;
              DrawLeftV += InDeltaDv;
            } 
          _DestBuffer += _AddDest;
          _LeftX += _LeftDx;
          _RightX += _RightDx;
          _LeftU += _LeftDu;
          _RightU += _RightDu;
          _LeftV += _LeftDv;
          _RightV += _RightDv;
        } 
    } 
  else
    {
      for (i=0;i<nLoopCount;i++)
        {
          DrawLeftX = (_LeftX>>16);
          DrawRightX = (_RightX>>16);
          DrawLeftU = _LeftU;
          DrawRightU = _RightU;
          DrawLeftV = _LeftV;
          DrawRightV = _RightV;

          if ((DrawLeftX<=VMaxX)&&(DrawRightX>=VMinX))
            {
              TempLen = DrawRightX - DrawLeftX + 1;
              DivResult = _DivTable[TempLen];
              TempLen = 0;
              if (DrawLeftX<VMinX)
                {
                  TempLen = VMinX - DrawLeftX;
                  DrawLeftX = VMinX;
                } 
              if (DrawRightX>VMaxX)
                {
                  DrawRightX = VMaxX;
                } 

              InDeltaDu = DivResult*(DrawRightU-DrawLeftU);
              InDeltaDv = DivResult*(DrawRightV-DrawLeftV);

              DrawLeftU += InDeltaDu*TempLen;
              DrawLeftV += InDeltaDv*TempLen;

              DrawBuffer = _DestBuffer + DrawLeftX;
              DrawLen = DrawRightX - DrawLeftX + 1;
              for (j=0;j<DrawLen;j++)
                {
                  TexOffset = ((DrawLeftV>>8)&0xFF00) + (DrawLeftU>>16);
                  bColor = *DrawBuffer;
                  fColor = _TextureBuffer[TexOffset];
                  *DrawBuffer = _BlendBuffer[(fColor<<8)+bColor];
                  DrawBuffer++;
                  DrawLeftU += InDeltaDu;
                  DrawLeftV += InDeltaDv;
                } 
            } 

          _DestBuffer += _AddDest;
          _LeftX += _LeftDx;
          _RightX += _RightDx;
          _LeftU += _LeftDu;
          _RightU += _RightDu;
          _LeftV += _LeftDv;
          _RightV += _RightDv;
        } 
    } 
} 
                             





VOID _DrawTextureFlatTransHorz ( LONG nColor, LONG nLoopCount, LONG SkipRow,
                                 LONG SideClipped )
{
  if (SkipRow!=0)
    {
      _LeftX += _LeftDx*SkipRow;
      _RightX += _RightDx*SkipRow;
      _LeftU += _LeftDu*SkipRow;
      _RightU += _RightDu*SkipRow;
      _LeftV += _LeftDv*SkipRow;
      _RightV += _RightDv*SkipRow;
      _DestBuffer += _AddDest*SkipRow;
    } 

  nColor &= 0xFF;

  LONG i,j,DrawLen;
  LONG DrawLeftX,DrawRightX;
  LONG DrawLeftU,DrawRightU;
  LONG DrawLeftV,DrawRightV;
  BYTE *DrawBuffer;
  nLoopCount -= SkipRow;

  float DivResult;
  LONG TexOffset;
  LONG ShadeLevel;
  BYTE TexColor;
  LONG bColor;
  LONG fColor;

  ShadeLevel = _LeftI>>8;  

  if (!SideClipped)
    {
      for (i=0;i<nLoopCount;i++)
        {
          DrawLeftX = (_LeftX>>16);
          DrawRightX = (_RightX>>16);
          DrawLeftU = _LeftU;
          DrawRightU = _RightU;
          DrawLeftV = _LeftV;
          DrawRightV = _RightV;

          DrawBuffer = _DestBuffer + DrawLeftX;
          DrawLen = DrawRightX - DrawLeftX + 1;
          DivResult = _DivTable[DrawLen];
          InDeltaDu = DivResult*(DrawRightU-DrawLeftU);
          InDeltaDv = DivResult*(DrawRightV-DrawLeftV);

          for (j=0;j<DrawLen;j++)
            {
              TexOffset = ((DrawLeftV>>8)&0xFF00) + (DrawLeftU>>16);
              TexColor = _TextureBuffer[TexOffset];
              bColor = *DrawBuffer;
              fColor = _ShadeBuffer[ShadeLevel+TexColor];
              *DrawBuffer = _BlendBuffer[(fColor<<8)+bColor];
              DrawBuffer++;
              DrawLeftU += InDeltaDu;
              DrawLeftV += InDeltaDv;
            } 
          _DestBuffer += _AddDest;
          _LeftX += _LeftDx;
          _RightX += _RightDx;
          _LeftU += _LeftDu;
          _RightU += _RightDu;
          _LeftV += _LeftDv;
          _RightV += _RightDv;
        } 
    } 
  else
    {
      for (i=0;i<nLoopCount;i++)
        {
          DrawLeftX = (_LeftX>>16);
          DrawRightX = (_RightX>>16);
          DrawLeftU = _LeftU;
          DrawRightU = _RightU;
          DrawLeftV = _LeftV;
          DrawRightV = _RightV;

          if ((DrawLeftX<=VMaxX)&&(DrawRightX>=VMinX))
            {
              TempLen = DrawRightX - DrawLeftX + 1;
              DivResult = _DivTable[TempLen];
              TempLen = 0;
              if (DrawLeftX<VMinX)
                {
                  TempLen = VMinX - DrawLeftX;
                  DrawLeftX = VMinX;
                } 
              if (DrawRightX>VMaxX)
                {
                  DrawRightX = VMaxX;
                } 

              InDeltaDu = DivResult*(DrawRightU-DrawLeftU);
              InDeltaDv = DivResult*(DrawRightV-DrawLeftV);

              DrawLeftU += InDeltaDu*TempLen;
              DrawLeftV += InDeltaDv*TempLen;

              DrawBuffer = _DestBuffer + DrawLeftX;
              DrawLen = DrawRightX - DrawLeftX + 1;
              for (j=0;j<DrawLen;j++)
                {
                  TexOffset = ((DrawLeftV>>8)&0xFF00) + (DrawLeftU>>16);
                  TexColor = _TextureBuffer[TexOffset];
                  bColor = *DrawBuffer;
                  fColor = _ShadeBuffer[ShadeLevel+TexColor];
                  *DrawBuffer = _BlendBuffer[(fColor<<8)+bColor];
                  DrawBuffer++;
                  DrawLeftU += InDeltaDu;
                  DrawLeftV += InDeltaDv;
                } 
            } 

          _DestBuffer += _AddDest;
          _LeftX += _LeftDx;
          _RightX += _RightDx;
          _LeftU += _LeftDu;
          _RightU += _RightDu;
          _LeftV += _LeftDv;
          _RightV += _RightDv;
        } 
    } 
} 
                             





VOID _DrawTextureGrdTransHorz ( LONG nColor, LONG nLoopCount, LONG SkipRow,
                                LONG SideClipped )
{
  if (SkipRow!=0)
    {
      _LeftX += _LeftDx*SkipRow;
      _RightX += _RightDx*SkipRow;
      _LeftU += _LeftDu*SkipRow;
      _RightU += _RightDu*SkipRow;
      _LeftV += _LeftDv*SkipRow;
      _RightV += _RightDv*SkipRow;
      _LeftI += _LeftDI*SkipRow;
      _RightI += _RightDI*SkipRow;
      _DestBuffer += _AddDest*SkipRow;
    } 

  nColor &= 0xFF;

  LONG i,j,DrawLen;
  LONG DrawLeftX,DrawRightX;
  LONG DrawLeftU,DrawRightU;
  LONG DrawLeftV,DrawRightV;
  LONG DrawLeftI,DrawRightI;
  BYTE *DrawBuffer;
  nLoopCount -= SkipRow;

  float DivResult;
  LONG TexOffset;
  LONG ShadeLevel;
  BYTE TexColor;
  LONG bColor;
  LONG fColor;

  ShadeLevel = _LeftI>>8;  

  if (!SideClipped)
    {
      for (i=0;i<nLoopCount;i++)
        {
          DrawLeftX = (_LeftX>>16);
          DrawRightX = (_RightX>>16);
          DrawLeftU = _LeftU;
          DrawRightU = _RightU;
          DrawLeftV = _LeftV;
          DrawRightV = _RightV;
          DrawLeftI = _LeftI;
          DrawRightI = _RightI;

          DrawBuffer = _DestBuffer + DrawLeftX;
          DrawLen = DrawRightX - DrawLeftX + 1;
          DivResult = _DivTable[DrawLen];
          InDeltaDu = DivResult*(DrawRightU-DrawLeftU);
          InDeltaDv = DivResult*(DrawRightV-DrawLeftV);
          
          if (DrawLen>0)
            InDeltaDI = (DrawRightI-DrawLeftI) / DrawLen;

          for (j=0;j<DrawLen;j++)
            {
              TexOffset = ((DrawLeftV>>8)&0xFF00) + (DrawLeftU>>16);
              TexColor = _TextureBuffer[TexOffset];
              bColor = *DrawBuffer;
              ShadeLevel = (DrawLeftI>>8)&0xFF00;
              fColor = _ShadeBuffer[ShadeLevel+TexColor];
              *DrawBuffer = _BlendBuffer[(fColor<<8)+bColor];
              DrawBuffer++;
              DrawLeftU += InDeltaDu;
              DrawLeftV += InDeltaDv;
              DrawLeftI += InDeltaDI;
            } 
          _DestBuffer += _AddDest;
          _LeftX += _LeftDx;
          _RightX += _RightDx;
          _LeftU += _LeftDu;
          _RightU += _RightDu;
          _LeftV += _LeftDv;
          _RightV += _RightDv;
          _LeftI += _LeftDI;
          _RightI += _RightDI;
        } 
    } 
  else
    {
      for (i=0;i<nLoopCount;i++)
        {
          DrawLeftX = (_LeftX>>16);
          DrawRightX = (_RightX>>16);
          DrawLeftU = _LeftU;
          DrawRightU = _RightU;
          DrawLeftV = _LeftV;
          DrawRightV = _RightV;
          DrawLeftI = _LeftI;
          DrawRightI = _RightI;

          if ((DrawLeftX<=VMaxX)&&(DrawRightX>=VMinX))
            {
              DrawLen = DrawRightX - DrawLeftX + 1;
              DivResult = _DivTable[DrawLen];
              TempLen = 0;
              if (DrawLeftX<VMinX)
                {
                  TempLen = VMinX - DrawLeftX;
                  DrawLeftX = VMinX;
                } 
              if (DrawRightX>VMaxX)
                {
                  DrawRightX = VMaxX;
                } 

              InDeltaDu = DivResult*(DrawRightU-DrawLeftU);
              InDeltaDv = DivResult*(DrawRightV-DrawLeftV);
              
              if (DrawLen>0)
                InDeltaDI = (DrawRightI-DrawLeftI) / DrawLen;

               DrawLeftU += InDeltaDu*TempLen;
               DrawLeftV += InDeltaDv*TempLen;
               DrawLeftI += InDeltaDI*TempLen;

               DrawBuffer = _DestBuffer + DrawLeftX;
               DrawLen = DrawRightX - DrawLeftX + 1;
               for (j=0;j<DrawLen;j++)
                 {
                   TexOffset = ((DrawLeftV>>8)&0xFF00) + (DrawLeftU>>16);
                   TexColor = _TextureBuffer[TexOffset];
                   bColor = *DrawBuffer;
                   ShadeLevel = (DrawLeftI>>8)&0xFF00;
                   fColor = _ShadeBuffer[ShadeLevel+TexColor];
                   *DrawBuffer = _BlendBuffer[(fColor<<8)+bColor];
                   DrawBuffer++;
                   DrawLeftU += InDeltaDu;
                   DrawLeftV += InDeltaDv;
                   DrawLeftI += InDeltaDI;
                 } 
             } 

           _DestBuffer += _AddDest;
           _LeftX += _LeftDx;
           _RightX += _RightDx;
           _LeftU += _LeftDu;
           _RightU += _RightDu;
           _LeftV += _LeftDv;
           _RightV += _RightDv;
           _LeftI += _LeftDI;
           _RightI += _RightDI;
         } 
     } 
} 
                             
                                      





VOID _DrawSolidNoHazeHorz ( LONG nColor, LONG nLoopCount, LONG SkipRow,
                            LONG SideClipped )
{
  if (SkipRow!=0)
    {
      _LeftX += _LeftDx*SkipRow;
      _RightX += _RightDx*SkipRow;
      _LeftH += _LeftDh*SkipRow;
      _RightH += _RightDh*SkipRow;
      _DestBuffer += _AddDest*SkipRow;
    } 

  nColor &= 0xFF;

  LONG i,j,DrawLen;
  LONG DrawLeftX,DrawRightX;
  LONG DrawLeftH,DrawRightH;
  BYTE *DrawBuffer;
  nLoopCount -= SkipRow;

  float DivResult;
  LONG HazeOffset;

  if (!SideClipped)
    {
      for (i=0;i<nLoopCount;i++)
        {
          DrawLeftX = (_LeftX>>16);
          DrawRightX = (_RightX>>16);
          DrawLeftH = _LeftH;
          DrawRightH = _RightH;

          DrawBuffer = _DestBuffer + DrawLeftX;
          DrawLen = DrawRightX - DrawLeftX + 1;
          DivResult = _DivTable[DrawLen];
          
          if (DrawLen>0)
            InDeltaDh = (DrawRightH-DrawLeftH) / DrawLen;

          for (j=0;j<DrawLen;j++)
             {
               HazeOffset = (DrawLeftH>>8)&0xFF00;
               *DrawBuffer = _HazeBuffer[HazeOffset+nColor];
               DrawBuffer++;
               DrawLeftH += InDeltaDh;
             } 
          _DestBuffer += _AddDest;
          _LeftX += _LeftDx;
          _RightX += _RightDx;
          _LeftH += _LeftDh;
          _RightH += _RightDh;
        } 
    } 
  else
    {
      for (i=0;i<nLoopCount;i++)
        {
          DrawLeftX = (_LeftX>>16);
          DrawRightX = (_RightX>>16);
          DrawLeftH = _LeftH;
          DrawRightH = _RightH;

          if ((DrawLeftX<=VMaxX)&&(DrawRightX>=VMinX))
            {
              DrawLen = DrawRightX - DrawLeftX + 1;
              DivResult = _DivTable[DrawLen];
              TempLen = 0;
              if (DrawLeftX<VMinX)
                {
                  TempLen = VMinX - DrawLeftX;
                  DrawLeftX = VMinX;
                } 
              if (DrawRightX>VMaxX)
                {
                  DrawRightX = VMaxX;
                } 

              
              if (DrawLen>0)
                InDeltaDh = (DrawRightH-DrawLeftH) / DrawLen;
              DrawLeftH += InDeltaDh*TempLen;
              DrawBuffer = _DestBuffer + DrawLeftX;
              DrawLen = DrawRightX - DrawLeftX + 1;
              for (j=0;j<DrawLen;j++)
                {
                  HazeOffset = (DrawLeftH>>8)&0xFF00;
                  *DrawBuffer = _HazeBuffer[HazeOffset+nColor];
                  DrawBuffer++;
                  DrawLeftH += InDeltaDh;
                } 
            } 

          _DestBuffer += _AddDest;
          _LeftX += _LeftDx;
          _RightX += _RightDx;
          _LeftH += _LeftDh;
          _RightH += _RightDh;
        } 
    } 
} 
                             





VOID _DrawSolidGrdHazeHorz ( LONG nColor, LONG nLoopCount, LONG SkipRow,
                             LONG SideClipped )
{
  if (SkipRow!=0)
    {
      _LeftX += _LeftDx*SkipRow;
      _RightX += _RightDx*SkipRow;
      _LeftI += _LeftDI*SkipRow;
      _RightI += _RightDI*SkipRow;
      _LeftH += _LeftDh*SkipRow;
      _RightH += _RightDh*SkipRow;
      _DestBuffer += _AddDest*SkipRow;
    } 

  nColor &= 0xFF;

  LONG i,j,DrawLen;
  LONG DrawLeftX,DrawRightX;
  LONG DrawLeftI,DrawRightI;
  LONG DrawLeftH,DrawRightH;
  BYTE *DrawBuffer;
  nLoopCount -= SkipRow;

  float DivResult;
  LONG ShadeOffset;
  LONG HazeOffset;

  if (!SideClipped)
    {
      for (i=0;i<nLoopCount;i++)
        {
          DrawLeftX = (_LeftX>>16);
          DrawRightX = (_RightX>>16);
          DrawLeftI = _LeftI;
          DrawRightI = _RightI;
          DrawLeftH = _LeftH;
          DrawRightH = _RightH;

          DrawBuffer = _DestBuffer + DrawLeftX;
          DrawLen = DrawRightX - DrawLeftX + 1;
          DivResult = _DivTable[DrawLen];
          
          if (DrawLen>0)
            {
              InDeltaDI = (DrawRightI-DrawLeftI) / DrawLen;
              InDeltaDh = (DrawRightH-DrawLeftH) / DrawLen;
            } 

          for (j=0;j<DrawLen;j++)
             {
               ShadeOffset = (DrawLeftI>>8)&0xFF00;
               HazeOffset = ((DrawLeftH>>8)&0xFF00) + _ShadeBuffer[ShadeOffset+nColor];
               *DrawBuffer = _HazeBuffer[HazeOffset];
               DrawBuffer++;
               DrawLeftI += InDeltaDI;
               DrawLeftH += InDeltaDh;
             } 
          _DestBuffer += _AddDest;
          _LeftX += _LeftDx;
          _RightX += _RightDx;
          _LeftI += _LeftDI;
          _RightI += _RightDI;
          _LeftH += _LeftDh;
          _RightH += _RightDh;
        } 
    } 
  else
    {
      for (i=0;i<nLoopCount;i++)
        {
          DrawLeftX = (_LeftX>>16);
          DrawRightX = (_RightX>>16);
          DrawLeftI = _LeftI;
          DrawRightI = _RightI;
          DrawLeftH = _LeftH;
          DrawRightH = _RightH;

          if ((DrawLeftX<=VMaxX)&&(DrawRightX>=VMinX))
            {
              DrawLen = DrawRightX - DrawLeftX + 1;
              DivResult = _DivTable[DrawLen];
              TempLen = 0;
              if (DrawLeftX<VMinX)
                {
                  TempLen = VMinX - DrawLeftX;
                  DrawLeftX = VMinX;
                } 
              if (DrawRightX>VMaxX)
                {
                  DrawRightX = VMaxX;
                } 

              
              if (DrawLen>0)
                {
                  InDeltaDI = (DrawRightI-DrawLeftI) / DrawLen;
                  InDeltaDh = (DrawRightH-DrawLeftH) / DrawLen;
                } 
              DrawLeftI += InDeltaDI*TempLen;
              DrawLeftH += InDeltaDh*TempLen;
              DrawBuffer = _DestBuffer + DrawLeftX;
              DrawLen = DrawRightX - DrawLeftX + 1;
              for (j=0;j<DrawLen;j++)
                {
                  ShadeOffset = (DrawLeftI>>8)&0xFF00;
                  HazeOffset = ((DrawLeftH>>8)&0xFF00) + _ShadeBuffer[ShadeOffset+nColor];
                  *DrawBuffer = _HazeBuffer[HazeOffset];
                  DrawBuffer++;
                  DrawLeftI += InDeltaDI;
                  DrawLeftH += InDeltaDh;
                } 
            } 

          _DestBuffer += _AddDest;
          _LeftX += _LeftDx;
          _RightX += _RightDx;
          _LeftI += _LeftDI;
          _RightI += _RightDI;
          _LeftH += _LeftDh;
          _RightH += _RightDh;
        } 
    } 
} 
                             





VOID _DrawTextureNoHazeHorz ( LONG nColor, LONG nLoopCount, LONG SkipRow,
                              LONG SideClipped )
{
  if (SkipRow!=0)
    {
      _LeftX += _LeftDx*SkipRow;
      _RightX += _RightDx*SkipRow;
      _LeftU += _LeftDu*SkipRow;
      _RightU += _RightDu*SkipRow;
      _LeftV += _LeftDv*SkipRow;
      _RightV += _RightDv*SkipRow;
      _LeftH += _LeftDh*SkipRow;
      _RightH += _RightDh*SkipRow;
      _DestBuffer += _AddDest*SkipRow;
    } 

  nColor &= 0xFF;

  LONG i,j,DrawLen;
  LONG DrawLeftX,DrawRightX;
  LONG DrawLeftU,DrawRightU;
  LONG DrawLeftV,DrawRightV;
  LONG DrawLeftH,DrawRightH;
  BYTE *DrawBuffer;
  nLoopCount -= SkipRow;

  float DivResult;
  LONG TexOffset;
  LONG HazeOffset;

  if (!SideClipped)
    {
      for (i=0;i<nLoopCount;i++)
        {
          DrawLeftX = (_LeftX>>16);
          DrawRightX = (_RightX>>16);
          DrawLeftU = _LeftU;
          DrawRightU = _RightU;
          DrawLeftV = _LeftV;
          DrawRightV = _RightV;
          DrawLeftH = _LeftH;
          DrawRightH = _RightH;

          DrawBuffer = _DestBuffer + DrawLeftX;
          DrawLen = DrawRightX - DrawLeftX + 1;
          DivResult = _DivTable[DrawLen];
          InDeltaDu = DivResult*(DrawRightU-DrawLeftU);
          InDeltaDv = DivResult*(DrawRightV-DrawLeftV);
          if (DrawLen>0)
            {
              InDeltaDh = (DrawRightH-DrawLeftH) / DrawLen;
            } 

          for (j=0;j<DrawLen;j++)
            {
              TexOffset = ((DrawLeftV>>8)&0xFF00) + (DrawLeftU>>16);
              HazeOffset = ((DrawLeftH>>8)&0xFF00) + _TextureBuffer[TexOffset];
              *DrawBuffer = _HazeBuffer[HazeOffset];
              DrawBuffer++;
              DrawLeftU += InDeltaDu;
              DrawLeftV += InDeltaDv;
              DrawLeftH += InDeltaDh;
            } 
          _DestBuffer += _AddDest;
          _LeftX += _LeftDx;
          _RightX += _RightDx;
          _LeftU += _LeftDu;
          _RightU += _RightDu;
          _LeftV += _LeftDv;
          _RightV += _RightDv;
          _LeftH += _LeftDh;
          _RightH += _RightDh;
        } 
    } 
  else
    {
      for (i=0;i<nLoopCount;i++)
        {
          DrawLeftX = (_LeftX>>16);
          DrawRightX = (_RightX>>16);
          DrawLeftU = _LeftU;
          DrawRightU = _RightU;
          DrawLeftV = _LeftV;
          DrawRightV = _RightV;
          DrawLeftH = _LeftH;
          DrawRightH = _RightH;

          if ((DrawLeftX<=VMaxX)&&(DrawRightX>=VMinX))
            {
              DrawLen = TempLen = DrawRightX - DrawLeftX + 1;
              DivResult = _DivTable[TempLen];
              TempLen = 0;
              if (DrawLeftX<VMinX)
                {
                  TempLen = VMinX - DrawLeftX;
                  DrawLeftX = VMinX;
                } 
              if (DrawRightX>VMaxX)
                {
                  DrawRightX = VMaxX;
                } 

              if (DrawLen>0)
                {
                  InDeltaDh = (DrawRightH-DrawLeftH) / DrawLen;
                } 
                
              InDeltaDu = DivResult*(DrawRightU-DrawLeftU);
              InDeltaDv = DivResult*(DrawRightV-DrawLeftV);

              DrawLeftU += InDeltaDu*TempLen;
              DrawLeftV += InDeltaDv*TempLen;
              DrawLeftH += InDeltaDh*TempLen;

              DrawBuffer = _DestBuffer + DrawLeftX;
              DrawLen = DrawRightX - DrawLeftX + 1;
                
              for (j=0;j<DrawLen;j++)
                {
                  TexOffset = ((DrawLeftV>>8)&0xFF00) + (DrawLeftU>>16);
                  HazeOffset = ((DrawLeftH>>8)&0xFF00) + _TextureBuffer[TexOffset];
                  *DrawBuffer = _HazeBuffer[HazeOffset];
                  DrawBuffer++;
                  DrawLeftU += InDeltaDu;
                  DrawLeftV += InDeltaDv;
                  DrawLeftH += InDeltaDh;
                } 
            } 

          _DestBuffer += _AddDest;
          _LeftX += _LeftDx;
          _RightX += _RightDx;
          _LeftU += _LeftDu;
          _RightU += _RightDu;
          _LeftV += _LeftDv;
          _RightV += _RightDv;
          _LeftH += _LeftDh;
          _RightH += _RightDh;
        } 
    } 
} 
                             





VOID _DrawTextureFlatHazeHorz ( LONG nColor, LONG nLoopCount, LONG SkipRow,
                                LONG SideClipped )
{
  if (SkipRow!=0)
    {
      _LeftX += _LeftDx*SkipRow;
      _RightX += _RightDx*SkipRow;
      _LeftU += _LeftDu*SkipRow;
      _RightU += _RightDu*SkipRow;
      _LeftV += _LeftDv*SkipRow;
      _RightV += _RightDv*SkipRow;
      _LeftH += _LeftDh*SkipRow;
      _RightH += _RightDh*SkipRow;
      _DestBuffer += _AddDest*SkipRow;
    } 

  nColor &= 0xFF;

  LONG i,j,DrawLen;
  LONG DrawLeftX,DrawRightX;
  LONG DrawLeftU,DrawRightU;
  LONG DrawLeftV,DrawRightV;
  LONG DrawLeftH,DrawRightH;
  BYTE *DrawBuffer;
  nLoopCount -= SkipRow;

  float DivResult;
  LONG TexOffset;
  LONG HazeOffset;
  LONG ShadeLevel;
  BYTE TexColor;

  ShadeLevel = _LeftI>>8;  

  if (!SideClipped)
    {
      for (i=0;i<nLoopCount;i++)
        {
          DrawLeftX = (_LeftX>>16);
          DrawRightX = (_RightX>>16);
          DrawLeftU = _LeftU;
          DrawRightU = _RightU;
          DrawLeftV = _LeftV;
          DrawRightV = _RightV;
          DrawLeftH = _LeftH;
          DrawRightH = _RightH;

          DrawBuffer = _DestBuffer + DrawLeftX;
          DrawLen = DrawRightX - DrawLeftX + 1;
          DivResult = _DivTable[DrawLen];
          InDeltaDu = DivResult*(DrawRightU-DrawLeftU);
          InDeltaDv = DivResult*(DrawRightV-DrawLeftV);

          if (DrawLen>0)
            {
              InDeltaDh = (DrawRightH-DrawLeftH) / DrawLen;
            } 
            
          for (j=0;j<DrawLen;j++)
            {
              TexOffset = ((DrawLeftV>>8)&0xFF00) + (DrawLeftU>>16);
              TexColor = _TextureBuffer[TexOffset];
              HazeOffset = ((DrawLeftH>>8)&0xFF00) + _ShadeBuffer[ShadeLevel+TexColor];
              *DrawBuffer = _HazeBuffer[HazeOffset];
              DrawBuffer++;
              DrawLeftU += InDeltaDu;
              DrawLeftV += InDeltaDv;
              DrawLeftH += InDeltaDh;
            } 
          _DestBuffer += _AddDest;
          _LeftX += _LeftDx;
          _RightX += _RightDx;
          _LeftU += _LeftDu;
          _RightU += _RightDu;
          _LeftV += _LeftDv;
          _RightV += _RightDv;
          _LeftH += _LeftDh;
          _RightH += _RightDh;
        } 
    } 
  else
    {
      for (i=0;i<nLoopCount;i++)
        {
          DrawLeftX = (_LeftX>>16);
          DrawRightX = (_RightX>>16);
          DrawLeftU = _LeftU;
          DrawRightU = _RightU;
          DrawLeftV = _LeftV;
          DrawRightV = _RightV;
          DrawLeftH = _LeftH;
          DrawRightH = _RightH;

          if ((DrawLeftX<=VMaxX)&&(DrawRightX>=VMinX))
            {
              DrawLen = TempLen = DrawRightX - DrawLeftX + 1;
              DivResult = _DivTable[TempLen];
              TempLen = 0;
              if (DrawLeftX<VMinX)
                {
                  TempLen = VMinX - DrawLeftX;
                  DrawLeftX = VMinX;
                } 
              if (DrawRightX>VMaxX)
                {
                  DrawRightX = VMaxX;
                } 

              InDeltaDu = DivResult*(DrawRightU-DrawLeftU);
              InDeltaDv = DivResult*(DrawRightV-DrawLeftV);

              if (DrawLen>0)
                {
                  InDeltaDh = (DrawRightH-DrawLeftH) / DrawLen;
                } 
            
              DrawLeftU += InDeltaDu*TempLen;
              DrawLeftV += InDeltaDv*TempLen;
              DrawLeftH += InDeltaDh*TempLen;

              DrawBuffer = _DestBuffer + DrawLeftX;
              DrawLen = DrawRightX - DrawLeftX + 1;
              for (j=0;j<DrawLen;j++)
                {
                  TexOffset = ((DrawLeftV>>8)&0xFF00) + (DrawLeftU>>16);
                  TexColor = _TextureBuffer[TexOffset];
                  HazeOffset = ((DrawLeftH>>8)&0xFF00) + _ShadeBuffer[ShadeLevel+TexColor];
                  *DrawBuffer = _HazeBuffer[HazeOffset];
                  DrawBuffer++;
                  DrawLeftU += InDeltaDu;
                  DrawLeftV += InDeltaDv;
                  DrawLeftH += InDeltaDh;
                } 
            } 

          _DestBuffer += _AddDest;
          _LeftX += _LeftDx;
          _RightX += _RightDx;
          _LeftU += _LeftDu;
          _RightU += _RightDu;
          _LeftV += _LeftDv;
          _RightV += _RightDv;
          _LeftH += _LeftDh;
          _RightH += _RightDh;
        } 
    } 
} 
                             





VOID _DrawTextureGrdHazeHorz ( LONG nColor, LONG nLoopCount, LONG SkipRow,
                               LONG SideClipped )
{
  if (SkipRow!=0)
    {
      _LeftX += _LeftDx*SkipRow;
      _RightX += _RightDx*SkipRow;
      _LeftU += _LeftDu*SkipRow;
      _RightU += _RightDu*SkipRow;
      _LeftV += _LeftDv*SkipRow;
      _RightV += _RightDv*SkipRow;
      _LeftI += _LeftDI*SkipRow;
      _RightI += _RightDI*SkipRow;
      _LeftH += _LeftDh*SkipRow;
      _RightH += _RightDh*SkipRow;
      _DestBuffer += _AddDest*SkipRow;
    } 

  nColor &= 0xFF;

  LONG i,j,DrawLen;
  LONG DrawLeftX,DrawRightX;
  LONG DrawLeftU,DrawRightU;
  LONG DrawLeftV,DrawRightV;
  LONG DrawLeftI,DrawRightI;
  LONG DrawLeftH,DrawRightH;
  BYTE *DrawBuffer;
  nLoopCount -= SkipRow;

  float DivResult;
  LONG TexOffset;
  LONG ShadeLevel;
  LONG HazeOffset;
  BYTE TexColor;

  ShadeLevel = _LeftI>>8;  

  if (!SideClipped)
    {
      for (i=0;i<nLoopCount;i++)
        {
          DrawLeftX = (_LeftX>>16);
          DrawRightX = (_RightX>>16);
          DrawLeftU = _LeftU;
          DrawRightU = _RightU;
          DrawLeftV = _LeftV;
          DrawRightV = _RightV;
          DrawLeftI = _LeftI;
          DrawRightI = _RightI;
          DrawLeftH = _LeftH;
          DrawRightH = _RightH;

          DrawBuffer = _DestBuffer + DrawLeftX;
          DrawLen = DrawRightX - DrawLeftX + 1;
          DivResult = _DivTable[DrawLen];
          InDeltaDu = DivResult*(DrawRightU-DrawLeftU);
          InDeltaDv = DivResult*(DrawRightV-DrawLeftV);
          
          if (DrawLen>0)
            {
              InDeltaDI = (DrawRightI-DrawLeftI) / DrawLen;
              InDeltaDh = (DrawRightH-DrawLeftH) / DrawLen;
            } 

          for (j=0;j<DrawLen;j++)
            {
              TexOffset = ((DrawLeftV>>8)&0xFF00) + (DrawLeftU>>16);
              TexColor = _TextureBuffer[TexOffset];
              ShadeLevel = (DrawLeftI>>8)&0xFF00;
              HazeOffset = ((DrawLeftH>>8)&0xFF00) + _ShadeBuffer[ShadeLevel+TexColor];
              *DrawBuffer = _HazeBuffer[HazeOffset];
              DrawBuffer++;
              DrawLeftU += InDeltaDu;
              DrawLeftV += InDeltaDv;
              DrawLeftI += InDeltaDI;
              DrawLeftH += InDeltaDh;
            } 
          _DestBuffer += _AddDest;
          _LeftX += _LeftDx;
          _RightX += _RightDx;
          _LeftU += _LeftDu;
          _RightU += _RightDu;
          _LeftV += _LeftDv;
          _RightV += _RightDv;
          _LeftI += _LeftDI;
          _RightI += _RightDI;
          _LeftH += _LeftDh;
          _RightH += _RightDh;
        } 
    } 
  else
    {
      for (i=0;i<nLoopCount;i++)
        {
          DrawLeftX = (_LeftX>>16);
          DrawRightX = (_RightX>>16);
          DrawLeftU = _LeftU;
          DrawRightU = _RightU;
          DrawLeftV = _LeftV;
          DrawRightV = _RightV;
          DrawLeftI = _LeftI;
          DrawRightI = _RightI;
          DrawLeftH = _LeftH;
          DrawRightH = _RightH;

          if ((DrawLeftX<=VMaxX)&&(DrawRightX>=VMinX))
            {
              DrawLen = DrawRightX - DrawLeftX + 1;
              DivResult = _DivTable[DrawLen];
              TempLen = 0;
              if (DrawLeftX<VMinX)
                {
                  TempLen = VMinX - DrawLeftX;
                  DrawLeftX = VMinX;
                } 
              if (DrawRightX>VMaxX)
                {
                  DrawRightX = VMaxX;
                } 

              InDeltaDu = DivResult*(DrawRightU-DrawLeftU);
              InDeltaDv = DivResult*(DrawRightV-DrawLeftV);
              
              if (DrawLen>0)
                {
                  InDeltaDI = (DrawRightI-DrawLeftI) / DrawLen;
                  InDeltaDh = (DrawRightH-DrawLeftH) / DrawLen;
                } 

               DrawLeftU += InDeltaDu*TempLen;
               DrawLeftV += InDeltaDv*TempLen;
               DrawLeftI += InDeltaDI*TempLen;
               DrawLeftH += InDeltaDh*TempLen;

               DrawBuffer = _DestBuffer + DrawLeftX;
               DrawLen = DrawRightX - DrawLeftX + 1;
               for (j=0;j<DrawLen;j++)
                 {
                   TexOffset = ((DrawLeftV>>8)&0xFF00) + (DrawLeftU>>16);
                   TexColor = _TextureBuffer[TexOffset];
                   ShadeLevel = (DrawLeftI>>8)&0xFF00;
                   HazeOffset = ((DrawLeftH>>8)&0xFF00) + _ShadeBuffer[ShadeLevel+TexColor];
                   *DrawBuffer = _HazeBuffer[HazeOffset];
                   DrawBuffer++;
                   DrawLeftU += InDeltaDu;
                   DrawLeftV += InDeltaDv;
                   DrawLeftI += InDeltaDI;
                   DrawLeftH += InDeltaDh;
                 } 
             } 

           _DestBuffer += _AddDest;
           _LeftX += _LeftDx;
           _RightX += _RightDx;
           _LeftU += _LeftDu;
           _RightU += _RightDu;
           _LeftV += _LeftDv;
           _RightV += _RightDv;
           _LeftI += _LeftDI;
           _RightI += _RightDI;
           _LeftH += _LeftDh;
           _RightH += _RightDh;
         } 
     } 
} 
                             

