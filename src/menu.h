#if !defined( MENU_H )
#define MENU_H

#include "common.h"
#include "enums.h"

typedef struct qMenuRenderState_t qMenuRenderState_t;

typedef struct qMenuOption_t
{
   char label[STRLEN_SHORT];
   qMenuCommand_t command;
}
qMenuOption_t;

typedef struct qMenu_t
{
   qMenuOption_t* options;
   uint32_t optionCount;
   uint32_t selectedIndex;
}
qMenu_t;

typedef struct qMenus_t
{
   qMenu_t* map;
   qMenu_t* battleAction;
}
qMenus_t;

qMenus_t* qMenus_Create();
void qMenus_Destroy( qMenus_t* menus );
void qMenu_ScrollUp( qMenu_t* menu, qMenuRenderState_t* renderState );
void qMenu_ScrollDown( qMenu_t* menu, qMenuRenderState_t* renderState );

#endif // MENU_H
