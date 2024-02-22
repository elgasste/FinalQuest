#if !defined( ENEMY_H )
#define ENEMY_H

#include "common.h"
#include "enums.h"

typedef struct qBattleStats_t qBattleStats_t;
typedef struct qBattleSprite_t qBattleSprite_t;
typedef struct qRenderObjects_t qRenderObjects_t;

typedef struct qEnemyTemplate_t
{
   char name[STRLEN_SHORT];
   qIndefiniteArticle_t indefiniteArticle;
   qBattleStats_t* baseStats;
   qBattleStats_t* statsSpread;
   uint32_t spriteTextureIndex;
   sfVector2u spriteSize;
   float spriteFrameSeconds;
}
qEnemyTemplate_t;

typedef struct qEnemy_t
{
   char name[STRLEN_SHORT];
   qIndefiniteArticle_t indefiniteArticle;
   qBattleStats_t* stats;
   qBattleSprite_t* sprite;
}
qEnemy_t;

qEnemy_t* qEnemy_Create( qEnemyTemplate_t* template, qRenderObjects_t* renderObjects );
void qEnemy_Destroy( qEnemy_t* enemy );

#endif // ENEMY_H
