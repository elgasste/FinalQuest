#if !defined( BATTLE_SPRITE_TEXTURE_H )
#define BATTLE_SPRITE_TEXTURE_H

#include "common.h"
#include "enums.h"

typedef struct qBattleSpriteTexture_t
{
   sfTexture* sfmlTexture;
   sfSprite* sfmlSprite;

   uint32_t idleFrameCount;
   uint32_t attackFrameCount;
   uint32_t damageFrameCount;
   uint32_t deathFrameCount;
}
qBattleSpriteTexture_t;

void qBattleSpriteTexture_Setup( qBattleSpriteTexture_t* texture,
                                 const char* filePath,
                                 uint32_t idleFrameCount,
                                 uint32_t attackFrameCount,
                                 uint32_t damageFrameCount,
                                 uint32_t deathFrameCount );
void qBattleSpriteTexture_Cleanup( qBattleSpriteTexture_t* texture );
uint32_t qBattleSpriteTexture_GetFrameCountFromState( qBattleSpriteTexture_t* texture,
                                                      qBattleSpriteState_t state );

#endif // BATTLE_SPRITE_TEXTURE_H
