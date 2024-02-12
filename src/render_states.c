#include "render_states.h"
#include "clock.h"

static qDebugBarRenderState_t* qRenderStates_CreateDebugBar();
static qMenuRenderState_t* qRenderStates_CreateMenu();
static void qRenderStates_DestroyDebugBar( qDebugBarRenderState_t* state );
static void qRenderStates_DestroyMenu( qMenuRenderState_t* state );

qRenderStates_t* qRenderStates_Create()
{
   qRenderStates_t* states = (qRenderStates_t*)qAlloc( sizeof( qRenderStates_t ), sfTrue );

   states->debugBar = qRenderStates_CreateDebugBar();
   states->menu = qRenderStates_CreateMenu();

   return states;
}

static qDebugBarRenderState_t* qRenderStates_CreateDebugBar()
{
   qDebugBarRenderState_t* state = (qDebugBarRenderState_t*)qAlloc( sizeof( qDebugBarRenderState_t ), sfTrue );

   state->isVisible = sfFalse;
   state->visibleSeconds = 3;
   state->elapsedSeconds = 0;
   state->msgBuffer = (char*)qCalloc( STRLEN_DEFAULT, sizeof( char ), sfTrue );
   state->msgBufferLen = STRLEN_DEFAULT;

   return state;
}

static qMenuRenderState_t* qRenderStates_CreateMenu()
{
   qMenuRenderState_t* state = (qMenuRenderState_t*)qAlloc( sizeof( qMenuRenderState_t ), sfTrue );

   qRenderStates_ResetMenu( state );

   return state;
}

void qRenderStates_Destroy( qRenderStates_t* states )
{
   qRenderStates_DestroyDebugBar( states->debugBar );
   qRenderStates_DestroyMenu( states->menu );

   qFree( states, sizeof( qRenderStates_t ), sfTrue );
}

static void qRenderStates_DestroyDebugBar( qDebugBarRenderState_t* state )
{
   qFree( state->msgBuffer, sizeof( char ) * state->msgBufferLen, sfTrue );
   qFree( state, sizeof( qDebugBarRenderState_t ), sfTrue );
}

static void qRenderStates_DestroyMenu( qMenuRenderState_t* state )
{
   qFree( state, sizeof( qMenuRenderState_t ), sfTrue );
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

void qRenderStates_ResetMenu( qMenuRenderState_t* state )
{
   state->showCarat = sfTrue;
   state->caratElapsedSeconds = 0;
}
