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


#ifndef __G3DPOLY_H__
#define __G3DPOLY_H__

#include "stdgfx.h"
#include "g3dshape.h"
#include "g3dmatrl.h"

class G3DPOLY : public G3DSHAPE
  {
    public :
      BOOLEAN Visible;
      LONG Points[3];
      float Intensity;
      FLPVECTOR2D TexCoords[3];
      G3DMATERIAL *Material;
    
      G3DPOLY ();
      virtual ~G3DPOLY ();
  }; 
  
#endif  





