#include "renderer.h"
#include "render_objects.h"
#include "render_states.h"
#include "game.h"
#include "window.h"
#include "clock.h"
#include "time_util.h"
#include "map.h"
#include "actor.h"
#include "entity.h"
#include "character.h"
#include "actor_sprite.h"
#include "physics.h"
#include "menu.h"
#include "text_util.h"
#include "battle.h"
#include "battle_stats.h"
#include "enemy.h"
#include "battle_sprite.h"

static void qRenderer_DrawDiagnostics( qGame_t* game );
static void qRenderer_DrawDebugBar( qGame_t* game );
static void qRenderer_DrawScreenFade( qGame_t* game );
static void qRenderer_SetMapView( qGame_t* game );
static void qRenderer_DrawMap( qGame_t* game );
static void qRenderer_DrawCharacterStats( qCharacter_t* character, qCharacterStatsRenderObjects_t* objects, qWindow_t* window, sfBool full );
static void qRenderer_DrawMenu( qMenu_t* menu, qMenuRenderObjects_t* objects, qMenuRenderState_t* state, qWindow_t* window );
static void qRenderer_DrawDialogBox( qGame_t* game, qDialogBoxRenderObjects_t* objects, sfBool scroll );
static void qRenderer_DrawActors( qGame_t* game );
static void qRenderer_DrawEnemies( qGame_t* game );
static void qRenderer_OrderActors( qGame_t* game );

qRenderer_t* qRenderer_Create()
{
   sfVector2f windowBackgroundSize = { WINDOW_WIDTH, WINDOW_HEIGHT };
   sfVector2f windowBackgroundPosition = { 0, 0 };

   qRenderer_t* renderer = (qRenderer_t*)qAlloc( sizeof( qRenderer_t ), sfTrue );

   renderer->renderObjects = qRenderObjects_Create();
   renderer->renderStates = qRenderStates_Create();

   renderer->windowBackgroundRect = qsfRectangleShape_Create();
   sfRectangleShape_setSize( renderer->windowBackgroundRect, windowBackgroundSize );
   sfRectangleShape_setPosition( renderer->windowBackgroundRect, windowBackgroundPosition );
   sfRectangleShape_setFillColor( renderer->windowBackgroundRect, sfBlack );

   renderer->mapViewRect.width = WINDOW_WIDTH / GRAPHICS_SCALE;
   renderer->mapViewRect.height = WINDOW_HEIGHT / GRAPHICS_SCALE;

   return renderer;
}

void qRenderer_Destroy( qRenderer_t* renderer )
{
   qsfRectangleShape_Destroy( renderer->windowBackgroundRect );

   qRenderStates_Destroy( renderer->renderStates );
   qRenderObjects_Destroy( renderer->renderObjects );

   qFree( renderer, sizeof( qRenderer_t ), sfTrue );
}

void qRenderer_UpdateActors( qGame_t* game )
{
   uint32_t i;
   qActor_t* actor;

   for ( i = 0; i < game->actorCount; i++ )
   {
      actor = &( game->actors[i] );
      game->renderer->orderedActors[i] = actor;
   }

   qRenderer_OrderActors( game );

   game->renderer->controllingActorCache = game->controllingCharacter->actor;
}

