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


#ifndef __LG3D256_H__
#define __LG3D256_H__

#include "stdgfx.h"
#include "g3ddraw.h"

extern "C" {
    VOID _SetEdgeData ( FXPEDGE *LeftEdgePtr, FXPEDGE *RightEdgePtr,
                        LONG EdgeSize );
    VOID _SetDestData ( BYTE *DestBuffer, LONG AddDest );
    VOID _SetGradientData ( FXPGRADIENT *Gradient );
    VOID _SetShadeTable ( BYTE *Table );
    VOID _SetTextureBuffer ( BYTE *Buffer );

    FXPEDGE* _ScanEdgeSolid ( FXPPOINT2D *Point1, FXPPOINT2D *Point2,
                              FXPEDGE *EdgePtr, LONG ExtraHt );
    VOID _DrawSolidPoly ( LONG Color, LONG Intensity,
                          LONG Skip, LONG LoopCount );

    FXPEDGE* _ScanEdgeSolidGouraud ( FXPPOINT2D *Point1, FXPPOINT2D *Point2,
                                     FXPEDGE *EdgePtr, LONG ExtraHt );
    VOID _DrawSolidGouraudPoly ( LONG Color, LONG Intensity,
                                 LONG Skip, LONG LoopCount );

    FXPEDGE* _ScanEdgeTexture ( FXPPOINT2D *Point1, FXPPOINT2D *Point2,
                                FXPEDGE *EdgePtr, LONG ExtraHt );
    VOID _DrawTexturePoly ( LONG Color, LONG Intensity,
                            LONG Skip, LONG LoopCount );    
  }

#endif
