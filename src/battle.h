#if !defined( BATTLE_H )
#define BATTLE_H

#include "common.h"
#include "enums.h"

typedef struct qEnemy_t qEnemy_t;
typedef struct qGame_t qGame_t;

typedef struct qBattle_t
{
   qEnemy_t* enemy;
   qBattleAction_t action;
}
qBattle_t;

qBattle_t* qBattle_Create( qGame_t* game );
void qBattle_Destroy( qBattle_t* battle );
void qBattle_Begin( qGame_t* game );
void qBattle_Action( qGame_t* game, qBattleAction_t action );

#endif // BATTLE_H
