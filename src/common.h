#if !defined( COMMON_H )
#define COMMON_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SFML/Graphics.h>

#include "strings.h"

#define STRLEN_DEFAULT              512
#define STRLEN_SHORT                64

#define LOG_FILENAME                "log.txt"

#define GRAPHICS_SCALE              3

#define WINDOW_WIDTH                ( 768 * GRAPHICS_SCALE )
#define WINDOW_HEIGHT               ( 432 * GRAPHICS_SCALE )
#define WINDOW_BPP                  32
#define WINDOW_STYLE                sfTitlebar | sfClose

#define GAME_FPS                    60

#define DEBUG_FONT                  "resources/fonts/consolas.ttf"
#define GAME_FONT                   "resources/fonts/abaddon_bold.ttf"
#define GAME_FONT_SIZE              16
#define GAME_FONT_COLOR             sfColor_fromRGB( 224, 224, 224 )
#define DIALOG_BACKDROP_LIGHTCOLOR  sfColor_fromRGBA( 0, 0, 0, 192 )
#define DIALOG_BACKDROP_DARKCOLOR   sfColor_fromRGBA( 255, 255, 255, 24 )

#define MAP_TILE_SIZE               32

#define CHEAT_NOCLIP                "fqclip"
#define CHEAT_FAST                  "fqfast"
#define CHEAT_ENCOUNTER             "fqfight"
#define CHEAT_NOENCOUNTERS          "fqinvis"
#define CHEAT_CLEAR                 "fqclear"

#define FAST_VELOCITY               500.0f

#define MAX_ACTORS                  256

#define BATTLE_ACTION_PAUSESECONDS  0.6f

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
