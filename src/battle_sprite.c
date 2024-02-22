#include "battle_sprite.h"
#include "battle_sprite_texture.h"
#include "clock.h"

qBattleSprite_t* qBattleSprite_Create( qBattleSpriteTexture_t* texture, sfVector2u size, float frameSeconds )
{
   sfIntRect textureRect = { 0, 0, size.x, size.y };
   sfVector2f scale = { GRAPHICS_SCALE, GRAPHICS_SCALE };

   qBattleSprite_t* sprite = (qBattleSprite_t*)qAlloc( sizeof( qBattleSprite_t ), sfTrue );

   sprite->texture = texture;
   sprite->sfmlSprite = qsfSprite_Create();
   sprite->size = size;

   sfSprite_setTexture( sprite->sfmlSprite, texture->sfmlTexture, sfFalse );
   sfSprite_setTextureRect( sprite->sfmlSprite, textureRect );
   sfSprite_scale( sprite->sfmlSprite, scale );

   sprite->frameSeconds = frameSeconds;
   sprite->elapsedSeconds = 0;

   sprite->state = (qBattleSpriteState_t)0;

   return sprite;
}

void qBattleSprite_Destroy( qBattleSprite_t* sprite )
{
   qsfSprite_Destroy( sprite->sfmlSprite );

   qFree( sprite, sizeof( qBattleSprite_t ), sfTrue );
}

void qBattleSprite_SetPosition( qBattleSprite_t* sprite, sfVector2f pos )
{
   sfSprite_setPosition( sprite->sfmlSprite, pos );
}

void qBattleSprite_SetState( qBattleSprite_t* sprite, qBattleSpriteState_t state )
{
   sfIntRect textureRect = { 0, 0, sprite->size.x, sprite->size.y };

   sprite->state = state;
   sprite->elapsedSeconds = 0;

   if ( state == qBattleSpriteState_Dead )
   {
      textureRect.left = sprite->size.x * ( qBattleSpriteTexture_GetFrameCountFromState( sprite->texture, state ) - 1 );
      textureRect.top = sprite->size.y * (uint32_t)qBattleSpriteState_Death;
   }
   else
   {
      textureRect.top = sprite->size.y * (uint32_t)state;
   }

   sfSprite_setTextureRect( sprite->sfmlSprite, textureRect );
}

void qBattleSprite_Tic( qBattleSprite_t* sprite, qClock_t* clock )
{
   sfIntRect textureRect = sfSprite_getTextureRect( sprite->sfmlSprite );
   uint32_t frameCount;

   if ( sprite->state != qBattleSpriteState_Dead )
   {
      sprite->elapsedSeconds += clock->frameDeltaSeconds;

      while ( sprite->elapsedSeconds > sprite->frameSeconds )
      {
         sprite->elapsedSeconds -= sprite->frameSeconds;

         textureRect.left += sprite->size.x;
         frameCount = qBattleSpriteTexture_GetFrameCountFromState( sprite->texture, sprite->state );

         if ( textureRect.left >= (int32_t)( sprite->size.x * frameCount ) )
         {
            textureRect.left = 0;
         }
      }
   }
}
