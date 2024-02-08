#if !defined( CLOCK_H )
#define CLOCK_H

#include "common.h"

typedef struct qClock_t
{
   sfClock* sfmlClock;

   float frameDeltaSeconds;
   sfInt64 maxFrameDurationMicro;
   sfTime frameStartTime;
   uint32_t totalFrameCount;
   uint32_t lagFrameCount;

   sfInt64 absoluteStartMicro;
   sfInt64 realTotalDurationMicro;
}
qClock_t;

qClock_t* gmClock_Create();
void qClock_Destroy( qClock_t* clock );
void qClock_StartFrame( qClock_t* clock );
void qClock_EndFrame( qClock_t* clock );

#endif // CLOCK_H
