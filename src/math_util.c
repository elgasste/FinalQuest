#include "math_util.h"

sfBool qMathUtil_RectsOverlap( float l1x, float t1y, float r1x, float b1y, float l2x, float t2y, float r2x, float b2y )
{
   if ( l1x > r2x || l2x > r1x )
   {
      return sfFalse;
   }

   if ( t1y > b2y || t2y > b1y )
   {
      return sfFalse;
   }

   return sfTrue;
}
