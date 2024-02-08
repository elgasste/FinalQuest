#include "game.h"
#include "window.h"
#include "clock.h"
#include "input_state.h"
#include "input_handler.h"
#include "renderer.h"
#include "render_states.h"

static void qGame_Tic( qGame_t* game );

qGame_t* qGame_Create()
{
   qGame_t* game = (qGame_t*)qAlloc( sizeof( qGame_t ), sfTrue );

   game->window = qWindow_Create();
   game->clock = gmClock_Create();
   game->inputState = qInputState_Create();
   game->renderer = qRenderer_Create();

   game->showDiagnostics = sfFalse;

   return game;
}

void qGame_Destroy( qGame_t* game )
{
   qRenderer_Destroy( game->renderer );
   qInputState_Destroy( game->inputState );
   qClock_Destroy( game->clock );
   qWindow_Destroy( game->window );

   qFree( game, sizeof( qGame_t ), sfTrue );
}

void qGame_Run( qGame_t* game )
{
   while ( qWindow_IsOpen( game->window ) )
   {
      qClock_StartFrame( game->clock );

      qInputState_Reset( game->inputState );
      qWindow_HandleEvents( game );
      qInputHandler_HandleInput( game );
      qGame_Tic( game );
      qRenderer_Render( game );

      if ( game->window->wantToClose )
      {
         qGame_Close( game );
      }

      qClock_EndFrame( game->clock );
   }
}

static void qGame_Tic( qGame_t* game )
{
   qRenderStates_Tic( game->renderer->renderStates, game->clock );
}

void qGame_Close( qGame_t* game )
{
   qWindow_Close( game->window );
}

void qGame_ShowDebugMessage( qGame_t* game, const char* msg )
{
   qDebugBarRenderState_t* state = game->renderer->renderStates->debugBar;

   snprintf( state->msgBuffer, state->msgBufferLen, "%s", msg );
   state->isVisible = sfTrue;
   state->elapsedSeconds = 0;
}
