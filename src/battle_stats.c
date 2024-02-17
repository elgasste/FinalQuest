#include "battle_stats.h"

qBattleStats_t* qBattleStats_Create()
{
   qBattleStats_t* stats = (qBattleStats_t*)qAlloc( sizeof( qBattleStats_t ), sfTrue );

   return stats;
}

void qBattleStats_Destroy( qBattleStats_t* stats )
{
   qFree( stats, sizeof( qBattleStats_t ), sfTrue );
}
