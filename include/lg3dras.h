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



#ifndef __LG3D_H__
#define __LG3D_H__

#include "stdgfx.h"
#include "g3ddef.h"

#if defined (__USEASM__)
  extern "C"
    {
      LONG _LeftX, _LeftY, _LeftU, _LeftV, _LeftI, _LeftH, _LeftZ;  
      LONG _RightX, _RightY, _RightU, _RightV, _RightI, _RightH, _RightZ;
      LONG _LeftDx, _LeftDy, _LeftDu, _LeftDv, _LeftDI, _LeftDh, _LeftDz;  
      LONG _RightDx, _RightDy, _RightDu, _RightDv, _RightDI, _RightDh, _RightDz;
      BYTE *_DestBuffer;
      LONG _AddDest;
      BYTE *_ShadeBuffer;
      BYTE *_TextureBuffer;
      BYTE *_BlendBuffer;
      BYTE *_HazeBuffer;
      float *_DivTable;
    
      VOID _SetViewPortData ( LONG MinX, LONG MinY, LONG MaxX, LONG MaxY );
      VOID _SetFXPGradientData ( FXPGRADIENT *Gradient );
      
      VOID _DrawSolidNoHorz ( LONG Color, LONG LoopCount, LONG SkipRow,
                              LONG SideClipped );
      VOID _DrawSolidGrdHorz ( LONG Color, LONG LoopCount, LONG SkipRow,
                               LONG SideClipped );
      VOID _DrawTextureNoHorz ( LONG Color, LONG LoopCount, LONG SkipRow,
                                LONG SideClipped );
      VOID _DrawTextureFlatHorz ( LONG Color, LONG LoopCount, LONG SkipRow,
                                  LONG SideClipped );
      VOID _DrawTextureGrdHorz ( LONG Color, LONG LoopCount, LONG SkipRow,
                                 LONG SideClipped );

      VOID _DrawSolidNoTransHorz ( LONG Color, LONG LoopCount, LONG SkipRow,
                                   LONG SideClipped );
      VOID _DrawSolidGrdTransHorz ( LONG Color, LONG LoopCount, LONG SkipRow,
                                    LONG SideClipped );
      VOID _DrawTextureNoTransHorz ( LONG Color, LONG LoopCount, LONG SkipRow,
                                     LONG SideClipped );
      VOID _DrawTextureFlatTransHorz ( LONG Color, LONG LoopCount, LONG SkipRow,
                                       LONG SideClipped );
      VOID _DrawTextureGrdTransHorz ( LONG Color, LONG LoopCount, LONG SkipRow,
                                      LONG SideClipped );

      VOID _DrawSolidNoHazeHorz ( LONG Color, LONG LoopCount, LONG SkipRow,
                                  LONG SideClipped );
      VOID _DrawSolidGrdHazeHorz ( LONG Color, LONG LoopCount, LONG SkipRow,
                                   LONG SideClipped );
      VOID _DrawTextureNoHazeHorz ( LONG Color, LONG LoopCount, LONG SkipRow,
                                    LONG SideClipped );
      VOID _DrawTextureFlatHazeHorz ( LONG Color, LONG LoopCount, LONG SkipRow,
                                      LONG SideClipped );
      VOID _DrawTextureGrdHazeHorz ( LONG Color, LONG LoopCount, LONG SkipRow,
                                     LONG SideClipped );
    }
#else      
      extern LONG _LeftX, _LeftY, _LeftU, _LeftV, _LeftI, _LeftH, _LeftZ;  
      extern LONG _RightX, _RightY, _RightU, _RightV, _RightI, _RightH, _RightZ;
      extern LONG _LeftDx, _LeftDy, _LeftDu, _LeftDv, _LeftDI, _LeftDh, _LeftDz;  
      extern LONG _RightDx, _RightDy, _RightDu, _RightDv, _RightDI, _RightDh, _RightDz;
      extern BYTE *_DestBuffer;
      extern LONG _AddDest;
      extern BYTE *_ShadeBuffer;
      extern BYTE *_TextureBuffer;
      extern BYTE *_BlendBuffer;
      extern BYTE *_HazeBuffer;
      extern float *_DivTable;
    
      VOID _SetViewPortData ( LONG MinX, LONG MinY, LONG MaxX, LONG MaxY );
      
      VOID _DrawSolidNoHorz ( LONG Color, LONG LoopCount, LONG SkipRow,
                              LONG SideClipped );
      VOID _DrawSolidGrdHorz ( LONG Color, LONG LoopCount, LONG SkipRow,
                               LONG SideClipped );
      VOID _DrawTextureNoHorz ( LONG Color, LONG LoopCount, LONG SkipRow,
                                LONG SideClipped );
      VOID _DrawTextureFlatHorz ( LONG Color, LONG LoopCount, LONG SkipRow,
                                  LONG SideClipped );
      VOID _DrawTextureGrdHorz ( LONG Color, LONG LoopCount, LONG SkipRow,
                                 LONG SideClipped );
                                 
      VOID _DrawSolidNoTransHorz ( LONG Color, LONG LoopCount, LONG SkipRow,
                                   LONG SideClipped );
      VOID _DrawSolidGrdTransHorz ( LONG Color, LONG LoopCount, LONG SkipRow,
                                    LONG SideClipped );
      VOID _DrawTextureNoTransHorz ( LONG Color, LONG LoopCount, LONG SkipRow,
                                     LONG SideClipped );
      VOID _DrawTextureFlatTransHorz ( LONG Color, LONG LoopCount, LONG SkipRow,
                                       LONG SideClipped );
      VOID _DrawTextureGrdTransHorz ( LONG Color, LONG LoopCount, LONG SkipRow,
                                      LONG SideClipped );
                                      
      VOID _DrawSolidNoHazeHorz ( LONG Color, LONG LoopCount, LONG SkipRow,
                                  LONG SideClipped );
      VOID _DrawSolidGrdHazeHorz ( LONG Color, LONG LoopCount, LONG SkipRow,
                                   LONG SideClipped );
      VOID _DrawTextureNoHazeHorz ( LONG Color, LONG LoopCount, LONG SkipRow,
                                    LONG SideClipped );
      VOID _DrawTextureFlatHazeHorz ( LONG Color, LONG LoopCount, LONG SkipRow,
                                      LONG SideClipped );
      VOID _DrawTextureGrdHazeHorz ( LONG Color, LONG LoopCount, LONG SkipRow,
                                     LONG SideClipped );
#endif

#endif

