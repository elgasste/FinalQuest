#if !defined( RENDER_OBJECTS_H )
#define RENDER_OBJECTS_H

#include "common.h"

typedef struct qActorSpriteTexture_t qActorSpriteTexture_t;
typedef struct qBattleSpriteTexture_t qBattleSpriteTexture_t;

typedef struct qDiagnosticsRenderObjects_t
{
   sfRectangleShape* backgroundRect;
   sfFont* font;
   sfText* text;
   sfVector2f textPosition;
   float lineSize;
}
qDiagnosticsRenderObjects_t;

typedef struct qDebugBarRenderObjects_t
{
   sfRectangleShape* backgroundRect;
   sfFont* font;
   sfText* text;
}
qDebugBarRenderObjects_t;

typedef struct qMapRenderObjects_t
{
   sfTexture* tilesetTexture;
   sfSprite* tileSprite;
}
qMapRenderObjects_t;

typedef struct qMenuRenderObjects_t
{
   sfConvexShape* backgroundShape;
   sfFont* font;
   sfText* text;
   sfVector2f pos;
   sfVector2f itemsOffset;
   sfVector2f caratOffset;
   float lineSize;
}
qMenuRenderObjects_t;

typedef struct qScreenFadeRenderObjects_t
{
   sfRectangleShape* screenRect;
   sfColor lightColor;
   sfColor darkColor;
}
qScreenFadeRenderObjects_t;

typedef struct qDialogBoxRenderObjects_t
{
   sfConvexShape* backgroundShape;
   sfFont* font;
   sfText* text;
   sfVector2f pos;
   sfVector2f textOffset;
   float textWidth;
   float lineSize;
   char message[STRLEN_DEFAULT];
}
qDialogBoxRenderObjects_t;

typedef struct qCharacterStatsRenderObjects_t
{
   sfConvexShape* backgroundShape;
   sfFont* font;
   sfText* text;
   sfVector2f pos;
   sfVector2f nameOffset;
   sfVector2f labelsOffset;
   sfVector2f statsOffset;
   float lineSize;
}
qCharacterStatsRenderObjects_t;

typedef struct qRenderObjects_t
{
   qDiagnosticsRenderObjects_t* diagnostics;
   qDebugBarRenderObjects_t* debugBar;
   qMapRenderObjects_t* map;
   qMenuRenderObjects_t* mapMenu;
   qScreenFadeRenderObjects_t* screenFade;
   qMenuRenderObjects_t* battleActionMenu;
   qDialogBoxRenderObjects_t* battleDialogBoxLarge;
   qDialogBoxRenderObjects_t* battleDialogBoxSmall;
   qCharacterStatsRenderObjects_t* mapCharacterStats;
   qCharacterStatsRenderObjects_t* battleCharacterStats;

   qActorSpriteTexture_t* actorSpriteTextures;
   uint32_t actorSpriteTextureCount;
   qBattleSpriteTexture_t* battleSpriteTextures;
   uint32_t battleSpriteTextureCount;
}
qRenderObjects_t;

qRenderObjects_t* qRenderObjects_Create();
void qRenderObjects_Destroy( qRenderObjects_t* renderObjects );

#endif // RENDER_OBJECTS_H