void qRenderer_Render( qGame_t* game )
{
   qRenderObjects_t* renderObjects = game->renderer->renderObjects;
   qRenderStates_t* renderStates = game->renderer->renderStates;

   qWindow_DrawRectangleShape( game->window, game->renderer->windowBackgroundRect );

   switch ( game->state )
   {
      case qGameState_Map:
      case qGameState_FadeMapToBattle:
      case qGameState_FadeBattleToMap:
         qRenderer_SetMapView( game );
         qRenderer_DrawMap( game );
         qRenderer_DrawActors( game );
         break;
      case qGameState_MapMenu:
         qRenderer_SetMapView( game );
         qRenderer_DrawMap( game );
         qRenderer_DrawActors( game );
         qRenderer_DrawCharacterStats( game->controllingCharacter, renderObjects->mapCharacterStats, game->window, sfTrue );
         qRenderer_DrawMenu( game->menus->map, renderObjects->mapMenu, renderStates->menu, game->window );
         break;
      case qGameState_FadeBattleIn:
         qRenderer_DrawEnemies( game );
         break;
      case qGameState_BattleIntro:
         qRenderer_DrawDialogBox( game, renderObjects->battleDialogBoxLarge, sfTrue );
         qRenderer_DrawEnemies( game );
         break;
      case qGameState_BattleAction:
      case qGameState_BattleResult:
      case qGameState_FadeBattleOut:
         qRenderer_DrawCharacterStats( game->controllingCharacter, renderObjects->battleCharacterStats, game->window, sfFalse );
         qRenderer_DrawDialogBox( game, renderObjects->battleDialogBoxLarge, sfTrue );
         qRenderer_DrawEnemies( game );
         break;
      case qGameState_BattleChooseAction:
         qRenderer_DrawCharacterStats( game->controllingCharacter, renderObjects->battleCharacterStats, game->window, sfFalse );
         qRenderer_DrawMenu( game->menus->battleAction, renderObjects->battleActionMenu, renderStates->menu, game->window );
         qRenderer_DrawDialogBox( game, renderObjects->battleDialogBoxSmall, sfFalse );
         qRenderer_DrawEnemies( game );
         break;
   }

   qRenderer_DrawScreenFade( game );
   qRenderer_DrawDebugBar( game );

   if ( game->showDiagnostics )
   {
      qRenderer_DrawDiagnostics( game );
   }

   qWindow_Display( game->window );
}

int qRenderer_ActorCmp( const void* a, const void* b )
{
   qActor_t* actorA = *(qActor_t**)a;
   qActor_t* actorB = *(qActor_t**)b;

   return ( actorA->entity->mapPos.y > actorB->entity->mapPos.y );
}

static void qRenderer_OrderActors( qGame_t* game )
{
   qsort( game->renderer->orderedActors, game->actorCount, sizeof( qActor_t* ), qRenderer_ActorCmp );
}

static void qRenderer_DrawDiagnostics( qGame_t* game )
{
   char msg[STRLEN_DEFAULT];
   char timeStr[STRLEN_SHORT];
   qDiagnosticsRenderObjects_t* objects = game->renderer->renderObjects->diagnostics;

   qWindow_DrawRectangleShape( game->window, objects->backgroundRect );

   objects->textPosition.y = 4;
   sfText_setPosition( objects->text, objects->textPosition );
   snprintf( msg, STRLEN_DEFAULT - 1, STR_FRAMERATEFORMATTER, GAME_FPS );
   sfText_setString( objects->text, msg );
   qWindow_DrawText( game->window, objects->text );

   objects->textPosition.y += objects->lineSize;
   sfText_setPosition( objects->text, objects->textPosition );
   snprintf( msg, STRLEN_DEFAULT - 1, STR_TOTALFRAMESFORMATTER, game->clock->totalFrameCount );
   sfText_setString( objects->text, msg );
   qWindow_DrawText( game->window, objects->text );

   objects->textPosition.y += objects->lineSize;
   sfText_setPosition( objects->text, objects->textPosition );
   snprintf( msg, STRLEN_DEFAULT - 1, STR_LAGFRAMESFORMATTER, game->clock->lagFrameCount );
   sfText_setString( objects->text, msg );
   qWindow_DrawText( game->window, objects->text );

   objects->textPosition.y += objects->lineSize;
   sfText_setPosition( objects->text, objects->textPosition );
   qTimeUtil_FormatTime( timeStr, STRLEN_SHORT - 1, (int32_t)( game->clock->realTotalDurationMicro / 1000000 ) );
   snprintf( msg, STRLEN_DEFAULT - 1, STR_ELAPSEDTIMEFORMATTER, timeStr );
   sfText_setString( objects->text, msg );
   qWindow_DrawText( game->window, objects->text );
}

