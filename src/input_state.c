#include "input_state.h"

qInputState_t* qInputState_Create()
{
   qInputState_t* inputState = (qInputState_t*)qAlloc( sizeof( qInputState_t ), sfTrue );

   inputState->keysPressed = (sfBool*)qCalloc( sfKeyCount, sizeof( sfBool ), sfTrue );
   inputState->keysReleased = (sfBool*)qCalloc( sfKeyCount, sizeof( sfBool ), sfTrue );
   inputState->keyWasPressed = sfFalse;
   inputState->lastPressedKey = sfKeyCount;

   return inputState;
}

void qInputState_Destroy( qInputState_t* inputState )
{
   qFree( inputState->keysPressed, sizeof( sfBool) * sfKeyCount, sfTrue );
   qFree( inputState->keysReleased, sizeof( sfBool ) * sfKeyCount, sfTrue );
   qFree( inputState, sizeof( qInputState_t ), sfTrue );
}

void qInputState_Reset( qInputState_t* inputState )
{
   int i;

   for ( i = 0; i < (int)sfKeyCount; i++ )
   {
      inputState->keysPressed[i] = sfFalse;
      inputState->keysReleased[i] = sfFalse;
      inputState->keyWasPressed = sfFalse;
   }
}

void qInputState_SetKeyPressed( qInputState_t* inputState, sfKeyCode keyCode )
{
   if ( keyCode >= 0 && keyCode < sfKeyCount )
   {
      inputState->keysPressed[keyCode] = sfTrue;
      inputState->keyWasPressed = sfTrue;
      inputState->lastPressedKey = keyCode;
   }
}

void qInputState_SetKeyReleased( qInputState_t* inputState, sfKeyCode keyCode )
{
   if ( keyCode >= 0 && keyCode < sfKeyCount )
   {
      inputState->keysReleased[keyCode] = sfTrue;
   }
}

sfBool qInputState_WasKeyPressed( qInputState_t* inputState, sfKeyCode keyCode )
{
   return ( keyCode >= 0 && keyCode < sfKeyCount ) ? inputState->keysPressed[keyCode] : sfFalse;
}

sfBool qInputState_WasKeyReleased( qInputState_t* inputState, sfKeyCode keyCode )
{
   return ( keyCode >= 0 && keyCode < sfKeyCount) ? inputState->keysReleased[keyCode] : sfFalse;
}

sfBool qInputState_IsKeyDown( sfKeyCode keyCode )
{
   return keyCode >= 0 ? sfKeyboard_isKeyPressed( keyCode ) : sfFalse;
}

sfBool qInputState_IsAnyKeyDown()
{
   int i;

   for ( i = 0; i < (int)sfKeyCount; i++ )
   {
      if ( sfKeyboard_isKeyPressed( i ) )
      {
         return sfTrue;
      }
   }

   return sfFalse;
}
