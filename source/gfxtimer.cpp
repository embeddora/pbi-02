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



#include "stdgfx.h"
#include "gfxtimer.h"

MYTIMER::MYTIMER () : DCIClass ()
  {
  } 

MYTIMER::~MYTIMER ()
  {
  } 

BOOLEAN MYTIMER::Init ()
  {
    return SUCCESS;
  } 

VOID MYTIMER::DeInit ()
  {
  } 

VOID MYTIMER::StartTimer ()
  {
    StartClock = clock ();
  } 

VOID MYTIMER::EndTimer ()
  {
    EndClock = clock ();
  } 

double MYTIMER::GetFrameRate ( LONG Count )
  {
    #ifndef __FORUNIX__
      double FrameRate;

      if (StartClock==EndClock)
        return 0;

      FrameRate = (double)Count*CLOCKS_PER_SEC / (double)(EndClock-StartClock);
      return FrameRate;
    #else
      if (Count)
        {}
      return 0;
    #endif 
  } 

VOID MYTIMER::GetDateTime ( DATETIMETYPE *DateTime )
  {
    if (DateTime==NULL)
      return;
        
    #if defined (__FORWATCOM__)
      struct dosdate_t Date;
      struct dostime_t Time;

      _dos_getdate ( &Date );
      _dos_gettime ( &Time );
      
      DateTime->Year = Date.year;
      DateTime->Month = Date.month;
      DateTime->Day = Date.day;
      DateTime->Hour = Time.hour;
      DateTime->Minutes = Time.minute;
      
    #elif defined (__FORBORLAND__)
      struct date Date;
      struct time Time;

      getdate ( &Date );
      gettime ( &Time );

      DateTime->Year = Date.da_year;
      DateTime->Month = Date.da_mon;
      DateTime->Day = Date.da_day;
      DateTime->Hour = Time.ti_hour;
      DateTime->Minutes = Time.ti_min;
    #endif  
  } 


