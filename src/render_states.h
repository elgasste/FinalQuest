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

typedef struct qScreenFadeRenderState_t
{
   sfBool isRunning;
   sfBool isFading;
   sfBool isPausing;
   sfBool fadeOut;
   sfBool pause;
   sfBool isLightColor;
   float fadeSeconds;
   float pauseSeconds;
   float elapsedSeconds;
   void (*fadeCompleteFnc)( qGame_t* );
}
qScreenFadeRenderState_t;

typedef struct qTextScrollRenderState_t
{
   sfBool isRunning;
   uint32_t charCount;
   uint32_t currentCharIndex;
   float letterSeconds;
   float elapsedSeconds;
   sfBool canSkip;
}
qTextScrollRenderState_t;

typedef struct qRenderStates_t
{
   qDebugBarRenderState_t* debugBar;
   qMenuRenderState_t* menu;
   qScreenFadeRenderState_t* screenFade;
   qTextScrollRenderState_t* textScroll;
}
qRenderStates_t;

qRenderStates_t* qRenderStates_Create();
void qRenderStates_Destroy( qRenderStates_t* states );
void qRenderStates_Tic( qGame_t* game );
void qRenderStates_ResetMenu( qMenuRenderState_t* state );
void qRenderStates_ResetScreenFade( qScreenFadeRenderState_t* state );
void qRenderStates_StartScreenFade( qScreenFadeRenderState_t* state,
                                    sfBool fadeOut,
                                    sfBool pause,
                                    sfBool isLightColor,
                                    void (*fadeCompleteFnc)(qGame_t*) );
void qRenderStates_ResetTextScroll( qTextScrollRenderState_t* state );
void qRenderStates_StartTextScroll( qTextScrollRenderState_t* state, uint32_t charCount, sfBool canSkip );
void qRenderStates_SkipTextScroll( qTextScrollRenderState_t* state );

#endif // RENDER_STATES_H
