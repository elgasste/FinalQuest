#if !defined( RANDOM_H )
#define RANDOM_H

#include "common.h"

void qRandom_Seed();
uint32_t qRandom_UInt32( uint32_t min, uint32_t max );
uint8_t qRandom_Percent();

#endif // RANDOM_H
