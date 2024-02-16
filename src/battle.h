#if !defined( BATTLE_H )
#define BATTLE_H

#include "common.h"

typedef struct qGame_t qGame_t;

void qBattle_Begin( qGame_t* game );
void qBattle_Attack( qGame_t* game );
void qBattle_Defend( qGame_t* game );
void qBattle_Spell( qGame_t* game );
void qBattle_Item( qGame_t* game );
void qBattle_Flee( qGame_t* game );

#endif // BATTLE_H