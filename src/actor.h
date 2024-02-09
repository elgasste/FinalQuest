#if !defined( ACTOR_H )
#define ACTOR_H

#include "common.h"
#include "enums.h"

typedef struct qEntity_t qEntity_t;
typedef struct qSprite_t qSprite_t;

typedef struct qActor_t
{
   qEntity_t* entity;
   qSprite_t* sprite;
}
qActor_t;

qActor_t* qActor_Create( sfVector2f mapPos, sfVector2f mapHitBoxSize, float maxVelocity,
                         sfTexture* texture, uint32_t frames, float frameSeconds );
void qActor_Destroy( qActor_t* actor );
void qActor_SetDirection( qActor_t* actor, qDirection_t direction );

#endif // ACTOR_H
