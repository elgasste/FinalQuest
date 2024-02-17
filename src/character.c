#include "character.h"
#include "battle_stats.h"

void qCharacter_Setup( qCharacter_t* character, qActor_t* actor )
{
   character->name[0] = '\0';
   character->stats = qBattleStats_Create();
   character->actor = actor;
}

void qCharacter_Cleanup( qCharacter_t* character )
{
   qBattleStats_Destroy( character->stats );
}
