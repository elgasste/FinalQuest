#if !defined( RENDER_STATES_H )
#define RENDER_STATES_H

#include "common.h"

typedef struct qGame_t qGame_t;

typedef struct qDebugBarRenderState_t
{
   sfBool isVisible;
   float elapsedSeconds;
   float visibleSeconds;
   char* msgBuffer;
   size_t msgBufferLen;
}
qDebugBarRenderState_t;

typedef struct qMenuRenderState_t
{
   sfBool showCarat;
   float caratElapsedSeconds;
   float caratBlinkSeconds;
}
qMenuRenderState_t;

typedef struct qRenderStates_t
{
   qDebugBarRenderState_t* debugBar;
   qMenuRenderState_t* menu;
}
qRenderStates_t;

qRenderStates_t* qRenderStates_Create();
void qRenderStates_Destroy( qRenderStates_t* states );
void qRenderStates_Tic( qGame_t* game );
void qRenderStates_ResetMenu( qMenuRenderState_t* state );

#endif // RENDER_STATES_H
