// VERSION BETA_980203 beta980203 800293D0 112
#if defined(VERSION_BETA_980203)
// Beta version: JP C of src/func_80027EA8_slps01144.c (func_80027EA8_slps01144) (1.000 masked) [-O2 -G0] (tools/demo_sweep.py)
#define GAME_DISABLE_SELECT_MENU D_8009E94A_beta980203
#define GAME_PLAYER_MODE D_8009EE58_beta980203
#define GAME_FADE_SCREEN_CONTROL D_8009E51D_beta980203
#define GAME_SELECTED_SPAWN_POINT D_8009E52A_beta980203
typedef struct O { char pad[3]; char a; char pad2; unsigned char c; } O;
extern char GAME_DISABLE_SELECT_MENU;
extern unsigned char GVARS_PLAYER_HURT_STATE;
extern signed char GAME_PLAYER_MODE;
extern unsigned char GAME_FADE_SCREEN_CONTROL;
extern short GAME_SELECTED_SPAWN_POINT;

int func_800293D0_beta980203(O *o, char x, unsigned char y)
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
