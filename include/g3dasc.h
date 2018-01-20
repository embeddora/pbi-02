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


#ifndef __G3DASC_H__
#define __G3DASC_H__

#include "stdgfx.h"
#include "g3dfile.h"

class G3DASCTOOL : public G3DFILETOOL
  {
    protected :
      
      INT ReadASCToken ( FILEHANDLE f, STRING String );
      INT GoToASCToken ( FILEHANDLE f, STRING String );
      INT GetASCNumber ( FILEHANDLE f, double *Number );
      INT GetASCInt ( FILEHANDLE f, LONG *Number );
      INT GetASCName ( FILEHANDLE f, STRING Name );
      BOOLEAN LoadASCPolyObject ( FILEHANDLE f,
                                  G3DPOLYHEDRON *Object, 
                                  double Scale );    
    public :
      G3DASCTOOL ();
      virtual ~G3DASCTOOL ();
      
      virtual BOOLEAN Load ( FILEHANDLE f, LINKEDLIST<G3DOBJECT*> *ObjectList,
                             BOOLEAN ClockWise, double Scale );
      virtual BOOLEAN Save ( FILEHANDLE f, LINKEDLIST<G3DOBJECT*> *ObjectList,
                             BOOLEAN ClockWise, double Scale );
  }; 

#endif


