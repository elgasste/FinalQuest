#include "renderer.h"
#include "render_objects.h"
#include "render_states.h"
#include "game.h"
#include "window.h"
#include "clock.h"
#include "time_util.h"
#include "map.h"

static void qRenderer_DrawDiagnostics( qGame_t* game );
static void qRenderer_DrawDebugBar( qGame_t* game );
static void qRenderer_SetMapView( qGame_t* game );
static void qRenderer_DrawMap( qGame_t* game );

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

   renderer->mapViewRect.width = WINDOW_WIDTH;
   renderer->mapViewRect.height = WINDOW_HEIGHT;

   return renderer;
}

void qRenderer_Destroy( qRenderer_t* renderer )
{
   qsfRectangleShape_Destroy( renderer->windowBackgroundRect );

   qRenderStates_Destroy( renderer->renderStates );
   qRenderObjects_Destroy( renderer->renderObjects );

   qFree( renderer, sizeof( qRenderer_t ), sfTrue );
}

void qRenderer_Render( qGame_t* game )
{
   qWindow_DrawRectangleShape( game->window, game->renderer->windowBackgroundRect );

   qRenderer_SetMapView( game );
   qRenderer_DrawMap( game );

   qRenderer_DrawDebugBar( game );

   if ( game->showDiagnostics )
   {
      qRenderer_DrawDiagnostics( game );
   }

   qWindow_Display( game->window );
}

static void qRenderer_DrawDiagnostics( qGame_t* game )
{
   char msg[STRLEN_DEFAULT];
   char timeStr[STRLEN_SHORT];
   qDiagnosticsRenderObjects_t* objects = game->renderer->renderObjects->diagnostics;

   qWindow_DrawRectangleShape( game->window, objects->backgroundRect );

   objects->textPosition.y = 4;
   sfText_setPosition( objects->text, objects->textPosition );
   snprintf( msg, STRLEN_DEFAULT, STR_FRAMERATEFORMATTER, GAME_FPS );
   sfText_setString( objects->text, msg );
   qWindow_DrawText( game->window, objects->text );

   objects->textPosition.y += objects->lineSpacing;
   sfText_setPosition( objects->text, objects->textPosition );
   snprintf( msg, STRLEN_DEFAULT, STR_TOTALFRAMESFORMATTER, game->clock->totalFrameCount );
   sfText_setString( objects->text, msg );
   qWindow_DrawText( game->window, objects->text );

   objects->textPosition.y += objects->lineSpacing;
   sfText_setPosition( objects->text, objects->textPosition );
   snprintf( msg, STRLEN_DEFAULT, STR_LAGFRAMESFORMATTER, game->clock->lagFrameCount );
   sfText_setString( objects->text, msg );
   qWindow_DrawText( game->window, objects->text );

   objects->textPosition.y += objects->lineSpacing;
   sfText_setPosition( objects->text, objects->textPosition );
   qTimeUtil_FormatTime( timeStr, STRLEN_SHORT, (int32_t)( game->clock->realTotalDurationMicro / 1000000 ) );
   snprintf( msg, STRLEN_DEFAULT, STR_ELAPSEDTIMEFORMATTER, timeStr );
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

static void qRenderer_SetMapView( qGame_t* game )
{
   qRenderer_t* renderer = game->renderer;
   qMap_t* map = game->map;
   uint32_t scaledTileSize = MAP_TILE_SIZE * GRAPHICS_SCALE;
   sfVector2f mapSize = { (float)( map->tileCount.x * scaledTileSize ), (float)( map->tileCount.y * scaledTileSize ) };
   sfVector2f entityPos = { 0, 0 };

   renderer->mapViewRect.left = entityPos.x - ( WINDOW_WIDTH / 2 );
   renderer->mapViewRect.top = entityPos.y - ( WINDOW_HEIGHT / 2 );

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
      renderer->mapViewEnd.x = (uint32_t)( renderer->mapViewRect.width / scaledTileSize ) + 1;
   }
   else if ( ( renderer->mapViewRect.left + renderer->mapViewRect.width ) >= mapSize.x )
   {
      renderer->mapViewRect.left = mapSize.x - renderer->mapViewRect.width - 1;
      renderer->mapViewPadding.x = 0;
      renderer->mapTilePixelOffset.x = -( renderer->mapViewRect.left - (float)( (int32_t)renderer->mapViewRect.left / scaledTileSize ) * scaledTileSize );
      renderer->mapViewEnd.x = map->tileCount.x;
      renderer->mapViewStart.x = renderer->mapViewEnd.x - (uint32_t)( renderer->mapViewRect.width / scaledTileSize ) - 1;
   }
   else
   {
      renderer->mapTilePixelOffset.x = -( renderer->mapViewRect.left - (float)( (int32_t)renderer->mapViewRect.left / scaledTileSize ) * scaledTileSize );
      renderer->mapViewStart.x = (uint32_t)( renderer->mapViewRect.left / scaledTileSize );
      renderer->mapViewEnd.x = renderer->mapViewStart.x + (uint32_t)( renderer->mapViewRect.width / scaledTileSize ) + 1;
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
      renderer->mapViewEnd.y = (uint32_t)( renderer->mapViewRect.height / scaledTileSize ) + 1;
   }
   else if ( ( renderer->mapViewRect.top + renderer->mapViewRect.height ) >= mapSize.y )
   {
      renderer->mapViewRect.top = mapSize.y - renderer->mapViewRect.height - 1;
      renderer->mapViewPadding.y = 0;
      renderer->mapTilePixelOffset.y = -( renderer->mapViewRect.top - (float)( (int32_t)renderer->mapViewRect.top / scaledTileSize ) * scaledTileSize );
      renderer->mapViewEnd.y = map->tileCount.y;
      renderer->mapViewStart.y = renderer->mapViewEnd.y - (uint32_t)( renderer->mapViewRect.height / scaledTileSize ) - 1;
   }
   else
   {
      renderer->mapTilePixelOffset.y = -( renderer->mapViewRect.top - (float)( (int32_t)renderer->mapViewRect.top / scaledTileSize ) * scaledTileSize );
      renderer->mapViewStart.y = (uint32_t)( renderer->mapViewRect.top / scaledTileSize );
      renderer->mapViewEnd.y = renderer->mapViewStart.y + (uint32_t)( renderer->mapViewRect.height / scaledTileSize ) + 1;
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
   uint32_t scaledTileSize = MAP_TILE_SIZE * GRAPHICS_SCALE;

   for ( row = 0, tileRow = renderer->mapViewStart.y; tileRow <= renderer->mapViewEnd.y; row++, tileRow++ )
   {
      for ( col = 0, tileCol = renderer->mapViewStart.x; tileCol <= renderer->mapViewEnd.x; col++, tileCol++ )
      {
         tile = &( map->tiles[( tileRow * map->tileCount.x ) + tileCol] );

         tilePos.x = ( (float)col * scaledTileSize ) + renderer->mapTilePixelOffset.x + renderer->mapViewPadding.x;
         tilePos.y = ( (float)row * scaledTileSize ) + renderer->mapTilePixelOffset.y + renderer->mapViewPadding.y;
         textureRect.left = ( tile->textureIndex % textureTileCount.x ) * MAP_TILE_SIZE;
         textureRect.top = ( tile->textureIndex / textureTileCount.x ) * MAP_TILE_SIZE;

         sfSprite_setTextureRect( objects->tileSprite, textureRect );
         sfSprite_setPosition( objects->tileSprite, tilePos );

         qWindow_DrawSprite( game->window, objects->tileSprite );
      }
   }
}
