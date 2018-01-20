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


#ifndef __G3DCAMERA_H__
#define __G3DCAMERA_H__

#include "stdgfx.h"
#include "g3dobj.h"

#define OBJECT_TYPE_CAMERA  3

class G3DCAMERA : public G3DOBJECT
  {
    protected :      
    public :      
      G3DCAMERA ();
      virtual ~G3DCAMERA ();

      virtual VOID SetPosition ( float x, float y, float z );
  }; 

#endif

