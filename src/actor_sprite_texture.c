#include "actor_sprite_texture.h"

void qActorSpriteTexture_Setup( qActorSpriteTexture_t* texture, const char* filePath, uint32_t frameCount )
{
   texture->sfmlTexture = qsfTexture_CreateFromFile( filePath );
   texture->frameCount = frameCount;
}

void qActorSpriteTexture_Cleanup( qActorSpriteTexture_t* texture )
{
   qsfTexture_Destroy( texture->sfmlTexture );
}
