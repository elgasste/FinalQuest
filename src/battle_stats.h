#if !defined( BATTLE_STATS_H )
#define BATTLE_STATS_H

#include "common.h"

typedef struct qBattleStats_t
{
   uint32_t hitPoints;
   uint32_t magicPoints;
   uint32_t attackPower;
   uint32_t defensePower;
}
qBattleStats_t;

qBattleStats_t* qBattleStats_Create();
void qBattleStats_Destroy( qBattleStats_t* stats );

#endif // BATTLE_STATS_H
