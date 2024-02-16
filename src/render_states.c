#include "render_states.h"
#include "clock.h"
#include "game.h"
#include "renderer.h"

static qDebugBarRenderState_t* qRenderStates_CreateDebugBar();
static qMenuRenderState_t* qRenderStates_CreateMenu();
static qScreenFadeRenderState_t* qRenderStates_CreateScreenFade();
static qTextScrollRenderState_t* qRenderStates_CreateTextScroll();
static qActorSwapRenderState_t* qRenderStates_CreateActorSwap();
static void qRenderStates_DestroyDebugBar( qDebugBarRenderState_t* state );
static void qRenderStates_DestroyMenu( qMenuRenderState_t* state );
static void qRenderStates_DestroyScreenFade( qScreenFadeRenderState_t* state );
static void qRenderStates_DestroyTextScroll( qTextScrollRenderState_t* state );
static void qRenderStates_DestroyActorSwap( qActorSwapRenderState_t* state );
static void qRenderStates_TicMenu( qGame_t* game );
static void qRenderStates_ResetScreenFade( qScreenFadeRenderState_t* state );
static void qRenderStates_TicScreenFade( qGame_t* game );
static void qRenderStates_ResetTextScroll( qTextScrollRenderState_t* state );
static void qRenderStates_TicTextScroll( qGame_t* game );
static void qRenderStates_ResetActorSwap( qActorSwapRenderState_t* state );
static void qRenderStates_TicActorSwap( qGame_t* game );

qRenderStates_t* qRenderStates_Create()
{
   qRenderStates_t* states = (qRenderStates_t*)qAlloc( sizeof( qRenderStates_t ), sfTrue );

   states->debugBar = qRenderStates_CreateDebugBar();
   states->menu = qRenderStates_CreateMenu();
   states->screenFade = qRenderStates_CreateScreenFade();
   states->textScroll = qRenderStates_CreateTextScroll();
   states->actorSwap = qRenderStates_CreateActorSwap();

   return states;
}

static qDebugBarRenderState_t* qRenderStates_CreateDebugBar()
{
   qDebugBarRenderState_t* state = (qDebugBarRenderState_t*)qAlloc( sizeof( qDebugBarRenderState_t ), sfTrue );

   state->isVisible = sfFalse;
   state->visibleSeconds = 3;
   state->elapsedSeconds = 0;
   state->msgBuffer = (char*)qCalloc( STRLEN_DEFAULT, sizeof( char ), sfTrue );
   state->msgBufferLen = STRLEN_DEFAULT;

   return state;
}

static qMenuRenderState_t* qRenderStates_CreateMenu()
{
   qMenuRenderState_t* state = (qMenuRenderState_t*)qAlloc( sizeof( qMenuRenderState_t ), sfTrue );

   state->caratBlinkSeconds = 0.25f;
   qRenderStates_ResetMenu( state );

   return state;
}

static qScreenFadeRenderState_t* qRenderStates_CreateScreenFade()
{
   qScreenFadeRenderState_t* state = (qScreenFadeRenderState_t*)qAlloc( sizeof( qScreenFadeRenderState_t ), sfTrue );

   state->fadeSeconds = 0.4f;
   state->pauseSeconds = 0.1f;
   qRenderStates_ResetScreenFade( state );

   return state;
}

static qTextScrollRenderState_t* qRenderStates_CreateTextScroll()
{
   qTextScrollRenderState_t* state = (qTextScrollRenderState_t*)qAlloc( sizeof( qTextScrollRenderState_t ), sfTrue );

   state->letterSeconds = 0.012f;
   qRenderStates_ResetTextScroll( state );

   return state;
}

static qActorSwapRenderState_t* qRenderStates_CreateActorSwap()
{
   qActorSwapRenderState_t* state = (qActorSwapRenderState_t*)qAlloc( sizeof( qActorSwapRenderState_t ), sfTrue );

   state->swapSeconds = 0.25f;
   qRenderStates_ResetActorSwap( state );

   return state;
}

void qRenderStates_Destroy( qRenderStates_t* states )
{
   qRenderStates_DestroyActorSwap( states->actorSwap );
   qRenderStates_DestroyTextScroll( states->textScroll );
   qRenderStates_DestroyScreenFade( states->screenFade );
   qRenderStates_DestroyDebugBar( states->debugBar );
   qRenderStates_DestroyMenu( states->menu );

   qFree( states, sizeof( qRenderStates_t ), sfTrue );
}

static void qRenderStates_DestroyDebugBar( qDebugBarRenderState_t* state )
{
   qFree( state->msgBuffer, sizeof( char ) * state->msgBufferLen, sfTrue );
   qFree( state, sizeof( qDebugBarRenderState_t ), sfTrue );
}

static void qRenderStates_DestroyMenu( qMenuRenderState_t* state )
{
   qFree( state, sizeof( qMenuRenderState_t ), sfTrue );
}

static void qRenderStates_DestroyScreenFade( qScreenFadeRenderState_t* state )
{
   qFree( state, sizeof( qScreenFadeRenderState_t ), sfTrue );
}

static void qRenderStates_DestroyTextScroll( qTextScrollRenderState_t* state )
{
   qFree( state, sizeof( qTextScrollRenderState_t ), sfTrue );
}

static void qRenderStates_DestroyActorSwap( qActorSwapRenderState_t* state )
{
   qFree( state, sizeof( qActorSwapRenderState_t ), sfTrue );
}

