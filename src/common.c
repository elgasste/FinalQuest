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

sfRenderWindow* qRenderWindow_Create( sfVideoMode mode, const char* title, sfUint32 style, const sfContextSettings* settings )
{
   INCREMENT_SFML()
   return sfRenderWindow_create( mode, title, style, settings );
}

sfClock* qInnerClock_Create()
{
   INCREMENT_SFML()
   return sfClock_create();
}

sfRectangleShape* qRectangleShape_Create()
{
   INCREMENT_SFML()
   return sfRectangleShape_create();
}

sfCircleShape* qCircleShape_Create()
{
   INCREMENT_SFML()
   return sfCircleShape_create();
}

sfConvexShape* qConvexShape_Create()
{
   INCREMENT_SFML()
   return sfConvexShape_create();
}

sfFont* qFont_CreateFromFile( const char* filePath )
{
   INCREMENT_SFML()
   return sfFont_createFromFile( filePath );
}

sfText* qText_Create()
{
   INCREMENT_SFML()
   return sfText_create();
}

sfTexture* qTexture_CreateFromFile( const char* filePath )
{
   INCREMENT_SFML()
   return sfTexture_createFromFile( filePath, 0 );
}

sfSprite* qSprite_Create()
{
   INCREMENT_SFML()
   return sfSprite_create();
}

void qRenderWindow_Destroy( sfRenderWindow* window )
{
   DECREMENT_SFML()
   sfRenderWindow_destroy( window );
}

void qInnerClock_Destroy( sfClock* clock )
{
   DECREMENT_SFML()
   sfClock_destroy( clock );
}

void qRectangleShape_Destroy( sfRectangleShape* rect )
{
   DECREMENT_SFML()
   sfRectangleShape_destroy( rect );
}

void qCircleShape_Destroy( sfCircleShape* circle )
{
   DECREMENT_SFML()
   sfCircleShape_destroy( circle );
}

void qConvexShape_Destroy( sfConvexShape* shape )
{
   DECREMENT_SFML()
   sfConvexShape_destroy( shape );
}

void qFont_Destroy( sfFont* font )
{
   DECREMENT_SFML()
   sfFont_destroy( font );
}

void qText_Destroy( sfText* text )
{
   DECREMENT_SFML()
   sfText_destroy( text );
}

void qTexture_Destroy( sfTexture* texture )
{
   DECREMENT_SFML()
   sfTexture_destroy( texture );
}

void qSprite_Destroy( sfSprite* sprite )
{
   DECREMENT_SFML()
   sfSprite_destroy( sprite );
}
