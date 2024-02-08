#if !defined( RENDERER_H )
#define RENDERER_H

#include "common.h"

typedef struct qRenderObjects_t qRenderObjects_t;
typedef struct qRenderStates_t qRenderStates_t;
typedef struct qGame_t qGame_t;

typedef struct qRenderer_t
{
   qRenderObjects_t* renderObjects;
   qRenderStates_t* renderStates;
}
qRenderer_t;

qRenderer_t* qRenderer_Create();
void qRenderer_Destroy( qRenderer_t* renderer );
void qRenderer_Render( qGame_t* game );

#endif // RENDERER_H
