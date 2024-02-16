#if !defined( ENUMS_H )
#define ENUMS_H

typedef enum qGameState_t
{
   qGameState_Map = 0,
   qGameState_MapMenu,
   qGameState_FadeMapToBattle,
   qGameState_FadeBattleIn,
   qGameState_BattleChooseAction,
   qGameState_FadeBattleOut,
   qGameState_FadeBattleToMap
}
qGameState_t;

typedef enum qDirection_t
{
   qDirection_Left = 0,
   qDirection_Up,
   qDirection_Right,
   qDirection_Down
}
qDirection_t;

typedef enum qMenuCommand_t
{
   qMenuCommand_Quit = 0,
   qMenuCommand_CloseMenu,

   qMenuCommand_BattleAttack,
   qMenuCommand_BattleDefend,
   qMenuCommand_BattleSpell,
   qMenuCommand_BattleItem,
   qMenuCommand_BattleFlee
}
qMenuCommand_t;

#endif // ENUMS_H
