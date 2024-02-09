#if !defined( RENDER_OBJECTS_H )
#define RENDER_OBJECTS_H

#include "common.h"

typedef struct qDiagnosticsRenderObjects_t
{
   sfRectangleShape* backgroundRect;
   sfFont* font;
   sfText* text;
   sfVector2f textPosition;
   float lineSpacing;
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

typedef struct qRenderObjects_t
{
   qDiagnosticsRenderObjects_t* diagnostics;
   qDebugBarRenderObjects_t* debugBar;
   qMapRenderObjects_t* map;

   sfTexture* spriteTextures;
}
qRenderObjects_t;

qRenderObjects_t* qRenderObjects_Create();
void qRenderObjects_Destroy( qRenderObjects_t* renderObjects );

#endif // RENDER_OBJECTS_H
