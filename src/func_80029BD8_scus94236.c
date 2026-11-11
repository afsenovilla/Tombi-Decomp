// VERSION SCUS_94236 scus94236 80029BD8 112
#if defined(VERSION_SCUS_94236)
// USA version: JP C of src/func_80027EA8_slps01144.c (func_80027EA8_slps01144) (1.000 masked) [-O2 -G0] (tools/demo_sweep.py)
#define GAME_DISABLE_SELECT_MENU D_8009C10A
#define GAME_PLAYER_MODE D_8009C618
#define GAME_FADE_SCREEN_CONTROL D_8009BCDD
#define GAME_SELECTED_SPAWN_POINT D_8009BCEA
typedef struct O { char pad[3]; char a; char pad2; unsigned char c; } O;
extern char GAME_DISABLE_SELECT_MENU;
extern unsigned char GVARS_PLAYER_HURT_STATE;
extern signed char GAME_PLAYER_MODE;
extern unsigned char GAME_FADE_SCREEN_CONTROL;
extern short GAME_SELECTED_SPAWN_POINT;

int func_80029BD8_scus94236(O *o, char x, unsigned char y)
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
