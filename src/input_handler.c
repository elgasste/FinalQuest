#include "game.h"
#include "input_state.h"

void qInputHandler_HandleInput( qGame_t* game )
{
   if ( qInputState_WasKeyPressed( game->inputState, sfKeyEscape ) )
   {
      qGame_Close( game );
   }
   else if ( qInputState_WasKeyPressed( game->inputState, sfKeyF8 ) )
   {
      TOGGLE_BOOL( game->showDiagnostics );

      if ( game->showDiagnostics )
      {
         qGame_ShowDebugMessage( game, STR_DEBUG_DIAGNOSTICSON );
      }
      else
      {
         qGame_ShowDebugMessage( game, STR_DEBUG_DIAGNOSTICSOFF );
      }
   }
}
