#if !defined( INPUT_HANDLER_H )
#define INPUT_HANDLER_H

#include "common.h"

typedef struct qGame_t qGame_t;

typedef struct qInputHandler_t
{
   char cheatString[STRLEN_SHORT];
}
qInputHandler_t;

qInputHandler_t* qInputHandler_Create();
void qInputHandler_Destroy( qInputHandler_t* inputHandler );
void qInputHandler_HandleInput( qGame_t* game );

#endif // INPUT_HANDLER_H
