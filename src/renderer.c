#include "renderer.h"
#include "render_objects.h"
#include "render_states.h"
#include "game.h"
#include "window.h"
#include "clock.h"
#include "time_util.h"

static void qRenderer_DrawDiagnostics( qGame_t* game );
static void qRenderer_DrawDebugBar( qGame_t* game );

qRenderer_t* qRenderer_Create()
{
   qRenderer_t* renderer = (qRenderer_t*)qAlloc( sizeof( qRenderer_t ), sfTrue );

   renderer->renderObjects = qRenderObjects_Create();
   renderer->renderStates = qRenderStates_Create();

   return renderer;
}

void qRenderer_Destroy( qRenderer_t* renderer )
{
   qRenderStates_Destroy( renderer->renderStates );
   qRenderObjects_Destroy( renderer->renderObjects );

   qFree( renderer, sizeof( qRenderer_t ), sfTrue );
}

void qRenderer_Render( qGame_t* game )
{
   qWindow_DrawRectangleShape( game->window, game->renderer->renderObjects->windowBackgroundRect );

   qRenderer_DrawDebugBar( game );

   if ( game->showDiagnostics )
   {
      qRenderer_DrawDiagnostics( game );
   }

   qWindow_Display( game->window );
}

static void qRenderer_DrawDiagnostics( qGame_t* game )
{
   char msg[STRLEN_DEFAULT];
   char timeStr[STRLEN_SHORT];
   qDiagnosticsRenderObjects_t* objects = game->renderer->renderObjects->diagnosticsRenderObjects;

   qWindow_DrawRectangleShape( game->window, objects->backgroundRect );

   objects->textPosition.y = 4;
   sfText_setPosition( objects->text, objects->textPosition );
   snprintf( msg, STRLEN_DEFAULT, STR_FRAMERATEFORMATTER, GAME_FPS );
   sfText_setString( objects->text, msg );
   qWindow_DrawText( game->window, objects->text );

   objects->textPosition.y += objects->lineSpacing;
   sfText_setPosition( objects->text, objects->textPosition );
   snprintf( msg, STRLEN_DEFAULT, STR_TOTALFRAMESFORMATTER, game->clock->totalFrameCount );
   sfText_setString( objects->text, msg );
   qWindow_DrawText( game->window, objects->text );

   objects->textPosition.y += objects->lineSpacing;
   sfText_setPosition( objects->text, objects->textPosition );
   snprintf( msg, STRLEN_DEFAULT, STR_LAGFRAMESFORMATTER, game->clock->lagFrameCount );
   sfText_setString( objects->text, msg );
   qWindow_DrawText( game->window, objects->text );

   objects->textPosition.y += objects->lineSpacing;
   sfText_setPosition( objects->text, objects->textPosition );
   qTimeUtil_FormatTime( timeStr, STRLEN_SHORT, (int32_t)( game->clock->realTotalDurationMicro / 1000000 ) );
   snprintf( msg, STRLEN_DEFAULT, STR_ELAPSEDTIMEFORMATTER, timeStr );
   sfText_setString( objects->text, msg );
   qWindow_DrawText( game->window, objects->text );
}

static void qRenderer_DrawDebugBar( qGame_t* game )
{
   qDebugBarRenderState_t* state = game->renderer->renderStates->debugBar;
   qDebugBarRenderObjects_t* objects = game->renderer->renderObjects->debugBarRenderObjects;

   if ( state->isVisible )
   {
      sfText_setString( objects->text, state->msgBuffer );

      qWindow_DrawRectangleShape( game->window, objects->backgroundRect );
      qWindow_DrawText( game->window, objects->text );
   }
}
