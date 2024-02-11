#include "physics.h"
#include "game.h"
#include "actor.h"
#include "entity.h"
#include "map.h"
#include "clock.h"
#include "math_util.h"

#define COLLISION_PADDING 0.01f

static void qPhysics_TicActor( qGame_t* game, qActor_t* actor );
static void qPhysics_ClipActorToMapHorizontal( qGame_t* game, qActor_t* actor, sfVector2f* newPos );
static void qPhysics_ClipActorToMapVertical( qGame_t* game, qActor_t* actor, sfVector2f* newPos );
static void qPhysics_ClipActorToActors( qGame_t* game, qActor_t* actor, sfVector2f* newPos, sfBool horizontal );

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
   sfVector2f entityCenterPos;
   uint32_t newTileIndex;

   if ( actor == game->controllingActor && game->cheatFast && entity->velocity.x != 0 )
   {
      newPos.x += ( entity->velocity.x < 0 )
         ? -FAST_VELOCITY * game->clock->frameDeltaSeconds
         : FAST_VELOCITY * game->clock->frameDeltaSeconds;
   }
   else
   {
      newPos.x += entity->velocity.x * game->clock->frameDeltaSeconds;
   }

   qPhysics_ClipActorToMapHorizontal( game, actor, &newPos );
   qPhysics_ClipActorToActors( game, actor, &newPos, sfTrue );

   if ( actor == game->controllingActor && game->cheatFast && entity->velocity.y != 0 )
   {
      newPos.y += ( entity->velocity.y < 0 )
         ? -FAST_VELOCITY * game->clock->frameDeltaSeconds
         : FAST_VELOCITY * game->clock->frameDeltaSeconds;
   }
   else
   {
      newPos.y += entity->velocity.y * game->clock->frameDeltaSeconds;
   }

   qPhysics_ClipActorToMapVertical( game, actor, &newPos );
   qPhysics_ClipActorToActors( game, actor, &newPos, sfFalse );

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

static void qPhysics_ClipActorToMapHorizontal( qGame_t* game, qActor_t* actor, sfVector2f* newPos )
{
   qEntity_t* entity = actor->entity;
   qMapTile_t* tile;
   float mapSizeX = (float)( game->map->tileCount.x * MAP_TILE_SIZE );
   uint32_t startRow, endRow, row, col;

   if ( newPos->x <= 0 )
   {
      newPos->x = COLLISION_PADDING;
   }
   else if ( newPos->x + entity->mapHitBoxSize.x >= mapSizeX )
   {
      newPos->x = mapSizeX - entity->mapHitBoxSize.x - COLLISION_PADDING;
   }
   else if ( newPos->x != entity->mapPos.x && !game->cheatNoClip )
   {
      startRow = (uint32_t)( newPos->y / MAP_TILE_SIZE );
      endRow = (uint32_t)( ( newPos->y + entity->mapHitBoxSize.y ) / MAP_TILE_SIZE );
      col = ( newPos->x < entity->mapPos.x )
         ? (uint32_t)( newPos->x / MAP_TILE_SIZE )
         : (uint32_t)( ( newPos->x + entity->mapHitBoxSize.x ) / MAP_TILE_SIZE );

      for ( row = startRow; row <= endRow; row++ )
      {
         tile = &( game->map->tiles[( row * game->map->tileCount.x ) + col] );

         if ( !tile->isPassable )
         {
            newPos->x = ( newPos->x < entity->mapPos.x )
               ? ( ( col + 1 ) * MAP_TILE_SIZE ) + COLLISION_PADDING
               : ( col * MAP_TILE_SIZE ) - entity->mapHitBoxSize.x - COLLISION_PADDING;
            break;
         }
      }
   }
}

static void qPhysics_ClipActorToMapVertical( qGame_t* game, qActor_t* actor, sfVector2f* newPos )
{
   qEntity_t* entity = actor->entity;
   qMapTile_t* tile;
   float mapSizeY = (float)( game->map->tileCount.y * MAP_TILE_SIZE );
   uint32_t startCol, endCol, row, col;

   if ( newPos->y <= 0 )
   {
      newPos->y = COLLISION_PADDING;
   }
   else if ( newPos->y + entity->mapHitBoxSize.y >= mapSizeY )
   {
      newPos->y = mapSizeY - entity->mapHitBoxSize.y - COLLISION_PADDING;
   }
   else if ( newPos->y != entity->mapPos.y && !game->cheatNoClip )
   {
      startCol = (uint32_t)( newPos->x / MAP_TILE_SIZE );
      endCol = (uint32_t)( ( newPos->x + entity->mapHitBoxSize.x ) / MAP_TILE_SIZE );
      row = ( newPos->y < entity->mapPos.y )
         ? (uint32_t)( newPos->y / MAP_TILE_SIZE )
         : (uint32_t)( ( newPos->y + entity->mapHitBoxSize.y ) / MAP_TILE_SIZE );

      for ( col = startCol; col <= endCol; col++ )
      {
         tile = &( game->map->tiles[( row * game->map->tileCount.x ) + col] );

         if ( !tile->isPassable )
         {
            newPos->y = ( newPos->y < entity->mapPos.y )
               ? ( ( row + 1 ) * MAP_TILE_SIZE ) + COLLISION_PADDING
               : ( row * MAP_TILE_SIZE ) - entity->mapHitBoxSize.y - COLLISION_PADDING;
            break;
         }
      }
   }
}

static void qPhysics_ClipActorToActors( qGame_t* game, qActor_t* actor, sfVector2f* newPos, sfBool horizontal )
{
   qEntity_t* entity = actor->entity;
   qEntity_t* otherEntity;
   qActor_t* otherActor;
   uint32_t i;

   for ( i = 0; i < game->actorCount; i++ )
   {
      otherActor = &( game->actors[i] );

      if ( otherActor == actor )
      {
         continue;
      }

      otherEntity = otherActor->entity;

      if ( qMathUtil_RectsOverlap( newPos->x, newPos->y,
                                   newPos->x + entity->mapHitBoxSize.x, newPos->y + entity->mapHitBoxSize.y,
                                   otherEntity->mapPos.x, otherEntity->mapPos.y,
                                   otherEntity->mapPos.x + otherEntity->mapHitBoxSize.x, otherEntity->mapPos.y + otherEntity->mapHitBoxSize.y ) )
      {
         if ( horizontal && newPos->x >= otherEntity->mapPos.x && newPos->x <= otherEntity->mapPos.x )
         {
            if ( newPos->x < otherEntity->mapPos.x + ( otherEntity->mapHitBoxSize.x / 2 ) )
            {
               newPos->x = otherEntity->mapPos.x - entity->mapHitBoxSize.x - COLLISION_PADDING;
            }
            else
            {
               newPos->x = entity->mapPos.x + entity->mapHitBoxSize.x + COLLISION_PADDING;
            }
         }
         else if ( !horizontal && newPos->y >= otherEntity->mapPos.y && newPos->y <= otherEntity->mapPos.y )
         {
            if ( newPos->y < otherEntity->mapPos.y + ( otherEntity->mapHitBoxSize.y / 2 ) )
            {
               newPos->y = otherEntity->mapPos.y - entity->mapHitBoxSize.y - COLLISION_PADDING;
            }
            else
            {
               newPos->y = entity->mapPos.y + entity->mapHitBoxSize.y + COLLISION_PADDING;
            }
         }
      }
   }
}
