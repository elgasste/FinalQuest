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
#include "menu.h"

static void qGame_Tic( qGame_t* game );
static void qGame_ScreenFadeComplete( qGame_t* game );

qGame_t* qGame_Create()
{
   sfVector2u mapTileCount = { 56, 56 };
   uint32_t i, tileIndex;
   sfVector2f actor1Pos = { 896, 896 };
   sfVector2f actor2Pos = { 928, 896 };
   sfVector2f actor3Pos = { 912, 920 };
   sfVector2f humanHitBoxSize = { 26, 16 };
   sfVector2f dogHitBoxSize = { 24, 10 };
   sfVector2f humanSpriteOffset = { -3, -16 };
   sfVector2f dogSpriteOffset = { -4, -22 };

   qGame_t* game = (qGame_t*)qAlloc( sizeof( qGame_t ), sfTrue );

   qRandom_Seed();

   game->window = qWindow_Create();
   game->clock = gmClock_Create();
   game->inputState = qInputState_Create();
   game->inputHandler = qInputHandler_Create();
   game->renderer = qRenderer_Create();
   game->physics = qPhysics_Create();
   game->map = qMap_Create( mapTileCount );
   game->menus = qMenus_Create();

   // default to grass with 5% encounter rate
   for ( i = 0; i < mapTileCount.x * mapTileCount.y; i++ )
   {
      game->map->tiles[i].textureIndex = 0;
      game->map->tiles[i].isPassable = sfTrue;
      game->map->tiles[i].encounterRate = 5;
   }

   // randomly generate some trees
   for ( i = 0; i < 100; i++ )
   {
      tileIndex = qRandom_UInt32( 0, mapTileCount.x * mapTileCount.y );
      game->map->tiles[tileIndex].textureIndex = 25;
      game->map->tiles[tileIndex].isPassable = sfFalse;
      game->map->tiles[tileIndex].encounterRate = 0;
   }

   game->actorCount = 3;
   game->actors = (qActor_t*)qAlloc( sizeof( qActor_t ) * game->actorCount, sfTrue );
   qActor_Setup( &( game->actors[0] ), actor1Pos, humanHitBoxSize, 100.0f, &( game->renderer->renderObjects->spriteTextures[0] ), humanSpriteOffset, 0.15f );
   qActor_Setup( &( game->actors[1] ), actor2Pos, humanHitBoxSize, 90.0f, &( game->renderer->renderObjects->spriteTextures[1] ), humanSpriteOffset, 0.15f );
   qActor_Setup( &( game->actors[2] ), actor3Pos, dogHitBoxSize, 150.0f, &( game->renderer->renderObjects->spriteTextures[2] ), dogSpriteOffset, 0.15f );
   qActor_SetDirection( &( game->actors[0] ), qDirection_Right );
   qActor_SetDirection( &( game->actors[1] ), qDirection_Left );
   qActor_SetDirection( &( game->actors[2] ), qDirection_Up );
   game->controllingActor = &( game->actors[0] );
   game->controllingActorIndex = 0;
   qRenderer_UpdateActors( game );
   qPhysics_ResetActorTileCache( game );

   game->showDiagnostics = sfFalse;
   game->cheatNoClip = sfFalse;
   game->cheatFast = sfFalse;
   game->cheatNoEncounters = sfFalse;

   return game;
}

void qGame_Destroy( qGame_t* game )
{
   uint32_t i;

   for ( i = 0; i < game->actorCount; i++ )
   {
      qActor_Cleanup( &( game->actors[i] ) );
   }

   qFree( game->actors, sizeof( qActor_t ) * game->actorCount, sfTrue );

   qMenus_Destroy( game->menus );
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
   qGame_SetState( game, qGameState_Map );

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
   if ( !game->renderer->renderStates->screenFade->isRunning )
   {
      qPhysics_Tic( game );
   }

   qRenderStates_Tic( game );
}

void qGame_Close( qGame_t* game )
{
   qWindow_Close( game->window );
}

void qGame_ShowDebugMessage( qGame_t* game, const char* msg )
{
   qDebugBarRenderState_t* state = game->renderer->renderStates->debugBar;

   snprintf( state->msgBuffer, state->msgBufferLen - 1, "%s", msg );
   state->isVisible = sfTrue;
   state->elapsedSeconds = 0;
}

void qGame_SwitchControllingActor( qGame_t* game )
{
   game->controllingActorIndex++;

   if ( game->controllingActorIndex >= game->actorCount )
   {
      game->controllingActorIndex = 0;
   }

   game->controllingActor = &( game->actors[game->controllingActorIndex] );
   qPhysics_ResetActorTileCache( game );
}

void qGame_SetState( qGame_t* game, qGameState_t state )
{
   switch ( state )
   {
      case qGameState_Map:
         qRenderStates_ResetMenu( game->renderer->renderStates->menu );
         game->menus->map->selectedIndex = 0;
         break;
      case qGameState_FadeMapToBattle:
         qRenderStates_StartScreenFade( game->renderer->renderStates->screenFade, sfTrue, sfTrue, sfTrue, &qGame_ScreenFadeComplete );
         break;
      case qGameState_FadeBattleOut:
         qRenderStates_StartScreenFade( game->renderer->renderStates->screenFade, sfTrue, sfTrue, sfFalse, &qGame_ScreenFadeComplete );
         break;
   }

   game->state = state;
}

void qGame_ExecuteMenuCommand( qGame_t* game, qMenuCommand_t command )
{
   switch ( command )
   {
      case qMenuCommand_Quit:
         qGame_Close( game );
         break;
      case qMenuCommand_CloseMenu:
         if ( game->state == qGameState_MapMenu )
         {
            qGame_SetState( game, qGameState_Map );
         }
         break;
   }
}

void qGame_RollEncounter( qGame_t* game, uint32_t mapTileIndex, sfBool force )
{
   qMapTile_t* tile = &( game->map->tiles[mapTileIndex] );

   if ( force || ( !game->cheatNoEncounters && tile->encounterRate > 0 && qRandom_Percent() <= tile->encounterRate ) )
   {
      qGame_SetState( game, qGameState_FadeMapToBattle );
   }
}

static void qGame_ScreenFadeComplete( qGame_t* game )
{
   switch ( game->state )
   {
      case qGameState_FadeMapToBattle:
         qGame_SetState( game, qGameState_FadeBattleIn );
         qRenderStates_StartScreenFade( game->renderer->renderStates->screenFade, sfFalse, sfFalse, sfTrue, &qGame_ScreenFadeComplete );
         break;
      case qGameState_FadeBattleIn:
         qGame_SetState( game, qGameState_Battle );
         break;
      case qGameState_FadeBattleOut:
         qGame_SetState( game, qGameState_FadeBattleToMap );
         qRenderStates_StartScreenFade( game->renderer->renderStates->screenFade, sfFalse, sfFalse, sfFalse, &qGame_ScreenFadeComplete );
         break;
      case qGameState_FadeBattleToMap:
         qGame_SetState( game, qGameState_Map );
         break;
   }
}
