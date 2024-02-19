#if !defined( ACTOR_SPRITE_H )
#define ACTOR_SPRITE_H

#include "common.h"
#include "enums.h"

typedef struct qClock_t qClock_t;
typedef struct qActorSpriteTexture_t qActorSpriteTexture_t;

typedef struct qActorSprite_t
{
   qActorSpriteTexture_t* texture;
   sfSprite* sfmlSprite;
   float frameSeconds;
   float elapsedSeconds;
}
qActorSprite_t;

qActorSprite_t* qSprite_Create( qActorSpriteTexture_t* texture, float frameSeconds );
void qActorSprite_Destroy( qActorSprite_t* sprite );
void qActorSprite_SetPosition( qActorSprite_t* sprite, sfVector2f pos );
void qActorSprite_SetDirection( qActorSprite_t* sprite, qDirection_t direction );
void qActorSprite_Tic( qActorSprite_t* sprite, qDirection_t direction, qClock_t* clock );
void qActorSprite_Stop( qActorSprite_t* sprite );

#endif // ACTOR_SPRITE_H
