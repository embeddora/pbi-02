




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


