#if !defined( COMMON_H )
#define COMMON_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <SFML/Graphics.h>

#include "strings.h"

#define STRLEN_DEFAULT  512
#define STRLEN_SHORT    32

#define LOG_FILENAME    "log.txt"

#define GRAPHICS_SCALE  2

#define WINDOW_WIDTH    512 * GRAPHICS_SCALE
#define WINDOW_HEIGHT   480 * GRAPHICS_SCALE
#define WINDOW_BPP      32
#define WINDOW_STYLE    sfTitlebar | sfClose

#define GAME_FPS        60

#define DEBUG_FONT      "resources/fonts/consolas.ttf"

#define MAP_TILE_SIZE   32

#define TOGGLE_BOOL( x ) x = x ? sfFalse : sfTrue;

// globals
uint64_t g_memAllocated;
uint64_t g_memFreed;
uint64_t g_memCounter;
uint64_t g_maxMemCounter;
uint32_t g_sfmlObjectsCreated;
uint32_t g_sfmlObjectsDestroyed;
uint32_t g_sfmlObjectCounter;
uint32_t g_maxSfmlObjectCounter;

// logging and error handling
void qLog_Clear();
void qLog_Msg( const char* msg );
void qLog_Newline();
void qExitWithError( const char* msg );

// memory utilities
void* qAlloc( size_t size, sfBool track );
void* qCalloc( size_t count, size_t size, sfBool track );
void qFree( void* obj, size_t size, sfBool track );

sfRenderWindow* qsfRenderWindow_Create( sfVideoMode mode, const char* title, sfUint32 style, const sfContextSettings* settings );
sfClock* qsfClock_Create();
sfRectangleShape* qsfRectangleShape_Create();
sfCircleShape* qsfCircleShape_Create();
sfConvexShape* qsfConvexShape_Create();
sfFont* qsfFont_CreateFromFile( const char* filePath );
sfText* qsfText_Create();
sfTexture* qsfTexture_CreateFromFile( const char* filePath );
sfSprite* qsfSprite_Create();

void qsfRenderWindow_Destroy( sfRenderWindow* window );
void qsfClock_Destroy( sfClock* clock );
void qsfRectangleShape_Destroy( sfRectangleShape* rect );
void qsfCircleShape_Destroy( sfCircleShape* circle );
void qsfConvexShape_Destroy( sfConvexShape* shape );
void qsfFont_Destroy( sfFont* font );
void qsfText_Destroy( sfText* text );
void qsfTexture_Destroy( sfTexture* texture );
void qsfSprite_Destroy( sfSprite* sprite );

#endif // COMMON_H
