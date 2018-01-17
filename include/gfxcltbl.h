




#ifndef __GFXCLTABLE_H__
#define __GFXCLTABLE_H__

#include "stdgfx.h"
#include "gfxpal.h"
#include "gfxerror.h"

#define TABLE_MATCH  1
#define TABLE_SHADE  2
#define TABLE_HAZE   3
#define TABLE_BLEND  4

typedef LONG (*COLTBLPROC) ( float Percentage );

class COLORTABLE : public MYOBJECT
  {
    protected :
      LONG NumLevels;
      LONG NumColors;
      LONG TableType;
      BYTE *Table;
      COLTBLPROC ProcessIndicator;
      RGBPALETTE *Pal;
       
      BOOLEAN Create ( LONG NumL, LONG NumCol );
       
    public :
      COLORTABLE ();
      virtual ~COLORTABLE ();
      
      VOID Destroy ();
      
      LONG GetNumColors () { return NumColors; };
      LONG GetNumLevels () { return NumLevels; };
      BYTE *GetTable () { return Table; };
      LONG GetTableType () { return TableType; };

      RGBPALETTE* GetPalette () { return Pal; };
      VOID CopyPalette ( RGBPALETTE *Pal );
            
      BOOLEAN CreateMatchTable ( RGBPALETTE *Pal2 );
      BOOLEAN CreateShadeTable ( LONG NumLevels, LONG Highest, LONG Lowest );
      BOOLEAN CreateHazeTable ( RGBCOLOR TargetColor, LONG NumLevel );
      BOOLEAN CreateBlendTable ( LONG NumLevels, float StartRatio,
                                 float EndRatio );
      BOOLEAN Save ( STRING FileName );
      BOOLEAN Load ( STRING FileName );

      VOID SetProcessIndicator ( COLTBLPROC Proc );
  }; 

#endif

