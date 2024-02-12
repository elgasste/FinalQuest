#include "sprite_texture.h"

void qSpriteTexture_Setup( qSpriteTexture_t* texture, const char* filePath, uint32_t frameCount )
{
   texture->sfmlTexture = qsfTexture_CreateFromFile( filePath );
   texture->frameCount = frameCount;
}

void qSpriteTexture_Cleanup( qSpriteTexture_t* texture )
{
   qsfTexture_Destroy( texture->sfmlTexture );
}
