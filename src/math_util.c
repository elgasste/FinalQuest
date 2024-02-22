#include "math_util.h"
#include "random.h"

sfBool qMathUtil_Vector2fEqual( sfVector2f* v1, sfVector2f* v2 )
{
   return v1->x == v2->x && v1->y == v2->y;
}

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

uint32_t qMathUtil_ApplySpread( uint32_t* base, uint32_t* spread, sfBool allowZero )
{
   uint32_t result;
   uint32_t delta = qRandom_UInt32( 0, *spread );
   sfBool add = qRandom_UInt32( 0, 1 );

   if ( add )
   {
      result = *base + delta;
   }
   else
   {
      result = ( delta <= *base ) ? *base - delta : 0;

      if ( result == 0 && !allowZero )
      {
         result = 1;
      }
   }

   return result;
}
