#if !defined( RENDER_STATES_H )
#define RENDER_STATES_H

#include "common.h"

typedef struct qClock_t qClock_t;

typedef struct qDebugBarRenderState_t
{
   sfBool isVisible;
   float elapsedSeconds;
   float visibleSeconds;
   char* msgBuffer;
   size_t msgBufferLen;
}
qDebugBarRenderState_t;

typedef struct qRenderStates_t
{
   qDebugBarRenderState_t* debugBar;
}
qRenderStates_t;

qRenderStates_t* qRenderStates_Create();
void qRenderStates_Destroy( qRenderStates_t* states );
void qRenderStates_Tic( qRenderStates_t* states, qClock_t* clock );

#endif // RENDER_STATES_H
