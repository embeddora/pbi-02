




#ifndef __G3DLIGHT_H__
#define __G3DLIGHT_H__

#include "stdgfx.h"
#include "g3dobj.h"

#define OBJECT_TYPE_LIGHT  2

#define LIGHT_TYPE_SPOT    1
#define LIGHT_TYPE_DIR     2

class G3DLIGHT : public G3DOBJECT
  {
    protected :
      float MinIntensity,MaxIntensity;
      float ScaleFactor;
      COLORTABLE *ShadeTable;
      LONG LightType;
      
    public :
      G3DLIGHT ();
      virtual ~G3DLIGHT ();

      VOID SetShadeTable ( COLORTABLE *NewShadeTable );
      COLORTABLE* GetShadeTable () { return ShadeTable; };

      VOID SetLightType ( LONG Type ) { LightType = Type; };
      LONG GetLightType () { return LightType; };

      VOID SetMinIntensity ( float Intensity ) { MinIntensity = Intensity*65536; };
      VOID SetMaxIntensity ( float Intensity ) { MaxIntensity = Intensity*65536; };
      VOID SetScaleFactor ( float Factor ) { ScaleFactor = Factor*65536; };
      
      float ComputeIntensity ( FLPVECTOR3D *Point, FLPVECTOR3D *Normal );
      virtual VOID SetPosition ( float x, float y, float z );
  }; 

#endif



