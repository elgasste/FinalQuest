#include "entity.h"

qEntity_t* qEntity_Create( sfVector2f mapPos, sfVector2f mapHitBoxSize, float maxVelocity )
{
   qEntity_t* entity = (qEntity_t*)qAlloc( sizeof( qEntity_t ), sfTrue );

   entity->mapPos = mapPos;
   entity->mapHitBoxSize = mapHitBoxSize;
   entity->velocity.x = 0;
   entity->velocity.y = 0;
   entity->maxVelocity = maxVelocity;
   entity->direction = (qDirection_t)0;

   return entity;
}

void qEntity_Destroy( qEntity_t* entity )
{
   qFree( entity, sizeof( qEntity_t ), sfTrue );
}
