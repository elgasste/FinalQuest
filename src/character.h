#if !defined( CHARACTER_H )
#define CHARACTER_H

#include "common.h"

typedef struct qBattleStats_t qBattleStats_t;
typedef struct qActor_t qActor_t;

typedef struct qCharacter_t
{
   char name[STRLEN_SHORT];
   qBattleStats_t* stats;
   qActor_t* actor;
}
qCharacter_t;

void qCharacter_Setup( qCharacter_t* character, qActor_t* actor );
void qCharacter_Cleanup( qCharacter_t* stats );

#endif // CHARACTER_H