static void qRenderer_DrawDebugBar( qGame_t* game )
{
   qDebugBarRenderState_t* state = game->renderer->renderStates->debugBar;
   qDebugBarRenderObjects_t* objects = game->renderer->renderObjects->debugBar;

   if ( state->isVisible )
   {
      sfText_setString( objects->text, state->msgBuffer );

      qWindow_DrawRectangleShape( game->window, objects->backgroundRect );
      qWindow_DrawText( game->window, objects->text );
   }
}

static void qRenderer_DrawScreenFade( qGame_t* game )
{
   float screenFadePercentage;
   qRenderer_t* renderer = game->renderer;
   qScreenFadeRenderState_t* screenFadeState = renderer->renderStates->screenFade;
   qScreenFadeRenderObjects_t* screenFadeObjects = renderer->renderObjects->screenFade;

   if ( screenFadeState->isRunning )
   {
      screenFadePercentage = screenFadeState->isPausing ? 1.0f : screenFadeState->elapsedSeconds / screenFadeState->fadeSeconds;

      if ( !screenFadeState->fadeOut )
      {
         screenFadePercentage = 1.0f - screenFadePercentage;
      }

      if ( screenFadeState->isLightColor )
      {
         screenFadeObjects->lightColor.a = (sfUint8)( 255 * screenFadePercentage );
         sfRectangleShape_setFillColor( screenFadeObjects->screenRect, screenFadeObjects->lightColor );
      }
      else
      {
         screenFadeObjects->darkColor.a = (sfUint8)( 255 * screenFadePercentage );
         sfRectangleShape_setFillColor( screenFadeObjects->screenRect, screenFadeObjects->darkColor );
      }

      qWindow_DrawRectangleShape( game->window, screenFadeObjects->screenRect );
   }
}

