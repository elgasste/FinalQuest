#include <time.h>
#include <string.h>

#include "common.h"

#define INCREMENT_MEM( s ) {\
      g_memAllocated += s;\
      g_memCounter += s;\
      if ( g_memCounter > g_maxMemCounter )\
         g_maxMemCounter = g_memCounter;\
   }
#define INCREMENT_SFML() {\
      g_sfmlObjectsCreated++;\
      g_sfmlObjectCounter++;\
      if ( g_sfmlObjectCounter > g_maxSfmlObjectCounter )\
         g_maxSfmlObjectCounter = g_sfmlObjectCounter;\
   }
#define DECREMENT_MEM( s ) { g_memFreed += s; g_memCounter -= s; }
#define DECREMENT_SFML() { g_sfmlObjectsDestroyed++; }

void qLog_Clear()
{
   FILE* logFile = fopen( LOG_FILENAME, "w" );
   fclose( logFile );
}

void qLog_Msg( const char* msg )
{
   FILE* logFile = 0;
   time_t t = time( 0 );
   struct tm* tm;
   char errorMsg[STRLEN_DEFAULT];

   logFile = fopen( LOG_FILENAME, "a" );

   if ( !logFile )
   {
      snprintf( errorMsg, STRLEN_DEFAULT, "%s: \"%s\"", STR_ERROR_OPENLOGFILE, msg );
      qExitWithError( errorMsg );
   }

   tm = localtime( &t );

   // NOTE: the logFile check here is there to make Visual Studio's analyzer happy
   if ( strcmp( msg, "\n" ) == 0 && logFile )
   {
      if ( fprintf( logFile, msg ) < 0 )
      {
         snprintf( errorMsg, STRLEN_DEFAULT, "%s: \"%s\"", STR_ERROR_WRITELOGFILE, msg );
         qExitWithError( errorMsg );
      }
   }
   else if ( logFile )
   {
      if ( fprintf( logFile, "%d-%02d-%02d %02d:%02d:%02d - %s\n",
                    tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday,
                    tm->tm_hour, tm->tm_min, tm->tm_sec,
                    msg ) < 0 )
      {
         snprintf( errorMsg, STRLEN_DEFAULT, "%s: \"%s\"", STR_ERROR_WRITELOGFILE, msg );
         qExitWithError( errorMsg );
      }
   }

   if ( logFile && fclose( logFile ) )
   {
      qExitWithError( STR_ERROR_CLOSELOGFILE );
   }
}

void qLog_Newline()
{
   qLog_Msg( "\n" );
}

void qExitWithError( const char* msg )
{
   char output[STRLEN_DEFAULT];
   snprintf( output, STRLEN_DEFAULT, "%s: %s\n\n", STR_ERROR, msg );
   printf( output );
   exit( 1 );
}

void* qAlloc( size_t size, sfBool track )
{
   void* mem = malloc( size );

   if ( !mem )
   {
      qExitWithError( STR_ERROR_MALLOC );
   }
   if ( track )
   {
      INCREMENT_MEM( size )
   }

   return mem;
}

void* qCalloc( size_t count, size_t size, sfBool track )
{
   void* mem = calloc( count, size );

   if ( !mem )
   {
      qExitWithError( STR_ERROR_MALLOC );
   }
   if ( track )
   {
      INCREMENT_MEM( size * count )
   }

   return mem;
}

void qFree( void* obj, size_t size, sfBool track )
{
   free( obj );

   if ( track )
   {
      DECREMENT_MEM( size )
   }
}

sfRenderWindow* qsfRenderWindow_Create( sfVideoMode mode, const char* title, sfUint32 style, const sfContextSettings* settings )
{
   INCREMENT_SFML()
   return sfRenderWindow_create( mode, title, style, settings );
}

sfClock* qsfClock_Create()
{
   INCREMENT_SFML()
   return sfClock_create();
}

sfRectangleShape* qsfRectangleShape_Create()
{
   INCREMENT_SFML()
   return sfRectangleShape_create();
}

sfCircleShape* qsfCircleShape_Create()
{
   INCREMENT_SFML()
   return sfCircleShape_create();
}

sfConvexShape* qsfConvexShape_Create()
{
   INCREMENT_SFML()
   return sfConvexShape_create();
}

sfFont* qsfFont_CreateFromFile( const char* filePath )
{
   INCREMENT_SFML()
   return sfFont_createFromFile( filePath );
}

sfText* qsfText_Create()
{
   INCREMENT_SFML()
   return sfText_create();
}

sfTexture* qsfTexture_CreateFromFile( const char* filePath )
{
   INCREMENT_SFML()
   return sfTexture_createFromFile( filePath, 0 );
}

sfSprite* qsfSprite_Create()
{
   INCREMENT_SFML()
   return sfSprite_create();
}

void qsfRenderWindow_Destroy( sfRenderWindow* window )
{
   DECREMENT_SFML()
   sfRenderWindow_destroy( window );
}

void qsfClock_Destroy( sfClock* clock )
{
   DECREMENT_SFML()
   sfClock_destroy( clock );
}

void qsfRectangleShape_Destroy( sfRectangleShape* rect )
{
   DECREMENT_SFML()
   sfRectangleShape_destroy( rect );
}

void qsfCircleShape_Destroy( sfCircleShape* circle )
{
   DECREMENT_SFML()
   sfCircleShape_destroy( circle );
}

void qsfConvexShape_Destroy( sfConvexShape* shape )
{
   DECREMENT_SFML()
   sfConvexShape_destroy( shape );
}

void qsfFont_Destroy( sfFont* font )
{
   DECREMENT_SFML()
   sfFont_destroy( font );
}

void qsfText_Destroy( sfText* text )
{
   DECREMENT_SFML()
   sfText_destroy( text );
}

void qsfTexture_Destroy( sfTexture* texture )
{
   DECREMENT_SFML()
   sfTexture_destroy( texture );
}

void qsfSprite_Destroy( sfSprite* sprite )
{
   DECREMENT_SFML()
   sfSprite_destroy( sprite );
}
