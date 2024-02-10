#include <string.h>

#include "input_handler.h"
#include "game.h"
#include "input_state.h"
#include "actor.h"
#include "entity.h"

static void qInputHandler_HandleMapInput( qGame_t* game );
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
   if ( qInputState_WasKeyPressed( game->inputState, sfKeyEscape ) )
   {
      qGame_Close( game );
      return;
   }

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

   qInputHandler_HandleMapInput( game );
}

static void qInputHandler_HandleMapInput( qGame_t* game )
{
   qActor_t* actor = game->controllingActor;
   qEntity_t* entity = actor->entity;

   sfBool leftIsDown = qInputState_IsKeyDown( sfKeyLeft );
   sfBool upIsDown = qInputState_IsKeyDown( sfKeyUp );
   sfBool rightIsDown = qInputState_IsKeyDown( sfKeyRight );
   sfBool downIsDown = qInputState_IsKeyDown( sfKeyDown );

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

static void qInputHandler_CheckCheats( qGame_t* game )
{
   int32_t cheatStringLength, i, l, lastIndex, matchCount;
   static const char* cheats[] = {
      CHEAT_NOCLIP,
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
      snprintf( cheatMsg, STRLEN_SHORT, STR_CHEAT_NOCLIPFORMATTER, game->cheatNoClip ? STR_ON : STR_OFF );
      qGame_ShowDebugMessage( game, cheatMsg );
   }
   else if ( !strcmp( cheat, CHEAT_CLEAR ) )
   {
      game->cheatNoClip = sfFalse;
      qGame_ShowDebugMessage( game, STR_CHEAT_CLEARED );
   }

   cheat[0] = '\0';
}
