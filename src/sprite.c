#include "sprite.h"
#include "sprite_texture.h"
#include "clock.h"

qSprite_t* qSprite_Create( qSpriteTexture_t* texture, float frameSeconds )
{
   sfIntRect textureRect = { 0, 0, 32, 32 };
   sfVector2f scale = { GRAPHICS_SCALE, GRAPHICS_SCALE };

   qSprite_t* sprite = (qSprite_t*)qAlloc( sizeof( qSprite_t ), sfTrue );

   sprite->texture = texture;
   sprite->sfmlSprite = qsfSprite_Create();

   sfSprite_setTexture( sprite->sfmlSprite, texture->sfmlTexture, sfFalse );
   sfSprite_setTextureRect( sprite->sfmlSprite, textureRect );
   sfSprite_scale( sprite->sfmlSprite, scale );

   sprite->frameSeconds = frameSeconds;
   sprite->elapsedSeconds = 0;

   return sprite;
}

void qSprite_Destroy( qSprite_t* sprite )
{
   qsfSprite_Destroy( sprite->sfmlSprite );

   qFree( sprite, sizeof( qSprite_t ), sfTrue );
}

void qSprite_SetPosition( qSprite_t* sprite, sfVector2f pos )
{
   sfSprite_setPosition( sprite->sfmlSprite, pos );
}

void qSprite_SetDirection( qSprite_t* sprite, qDirection_t direction )
{
   sfIntRect textureRect = sfSprite_getTextureRect( sprite->sfmlSprite );
   textureRect.top = (int32_t)direction * textureRect.height;
   sfSprite_setTextureRect( sprite->sfmlSprite, textureRect );
}

void qSprite_Tic( qSprite_t* sprite, qDirection_t direction, qClock_t* clock )
{
   sfIntRect textureRect = sfSprite_getTextureRect( sprite->sfmlSprite );
   textureRect.top = (int32_t)direction * textureRect.height;

   sprite->elapsedSeconds += clock->frameDeltaSeconds;

   while ( sprite->elapsedSeconds > sprite->frameSeconds )
   {
      sprite->elapsedSeconds -= sprite->frameSeconds;
      textureRect.left += textureRect.width;

      if ( textureRect.left >= (int32_t)( textureRect.width * sprite->texture->frameCount ) )
      {
         textureRect.left = 0;
      }
   }

   sfSprite_setTextureRect( sprite->sfmlSprite, textureRect );
}

void qSprite_Stop( qSprite_t* sprite )
{
   sfIntRect textureRect = sfSprite_getTextureRect( sprite->sfmlSprite );
   textureRect.left = 0;
   sfSprite_setTextureRect( sprite->sfmlSprite, textureRect );
   sprite->elapsedSeconds = 0;
}
