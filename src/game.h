#if !defined( GAME_H )
#define GAME_H

#include "common.h"
#include "enums.h"

typedef struct qWindow_t qWindow_t;
typedef struct qClock_t qClock_t;
typedef struct qInputState_t qInputState_t;
typedef struct qInputHandler_t qInputHandler_t;
typedef struct qRenderer_t qRenderer_t;
typedef struct qPhysics_t qPhysics_t;
typedef struct qMap_t qMap_t;
typedef struct qActor_t qActor_t;
typedef struct qMenus_t qMenus_t;

typedef struct qGame_t
{
   qWindow_t* window;
   qClock_t* clock;
   qInputState_t* inputState;
   qInputHandler_t* inputHandler;
   qRenderer_t* renderer;
   qPhysics_t* physics;

   qGameState_t state;

   qMap_t* map;
   qMenus_t* menus;

   sfBool isMenuOpen;

   qActor_t* actors;
   uint32_t actorCount;
   qActor_t* controllingActor;
   uint32_t controllingActorIndex;

   sfBool showDiagnostics;
   sfBool cheatNoClip;
   sfBool cheatFast;
   sfBool cheatNoEncounters;
}
qGame_t;

qGame_t* qGame_Create();
void qGame_Destroy( qGame_t* game );
void qGame_Run( qGame_t* game );
void qGame_Close( qGame_t* game );
void qGame_ShowDebugMessage( qGame_t* game, const char* msg );
void qGame_SwitchControllingActor( qGame_t* game );
void qGame_SetState( qGame_t* game, qGameState_t state );
void qGame_ExecuteMenuCommand( qGame_t* game, qMenuCommand_t command );
void qGame_RollEncounter( qGame_t* game, uint32_t mapTileIndex, sfBool force );

#endif // GAME_H
