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



#ifndef __GFXTIMER_H__
#define __GFXTIMER_H__

#include "stdgfx.h"
#include "gfxdev.h"
#include <time.h>

struct DATETIMETYPE
  {
    SHORT Year;
    BYTE  Month;
    BYTE  Day;
    BYTE  Hour;
    BYTE  Minutes;      
  }; 

class MYTIMER : public DCIClass
  {
    protected :
      LONG StartClock,EndClock;

    public :
      MYTIMER ();
      virtual ~MYTIMER ();

      virtual BOOLEAN Init ();
      virtual VOID DeInit ();

      VOID StartTimer ();
      VOID EndTimer ();
      double GetFrameRate ( LONG Count );

      VOID GetDateTime ( DATETIMETYPE *DateTime );
  }; 

#endif
