




#include "stdgfx.h"
#include "g3dlight.h"

G3DLIGHT::G3DLIGHT () : G3DOBJECT ()
  {
    ShadeTable = NULL;
    MinIntensity = (float)0;
    MaxIntensity = (float)0;
    ScaleFactor = (float)0;
    Moved = Rotated = FALSE;
  } 

G3DLIGHT::~G3DLIGHT ()
  {
  } 

VOID G3DLIGHT::SetPosition ( float x, float y, float z )
  {
    G3DOBJECT::SetPosition ( (float)0, (float)0, (float)0 );
    Move ( x, y, z );
  } 

VOID G3DLIGHT::SetShadeTable ( COLORTABLE *NewShadeTable )
  {
    ShadeTable = NewShadeTable;
    if (ShadeTable!=NULL)
      {
        MinIntensity = (float)0;
        MaxIntensity = (float)ShadeTable->GetNumLevels()*65536;
        ScaleFactor = (float)ShadeTable->GetNumLevels ()*65536;  
      } 
  } 

float G3DLIGHT::ComputeIntensity ( FLPVECTOR3D *Point, FLPVECTOR3D *Normal )
  {
    FLPVECTOR3D Dir;
    float DotProduct;

    FLPVectorSub ( &FLPWorldPosition, Point, &Dir );
    FLPVectorNormalize ( &Dir );

    DotProduct = FLPVectorDot ( &Dir, Normal );
    if (DotProduct<0)
      return MinIntensity;

    float Intensity;

    Intensity = ScaleFactor*DotProduct;
    if (Intensity<MinIntensity)
      return MinIntensity;
    else if (Intensity>MaxIntensity)
      return MaxIntensity;
    return Intensity;  
  } 


