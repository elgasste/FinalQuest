#include "actor.h"
#include "entity.h"

qActor_t* qActor_Create( sfVector2f mapPos, sfVector2f mapHitBoxSize, float maxVelocity )
{
   qActor_t* actor = (qActor_t*)qAlloc( sizeof( qActor_t ), sfTrue );

   actor->entity = qEntity_Create( mapPos, mapHitBoxSize, maxVelocity );

   return actor;
}

void qActor_Destroy( qActor_t* actor )
{
   qEntity_Destroy( actor->entity );

   qFree( actor, sizeof( qActor_t ), sfTrue );
}
