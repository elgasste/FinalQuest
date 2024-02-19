#include "battle.h"
#include "game.h"
#include "renderer.h"
#include "render_objects.h"
#include "render_states.h"
#include "actor.h"
#include "character.h"
#include "sprite.h"
#include "enemy.h"
#include "battle_stats.h"

qBattle_t* qBattle_Create()
{
   qBattle_t* battle = (qBattle_t*)qAlloc( sizeof( qBattle_t ), sfTrue );

   battle->enemy = qEnemy_Create();

   snprintf( battle->enemy->name, STRLEN_SHORT, "batfuck" );
   battle->enemy->stats->hitPoints = 12;
   battle->enemy->stats->defensePower = 5;
   battle->enemy->stats->attackPower = 5;
   battle->enemy->stats->defensePower = 5;

   return battle;
}

void qBattle_Destroy( qBattle_t* battle )
{
   qEnemy_Destroy( battle->enemy );

   qFree( battle, sizeof( qBattle_t ), sfTrue );
}

void qBattle_Begin( qGame_t* game )
{
   qRenderObjects_t* renderObjects = game->renderer->renderObjects;

   qSprite_Stop( game->controllingCharacter->actor->sprite );

   snprintf( renderObjects->battleDialogBoxLarge->message, STRLEN_DEFAULT - 1, "Something invisible approaches!" );
   snprintf( renderObjects->battleDialogBoxSmall->message, STRLEN_DEFAULT - 1, STR_BATTLE_SELECTACTION );

   qGame_SetState( game, qGameState_BattleIntro );
   qRenderStates_StartTextScroll( game->renderer->renderStates->textScroll, (uint32_t)strlen( renderObjects->battleDialogBoxLarge->message ), sfTrue );
}

void qBattle_Attack( qGame_t* game )
{
   qRenderObjects_t* renderObjects = game->renderer->renderObjects;

   snprintf( renderObjects->battleDialogBoxLarge->message, STRLEN_DEFAULT - 1, "You flailed your arms so comically that the enemy died of laughter." );

   qGame_SetState( game, qGameState_BattleResult );
   qRenderStates_StartTextScroll( game->renderer->renderStates->textScroll, (uint32_t)strlen( renderObjects->battleDialogBoxLarge->message ), sfTrue );
}

void qBattle_Defend( qGame_t* game )
{
   qRenderObjects_t* renderObjects = game->renderer->renderObjects;

   snprintf( renderObjects->battleDialogBoxLarge->message, STRLEN_DEFAULT - 1, "You hid behind your shield so well that the enemy forgot you were even there, and ran off." );

   qGame_SetState( game, qGameState_BattleResult );
   qRenderStates_StartTextScroll( game->renderer->renderStates->textScroll, (uint32_t)strlen( renderObjects->battleDialogBoxLarge->message ), sfTrue );
}

void qBattle_Spell( qGame_t* game )
{
   qRenderObjects_t* renderObjects = game->renderer->renderObjects;

   snprintf( renderObjects->battleDialogBoxLarge->message, STRLEN_DEFAULT - 1, "You cast \"Brazilius Waxus\", and the enemy died just thinking of the pain." );

   qGame_SetState( game, qGameState_BattleResult );
   qRenderStates_StartTextScroll( game->renderer->renderStates->textScroll, (uint32_t)strlen( renderObjects->battleDialogBoxLarge->message ), sfTrue );
}

void qBattle_Item( qGame_t* game )
{
   qRenderObjects_t* renderObjects = game->renderer->renderObjects;

   snprintf( renderObjects->battleDialogBoxLarge->message, STRLEN_DEFAULT - 1, "You threw your spare change right into the enemy's eye, and it died." );

   qGame_SetState( game, qGameState_BattleResult );
   qRenderStates_StartTextScroll( game->renderer->renderStates->textScroll, (uint32_t)strlen( renderObjects->battleDialogBoxLarge->message ), sfTrue );
}

void qBattle_Flee( qGame_t* game )
{
   qRenderObjects_t* renderObjects = game->renderer->renderObjects;

   snprintf( renderObjects->battleDialogBoxLarge->message, STRLEN_DEFAULT - 1, "You ran away, like a little girl. Like a tiny, helpless little girl." );

   qGame_SetState( game, qGameState_BattleResult );
   qRenderStates_StartTextScroll( game->renderer->renderStates->textScroll, (uint32_t)strlen( renderObjects->battleDialogBoxLarge->message ), sfTrue );
}
