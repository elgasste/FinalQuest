#include "game.h"

int main()
{
   char memStr[STRLEN_DEFAULT];

   g_memAllocated = 0;
   g_memFreed = 0;
   g_memCounter = 0;
   g_maxMemCounter = 0;
   g_sfmlObjectsCreated = 0;
   g_sfmlObjectsDestroyed = 0;
   g_sfmlObjectCounter = 0;
   g_maxSfmlObjectCounter = 0;

   qLog_Clear();

   printf( STR_CONSOLE_LOADING );
   qLog_Msg( STR_MAIN_LOADINGGAME );
   qGame_t* game = qGame_Create();
   printf( STR_CONSOLE_HAVEFUN );

   qLog_Msg( STR_MAIN_RUNNINGGAME );
   qGame_Run( game );

   printf( STR_CONSOLE_CLEANINGUP );
   qLog_Msg( STR_MAIN_GAMECLOSED );
   qGame_Destroy( game );
   printf( STR_CONSOLE_BURRITO );

   snprintf( memStr, STRLEN_DEFAULT, "%s: %llu", STR_MAIN_MEMALLOCATED, g_memAllocated );
   qLog_Msg( memStr );
   snprintf( memStr, STRLEN_DEFAULT, "%s: %llu", STR_MAIN_MEMFREED, g_memFreed );
   qLog_Msg( memStr );
   snprintf( memStr, STRLEN_DEFAULT, "%s: %llu", STR_MAIN_MEMMAX, g_maxMemCounter );
   qLog_Msg( memStr );
   snprintf( memStr, STRLEN_DEFAULT, "%s: %u", STR_MAIN_SFMLOBJCREATED, g_sfmlObjectsCreated );
   qLog_Msg( memStr );
   snprintf( memStr, STRLEN_DEFAULT, "%s: %u", STR_MAIN_SFMLOBJDESTROYED, g_sfmlObjectsDestroyed );
   qLog_Msg( memStr );
   snprintf( memStr, STRLEN_DEFAULT, "%s: %u", STR_MAIN_SFMLOBJMAX, g_maxSfmlObjectCounter );
   qLog_Msg( memStr );
   qLog_Newline();

   return 0;
}
