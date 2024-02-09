#if !defined( SPRITE_TEXTURE_H )
#define SPRITE_TEXTURE_H

#include "common.h"

typedef struct qSpriteTexture_t
{
   sfTexture* sfmlTexture;
   uint32_t frameCount;
}
qSpriteTexture_t;

qSpriteTexture_t* qSpriteTexture_Create( const char* filePath, uint32_t frameCount );
void qSpriteTexture_Destroy( qSpriteTexture_t* texture );

#endif // SPRITE_TEXTURE_H
