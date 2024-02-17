#include "render_objects.h"
#include "sprite_texture.h"

static qDiagnosticsRenderObjects_t* qRenderObjects_CreateDiagnostics();
static qDebugBarRenderObjects_t* qRenderObjects_CreateDebugBar();
static qMapRenderObjects_t* qRenderObjects_CreateMap();
static qMenuRenderObjects_t* qRenderObjects_CreateMenu( sfVector2f pos, sfVector2f size, sfColor backgroundColor );
static qScreenFadeRenderObjects_t* qRenderObjects_CreateScreenFade();
static qDialogBoxRenderObjects_t* qRenderObjects_CreateBattleDialogBoxLarge();
static qDialogBoxRenderObjects_t* qRenderObjects_CreateBattleDialogBoxSmall();
static qCharacterStatsRenderObjects_t* qRenderObjects_CreateCharacterStats( sfVector2f pos, float height, sfColor backgroundColor );
static void qRenderObjects_DestroyDiagnostics( qDiagnosticsRenderObjects_t* objects );
static void qRenderObjects_DestroyDebugBar( qDebugBarRenderObjects_t* objects );
static void qRenderObjects_DestroyMap( qMapRenderObjects_t* objects );
static void qRenderObjects_DestroyMenu( qMenuRenderObjects_t* objects );
static void qRenderObjects_DestroyScreenFade( qScreenFadeRenderObjects_t* objects );
static void qRenderObjects_DestroyDialogBox( qDialogBoxRenderObjects_t* objects );
static void qRenderObjects_DestroyCharacterStats( qCharacterStatsRenderObjects_t* objects );
static void gmRenderObjects_BuildDialogBackground( sfConvexShape* shape,
                                                   float x, float y,
                                                   float w, float h,
                                                   float cornerRadius,
                                                   sfColor color );

qRenderObjects_t* qRenderObjects_Create()
{
   sfVector2f mapMenuPos = { 32 * GRAPHICS_SCALE, 192 * GRAPHICS_SCALE };
   sfVector2f mapMenuSize = { 136 * GRAPHICS_SCALE, 76 * GRAPHICS_SCALE };
   sfVector2f battleMenuPos = { 32 * GRAPHICS_SCALE, WINDOW_HEIGHT - ( 160 * GRAPHICS_SCALE ) };
   sfVector2f battleMenuSize = { 136 * GRAPHICS_SCALE, 132 * GRAPHICS_SCALE };
   sfVector2f characterStatsPos = { 32 * GRAPHICS_SCALE, ( 32 - 4 ) * GRAPHICS_SCALE };

   qRenderObjects_t* renderObjects = (qRenderObjects_t*)qAlloc( sizeof( qRenderObjects_t ), sfTrue );
   renderObjects->diagnostics = qRenderObjects_CreateDiagnostics();
   renderObjects->debugBar = qRenderObjects_CreateDebugBar();
   renderObjects->map = qRenderObjects_CreateMap();
   renderObjects->mapMenu = qRenderObjects_CreateMenu( mapMenuPos, mapMenuSize, DIALOG_BACKDROP_LIGHTCOLOR );
   renderObjects->screenFade = qRenderObjects_CreateScreenFade();
   renderObjects->battleActionMenu = qRenderObjects_CreateMenu( battleMenuPos, battleMenuSize, DIALOG_BACKDROP_DARKCOLOR );
   renderObjects->battleDialogBoxLarge = qRenderObjects_CreateBattleDialogBoxLarge();
   renderObjects->battleDialogBoxSmall = qRenderObjects_CreateBattleDialogBoxSmall();
   renderObjects->mapCharacterStats = qRenderObjects_CreateCharacterStats( characterStatsPos, 140 * GRAPHICS_SCALE, DIALOG_BACKDROP_LIGHTCOLOR );
   renderObjects->battleCharacterStats = qRenderObjects_CreateCharacterStats( characterStatsPos, 100 * GRAPHICS_SCALE, DIALOG_BACKDROP_DARKCOLOR );

   renderObjects->spriteTextureCount = 3;
   renderObjects->spriteTextures = (qSpriteTexture_t*)qAlloc( sizeof( qSpriteTexture_t ) * renderObjects->spriteTextureCount, sfTrue );
   qSpriteTexture_Setup( &( renderObjects->spriteTextures[0] ), "resources/textures/sprites/male0.png", 4 );
   qSpriteTexture_Setup( &( renderObjects->spriteTextures[1] ), "resources/textures/sprites/female0.png", 4 );
   qSpriteTexture_Setup( &( renderObjects->spriteTextures[2] ), "resources/textures/sprites/dog0.png", 4 );

   return renderObjects;
}

