#include "battle.h"
#include "game.h"
#include "renderer.h"
#include "render_objects.h"

void qBattle_Begin( qGame_t* game )
{
   snprintf( game->renderer->renderObjects->battleDialogBoxLarge->message, STRLEN_DEFAULT - 1, "Something invisible approaches!" );
   qGame_SetState( game, qGameState_BattleIntro );
}
