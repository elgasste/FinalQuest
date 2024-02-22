#if !defined( ACTOR_SPRITE_TEXTURE_H )
#define ACTOR_SPRITE_TEXTURE_H

#include "common.h"

typedef struct qActorSpriteTexture_t
{
   sfTexture* sfmlTexture;
   uint32_t frameCount;
}
qActorSpriteTexture_t;

void qActorSpriteTexture_Setup( qActorSpriteTexture_t* texture, const char* filePath, uint32_t frameCount );
void qActorSpriteTexture_Cleanup( qActorSpriteTexture_t* texture );

#endif // ACTOR_SPRITE_TEXTURE_H
