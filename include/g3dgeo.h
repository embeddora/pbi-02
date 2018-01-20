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


#ifndef __G3DGEO_H__
#define __G3DGEO_H__

#include "stdgfx.h"
#include "g3dfile.h"

class G3DGEOTOOL : public G3DFILETOOL
  {
    protected :
      BOOLEAN LoadPoints ( FILEHANDLE f, G3DPOLYHEDRON *Object,
                           LONG NumPoints, double Scale );
      BOOLEAN LoadFaces ( FILEHANDLE f, G3DPOLYHEDRON *Object );
      
      BOOLEAN SavePoints ( FILEHANDLE f, G3DPOLYHEDRON *Object,
                           LONG NumPoints, double Scale );
      BOOLEAN SaveFaces ( FILEHANDLE f, G3DPOLYHEDRON *Object );
    
    public :
      G3DGEOTOOL ();
      virtual ~G3DGEOTOOL ();

      virtual BOOLEAN Load ( FILEHANDLE f, LINKEDLIST<G3DOBJECT*> *ObjectList,
                             BOOLEAN ClockWise, double Scale );
      virtual BOOLEAN Save ( FILEHANDLE f, LINKEDLIST<G3DOBJECT*> *ObjectList,
                             BOOLEAN ClockWise, double Scale );
  }; 

#endif


