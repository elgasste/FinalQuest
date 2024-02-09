#include "game.h"
#include "input_state.h"
#include "actor.h"
#include "entity.h"

static void qInputHandler_HandleMapInput( qGame_t* game );

void qInputHandler_HandleInput( qGame_t* game )
{
   if ( qInputState_WasKeyPressed( game->inputState, sfKeyEscape ) )
   {
      qGame_Close( game );
      return;
   }

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
