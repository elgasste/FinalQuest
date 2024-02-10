#if !defined( GAME_H )
#define GAME_H

#include "common.h"

typedef struct qWindow_t qWindow_t;
typedef struct qClock_t qClock_t;
typedef struct qInputState_t qInputState_t;
typedef struct qInputHandler_t qInputHandler_t;
typedef struct qRenderer_t qRenderer_t;
typedef struct qPhysics_t qPhysics_t;
typedef struct qMap_t qMap_t;
typedef struct qActor_t qActor_t;

typedef struct qGame_t
{
   qWindow_t* window;
   qClock_t* clock;
   qInputState_t* inputState;
   qInputHandler_t* inputHandler;
   qRenderer_t* renderer;
   qPhysics_t* physics;
   qMap_t* map;
   qActor_t* actors;
   uint32_t actorCount;
   qActor_t* controllingActor;

   sfBool showDiagnostics;
   sfBool cheatNoClip;
}
qGame_t;

qGame_t* qGame_Create();
void qGame_Destroy( qGame_t* game );
void qGame_Run( qGame_t* game );
void qGame_Close( qGame_t* game );
void qGame_ShowDebugMessage( qGame_t* game, const char* msg );

#endif // GAME_H
