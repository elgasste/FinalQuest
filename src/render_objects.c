#include "render_objects.h"

static qDiagnosticsRenderObjects_t* qDiagnosticsRenderObjects_Create();
static qDebugBarRenderObjects_t* qDebugBarRenderObjects_Create();
static void qDiagnosticsRenderObjects_Destroy( qDiagnosticsRenderObjects_t* objects );
static void qDebugBarRenderObjects_Destroy( qDebugBarRenderObjects_t* objects );

qRenderObjects_t* qRenderObjects_Create()
{
   sfVector2f windowBackgroundSize = { WINDOW_WIDTH, WINDOW_HEIGHT };
   sfVector2f windowBackgroundPosition = { 0, 0 };

   qRenderObjects_t* renderObjects = (qRenderObjects_t*)qAlloc( sizeof( qRenderObjects_t ), sfTrue );
   renderObjects->diagnosticsRenderObjects = qDiagnosticsRenderObjects_Create();
   renderObjects->debugBarRenderObjects = qDebugBarRenderObjects_Create();

   renderObjects->windowBackgroundRect = qRectangleShape_Create();
   sfRectangleShape_setSize( renderObjects->windowBackgroundRect, windowBackgroundSize );
   sfRectangleShape_setPosition( renderObjects->windowBackgroundRect, windowBackgroundPosition );
   sfRectangleShape_setFillColor( renderObjects->windowBackgroundRect, sfBlack );

   return renderObjects;
}

static qDiagnosticsRenderObjects_t* qDiagnosticsRenderObjects_Create()
{
   sfVector2f backgroundSize = { 180, 64 };
   sfVector2f backgroundPos = { WINDOW_WIDTH - backgroundSize.x, 0 };

   qDiagnosticsRenderObjects_t* objects = (qDiagnosticsRenderObjects_t*)qAlloc( sizeof( qDiagnosticsRenderObjects_t ), sfTrue );

   objects->backgroundRect = qRectangleShape_Create();
   sfRectangleShape_setSize( objects->backgroundRect, backgroundSize );
   sfRectangleShape_setPosition( objects->backgroundRect, backgroundPos );
   sfRectangleShape_setFillColor( objects->backgroundRect, sfBlue );
   objects->font = qFont_CreateFromFile( "consolas.ttf" );
   objects->textPosition.x = WINDOW_WIDTH - backgroundSize.x + 8;
   objects->text = qText_Create();
   sfText_setFont( objects->text, objects->font );
   sfText_setCharacterSize( objects->text, 12 );
   sfText_setFillColor( objects->text, sfWhite );
   objects->lineSpacing = sfFont_getLineSpacing( objects->font, sfText_getCharacterSize( objects->text ) );

   return objects;
}

static qDebugBarRenderObjects_t* qDebugBarRenderObjects_Create()
{
   sfVector2f backgroundSize = { 250, 16 };
   sfVector2f backgroundPos = { 10, 10 };
   sfVector2f textPos = { 14, 10 };

   qDebugBarRenderObjects_t* objects = (qDebugBarRenderObjects_t*)qAlloc( sizeof( qDebugBarRenderObjects_t ), sfTrue );

   objects->backgroundRect = qRectangleShape_Create();
   sfRectangleShape_setSize( objects->backgroundRect, backgroundSize );
   sfRectangleShape_setPosition( objects->backgroundRect, backgroundPos );
   sfRectangleShape_setFillColor( objects->backgroundRect, sfWhite );
   objects->font = qFont_CreateFromFile( "consolas.ttf" );
   objects->text = qText_Create();
   sfText_setFont( objects->text, objects->font );
   sfText_setCharacterSize( objects->text, 12 );
   sfText_setFillColor( objects->text, sfBlack );
   sfText_setPosition( objects->text, textPos );

   return objects;
}

void qRenderObjects_Destroy( qRenderObjects_t* objects )
{
   qDebugBarRenderObjects_Destroy( objects->debugBarRenderObjects );
   qDiagnosticsRenderObjects_Destroy( objects->diagnosticsRenderObjects );

   qRectangleShape_Destroy( objects->windowBackgroundRect );

   qFree( objects, sizeof( qRenderObjects_t ), sfTrue );
}

void qDiagnosticsRenderObjects_Destroy( qDiagnosticsRenderObjects_t* objects )
{
   qText_Destroy( objects->text );
   qFont_Destroy( objects->font );
   qRectangleShape_Destroy( objects->backgroundRect );

   qFree( objects, sizeof( qDiagnosticsRenderObjects_t ), sfTrue );
}

static void qDebugBarRenderObjects_Destroy( qDebugBarRenderObjects_t* objects )
{
   qText_Destroy( objects->text );
   qFont_Destroy( objects->font );
   qRectangleShape_Destroy( objects->backgroundRect );

   qFree( objects, sizeof( qDebugBarRenderObjects_t ), sfTrue );
}
