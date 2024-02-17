#if !defined( RENDERER_H )
#define RENDERER_H

#include "common.h"

typedef struct qRenderObjects_t qRenderObjects_t;
typedef struct qRenderStates_t qRenderStates_t;
typedef struct qGame_t qGame_t;
typedef struct qActor_t qActor_t;

typedef struct qRenderer_t
{
   qRenderObjects_t* renderObjects;
   qRenderStates_t* renderStates;

   sfRectangleShape* windowBackgroundRect;

   sfFloatRect mapViewRect;
   sfVector2f mapViewPadding;
   sfVector2f mapTilePixelOffset;
   sfVector2u mapViewStart;
   sfVector2u mapViewEnd;

   qActor_t* orderedActors[MAX_ACTORS];
   qActor_t* controllingActorCache;
}
qRenderer_t;

qRenderer_t* qRenderer_Create();
void qRenderer_Destroy( qRenderer_t* renderer );
void qRenderer_UpdateActors( qGame_t* game );
void qRenderer_Render( qGame_t* game );
void qRenderer_SwitchControllingCharacter( qGame_t* game );

#endif // RENDERER_H
