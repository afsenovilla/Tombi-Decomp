// near-twin of tryStartItemInteraction (PAL)
// VERSION SLPS_01144 slps01144 80027EA8 112
#if defined(VERSION_SLPS_01144)
#define GAME_DISABLE_SELECT_MENU D_8009E94A
#define GAME_PLAYER_MODE D_8009EE58
#define GAME_FADE_SCREEN_CONTROL D_8009E51D
#define GAME_SELECTED_SPAWN_POINT D_8009E52A
typedef struct O { char pad[3]; char a; char pad2; unsigned char c; } O;
extern char GAME_DISABLE_SELECT_MENU;
extern unsigned char GVARS_PLAYER_HURT_STATE;
extern signed char GAME_PLAYER_MODE;
extern unsigned char GAME_FADE_SCREEN_CONTROL;
extern short GAME_SELECTED_SPAWN_POINT;

int func_80027EA8_slps01144(O *o, char x, unsigned char y)
{
    char *p = &GAME_DISABLE_SELECT_MENU;
    *p = 0;
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
#endif
