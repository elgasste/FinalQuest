#include "render_objects.h"

static qDiagnosticsRenderObjects_t* qDiagnosticsRenderObjects_Create();
static qDebugBarRenderObjects_t* qDebugBarRenderObjects_Create();
static qMapRenderObjects_t* qMapRenderObjects_Create();
static void qDiagnosticsRenderObjects_Destroy( qDiagnosticsRenderObjects_t* objects );
static void qDebugBarRenderObjects_Destroy( qDebugBarRenderObjects_t* objects );
static void qMapRenderObjects_Destroy( qMapRenderObjects_t* objects );

qRenderObjects_t* qRenderObjects_Create()
{
   qRenderObjects_t* renderObjects = (qRenderObjects_t*)qAlloc( sizeof( qRenderObjects_t ), sfTrue );
   renderObjects->diagnostics = qDiagnosticsRenderObjects_Create();
   renderObjects->debugBar = qDebugBarRenderObjects_Create();
   renderObjects->map = qMapRenderObjects_Create();

   return renderObjects;
}

static qDiagnosticsRenderObjects_t* qDiagnosticsRenderObjects_Create()
{
   sfVector2f backgroundSize = { 180, 64 };
   sfVector2f backgroundPos = { WINDOW_WIDTH - backgroundSize.x, 0 };

   qDiagnosticsRenderObjects_t* objects = (qDiagnosticsRenderObjects_t*)qAlloc( sizeof( qDiagnosticsRenderObjects_t ), sfTrue );

   objects->backgroundRect = qsfRectangleShape_Create();
   sfRectangleShape_setSize( objects->backgroundRect, backgroundSize );
   sfRectangleShape_setPosition( objects->backgroundRect, backgroundPos );
   sfRectangleShape_setFillColor( objects->backgroundRect, sfBlue );
   objects->font = qsfFont_CreateFromFile( DEBUG_FONT );
   objects->textPosition.x = WINDOW_WIDTH - backgroundSize.x + 8;
   objects->text = qsfText_Create();
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

   objects->backgroundRect = qsfRectangleShape_Create();
   sfRectangleShape_setSize( objects->backgroundRect, backgroundSize );
   sfRectangleShape_setPosition( objects->backgroundRect, backgroundPos );
   sfRectangleShape_setFillColor( objects->backgroundRect, sfWhite );
   objects->font = qsfFont_CreateFromFile( DEBUG_FONT );
   objects->text = qsfText_Create();
   sfText_setFont( objects->text, objects->font );
   sfText_setCharacterSize( objects->text, 12 );
   sfText_setFillColor( objects->text, sfBlack );
   sfText_setPosition( objects->text, textPos );

   return objects;
}

static qMapRenderObjects_t* qMapRenderObjects_Create()
{
   sfVector2f tilesetScale = { GRAPHICS_SCALE, GRAPHICS_SCALE };

   qMapRenderObjects_t* objects = (qMapRenderObjects_t*)qAlloc( sizeof( qMapRenderObjects_t ), sfTrue );

   objects->tilesetTexture = qsfTexture_CreateFromFile( "resources/textures/tiles/map_tileset.png" );

   objects->tileSprite = qsfSprite_Create();
   sfSprite_setTexture( objects->tileSprite, objects->tilesetTexture, sfFalse );
   sfSprite_scale( objects->tileSprite, tilesetScale );

   return objects;
}

void qRenderObjects_Destroy( qRenderObjects_t* objects )
{
   qMapRenderObjects_Destroy( objects->map );
   qDebugBarRenderObjects_Destroy( objects->debugBar );
   qDiagnosticsRenderObjects_Destroy( objects->diagnostics );

   qFree( objects, sizeof( qRenderObjects_t ), sfTrue );
}

void qDiagnosticsRenderObjects_Destroy( qDiagnosticsRenderObjects_t* objects )
{
   qsfText_Destroy( objects->text );
   qsfFont_Destroy( objects->font );
   qsfRectangleShape_Destroy( objects->backgroundRect );

   qFree( objects, sizeof( qDiagnosticsRenderObjects_t ), sfTrue );
}

static void qDebugBarRenderObjects_Destroy( qDebugBarRenderObjects_t* objects )
{
   qsfText_Destroy( objects->text );
   qsfFont_Destroy( objects->font );
   qsfRectangleShape_Destroy( objects->backgroundRect );

   qFree( objects, sizeof( qDebugBarRenderObjects_t ), sfTrue );
}

static void qMapRenderObjects_Destroy( qMapRenderObjects_t* objects )
{
   qsfSprite_Destroy( objects->tileSprite );
   qsfTexture_Destroy( objects->tilesetTexture );

   qFree( objects, sizeof( qMapRenderObjects_t ), sfTrue );
}
