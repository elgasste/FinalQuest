#include "render_states.h"
#include "clock.h"

static qDebugBarRenderState_t* qDebugBarRenderState_Create();
static void qDebugBarRenderState_Destroy( qDebugBarRenderState_t* state );

qRenderStates_t* qRenderStates_Create()
{
   qRenderStates_t* states = (qRenderStates_t*)qAlloc( sizeof( qRenderStates_t ), sfTrue );

   states->debugBar = qDebugBarRenderState_Create();

   return states;
}

static qDebugBarRenderState_t* qDebugBarRenderState_Create()
{
   qDebugBarRenderState_t* state = (qDebugBarRenderState_t*)qAlloc( sizeof( qDebugBarRenderState_t ), sfTrue );

   state->isVisible = sfFalse;
   state->visibleSeconds = 3;
   state->elapsedSeconds = 0;
   state->msgBuffer = (char*)qCalloc( STRLEN_DEFAULT, sizeof( char ), sfTrue );
   state->msgBufferLen = STRLEN_DEFAULT;

   return state;
}

void qRenderStates_Destroy( qRenderStates_t* states )
{
   qDebugBarRenderState_Destroy( states->debugBar );

   qFree( states, sizeof( qRenderStates_t ), sfTrue );
}

static void qDebugBarRenderState_Destroy( qDebugBarRenderState_t* state )
{
   qFree( state->msgBuffer, sizeof( char ) * state->msgBufferLen, sfTrue );

   qFree( state, sizeof( qDebugBarRenderState_t ), sfTrue );
}

void qRenderStates_Tic( qRenderStates_t* states, qClock_t* clock )
{
   if ( states->debugBar->isVisible )
   {
      states->debugBar->elapsedSeconds += clock->frameDeltaSeconds;

      if ( states->debugBar->elapsedSeconds > states->debugBar->visibleSeconds )
      {
         states->debugBar->isVisible = sfFalse;
         states->debugBar->elapsedSeconds = 0;
      }
   }
}
