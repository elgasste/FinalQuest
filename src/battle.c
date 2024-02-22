#include "battle.h"
#include "game.h"
#include "renderer.h"
#include "render_objects.h"
#include "render_states.h"
#include "actor.h"
#include "character.h"
#include "actor_sprite.h"
#include "enemy.h"
#include "battle_stats.h"
#include "text_util.h"
#include "battle_sprite.h"

qBattle_t* qBattle_Create( qGame_t* game )
{
   qBattle_t* battle = (qBattle_t*)qAlloc( sizeof( qBattle_t ), sfTrue );

   battle->enemy = qEnemy_Create( &( game->enemyTemplates[0] ), game->renderer->renderObjects );

   qBattleSprite_SetState( battle->enemy->sprite, qBattleSpriteState_Idle );

   return battle;
}

void qBattle_Destroy( qBattle_t* battle )
{
   qEnemy_Destroy( battle->enemy );

   qFree( battle, sizeof( qBattle_t ), sfTrue );
}

void qBattle_Begin( qGame_t* game )
{
   qBattle_t* battle = game->battle;
   qRenderObjects_t* renderObjects = game->renderer->renderObjects;

   qActorSprite_Stop( game->controllingCharacter->actor->sprite );

   snprintf( renderObjects->battleDialogBoxLarge->message,
             STRLEN_DEFAULT - 1,
             STR_BATTLE_INTROFORMATTER,
             qTextUtil_IndefiniteArticleFromEnum( battle->enemy->indefiniteArticle, sfFalse ),
             battle->enemy->name );
   snprintf( renderObjects->battleDialogBoxSmall->message, STRLEN_DEFAULT - 1, STR_BATTLE_SELECTACTION );

   qGame_SetState( game, qGameState_BattleIntro );
   qRenderStates_StartTextScroll( game->renderer->renderStates->textScroll, (uint32_t)strlen( renderObjects->battleDialogBoxLarge->message ), sfTrue );
}

void qBattle_Action( qGame_t* game, qBattleAction_t action )
{
   qRenderObjects_t* renderObjects = game->renderer->renderObjects;

   game->battle->action = action;

   switch ( action )
   {
      case qBattleAction_Attack:
         snprintf( renderObjects->battleDialogBoxLarge->message, STRLEN_DEFAULT - 1, "You flailed your arms so comically that the enemy died of laughter." );
         break;
      case qBattleAction_Defend:
         snprintf( renderObjects->battleDialogBoxLarge->message, STRLEN_DEFAULT - 1, "You hid behind your shield so well that the enemy forgot you were even there, and ran off." );
         break;
      case qBattleAction_Spell:
         snprintf( renderObjects->battleDialogBoxLarge->message, STRLEN_DEFAULT - 1, "You cast \"Brazilius Waxus\", and the enemy died just thinking of the pain." );
         break;
      case qBattleAction_Item:
         snprintf( renderObjects->battleDialogBoxLarge->message, STRLEN_DEFAULT - 1, "You threw your spare change right into the enemy's eye, and it died." );
         break;
      case qBattleAction_Flee:
         snprintf( renderObjects->battleDialogBoxLarge->message, STRLEN_DEFAULT - 1, "You ran away, like a little girl. Like a tiny, helpless little girl." );
         break;
   }

   qGame_SetState( game, qGameState_BattleResult );
   qRenderStates_StartTextScroll( game->renderer->renderStates->textScroll, (uint32_t)strlen( renderObjects->battleDialogBoxLarge->message ), sfTrue );
}
