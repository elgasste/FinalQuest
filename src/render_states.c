#include "render_states.h"
#include "clock.h"
#include "game.h"
#include "renderer.h"

static qDebugBarRenderState_t* qRenderStates_CreateDebugBar();
static qMenuRenderState_t* qRenderStates_CreateMenu();
static void qRenderStates_DestroyDebugBar( qDebugBarRenderState_t* state );
static void qRenderStates_DestroyMenu( qMenuRenderState_t* state );
static void qRenderStates_TicMenu( qGame_t* game );

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

   state->caratBlinkSeconds = 0.25f;
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

void qRenderStates_Tic( qGame_t* game )
{
   qRenderStates_t* states = game->renderer->renderStates;

   if ( states->debugBar->isVisible )
   {
      states->debugBar->elapsedSeconds += game->clock->frameDeltaSeconds;

      if ( states->debugBar->elapsedSeconds > states->debugBar->visibleSeconds )
      {
         states->debugBar->isVisible = sfFalse;
         states->debugBar->elapsedSeconds = 0;
      }
   }

   qRenderStates_TicMenu( game );
}

void qRenderStates_ResetMenu( qMenuRenderState_t* state )
{
   state->showCarat = sfTrue;
   state->caratElapsedSeconds = 0;
}

static void qRenderStates_TicMenu( qGame_t* game )
{
   qRenderStates_t* states = game->renderer->renderStates;

   if ( game->state == qGameState_MapMenu )
   {
      states->menu->caratElapsedSeconds += game->clock->frameDeltaSeconds;

      while( states->menu->caratElapsedSeconds > states->menu->caratBlinkSeconds )
      {
         TOGGLE_BOOL( states->menu->showCarat );
         states->menu->caratElapsedSeconds -= states->menu->caratBlinkSeconds;
      }
   }
}
