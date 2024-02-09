#include "physics.h"
#include "game.h"
#include "actor.h"
#include "entity.h"
#include "map.h"
#include "clock.h"

#define COLLISION_PADDING 0.01f

static void qPhysics_TicActor( qGame_t* game, qActor_t* actor );

qPhysics_t* qPhysics_Create()
{
   qPhysics_t* physics = (qPhysics_t*)qAlloc( sizeof( qPhysics_t ), sfTrue );

   physics->entityMapTileCache = 0;

   return physics;
}

void qPhysics_Destroy( qPhysics_t* physics )
{
   qFree( physics, sizeof( qPhysics_t ), sfTrue );
}

void qPhysics_Tic( qGame_t* game )
{
   uint32_t i;

   for ( i = 0; i < game->actorCount; i++ )
   {
      qPhysics_TicActor( game, &( game->actors[i] ) );
   }
}

void qPhysics_TicActor( qGame_t* game, qActor_t* actor )
{
   qEntity_t* entity = actor->entity;
   sfVector2f newPos = entity->mapPos;
   sfVector2f mapSize = { (float)( game->map->tileCount.x * MAP_TILE_SIZE ), (float)( game->map->tileCount.y * MAP_TILE_SIZE ) };
   sfVector2f entityCenterPos;
   uint32_t startRow, endRow, startCol, endCol, row, col;
   qMapTile_t* tile;
   uint32_t newTileIndex;

   newPos.x += entity->velocity.x * game->clock->frameDeltaSeconds;

   if ( newPos.x <= 0 )
   {
      newPos.x = COLLISION_PADDING;
   }
   else if ( newPos.x + entity->mapHitBoxSize.x >= mapSize.x )
   {
      newPos.x = mapSize.x - entity->mapHitBoxSize.x - COLLISION_PADDING;
   }
   else if ( newPos.x != entity->mapPos.x /*&& /*!game->cheatNoClip*/ )
   {
      startRow = (uint32_t)( newPos.y / MAP_TILE_SIZE );
      endRow = (uint32_t)( ( newPos.y + entity->mapHitBoxSize.y ) / MAP_TILE_SIZE );
      col = ( newPos.x < entity->mapPos.x )
         ? (uint32_t)( newPos.x / MAP_TILE_SIZE )
         : (uint32_t)( ( newPos.x + entity->mapHitBoxSize.x ) / MAP_TILE_SIZE );

      for ( row = startRow; row <= endRow; row++ )
      {
         tile = &( game->map->tiles[( row * game->map->tileCount.x ) + col] );

         if ( !tile->isPassable )
         {
            newPos.x = ( newPos.x < entity->mapPos.x )
               ? ( ( col + 1 ) * MAP_TILE_SIZE ) + COLLISION_PADDING
               : ( col * MAP_TILE_SIZE ) - entity->mapHitBoxSize.x - COLLISION_PADDING;
            break;
         }
      }
   }

   newPos.y += entity->velocity.y * game->clock->frameDeltaSeconds;

   if ( newPos.y <= 0 )
   {
      newPos.y = COLLISION_PADDING;
   }
   else if ( newPos.y + entity->mapHitBoxSize.y >= mapSize.y )
   {
      newPos.y = mapSize.y - entity->mapHitBoxSize.y - COLLISION_PADDING;
   }
   else if ( newPos.y != entity->mapPos.y /*&& !game->cheatNoClip*/ )
   {
      startCol = (uint32_t)( newPos.x / MAP_TILE_SIZE );
      endCol = (uint32_t)( ( newPos.x + entity->mapHitBoxSize.x ) / MAP_TILE_SIZE );
      row = ( newPos.y < entity->mapPos.y )
         ? (uint32_t)( newPos.y / MAP_TILE_SIZE )
         : (uint32_t)( ( newPos.y + entity->mapHitBoxSize.y ) / MAP_TILE_SIZE );

      for ( col = startCol; col <= endCol; col++ )
      {
         tile = &( game->map->tiles[( row * game->map->tileCount.x ) + col] );

         if ( !tile->isPassable )
         {
            newPos.y = ( newPos.y < entity->mapPos.y )
               ? ( ( row + 1 ) * MAP_TILE_SIZE ) + COLLISION_PADDING
               : ( row * MAP_TILE_SIZE ) - entity->mapHitBoxSize.y - COLLISION_PADDING;
            break;
         }
      }
   }

   entity->mapPos = newPos;
   qActor_Tic( actor, game->clock );
   entity->velocity.x = 0;
   entity->velocity.y = 0;

   entityCenterPos.x = entity->mapPos.x + ( entity->mapHitBoxSize.x / 2 );
   entityCenterPos.y = entity->mapPos.y + ( entity->mapHitBoxSize.y / 2 );
   newTileIndex = qMap_TileIndexFromPos( game->map, entityCenterPos );

   if ( newTileIndex != game->physics->entityMapTileCache )
   {
      game->physics->entityMapTileCache = newTileIndex;
   }
}
