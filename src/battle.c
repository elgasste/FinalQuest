#include "battle.h"
#include "game.h"
#include "renderer.h"
#include "render_objects.h"

void qBattle_Begin( qGame_t* game )
{
   qRenderObjects_t* renderObjects = game->renderer->renderObjects;

   snprintf( renderObjects->battleDialogBoxLarge->message, STRLEN_DEFAULT - 1, "Something invisible approaches!" );
   snprintf( renderObjects->battleDialogBoxSmall->message, STRLEN_DEFAULT - 1, STR_BATTLE_SELECTACTION );

   qGame_SetState( game, qGameState_BattleIntro );
}
