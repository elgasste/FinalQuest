#if !defined( SPRITE_TEXTURE_H )
#define SPRITE_TEXTURE_H

#include "common.h"

typedef struct qSpriteTexture_t
{
   sfTexture* sfmlTexture;
   uint32_t frameCount;
}
qSpriteTexture_t;

void qSpriteTexture_Setup( qSpriteTexture_t* texture, const char* filePath, uint32_t frameCount );
void qSpriteTexture_Cleanup( qSpriteTexture_t* texture );

#endif // SPRITE_TEXTURE_H