static void qRenderer_SetMapView( qGame_t* game )
{
   qRenderer_t* renderer = game->renderer;
   qActor_t* actor = game->controllingCharacter->actor;
   qMap_t* map = game->map;
   sfVector2f mapSize = { (float)( map->tileCount.x * MAP_TILE_SIZE ), (float)( map->tileCount.y * MAP_TILE_SIZE ) };
   sfVector2f actorCenter = {
      actor->entity->mapPos.x + ( actor->entity->mapHitBoxSize.x / 2 ),
      actor->entity->mapPos.y + ( actor->entity->mapHitBoxSize.y / 2 ) };
   sfVector2f oldActorCenter = {
      game->renderer->controllingActorCache->entity->mapPos.x + ( game->renderer->controllingActorCache->entity->mapHitBoxSize.x / 2 ),
      game->renderer->controllingActorCache->entity->mapPos.y + ( game->renderer->controllingActorCache->entity->mapHitBoxSize.y / 2 )
   };
   float swapPercentage;

   if ( game->renderer->renderStates->actorSwap->isRunning )
   {
      swapPercentage = game->renderer->renderStates->actorSwap->elapsedSeconds / game->renderer->renderStates->actorSwap->swapSeconds;
      actorCenter.x = oldActorCenter.x += ( actorCenter.x - oldActorCenter.x ) * swapPercentage;
      actorCenter.y = oldActorCenter.y += ( actorCenter.y - oldActorCenter.y ) * swapPercentage;
   }

   renderer->mapViewRect.left = actorCenter.x - ( ( WINDOW_WIDTH / GRAPHICS_SCALE ) / 2 );
   renderer->mapViewRect.top = actorCenter.y - ( ( WINDOW_HEIGHT / GRAPHICS_SCALE ) / 2 );

   if ( mapSize.x < renderer->mapViewRect.width )
   {
      renderer->mapViewPadding.x = ( renderer->mapViewRect.width - mapSize.x ) / 2;
      renderer->mapTilePixelOffset.x = 0;
      renderer->mapViewStart.x = 0;
      renderer->mapViewEnd.x = map->tileCount.x - 1;
   }
   else if ( renderer->mapViewRect.left < 0 )
   {
      renderer->mapViewRect.left = 0;
      renderer->mapViewPadding.x = 0;
      renderer->mapTilePixelOffset.x = 0;
      renderer->mapViewStart.x = 0;
      renderer->mapViewEnd.x = (uint32_t)( renderer->mapViewRect.width / MAP_TILE_SIZE ) + 1;
   }
   else if ( ( renderer->mapViewRect.left + renderer->mapViewRect.width ) >= mapSize.x )
   {
      renderer->mapViewRect.left = mapSize.x - renderer->mapViewRect.width - 1;
      renderer->mapViewPadding.x = 0;
      renderer->mapTilePixelOffset.x = -( renderer->mapViewRect.left - (float)( (int32_t)renderer->mapViewRect.left / MAP_TILE_SIZE ) * MAP_TILE_SIZE );
      renderer->mapViewEnd.x = map->tileCount.x;
      renderer->mapViewStart.x = renderer->mapViewEnd.x - (uint32_t)( renderer->mapViewRect.width / MAP_TILE_SIZE ) - 1;
   }
   else
   {
      renderer->mapTilePixelOffset.x = -( renderer->mapViewRect.left - (float)( (int32_t)renderer->mapViewRect.left / MAP_TILE_SIZE ) * MAP_TILE_SIZE );
      renderer->mapViewStart.x = (uint32_t)( renderer->mapViewRect.left / MAP_TILE_SIZE );
      renderer->mapViewEnd.x = renderer->mapViewStart.x + (uint32_t)( renderer->mapViewRect.width / MAP_TILE_SIZE ) + 1;
      renderer->mapViewPadding.x = 0;
   }

   if ( mapSize.y < renderer->mapViewRect.height )
   {
      renderer->mapViewPadding.y = ( renderer->mapViewRect.height - mapSize.y ) / 2;
      renderer->mapTilePixelOffset.y = 0;
      renderer->mapViewStart.y = 0;
      renderer->mapViewEnd.y = map->tileCount.y - 1;
   }
   else if ( renderer->mapViewRect.top < 0 )
   {
      renderer->mapViewRect.top = 0;
      renderer->mapViewPadding.y = 0;
      renderer->mapTilePixelOffset.y = 0;
      renderer->mapViewStart.y = 0;
      renderer->mapViewEnd.y = (uint32_t)( renderer->mapViewRect.height / MAP_TILE_SIZE ) + 1;
   }
   else if ( ( renderer->mapViewRect.top + renderer->mapViewRect.height ) >= mapSize.y )
   {
      renderer->mapViewRect.top = mapSize.y - renderer->mapViewRect.height - 1;
      renderer->mapViewPadding.y = 0;
      renderer->mapTilePixelOffset.y = -( renderer->mapViewRect.top - (float)( (int32_t)renderer->mapViewRect.top / MAP_TILE_SIZE ) * MAP_TILE_SIZE );
      renderer->mapViewEnd.y = map->tileCount.y;
      renderer->mapViewStart.y = renderer->mapViewEnd.y - (uint32_t)( renderer->mapViewRect.height / MAP_TILE_SIZE ) - 1;
   }
   else
   {
      renderer->mapTilePixelOffset.y = -( renderer->mapViewRect.top - (float)( (int32_t)renderer->mapViewRect.top / MAP_TILE_SIZE ) * MAP_TILE_SIZE );
      renderer->mapViewStart.y = (uint32_t)( renderer->mapViewRect.top / MAP_TILE_SIZE );
      renderer->mapViewEnd.y = renderer->mapViewStart.y + (uint32_t)( renderer->mapViewRect.height / MAP_TILE_SIZE ) + 1;
      renderer->mapViewPadding.y = 0;
   }
}

