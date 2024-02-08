#include "clock.h"

qClock_t* gmClock_Create()
{
   qClock_t* clock = (qClock_t*)qAlloc( sizeof( qClock_t ), sfTrue );

   clock->sfmlClock = qInnerClock_Create();
   clock->frameDeltaSeconds = 1.0f / GAME_FPS;
   clock->maxFrameDurationMicro = (sfInt64)( (double)clock->frameDeltaSeconds * 1000000 );
   clock->totalFrameCount = 0;
   clock->lagFrameCount = 0;

   clock->absoluteStartMicro = sfClock_getElapsedTime( clock->sfmlClock ).microseconds;
   clock->realTotalDurationMicro = 0;

   return clock;
}

void qClock_Destroy( qClock_t* clock )
{
   qInnerClock_Destroy( clock->sfmlClock );

   qFree( clock, sizeof( qClock_t ), sfTrue );
}

void qClock_StartFrame( qClock_t* clock )
{
   clock->frameStartTime = sfClock_getElapsedTime( clock->sfmlClock );
}

void qClock_EndFrame( qClock_t* clock )
{
   sfTime frameEndTime = sfClock_getElapsedTime( clock->sfmlClock );
   sfTime sleepTime = { 0 };
   sfInt64 frameDurationMicro = frameEndTime.microseconds - clock->frameStartTime.microseconds;

   clock->totalFrameCount++;
   clock->realTotalDurationMicro = frameEndTime.microseconds - clock->absoluteStartMicro;

   if ( frameDurationMicro > clock->maxFrameDurationMicro )
   {
      clock->lagFrameCount++;
   }
   else
   {
      sleepTime.microseconds = clock->maxFrameDurationMicro - frameDurationMicro;
      sfSleep( sleepTime );
   }
}
