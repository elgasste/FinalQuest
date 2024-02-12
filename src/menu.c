#include "menu.h"
#include "render_states.h"

qMenus_t* qMenus_Create()
{
   qMenus_t* menus = (qMenus_t*)qAlloc( sizeof( qMenus_t ), sfTrue );

   menus->map = (qMenu_t*)qAlloc( sizeof( qMenu_t ), sfTrue );
   menus->map->optionCount = 2;
   menus->map->options = (qMenuOption_t*)qAlloc( sizeof( qMenuOption_t ) * 2, sfTrue );
   menus->map->options[0].command = qMenuCommand_CloseMenu;
   snprintf( menus->map->options[0].label, STRLEN_SHORT - 1, STR_MENU_CLOSEMENU );
   menus->map->options[1].command = qMenuCommand_Quit;
   snprintf( menus->map->options[1].label, STRLEN_SHORT - 1, STR_MENU_QUITGAME );

   return menus;
}

void qMenus_Destroy( qMenus_t* menus )
{
   qFree( menus->map->options, sizeof( qMenuOption_t ) * menus->map->optionCount, sfTrue );
   qFree( menus->map, sizeof( qMenu_t ), sfTrue );
   qFree( menus, sizeof( qMenus_t ), sfTrue );
}

void qMenu_ScrollUp( qMenu_t* menu, qMenuRenderState_t* renderState )
{
   if ( menu->selectedIndex > 0 )
   {
      menu->selectedIndex--;
   }

   qRenderStates_ResetMenu( renderState );
}

void qMenu_ScrollDown( qMenu_t* menu, qMenuRenderState_t* renderState )
{
   if ( menu->selectedIndex < menu->optionCount - 1 )
   {
      menu->selectedIndex++;
   }

   qRenderStates_ResetMenu( renderState );
}