static void qRenderer_DrawMap( qGame_t* game )
{
   qRenderer_t* renderer = game->renderer;
   qMap_t* map = game->map;
   uint32_t tileRow, tileCol, row, col;
   qMapRenderObjects_t* objects = game->renderer->renderObjects->map;
   qMapTile_t* tile;
   sfVector2f tilePos = { 0, 0 };
   sfIntRect textureRect = { 0, 0, MAP_TILE_SIZE, MAP_TILE_SIZE };
   sfVector2u textureSize = sfTexture_getSize( game->renderer->renderObjects->map->tilesetTexture );
   sfVector2u textureTileCount = { textureSize.x / MAP_TILE_SIZE, textureSize.y / MAP_TILE_SIZE };

   for ( row = 0, tileRow = renderer->mapViewStart.y; tileRow <= renderer->mapViewEnd.y; row++, tileRow++ )
   {
      for ( col = 0, tileCol = renderer->mapViewStart.x; tileCol <= renderer->mapViewEnd.x; col++, tileCol++ )
      {
         tile = &( map->tiles[( tileRow * map->tileCount.x ) + tileCol] );

         textureRect.left = ( tile->textureIndex % textureTileCount.x ) * MAP_TILE_SIZE;
         textureRect.top = ( tile->textureIndex / textureTileCount.x ) * MAP_TILE_SIZE;
         tilePos.x = ( ( (float)col * MAP_TILE_SIZE ) + renderer->mapTilePixelOffset.x + renderer->mapViewPadding.x ) * GRAPHICS_SCALE;
         tilePos.y = ( ( (float)row * MAP_TILE_SIZE ) + renderer->mapTilePixelOffset.y + renderer->mapViewPadding.y ) * GRAPHICS_SCALE;

         sfSprite_setTextureRect( objects->tileSprite, textureRect );
         sfSprite_setPosition( objects->tileSprite, tilePos );

         qWindow_DrawSprite( game->window, objects->tileSprite );
      }
   }
}

static void qRenderer_DrawCharacterStats( qCharacter_t* character, qCharacterStatsRenderObjects_t* objects, qWindow_t* window, sfBool full )
{
   sfVector2f pos;
   char statVal[STRLEN_SHORT];

   qWindow_DrawConvexShape( window, objects->backgroundShape );

   pos.x = objects->pos.x + objects->nameOffset.x;
   pos.y = objects->pos.y + objects->nameOffset.y;
   sfText_setPosition( objects->text, pos );
   sfText_setString( objects->text, character->name );
   qWindow_DrawText( window, objects->text );

   pos.x = objects->pos.x + objects->labelsOffset.x;
   pos.y = objects->pos.y + objects->labelsOffset.y;
   sfText_setPosition( objects->text, pos );
   sfText_setString( objects->text, STR_STATS_HITPOINTS );
   qWindow_DrawText( window, objects->text );
   pos.y += objects->lineSize;
   sfText_setPosition( objects->text, pos );
   sfText_setString( objects->text, STR_STATS_MAGICPOINTS );
   qWindow_DrawText( window, objects->text );

   if ( full )
   {
      pos.y += objects->lineSize;
      sfText_setPosition( objects->text, pos );
      sfText_setString( objects->text, STR_STATS_ATTACKPOWER );
      qWindow_DrawText( window, objects->text );
      pos.y += objects->lineSize;
      sfText_setPosition( objects->text, pos );
      sfText_setString( objects->text, STR_STATS_DEFENSEPOWER );
      qWindow_DrawText( window, objects->text );
   }

   snprintf( statVal, STRLEN_SHORT - 1, "%d", character->stats->hitPoints );
   pos.x = objects->pos.x + objects->statsOffset.x;
   pos.y = objects->pos.y + objects->statsOffset.y;
   sfText_setPosition( objects->text, pos );
   sfText_setString( objects->text, statVal );
   qWindow_DrawText( window, objects->text );
   snprintf( statVal, STRLEN_SHORT - 1, "%d", character->stats->magicPoints );
   pos.y += objects->lineSize;
   sfText_setPosition( objects->text, pos );
   sfText_setString( objects->text, statVal );
   qWindow_DrawText( window, objects->text );

   if ( full )
   {
      snprintf( statVal, STRLEN_SHORT - 1, "%d", character->stats->attackPower );
      pos.y += objects->lineSize;
      sfText_setPosition( objects->text, pos );
      sfText_setString( objects->text, statVal );
      qWindow_DrawText( window, objects->text );
      snprintf( statVal, STRLEN_SHORT - 1, "%d", character->stats->defensePower );
      pos.y += objects->lineSize;
      sfText_setPosition( objects->text, pos );
      sfText_setString( objects->text, statVal );
      qWindow_DrawText( window, objects->text );
   }
}

