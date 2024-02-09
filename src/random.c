#include <time.h>

#include "random.h"

void qRandom_Seed()
{
   srand( (uint32_t)time( 0 ) );
}

uint32_t qRandom_UInt32( uint32_t min, uint32_t max )
{
   // min and max inclusive
   return ( (uint32_t)rand() % ( ( max + 1 ) - min ) ) + min;
}

uint8_t qRandom_Percent()
{
   return (uint8_t)rand() % 101;
}