void qRenderStates_Tic( qGame_t* game )
{
   qRenderStates_t* states = game->renderer->renderStates;

   if ( states->debugBar->isVisible )
   {
      states->debugBar->elapsedSeconds += game->clock->frameDeltaSeconds;

      if ( states->debugBar->elapsedSeconds > states->debugBar->visibleSeconds )
      {
         states->debugBar->isVisible = sfFalse;
         states->debugBar->elapsedSeconds = 0;
      }
   }

   if ( states->screenFade->isRunning )
   {
      qRenderStates_TicScreenFade( game );
   }
   else
   {
      qRenderStates_TicMenu( game );
      qRenderStates_TicTextScroll( game );
      qRenderStates_TicActorSwap( game );
   }
}

void qRenderStates_ResetMenu( qMenuRenderState_t* state )
{
   state->showCarat = sfTrue;
   state->caratElapsedSeconds = 0;
}

static void qRenderStates_TicMenu( qGame_t* game )
{
   qRenderStates_t* states = game->renderer->renderStates;

   if ( game->isMenuOpen )
   {
      states->menu->caratElapsedSeconds += game->clock->frameDeltaSeconds;

      while( states->menu->caratElapsedSeconds > states->menu->caratBlinkSeconds )
      {
         TOGGLE_BOOL( states->menu->showCarat );
         states->menu->caratElapsedSeconds -= states->menu->caratBlinkSeconds;
      }
   }
}

static void qRenderStates_ResetScreenFade( qScreenFadeRenderState_t* state )
{
   state->isRunning = sfFalse;
   state->isFading = sfFalse;
   state->isPausing = sfFalse;
   state->elapsedSeconds = 0;
}

void qRenderStates_StartScreenFade( qScreenFadeRenderState_t* state,
                                    sfBool fadeOut,
                                    sfBool pause,
                                    sfBool isLightColor,
                                    void (*fadeCompleteFnc)(qGame_t*) )
{
   qRenderStates_ResetScreenFade( state );
   state->fadeOut = fadeOut;
   state->pause = pause;
   state->isLightColor = isLightColor;
   state->isRunning = sfTrue;
   state->isFading = sfTrue;
   state->fadeCompleteFnc = fadeCompleteFnc;
}

static void qRenderStates_TicScreenFade( qGame_t* game )
{
   qScreenFadeRenderState_t* state = game->renderer->renderStates->screenFade;

   if ( !state->isRunning )
   {
      return;
   }

   state->elapsedSeconds += game->clock->frameDeltaSeconds;

   if ( state->isFading )
   {
      if ( state->elapsedSeconds > state->fadeSeconds )
      {
         state->isFading = sfFalse;

         if ( state->pause )
         {
            state->isPausing = sfTrue;
            state->elapsedSeconds = 0;
         }
         else
         {
            state->isRunning = sfFalse;
            ( *state->fadeCompleteFnc )( game );
         }
      }
   }
   else if ( state->isPausing )
   {
      if ( state->elapsedSeconds > state->pauseSeconds )
      {
         state->isPausing = sfFalse;
         state->isRunning = sfFalse;
         ( *state->fadeCompleteFnc )( game );
      }
   }
}

static void qRenderStates_ResetTextScroll( qTextScrollRenderState_t* state )
{
   state->isRunning = sfFalse;
   state->elapsedSeconds = 0;
   state->currentCharIndex = 0;
}

void qRenderStates_StartTextScroll( qTextScrollRenderState_t* state, uint32_t charCount, sfBool canSkip )
{
   qRenderStates_ResetTextScroll( state );

   state->charCount = charCount;
   state->canSkip = canSkip;
   state->isRunning = sfTrue;
}

void qRenderStates_SkipTextScroll( qTextScrollRenderState_t* state )
{
   state->currentCharIndex = state->charCount - 1;
   state->isRunning = sfFalse;
}

static void qRenderStates_TicTextScroll( qGame_t* game )
{
   qTextScrollRenderState_t* state = game->renderer->renderStates->textScroll;

   if ( state->isRunning )
   {
      state->elapsedSeconds += game->clock->frameDeltaSeconds;

      while ( state->elapsedSeconds > state->letterSeconds )
      {
         state->elapsedSeconds -= state->letterSeconds;
         state->currentCharIndex++;

         if ( state->currentCharIndex == state->charCount )
         {
            state->isRunning = sfFalse;
            break;
         }
      }
   }
}

static void qRenderStates_ResetActorSwap( qActorSwapRenderState_t* state )
{
   state->isRunning = sfFalse;
   state->elapsedSeconds = 0;
}

void qRenderStates_StartActorSwap( qActorSwapRenderState_t* state, void (*actorSwapCompleteFnc)(qGame_t*) )
{
   qRenderStates_ResetActorSwap( state );
   state->actorSwapCompleteFnc = actorSwapCompleteFnc;
   state->isRunning = sfTrue;
}

static void qRenderStates_TicActorSwap( qGame_t* game )
{
   qActorSwapRenderState_t* state = game->renderer->renderStates->actorSwap;

   if ( state->isRunning )
   {
      state->elapsedSeconds += game->clock->frameDeltaSeconds;

      if ( state->elapsedSeconds > state->swapSeconds )
      {
         state->isRunning = sfFalse;
         ( *state->actorSwapCompleteFnc )( game );
      }
   }
}