static void qRenderer_DrawMenu( qMenu_t* menu, qMenuRenderObjects_t* objects, qMenuRenderState_t* state, qWindow_t* window )
{
   sfVector2f pos;
   uint32_t i;

   qWindow_DrawConvexShape( window, objects->backgroundShape );

   for ( i = 0; i < menu->optionCount; i++ )
   {
      if ( menu->selectedIndex == i && state->showCarat )
      {
         pos.x = objects->pos.x + objects->itemsOffset.x + objects->caratOffset.x;
         pos.y = objects->pos.y + objects->itemsOffset.y + objects->caratOffset.y + ( i * objects->lineSize );
         sfText_setPosition( objects->text, pos );
         sfText_setString( objects->text, STR_MENU_CARAT );
         qWindow_DrawText( window, objects->text );
      }

      pos.x = objects->pos.x + objects->itemsOffset.x;
      pos.y = objects->pos.y + objects->itemsOffset.y + ( i * objects->lineSize );
      sfText_setPosition( objects->text, pos );
      sfText_setString( objects->text, menu->options[i].label );
      qWindow_DrawText( window, objects->text );
   }
}

static void qRenderer_DrawDialogBox( qGame_t* game, qDialogBoxRenderObjects_t* objects, sfBool scroll )
{
   sfVector2f pos;

   qWindow_DrawConvexShape( game->window, objects->backgroundShape );

   if ( scroll )
   {
      qTextUtil_DrawWrappedScrollingText( game, objects );
   }
   else
   {
      pos.x = objects->pos.x + objects->textOffset.x;
      pos.y = objects->pos.y + objects->textOffset.y;
      sfText_setPosition( objects->text, pos );
      sfText_setString( objects->text, objects->message );

      qWindow_DrawText( game->window, objects->text );
   }
}

static void qRenderer_DrawActors( qGame_t* game )
{
   qRenderer_t* renderer = game->renderer;
   qActor_t* actor;
   sfVector2f spritePos;
   uint32_t i;

   if ( game->physics->actorMoved )
   {
      qRenderer_OrderActors( game );
   }

   for ( i = 0; i < game->actorCount; i++ )
   {
      actor = renderer->orderedActors[i];

      spritePos.x = ( renderer->mapViewPadding.x > 0 )
         ? ( actor->entity->mapPos.x + actor->spriteOffset.x + renderer->mapViewPadding.x ) * GRAPHICS_SCALE
         : ( actor->entity->mapPos.x + actor->spriteOffset.x - renderer->mapViewRect.left ) * GRAPHICS_SCALE;
      spritePos.y = ( renderer->mapViewPadding.y > 0 )
         ? ( actor->entity->mapPos.y + actor->spriteOffset.y + renderer->mapViewPadding.y ) * GRAPHICS_SCALE
         : ( actor->entity->mapPos.y + actor->spriteOffset.y - renderer->mapViewRect.top ) * GRAPHICS_SCALE;

      qActorSprite_SetPosition( actor->sprite, spritePos );
      qWindow_DrawSprite( game->window, actor->sprite->sfmlSprite );
   }
}

static void qRenderer_DrawEnemies( qGame_t* game )
{
   qBattleSprite_t* sprite = game->battle->enemy->sprite;
   sfVector2f pos = { ( WINDOW_WIDTH / 2 ) - ( ( (float)sprite->size.x / 2 ) * GRAPHICS_SCALE ),
                      ( WINDOW_HEIGHT / 2 ) - ( ( (float)sprite->size.y / 2 ) * GRAPHICS_SCALE ) };

   qBattleSprite_SetPosition( sprite, pos );
   qWindow_DrawSprite( game->window, sprite->sfmlSprite );
}

static void qRenderer_CharacterSwapCompleted( qGame_t* game )
{
   game->renderer->controllingActorCache = game->controllingCharacter->actor;
}

void qRenderer_SwitchControllingCharacter( qGame_t* game )
{
   if ( game->renderer->controllingActorCache != game->controllingCharacter->actor )
   {
      qRenderStates_StartActorSwap( game->renderer->renderStates->actorSwap, &qRenderer_CharacterSwapCompleted );
   }
}
