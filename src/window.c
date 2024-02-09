#include "window.h"
#include "game.h"
#include "input_state.h"

qWindow_t* qWindow_Create()
{
   sfVideoMode videoMode = { WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_BPP };
   qWindow_t* window = (qWindow_t*)qAlloc( sizeof( qWindow_t ), sfTrue );

   window->sfmlWindow = qRenderWindow_Create( videoMode, STR_WINDOW_TITLE, WINDOW_STYLE, 0 );

   sfRenderWindow_setKeyRepeatEnabled( window->sfmlWindow, sfFalse );
   window->wantToClose = sfFalse;

   return window;
}

void qWindow_Destroy( qWindow_t* window )
{
   qRenderWindow_Destroy( window->sfmlWindow );

   qFree( window, sizeof( qWindow_t ), sfTrue );
}

void qWindow_Display( qWindow_t* window )
{
   sfRenderWindow_display( window->sfmlWindow );
}

void qWindow_Close( qWindow_t* window )
{
   sfRenderWindow_close( window->sfmlWindow );
}

void qWindow_HandleEvents( qGame_t* game )
{
   sfEvent e;

   while ( sfRenderWindow_pollEvent( game->window->sfmlWindow, &e ) )
   {
      switch ( e.type )
      {
         case sfEvtClosed:
            game->window->wantToClose = sfTrue;
            break;
         case sfEvtKeyPressed:
            qInputState_SetKeyPressed( game->inputState, e.key.code );
            break;
         case sfEvtKeyReleased:
            qInputState_SetKeyReleased( game->inputState, e.key.code );
            break;
      }
   }
}

void qWindow_DrawRectangleShape( qWindow_t* window, sfRectangleShape* rect )
{
   sfRenderWindow_drawRectangleShape( window->sfmlWindow, rect, 0 );
}

void qWindow_DrawText( qWindow_t* window, sfText* text )
{
   sfRenderWindow_drawText( window->sfmlWindow, text, 0 );
}

void qWindow_DrawSprite( qWindow_t* window, sfSprite* sprite )
{
   sfRenderWindow_drawSprite( window->sfmlWindow, sprite, 0 );
}

sfBool qWindow_IsOpen( qWindow_t* window )
{
   return sfRenderWindow_isOpen( window->sfmlWindow );
}
