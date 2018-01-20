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



