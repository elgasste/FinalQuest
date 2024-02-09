#if !defined( ENTITY_H )
#define ENTITY_H

#include "common.h"
#include "enums.h"

typedef struct qEntity_t
{
   sfVector2f mapPos;
   sfVector2f mapHitBoxSize;
   sfVector2f velocity;
   float maxVelocity;
   qDirection_t direction;
}
qEntity_t;

qEntity_t* qEntity_Create( sfVector2f mapPos, sfVector2f mapHitBoxSize, float maxVelocity );
void qEntity_Destroy( qEntity_t* entity );

#endif // ENTITY_H
