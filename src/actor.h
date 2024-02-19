#if !defined( ACTOR_H )
#define ACTOR_H

#include "common.h"
#include "enums.h"

typedef struct qEntity_t qEntity_t;
typedef struct qActorSprite_t qActorSprite_t;
typedef struct qActorSpriteTexture_t qActorSpriteTexture_t;
typedef struct qClock_t qClock_t;

typedef struct qActor_t
{
   qEntity_t* entity;
   qActorSprite_t* sprite;
   sfVector2f spriteOffset;
}
qActor_t;

void qActor_Setup( qActor_t* actor, sfVector2f mapPos, sfVector2f mapHitBoxSize, float maxVelocity,
                   qActorSpriteTexture_t* spriteTexture, sfVector2f spriteOffset, float spriteFrameSeconds );
void qActor_Cleanup( qActor_t* actor );
void qActor_SetDirection( qActor_t* actor, qDirection_t direction );
void qActor_Tic( qActor_t* actor, qClock_t* clock );

#endif // ACTOR_H
