#if !defined( ENEMY_H )
#define ENEMY_H

#include "common.h"
#include "enums.h"

typedef struct qBattleStats_t qBattleStats_t;

typedef struct qEnemy_t
{
   char name[STRLEN_SHORT];
   qIndefiniteArticle_t indefiniteArticle;
   qBattleStats_t* stats;
}
qEnemy_t;

qEnemy_t* qEnemy_Create();
void qEnemy_Destroy( qEnemy_t* enemy );

#endif // ENEMY_H
