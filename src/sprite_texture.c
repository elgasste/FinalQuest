#include "sprite_texture.h"

qSpriteTexture_t* qSpriteTexture_Create( const char* filePath, uint32_t frameCount )
{
   qSpriteTexture_t* texture = (qSpriteTexture_t*)qAlloc( sizeof( qSpriteTexture_t ), sfTrue );

   texture->sfmlTexture = qsfTexture_CreateFromFile( filePath );
   texture->frameCount = frameCount;

   return texture;
}

void qSpriteTexture_Destroy( qSpriteTexture_t* texture )
{
   qsfTexture_Destroy( texture->sfmlTexture );

   qFree( texture, sizeof( qSpriteTexture_t ), sfTrue );
}
