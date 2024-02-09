#include "map.h"

qMap_t* qMap_Create( sfVector2u tileCount )
{
   qMap_t* map = (qMap_t*)qAlloc( sizeof( qMap_t ), sfTrue );

   map->tileCount = tileCount;
   map->tiles = (qMapTile_t*)qAlloc( sizeof( qMapTile_t ) * tileCount.x * tileCount.y, sfTrue );

   return map;
}

void qMap_Destroy( qMap_t* map )
{
   qFree( map->tiles, sizeof( qMapTile_t ) * map->tileCount.x * map->tileCount.y, sfTrue );
   qFree( map, sizeof( qMap_t ), sfTrue );
}

uint32_t qMap_TileIndexFromPos( qMap_t* map, sfVector2f pos )
{
   return ( (uint32_t)( pos.y / MAP_TILE_SIZE ) * map->tileCount.x ) + (uint32_t)( pos.x / MAP_TILE_SIZE );
}
