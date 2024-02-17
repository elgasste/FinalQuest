#if !defined( CHARACTER_H )
#define CHARACTER_H

#include "common.h"

typedef struct qActor_t qActor_t;

typedef struct qCharacterStats_t
{
   uint32_t hitPoints;
   uint32_t magicPoints;
   uint32_t attackPower;
   uint32_t defensePower;
}
qCharacterStats_t;

typedef struct qCharacter_t
{
   char name[STRLEN_SHORT];
   qCharacterStats_t* stats;
   qActor_t* actor;
}
qCharacter_t;

void qCharacter_Setup( qCharacter_t* character, qActor_t* actor );
void qCharacter_Cleanup( qCharacter_t* stats );

#endif // CHARACTER_H
