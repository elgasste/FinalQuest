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
#include "character.h"
#include "actor_sprite_texture.h"
#include "physics.h"
#include "menu.h"
#include "battle.h"
#include "battle_stats.h"
#include "enemy.h"
#include "battle_sprite.h"

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
   qActor_Setup( &( game->actors[0] ), actor1Pos, humanHitBoxSize, 100.0f, &( game->renderer->renderObjects->actorSpriteTextures[0] ), humanSpriteOffset, 0.15f );
   qActor_Setup( &( game->actors[1] ), actor2Pos, humanHitBoxSize, 90.0f, &( game->renderer->renderObjects->actorSpriteTextures[1] ), humanSpriteOffset, 0.15f );
   qActor_Setup( &( game->actors[2] ), actor3Pos, dogHitBoxSize, 150.0f, &( game->renderer->renderObjects->actorSpriteTextures[2] ), dogSpriteOffset, 0.15f );
   qActor_SetDirection( &( game->actors[0] ), qDirection_Right );
   qActor_SetDirection( &( game->actors[1] ), qDirection_Left );
   qActor_SetDirection( &( game->actors[2] ), qDirection_Up );

   game->characterCount = 3;
   game->characters = (qCharacter_t*)qAlloc( sizeof( qCharacter_t ) * game->characterCount, sfTrue );
   qCharacter_Setup( &( game->characters[0] ), &( game->actors[0] ) );
   qCharacter_Setup( &( game->characters[1] ), &( game->actors[1] ) );
   qCharacter_Setup( &( game->characters[2] ), &( game->actors[2] ) );
   game->controllingCharacter = &( game->characters[0] );
   game->controllingCharacterIndex = 0;

   snprintf( game->characters[0].name, STRLEN_SHORT - 1, "Heroman" );
   game->characters[0].stats->hitPoints = 100;
   game->characters[0].stats->magicPoints = 25;
   game->characters[0].stats->attackPower = 30;
   game->characters[0].stats->defensePower = 40;

   snprintf( game->characters[1].name, STRLEN_SHORT - 1, "Gurrrl" );
   game->characters[1].stats->hitPoints = 80;
   game->characters[1].stats->magicPoints = 60;
   game->characters[1].stats->attackPower = 10;
   game->characters[1].stats->defensePower = 15;

   snprintf( game->characters[2].name, STRLEN_SHORT - 1, "Dawg" );
   game->characters[2].stats->hitPoints = 10000;
   game->characters[2].stats->magicPoints = 0;
   game->characters[2].stats->attackPower = 500;
   game->characters[2].stats->defensePower = 500;

   game->enemyTemplateCount = 1;
   game->enemyTemplates = (qEnemyTemplate_t*)qAlloc( sizeof( qEnemyTemplate_t ) * game->enemyTemplateCount, sfTrue );

   snprintf( game->enemyTemplates[0].name, STRLEN_SHORT - 1, "Batfuck" );
   game->enemyTemplates[0].indefiniteArticle = qIndefiniteArticle_A;
   game->enemyTemplates[0].spriteTextureIndex = 0;
   game->enemyTemplates[0].spriteSize.x = 64;
   game->enemyTemplates[0].spriteSize.y = 64;
   game->enemyTemplates[0].spriteFrameSeconds = 0.11f;
   game->enemyTemplates[0].baseStats = (qBattleStats_t*)qAlloc( sizeof( qBattleStats_t ), sfTrue );
   game->enemyTemplates[0].baseStats->attackPower = 8;
   game->enemyTemplates[0].baseStats->defensePower = 4;
   game->enemyTemplates[0].baseStats->hitPoints = 20;
   game->enemyTemplates[0].baseStats->magicPoints = 0;
   game->enemyTemplates[0].statsSpread = (qBattleStats_t*)qAlloc( sizeof( qBattleStats_t ), sfTrue );
   game->enemyTemplates[0].statsSpread->attackPower = 0;
   game->enemyTemplates[0].statsSpread->defensePower = 0;
   game->enemyTemplates[0].statsSpread->hitPoints = 2;
   game->enemyTemplates[0].statsSpread->magicPoints = 0;

   qRenderer_UpdateActors( game );
   qPhysics_ResetActorTileCache( game );

   game->battle = 0;

   game->isMenuOpen = sfFalse;

   game->showDiagnostics = sfFalse;
   game->cheatNoClip = sfFalse;
   game->cheatFast = sfFalse;
   game->cheatNoEncounters = sfFalse;

   return game;
}

