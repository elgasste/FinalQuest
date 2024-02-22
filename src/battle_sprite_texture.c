#include "battle_sprite_texture.h"

void qBattleSpriteTexture_Setup( qBattleSpriteTexture_t* texture,
                                 const char* filePath,
                                 uint32_t idleFrameCount,
                                 uint32_t attackFrameCount,
                                 uint32_t damageFrameCount,
                                 uint32_t deathFrameCount )
{
   texture->sfmlTexture = qsfTexture_CreateFromFile( filePath );

   texture->idleFrameCount = idleFrameCount;
   texture->attackFrameCount = attackFrameCount;
   texture->damageFrameCount = damageFrameCount;
   texture->deathFrameCount = deathFrameCount;
}

void qBattleSpriteTexture_Cleanup( qBattleSpriteTexture_t* texture )
{
   qsfTexture_Destroy( texture->sfmlTexture );
}

uint32_t qBattleSpriteTexture_GetFrameCountFromState( qBattleSpriteTexture_t* texture,
                                                      qBattleSpriteState_t state )
{
   switch ( state )
   {
      case qBattleSpriteState_Idle:
         return texture->idleFrameCount;
         break;
      case qBattleSpriteState_Attack:
         return texture->attackFrameCount;
         break;
      case qBattleSpriteState_Damage:
         return texture->damageFrameCount;
         break;
      case qBattleSpriteState_Death:
         return texture->deathFrameCount;
         break;
   }

   return 0;
}
