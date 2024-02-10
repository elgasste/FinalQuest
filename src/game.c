#include "game.h"
#include "window.h"
#include "clock.h"
#include "input_state.h"
#include "input_handler.h"
#include "renderer.h"
#include "render_states.h"
#include "render_objects.h"
#include "map.h"
#include "random.h"
#include "actor.h"
#include "sprite_texture.h"
#include "physics.h"

static void qGame_Tic( qGame_t* game );

qGame_t* qGame_Create()
{
   sfVector2u mapTileCount = { 56, 56 };
   uint32_t i, tileIndex;
   sfVector2f actorPos = { 896, 896 };
   sfVector2f actorHitBoxSize = { 26, 16 };
   sfVector2f actorSpriteOffset = { -3, -16 };

   qGame_t* game = (qGame_t*)qAlloc( sizeof( qGame_t ), sfTrue );

   qRandom_Seed();

   game->window = qWindow_Create();
   game->clock = gmClock_Create();
   game->inputState = qInputState_Create();
   game->inputHandler = qInputHandler_Create();
   game->renderer = qRenderer_Create();
   game->physics = qPhysics_Create();
   game->map = qMap_Create( mapTileCount );

   // default to grass
   for ( i = 0; i < mapTileCount.x * mapTileCount.y; i++ )
   {
      game->map->tiles[i].textureIndex = 0;
      game->map->tiles[i].isPassable = sfTrue;
   }

   // randomly generate some trees
   for ( i = 0; i < 100; i++ )
   {
      tileIndex = qRandom_UInt32( 0, mapTileCount.x * mapTileCount.y );
      game->map->tiles[tileIndex].textureIndex = 25;
      game->map->tiles[tileIndex].isPassable = sfFalse;
   }

   // just one actor for now
   game->actors = qActor_Create( actorPos, actorHitBoxSize, 100.0f, &( game->renderer->renderObjects->spriteTextures[0] ), actorSpriteOffset, 0.15f );
   game->actorCount = 1;
   game->controllingActor = &( game->actors[0] );

   game->showDiagnostics = sfFalse;
   game->cheatNoClip = sfFalse;

   return game;
}

void qGame_Destroy( qGame_t* game )
{
   uint32_t i;

   for ( i = 0; i < game->actorCount; i++ )
   {
      qActor_Destroy( &( game->actors[i] ) );
   }

   qMap_Destroy( game->map );
   qPhysics_Destroy( game->physics );
   qRenderer_Destroy( game->renderer );
   qInputHandler_Destroy( game->inputHandler );
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
   qPhysics_Tic( game );
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
