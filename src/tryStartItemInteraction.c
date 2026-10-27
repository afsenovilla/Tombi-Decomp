// FUNC 800270a0 132 MAIN0
// MATCHING 800270a0 132
typedef struct O { char pad[3]; char a; char pad2; unsigned char c; } O;
extern char GAME_DISABLE_SELECT_MENU;
extern unsigned char GVARS_PLAYER_HURT_STATE;
extern signed char GAME_PLAYER_MODE;
extern unsigned char GAME_FADE_SCREEN_CONTROL;
extern short GAME_SELECTED_SPAWN_POINT;

int tryStartItemInteraction(O *o, char x, unsigned char y)
{
    char *p = &GAME_DISABLE_SELECT_MENU;
    *p = 0;
    if (GVARS_PLAYER_HURT_STATE != 0)
        return 0;
    if (GAME_PLAYER_MODE == 3)
        return 0;
    if (GAME_FADE_SCREEN_CONTROL == 2)
        return 0;
    GAME_PLAYER_MODE = 0;
    o->c++;
    o->a = x;
    GAME_SELECTED_SPAWN_POINT = y;
    *p = 1;
    return 1;
}
