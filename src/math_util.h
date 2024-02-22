#if !defined( MATH_UTIL_H )
#define MATH_UTIL_H

#include "common.h"

sfBool qMathUtil_Vector2fEqual( sfVector2f* v1, sfVector2f* v2 );
sfBool qMathUtil_RectsOverlap( float l1x, float t1y, float r1x, float b1y, float l2x, float t2y, float r2x, float b2y );
uint32_t qMathUtil_ApplySpread( uint32_t* base, uint32_t* spread, sfBool allowZero );

#endif // MATH_UTIL_H