static qDiagnosticsRenderObjects_t* qRenderObjects_CreateDiagnostics()
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
   objects->lineSize = sfFont_getLineSpacing( objects->font, sfText_getCharacterSize( objects->text ) );

   return objects;
}

static qDebugBarRenderObjects_t* qRenderObjects_CreateDebugBar()
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

static qMapRenderObjects_t* qRenderObjects_CreateMap()
{
   sfVector2f tilesetScale = { GRAPHICS_SCALE, GRAPHICS_SCALE };

   qMapRenderObjects_t* objects = (qMapRenderObjects_t*)qAlloc( sizeof( qMapRenderObjects_t ), sfTrue );

   objects->tilesetTexture = qsfTexture_CreateFromFile( "resources/textures/tiles/map_tileset.png" );

   objects->tileSprite = qsfSprite_Create();
   sfSprite_setTexture( objects->tileSprite, objects->tilesetTexture, sfFalse );
   sfSprite_scale( objects->tileSprite, tilesetScale );

   return objects;
}

static qMenuRenderObjects_t* qRenderObjects_CreateMenu( sfVector2f pos, sfVector2f size, sfColor backgroundColor )
{
   sfVector2f textScale = { GRAPHICS_SCALE, GRAPHICS_SCALE };

   qMenuRenderObjects_t* objects = (qMenuRenderObjects_t*)qAlloc( sizeof( qMenuRenderObjects_t ), sfTrue );

   objects->pos = pos;
   objects->itemsOffset.x = 32 * GRAPHICS_SCALE;
   objects->itemsOffset.y = 16 * GRAPHICS_SCALE;
   objects->caratOffset.x = -16 * GRAPHICS_SCALE;
   objects->caratOffset.y = 0;
   objects->lineSize = 20 * GRAPHICS_SCALE;

   objects->backgroundShape = qsfConvexShape_Create();
   gmRenderObjects_BuildDialogBackground( objects->backgroundShape,
                                          objects->pos.x, objects->pos.y,
                                          size.x, size.y,
                                          8 * GRAPHICS_SCALE,
                                          backgroundColor );

   objects->font = qsfFont_CreateFromFile( GAME_FONT );
   objects->text = qsfText_Create();
   sfText_setFont( objects->text, objects->font );
   sfText_setCharacterSize( objects->text, GAME_FONT_SIZE );
   sfText_scale( objects->text, textScale );
   sfText_setFillColor( objects->text, GAME_FONT_COLOR );

   return objects;
}

static qScreenFadeRenderObjects_t* qRenderObjects_CreateScreenFade()
{
   sfVector2f v = { 0, 0 };

   qScreenFadeRenderObjects_t* objects = (qScreenFadeRenderObjects_t*)qAlloc( sizeof( qScreenFadeRenderObjects_t ), sfTrue );

   objects->screenRect = qsfRectangleShape_Create();

   sfRectangleShape_setPosition( objects->screenRect, v );
   v.x = WINDOW_WIDTH;
   v.y = WINDOW_HEIGHT;
   sfRectangleShape_setSize( objects->screenRect, v );

   objects->lightColor = sfWhite;
   objects->darkColor = sfBlack;

   return objects;
}

static qDialogBoxRenderObjects_t* qRenderObjects_CreateBattleDialogBoxLarge()
{
   sfVector2f textScale = { GRAPHICS_SCALE, GRAPHICS_SCALE };

   qDialogBoxRenderObjects_t* objects = (qDialogBoxRenderObjects_t*)qAlloc( sizeof( qDialogBoxRenderObjects_t ), sfTrue );

   objects->pos.x = 32 * GRAPHICS_SCALE;
   objects->pos.y = WINDOW_HEIGHT - ( 160 * GRAPHICS_SCALE );
   objects->textOffset.x = 20 * GRAPHICS_SCALE;
   objects->textOffset.y = 16 * GRAPHICS_SCALE;
   objects->textWidth = 664 * GRAPHICS_SCALE;
   objects->lineSize = 20 * GRAPHICS_SCALE;

   objects->backgroundShape = qsfConvexShape_Create();
   gmRenderObjects_BuildDialogBackground( objects->backgroundShape,
                                          objects->pos.x, objects->pos.y,
                                          704 * GRAPHICS_SCALE, 132 * GRAPHICS_SCALE,
                                          8 * GRAPHICS_SCALE,
                                          DIALOG_BACKDROP_DARKCOLOR );

   objects->font = qsfFont_CreateFromFile( GAME_FONT );
   objects->text = qsfText_Create();
   sfText_setFont( objects->text, objects->font );
   sfText_setCharacterSize( objects->text, GAME_FONT_SIZE );
   sfText_scale( objects->text, textScale );
   sfText_setFillColor( objects->text, GAME_FONT_COLOR );

   objects->message[0] = '\0';

   return objects;
}

