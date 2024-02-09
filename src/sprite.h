#if !defined( SPRITE_H )
#define SPRITE_H

#include "common.h"
#include "enums.h"

typedef struct qClock_t qClock_t;

typedef struct qSprite_t
{
   sfTexture* texture;
   sfSprite* sfmlSprite;
   uint32_t frames;
   float frameSeconds;
   float elapsedSeconds;
}
qSprite_t;

qSprite_t* qSprite_Create( sfTexture* texture, uint32_t frames, float frameSeconds );
void qSprite_Destroy( qSprite_t* sprite );
void qSprite_SetPosition( qSprite_t* sprite, sfVector2f pos );
void qSprite_SetDirection( qSprite_t* sprite, qDirection_t direction );
void qSprite_Tic( qSprite_t* sprite, qDirection_t direction, qClock_t* clock );
void qSprite_Stop( qSprite_t* sprite );

#endif // SPRITE_H
