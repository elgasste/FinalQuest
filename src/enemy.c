#include "enemy.h"
#include "battle_stats.h"

qEnemy_t* qEnemy_Create()
{
   qEnemy_t* enemy = (qEnemy_t*)qAlloc( sizeof( qEnemy_t ), sfTrue );

   enemy->name[0] = '\0';
   enemy->stats = (qBattleStats_t*)qAlloc( sizeof( qBattleStats_t ), sfTrue );

   return enemy;
}

void qEnemy_Destroy( qEnemy_t* enemy )
{
   qFree( enemy->stats, sizeof( qBattleStats_t ), sfTrue );
   qFree( enemy, sizeof( qEnemy_t ), sfTrue );
}
