#if !defined( PHYSICS_H )
#define PHYSICS_H

#include "common.h"

typedef struct qGame_t qGame_t;

typedef struct qPhysics_t
{
   uint32_t entityMapTileCache;
}
qPhysics_t;

qPhysics_t* qPhysics_Create();
void qPhysics_Destroy( qPhysics_t* physics );
void qPhysics_Tic( qGame_t* game );

#endif // PHYSICS_H
