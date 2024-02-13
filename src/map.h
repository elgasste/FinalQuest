#if !defined( MAP_H )
#define MAP_H

#include "common.h"

typedef struct qMapTile_t
{
   uint32_t textureIndex;
   sfBool isPassable;
   uint8_t encounterRate;
}
qMapTile_t;

typedef struct qMap_t
{
   qMapTile_t* tiles;
   sfVector2u tileCount;
}
qMap_t;

qMap_t* qMap_Create( sfVector2u tileCount );
void qMap_Destroy( qMap_t* map );
uint32_t qMap_TileIndexFromPos( qMap_t* map, sfVector2f pos );

#endif // MAP_H
