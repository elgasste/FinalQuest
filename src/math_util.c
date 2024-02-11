#include "math_util.h"

sfBool qMathUtil_RectsOverlap( float l1x, float l1y, float r1x, float r1y, float l2x, float l2y, float r2x, float r2y )
{
   if ( l1x > r2x || l2x > r1x )
   {
      return sfFalse;
   }

   if ( r1y > l2y || r2y > l1y )
   {
      return sfFalse;
   }

   return sfTrue;
}
