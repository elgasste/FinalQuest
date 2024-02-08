#if !defined( INPUT_STATE_H )
#define INPUT_STATE_H

#include "common.h"

typedef struct qInputState_t
{
   sfBool* keysPressed;
   sfBool* keysReleased;
   sfBool keyWasPressed;
   sfKeyCode lastPressedKey;
}
qInputState_t;

qInputState_t* qInputState_Create();
void qInputState_Destroy( qInputState_t* inputState );

void qInputState_SetKeyPressed( qInputState_t* inputState, sfKeyCode keyCode );
void qInputState_SetKeyReleased( qInputState_t* inputState, sfKeyCode keyCode );
void qInputState_Reset( qInputState_t* inputState );
sfBool qInputState_WasKeyPressed( qInputState_t* inputState, sfKeyCode keyCode );
sfBool qInputState_WasKeyReleased( qInputState_t* inputState, sfKeyCode keyCode );
sfBool qInputState_IsKeyDown( sfKeyCode keyCode );
sfBool qInputState_IsAnyKeyDown();

#endif // INPUT_STATE_H
