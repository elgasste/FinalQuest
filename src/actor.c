#include "actor.h"
#include "entity.h"
#include "actor_sprite.h"

void qActor_Setup( qActor_t* actor, sfVector2f mapPos, sfVector2f mapHitBoxSize, float maxVelocity,
                   qActorSpriteTexture_t* spriteTexture, sfVector2f spriteOffset, float spriteFrameSeconds )
{
   actor->entity = qEntity_Create( mapPos, mapHitBoxSize, maxVelocity );
   actor->sprite = qSprite_Create( spriteTexture, spriteFrameSeconds );
   actor->spriteOffset = spriteOffset;
}

void qActor_Cleanup( qActor_t* actor )
{
   qActorSprite_Destroy( actor->sprite );
   qEntity_Destroy( actor->entity );
}

void qActor_SetDirection( qActor_t* actor, qDirection_t direction )
{
   actor->entity->direction = direction;
   qActorSprite_SetDirection( actor->sprite, direction );
}

void qActor_Tic( qActor_t* actor, qClock_t* clock )
{
   if ( actor->entity->velocity.x == 0 && actor->entity->velocity.y == 0 )
   {
      qActorSprite_Stop( actor->sprite );
   }
   else
   {
      qActorSprite_Tic( actor->sprite, actor->entity->direction, clock );
   }
}
