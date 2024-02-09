#if !defined( WINDOW_H )
#define WINDOW_H

#include "common.h"

typedef struct qGame_t qGame_t;

typedef struct qWindow_t
{
   sfRenderWindow* sfmlWindow;
   sfBool wantToClose;
}
qWindow_t;

qWindow_t* qWindow_Create();
void qWindow_Destroy( qWindow_t* window );
void qWindow_Display( qWindow_t* window );
void qWindow_Close( qWindow_t* window );
void qWindow_HandleEvents( qGame_t* game );
void qWindow_DrawRectangleShape( qWindow_t* window, sfRectangleShape* rect );
void qWindow_DrawText( qWindow_t* window, sfText* text );
void qWindow_DrawSprite( qWindow_t* window, sfSprite* sprite );
sfBool qWindow_IsOpen( qWindow_t* window );

#endif // WINDOW_H
