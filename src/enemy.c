#include "enemy.h"
#include "game.h"
#include "renderer.h"
#include "render_objects.h"
#include "battle_stats.h"
#include "battle_sprite.h"
#include "battle_sprite_texture.h"
#include "math_util.h"

qEnemy_t* qEnemy_Create( qEnemyTemplate_t* template, qRenderObjects_t* renderObjects )
{
   qEnemy_t* enemy = (qEnemy_t*)qAlloc( sizeof( qEnemy_t ), sfTrue );

   snprintf( enemy->name, STRLEN_SHORT - 1, "%s", template->name );
   enemy->indefiniteArticle = template->indefiniteArticle;

   enemy->stats = (qBattleStats_t*)qAlloc( sizeof( qBattleStats_t ), sfTrue );

   enemy->stats->attackPower = qMathUtil_ApplySpread( &( template->baseStats->attackPower ), &( template->statsSpread->attackPower ), sfFalse );
   enemy->stats->defensePower = qMathUtil_ApplySpread( &( template->baseStats->defensePower ), &( template->statsSpread->defensePower ), sfTrue );
   enemy->stats->hitPoints = qMathUtil_ApplySpread( &( template->baseStats->hitPoints ), &( template->statsSpread->hitPoints ), sfFalse );
   enemy->stats->magicPoints = qMathUtil_ApplySpread( &( template->baseStats->magicPoints ), &( template->statsSpread->magicPoints ), sfTrue );

   enemy->sprite = qBattleSprite_Create( &( renderObjects->battleSpriteTextures[template->spriteTextureIndex] ),
                                         template->spriteSize,
                                         template->spriteFrameSeconds );

   return enemy;
}

void qEnemy_Destroy( qEnemy_t* enemy )
{
   qBattleSprite_Destroy( enemy->sprite );

   qFree( enemy->stats, sizeof( qBattleStats_t ), sfTrue );
   qFree( enemy, sizeof( qEnemy_t ), sfTrue );
}
