#include "character.h"

void qCharacter_Setup( qCharacter_t* character, qActor_t* actor )
{
   character->name[0] = '\0';
   character->stats = (qCharacterStats_t*)qAlloc( sizeof( qCharacterStats_t ), sfTrue );
   character->actor = actor;
}

void qCharacter_Cleanup( qCharacter_t* character )
{
   qFree( character->stats, sizeof( qCharacterStats_t ), sfTrue );
}
