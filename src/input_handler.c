#include "input_handler.h"
#include "game.h"
#include "input_state.h"
#include "actor.h"
#include "entity.h"
#include "menu.h"
#include "renderer.h"
#include "render_states.h"

static void qInputHandler_HandleMapInput( qGame_t* game );
static void qInputHandler_HandleMapMenuInput( qGame_t* game );
static sfBool qInputHandler_HandleMenuSelection( qGame_t* game, qMenu_t* menu );
static void qInputHandler_CheckCheats( qGame_t* game );
static void qInputHandler_ApplyCheat( qGame_t* game );

qInputHandler_t* qInputHandler_Create()
{
   qInputHandler_t* inputHandler = (qInputHandler_t*)qAlloc( sizeof( qInputHandler_t ), sfTrue );

   inputHandler->cheatString[0] = '\0';

   return inputHandler;
}

void qInputHandler_Destroy( qInputHandler_t* inputHandler )
{
   qFree( inputHandler, sizeof( qInputHandler_t ), sfTrue );
}

void qInputHandler_HandleInput( qGame_t* game )
{
   qInputHandler_CheckCheats( game );

   if ( qInputState_WasKeyPressed( game->inputState, sfKeyF8 ) )
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

   switch ( game->state )
   {
      case qGameState_Map:
         qInputHandler_HandleMapInput( game );
         break;
      case qGameState_MapMenu:
         qInputHandler_HandleMapMenuInput( game );
         break;
      case qGameState_Battle:
         if ( game->inputState->keyWasPressed )
         {
            qGame_SetState( game, qGameState_Map );
         }
         break;
   }
}

static void qInputHandler_HandleMapInput( qGame_t* game )
{
   qActor_t* actor = game->controllingActor;
   qEntity_t* entity = actor->entity;

   sfBool leftIsDown = qInputState_IsKeyDown( sfKeyLeft );
   sfBool upIsDown = qInputState_IsKeyDown( sfKeyUp );
   sfBool rightIsDown = qInputState_IsKeyDown( sfKeyRight );
   sfBool downIsDown = qInputState_IsKeyDown( sfKeyDown );

   if ( qInputState_WasKeyPressed( game->inputState, sfKeyEscape ) )
   {
      qGame_SetState( game, qGameState_MapMenu );
      return;
   }

   if ( qInputState_WasKeyPressed( game->inputState, sfKeyTab ) )
   {
      qGame_SwitchControllingActor( game );
      return;
   }

   if ( leftIsDown && !rightIsDown )
   {
      entity->velocity.x = -entity->maxVelocity;

      if ( upIsDown || downIsDown )
      {
         entity->velocity.x *= 0.707f;
      }

      if ( !( upIsDown && entity->direction == qDirection_Up ) &&
           !( downIsDown && entity->direction == qDirection_Down ) )
      {
         entity->direction = qDirection_Left;
      }
   }

   if ( upIsDown && !downIsDown )
   {
      entity->velocity.y = -entity->maxVelocity;

      if ( leftIsDown || rightIsDown )
      {
         entity->velocity.y *= 0.707f;
      }

      if ( !( leftIsDown && entity->direction == qDirection_Left ) &&
           !( rightIsDown && entity->direction == qDirection_Right ) )
      {
         entity->direction = qDirection_Up;
      }
   }

   if ( rightIsDown && !leftIsDown )
   {
      entity->velocity.x = entity->maxVelocity;

      if ( upIsDown || downIsDown )
      {
         entity->velocity.x *= 0.707f;
      }

      if ( !( upIsDown && entity->direction == qDirection_Up ) &&
           !( downIsDown && entity->direction == qDirection_Down ) )
      {
         entity->direction = qDirection_Right;
      }
   }

   if ( downIsDown && !upIsDown )
   {
      entity->velocity.y = entity->maxVelocity;

      if ( leftIsDown || rightIsDown )
      {
         entity->velocity.y *= 0.707f;
      }

      if ( !( leftIsDown && entity->direction == qDirection_Left ) &&
           !( rightIsDown && entity->direction == qDirection_Right ) )
      {
         entity->direction = qDirection_Down;
      }
   }
}