void qGame_Destroy( qGame_t* game )
{
   uint32_t i;

   for ( i = 0; i < game->enemyTemplateCount; i++ )
   {
      qFree( game->enemyTemplates[i].statsSpread, sizeof( qBattleStats_t ), sfTrue );
      qFree( game->enemyTemplates[i].baseStats, sizeof( qBattleStats_t ), sfTrue );
   }

   qFree( game->enemyTemplates, sizeof( qEnemyTemplate_t ) * game->enemyTemplateCount, sfTrue );

   for ( i = 0; i < game->characterCount; i++ )
   {
      qCharacter_Cleanup( &( game->characters[i] ) );
   }

   qFree( game->characters, sizeof( qCharacter_t ) * game->characterCount, sfTrue );

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
   if ( game->state == qGameState_Map )
   {
      qPhysics_Tic( game );
   }

   qRenderStates_Tic( game );

   if ( game->battle )
   {
      qBattleSprite_Tic( game->battle->enemy->sprite, game->clock );
   }
}

void qGame_Close( qGame_t* game )
{
   if ( game->battle )
   {
      qBattle_Destroy( game->battle );
   }

   qWindow_Close( game->window );
}

void qGame_ShowDebugMessage( qGame_t* game, const char* msg )
{
   qDebugBarRenderState_t* state = game->renderer->renderStates->debugBar;

   snprintf( state->msgBuffer, state->msgBufferLen - 1, "%s", msg );
   state->isVisible = sfTrue;
   state->elapsedSeconds = 0;
}

void qGame_SwitchControllingCharacter( qGame_t* game )
{
   game->controllingCharacterIndex++;

   if ( game->controllingCharacterIndex >= game->characterCount )
   {
      game->controllingCharacterIndex = 0;
   }

   game->controllingCharacter = &( game->characters[game->controllingCharacterIndex] );

   qRenderer_SwitchControllingCharacter( game );
   qPhysics_ResetActorTileCache( game );
}

void qGame_SetState( qGame_t* game, qGameState_t state )
{
   switch ( state )
   {
      case qGameState_MapMenu:
         qRenderStates_ResetMenu( game->renderer->renderStates->menu );
         game->menus->map->selectedIndex = 0;
         game->isMenuOpen = sfTrue;
         break;
      case qGameState_FadeMapToBattle:
         qRenderStates_StartScreenFade( game->renderer->renderStates->screenFade, sfTrue, sfTrue, sfTrue, &qGame_ScreenFadeComplete );
         break;
      case qGameState_BattleChooseAction:
         qRenderStates_ResetMenu( game->renderer->renderStates->menu );
         game->menus->battleAction->selectedIndex = 0;
         game->isMenuOpen = sfTrue;
         break;
      case qGameState_FadeBattleOut:
         qRenderStates_StartScreenFade( game->renderer->renderStates->screenFade, sfTrue, sfTrue, sfFalse, &qGame_ScreenFadeComplete );
         break;
      default:
         game->isMenuOpen = sfFalse;
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
      case qMenuCommand_BattleAttack:
         qBattle_Action( game, qBattleAction_Attack );
         break;
      case qMenuCommand_BattleDefend:
         qBattle_Action( game, qBattleAction_Defend );
         break;
      case qMenuCommand_BattleSpell:
         qBattle_Action( game, qBattleAction_Spell );
         break;
      case qMenuCommand_BattleItem:
         qBattle_Action( game, qBattleAction_Item );
         break;
      case qMenuCommand_BattleFlee:
         qBattle_Action( game, qBattleAction_Flee );
         break;
   }
}

void qGame_RollEncounter( qGame_t* game, uint32_t mapTileIndex, sfBool force )
{
   qMapTile_t* tile = &( game->map->tiles[mapTileIndex] );

   if ( force || ( !game->cheatNoEncounters && tile->encounterRate > 0 && qRandom_Percent() <= tile->encounterRate ) )
   {
      game->battle = qBattle_Create( game );
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
         qBattle_Begin( game );
         break;
      case qGameState_FadeBattleOut:
         qGame_SetState( game, qGameState_FadeBattleToMap );
         qRenderStates_StartScreenFade( game->renderer->renderStates->screenFade, sfFalse, sfFalse, sfFalse, &qGame_ScreenFadeComplete );
         qBattle_Destroy( game->battle );
         game->battle = 0;
         break;
      case qGameState_FadeBattleToMap:
         qGame_SetState( game, qGameState_Map );
         break;
   }
}
