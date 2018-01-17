


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