static void qInputHandler_HandleMapMenuInput( qGame_t* game )
{
   qMenu_t* menu = game->menus->map;

   if ( qInputState_WasKeyPressed( game->inputState, sfKeyEscape ) )
   {
      qGame_SetState( game, qGameState_Map );
   }
   else if ( qInputHandler_HandleMenuSelection( game, game->menus->map ) )
   {
      qGame_ExecuteMenuCommand( game, menu->options[menu->selectedIndex].command );
   }
}

static sfBool qInputHandler_HandleMenuSelection( qGame_t* game, qMenu_t* menu )
{
   qMenuOption_t* selectedOption;

   if ( qInputState_WasKeyPressed( game->inputState, sfKeyUp ) )
   {
      qMenu_ScrollUp( menu, game->renderer->renderStates->menu );
   }
   else if ( qInputState_WasKeyPressed( game->inputState, sfKeyDown ) )
   {
      qMenu_ScrollDown( menu, game->renderer->renderStates->menu );
   }
   else if ( qInputState_WasKeyPressed( game->inputState, sfKeyReturn ) )
   {
      selectedOption = &( menu->options[menu->selectedIndex] );
      return sfTrue;
   }

   return sfFalse;
}

static void qInputHandler_CheckCheats( qGame_t* game )
{
   int32_t cheatStringLength, i, l, lastIndex, matchCount;
   static const char* cheats[] = {
      CHEAT_NOCLIP,
      CHEAT_FAST,
      CHEAT_CLEAR
   };
   static int32_t cheatCount = (int32_t)( sizeof( cheats ) / sizeof( const char* ) );
   qInputHandler_t* inputHandler = game->inputHandler;

   if ( !game->inputState->keyWasPressed )
   {
      return;
   }

   cheatStringLength = (int32_t)strlen( inputHandler->cheatString );
   inputHandler->cheatString[cheatStringLength] = (char)( game->inputState->lastPressedKey + 97 );
   cheatStringLength++;
   inputHandler->cheatString[cheatStringLength] = '\0';

   matchCount = cheatCount;
   lastIndex = cheatStringLength - 1;

   for ( i = 0; i < cheatCount; i++ )
   {
      l = (int32_t)strlen( cheats[i] );

      if ( lastIndex >= l || cheats[i][lastIndex] != inputHandler->cheatString[lastIndex] )
      {
         matchCount--;
      }
      else if ( cheatStringLength == l && !strcmp( cheats[i], inputHandler->cheatString ) )
      {
         qInputHandler_ApplyCheat( game );
         return;
      }
   }

   if ( matchCount == 0 )
   {
      inputHandler->cheatString[0] = '\0';
   }
}

static void qInputHandler_ApplyCheat( qGame_t* game )
{
   char cheatMsg[STRLEN_SHORT];
   char* cheat = game->inputHandler->cheatString;

   if ( !strcmp( cheat, CHEAT_NOCLIP ) )
   {
      TOGGLE_BOOL( game->cheatNoClip );
      snprintf( cheatMsg, STRLEN_SHORT - 1, STR_CHEAT_NOCLIPFORMATTER, game->cheatNoClip ? STR_ON : STR_OFF );
      qGame_ShowDebugMessage( game, cheatMsg );
   }
   else if ( !strcmp( cheat, CHEAT_FAST ) )
   {
      TOGGLE_BOOL( game->cheatFast );
      snprintf( cheatMsg, STRLEN_SHORT - 1, STR_CHEAT_FASTFORMATTER, game->cheatFast ? STR_ON : STR_OFF );
      qGame_ShowDebugMessage( game, cheatMsg );
   }
   else if ( !strcmp( cheat, CHEAT_CLEAR ) )
   {
      game->cheatNoClip = sfFalse;
      game->cheatFast = sfFalse;
      qGame_ShowDebugMessage( game, STR_CHEAT_CLEARED );
   }

   cheat[0] = '\0';
}