static qDialogBoxRenderObjects_t* qRenderObjects_CreateBattleDialogBoxSmall()
{
   sfVector2f textScale = { GRAPHICS_SCALE, GRAPHICS_SCALE };

   qDialogBoxRenderObjects_t* objects = (qDialogBoxRenderObjects_t*)qAlloc( sizeof( qDialogBoxRenderObjects_t ), sfTrue );

   objects->pos.x = 200 * GRAPHICS_SCALE;
   objects->pos.y = WINDOW_HEIGHT - ( 160 * GRAPHICS_SCALE );
   objects->textOffset.x = 20 * GRAPHICS_SCALE;
   objects->textOffset.y = 16 * GRAPHICS_SCALE;
   objects->textWidth = 496 * GRAPHICS_SCALE;
   objects->lineSize = 20 * GRAPHICS_SCALE;

   objects->backgroundShape = qsfConvexShape_Create();
   gmRenderObjects_BuildDialogBackground( objects->backgroundShape,
                                          objects->pos.x, objects->pos.y,
                                          536 * GRAPHICS_SCALE, 132 * GRAPHICS_SCALE,
                                          8 * GRAPHICS_SCALE,
                                          DIALOG_BACKDROP_DARKCOLOR );

   objects->font = qsfFont_CreateFromFile( GAME_FONT );
   objects->text = qsfText_Create();
   sfText_setFont( objects->text, objects->font );
   sfText_setCharacterSize( objects->text, GAME_FONT_SIZE );
   sfText_scale( objects->text, textScale );
   sfText_setFillColor( objects->text, GAME_FONT_COLOR );

   objects->message[0] = '\0';

   return objects;
}

qCharacterStatsRenderObjects_t* qRenderObjects_CreateCharacterStats( sfVector2f pos, float height, sfColor backgroundColor )
{
   sfVector2f textScale = { GRAPHICS_SCALE, GRAPHICS_SCALE };

   qCharacterStatsRenderObjects_t* objects = (qCharacterStatsRenderObjects_t*)qAlloc( sizeof( qCharacterStatsRenderObjects_t ), sfTrue );

   objects->lineSize = 20 * GRAPHICS_SCALE;
   objects->pos = pos;
   objects->nameOffset.x = 20 * GRAPHICS_SCALE;
   objects->nameOffset.y = 16 * GRAPHICS_SCALE;
   objects->labelsOffset.x = 36 * GRAPHICS_SCALE;
   objects->labelsOffset.y = objects->nameOffset.y + objects->lineSize + ( 4 * GRAPHICS_SCALE );
   objects->statsOffset.x = 72 * GRAPHICS_SCALE;
   objects->statsOffset.y = objects->labelsOffset.y;

   objects->backgroundShape = qsfConvexShape_Create();
   gmRenderObjects_BuildDialogBackground( objects->backgroundShape,
                                          objects->pos.x, objects->pos.y,
                                          136 * GRAPHICS_SCALE, height,
                                          8 * GRAPHICS_SCALE,
                                          backgroundColor );

   objects->font = qsfFont_CreateFromFile( GAME_FONT );
   objects->text = qsfText_Create();
   sfText_setFont( objects->text, objects->font );
   sfText_setCharacterSize( objects->text, GAME_FONT_SIZE );
   sfText_scale( objects->text, textScale );
   sfText_setFillColor( objects->text, GAME_FONT_COLOR );

   return objects;
}

