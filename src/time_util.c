#include "time_util.h"

void qTimeUtil_FormatTime( char* str, size_t size, int32_t seconds )
{
   int32_t h = seconds / 3600;
   int32_t m = ( seconds - ( h * 3600 ) ) / 60;
   int32_t s = ( seconds - ( h * 3600 ) ) - ( m * 60 );

   snprintf( str, size, "%02d:%02d:%02d", h, m, s );
}
