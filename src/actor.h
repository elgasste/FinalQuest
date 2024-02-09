#if !defined( ACTOR_H )
#define ACTOR_H

#include "common.h"

typedef struct qEntity_t qEntity_t;

typedef struct qActor_t
{
   qEntity_t* entity;
}
qActor_t;

qActor_t* qActor_Create( sfVector2f mapPos, sfVector2f mapHitBoxSize, float maxVelocity );
void qActor_Destroy( qActor_t* actor );

#endif // ACTOR_H
