#include "menu.h"
#include "render_states.h"

qMenus_t* qMenus_Create()
{
   qMenus_t* menus = (qMenus_t*)qAlloc( sizeof( qMenus_t ), sfTrue );

   menus->map = (qMenu_t*)qAlloc( sizeof( qMenu_t ), sfTrue );
   menus->map->optionCount = 0;
   menus->map->selectedIndex = 0;

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

