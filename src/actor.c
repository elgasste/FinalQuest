#include "actor.h"
#include "entity.h"
#include "sprite.h"

qActor_t* qActor_Create( sfVector2f mapPos, sfVector2f mapHitBoxSize, float maxVelocity,
                         qSpriteTexture_t* spriteTexture, sfVector2f spriteOffset, float spriteFrameSeconds )
{
   qActor_t* actor = (qActor_t*)qAlloc( sizeof( qActor_t ), sfTrue );

   actor->entity = qEntity_Create( mapPos, mapHitBoxSize, maxVelocity );
   actor->sprite = qSprite_Create( spriteTexture, spriteFrameSeconds );
   actor->spriteOffset = spriteOffset;

   return actor;
}

void qActor_Destroy( qActor_t* actor )
{
   qSprite_Destroy( actor->sprite );
   qEntity_Destroy( actor->entity );

   qFree( actor, sizeof( qActor_t ), sfTrue );
}

void qActor_SetDirection( qActor_t* actor, qDirection_t direction )
{
   actor->entity->direction = direction;
   qSprite_SetDirection( actor->sprite, direction );
}

void qActor_Tic( qActor_t* actor, qClock_t* clock )
{
   if ( actor->entity->velocity.x == 0 && actor->entity->velocity.y == 0 )
   {
      qSprite_Stop( actor->sprite );
   }
   else
   {
      qSprite_Tic( actor->sprite, actor->entity->direction, clock );
   }
}