void qRenderObjects_Destroy( qRenderObjects_t* objects )
{
   uint32_t i;

   for ( i = 0; i < objects->spriteTextureCount; i++ )
   {
      qSpriteTexture_Cleanup( &( objects->spriteTextures[i] ) );
   }

   qFree( objects->spriteTextures, sizeof( qSpriteTexture_t ) * objects->spriteTextureCount, sfTrue );

   qRenderObjects_DestroyCharacterStats( objects->battleCharacterStats );
   qRenderObjects_DestroyCharacterStats( objects->mapCharacterStats );
   qRenderObjects_DestroyDialogBox( objects->battleDialogBoxSmall );
   qRenderObjects_DestroyDialogBox( objects->battleDialogBoxLarge );
   qRenderObjects_DestroyMenu( objects->battleActionMenu );
   qRenderObjects_DestroyScreenFade( objects->screenFade );
   qRenderObjects_DestroyMenu( objects->mapMenu );
   qRenderObjects_DestroyMap( objects->map );
   qRenderObjects_DestroyDebugBar( objects->debugBar );
   qRenderObjects_DestroyDiagnostics( objects->diagnostics );

   qFree( objects, sizeof( qRenderObjects_t ), sfTrue );
}

void qRenderObjects_DestroyDiagnostics( qDiagnosticsRenderObjects_t* objects )
{
   qsfText_Destroy( objects->text );
   qsfFont_Destroy( objects->font );
   qsfRectangleShape_Destroy( objects->backgroundRect );

   qFree( objects, sizeof( qDiagnosticsRenderObjects_t ), sfTrue );
}

static void qRenderObjects_DestroyDebugBar( qDebugBarRenderObjects_t* objects )
{
   qsfText_Destroy( objects->text );
   qsfFont_Destroy( objects->font );
   qsfRectangleShape_Destroy( objects->backgroundRect );

   qFree( objects, sizeof( qDebugBarRenderObjects_t ), sfTrue );
}

static void qRenderObjects_DestroyMap( qMapRenderObjects_t* objects )
{
   qsfSprite_Destroy( objects->tileSprite );
   qsfTexture_Destroy( objects->tilesetTexture );

   qFree( objects, sizeof( qMapRenderObjects_t ), sfTrue );
}

static void qRenderObjects_DestroyMenu( qMenuRenderObjects_t* objects )
{
   qsfText_Destroy( objects->text );
   qsfFont_Destroy( objects->font );
   qsfConvexShape_Destroy( objects->backgroundShape );

   qFree( objects, sizeof( qMenuRenderObjects_t ), sfTrue );
}

static void qRenderObjects_DestroyScreenFade( qScreenFadeRenderObjects_t* objects )
{
   qsfRectangleShape_Destroy( objects->screenRect );

   qFree( objects, sizeof( qScreenFadeRenderObjects_t ), sfTrue );
}

static void qRenderObjects_DestroyDialogBox( qDialogBoxRenderObjects_t* objects )
{
   qsfText_Destroy( objects->text );
   qsfFont_Destroy( objects->font );
   qsfConvexShape_Destroy( objects->backgroundShape );

   qFree( objects, sizeof( qDialogBoxRenderObjects_t ), sfTrue );
}

static void qRenderObjects_DestroyCharacterStats( qCharacterStatsRenderObjects_t* objects )
{
   qsfText_Destroy( objects->text );
   qsfFont_Destroy( objects->font );
   qsfConvexShape_Destroy( objects->backgroundShape );

   qFree( objects, sizeof( qCharacterStatsRenderObjects_t ), sfTrue );
}

static void gmRenderObjects_BuildDialogBackground( sfConvexShape* shape,
                                                   float x, float y,
                                                   float w, float h,
                                                   float cornerRadius,
                                                   sfColor color )
{
   sfVector2f pt = { x, y };

   sfConvexShape_setPointCount( shape, 8 );
   sfConvexShape_setPosition( shape, pt );
   sfConvexShape_setFillColor( shape, color );

   pt.x = cornerRadius;
   pt.y = 0;
   sfConvexShape_setPoint( shape, 0, pt );
   pt.x = w - cornerRadius;
   sfConvexShape_setPoint( shape, 1, pt );
   pt.x = w;
   pt.y = cornerRadius;
   sfConvexShape_setPoint( shape, 2, pt );
   pt.y = h - cornerRadius;
   sfConvexShape_setPoint( shape, 3, pt );
   pt.x = w - cornerRadius;
   pt.y = h;
   sfConvexShape_setPoint( shape, 4, pt );
   pt.x = cornerRadius;
   sfConvexShape_setPoint( shape, 5, pt );
   pt.x = 0;
   pt.y = h - cornerRadius;
   sfConvexShape_setPoint( shape, 6, pt );
   pt.y = cornerRadius;
   sfConvexShape_setPoint( shape, 7, pt );
}
