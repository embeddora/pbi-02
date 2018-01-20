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



#ifndef __GFXERROR_H__
#define __GFXERROR_H__

#include "stdgfx.h"
#include "gfxsys.h"

#define ERR_TYPE LONG


#define ERR_NOERROR   0
#define ERR_UNKNOWN   1
#define ERR_NOMEMORY  2   
#define ERR_NOEXIST   3   
#define ERR_NOVALID   4   
#define ERR_FILEIO    5   
#define ERR_USER      6   

class ERRORHANDLER : public SYSTEMTOOL
  {
    protected :
      ERR_TYPE LastError;

    public :
      ERRORHANDLER ();
      virtual ~ERRORHANDLER ();

      void SetError ( ERR_TYPE Err );
      ERR_TYPE GetError ();
  }; 

extern ERRORHANDLER Error;

#endif


