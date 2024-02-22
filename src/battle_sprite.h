#if !defined( BATTLE_SPRITE_H )
#define BATTLE_SPRITE_H

#include "common.h"
#include "enums.h"

typedef struct qBattleSpriteTexture_t qBattleSpriteTexture_t;
typedef struct qClock_t qClock_t;

typedef struct qBattleSprite_t
{
   qBattleSpriteTexture_t* texture;
   sfSprite* sfmlSprite;
   sfVector2u size;
   float frameSeconds;
   float elapsedSeconds;
   qBattleSpriteState_t state;
}
qBattleSprite_t;

qBattleSprite_t* qBattleSprite_Create( qBattleSpriteTexture_t* texture, sfVector2u size, float frameSeconds );
void qBattleSprite_Destroy( qBattleSprite_t* sprite );
void qBattleSprite_SetPosition( qBattleSprite_t* sprite, sfVector2f pos );
void qBattleSprite_SetState( qBattleSprite_t* sprite, qBattleSpriteState_t state );
void qBattleSprite_Tic( qBattleSprite_t* sprite, qClock_t* clock );

#endif // BATTLE_SPRITE_H
