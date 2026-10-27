// FUNC 800263bc 276 MAIN0
// MATCHING 800263bc 276
// VERSION SLPM_80165 slpm80165 80025FF0 292
#if defined(VERSION_SLPM_80165)
// Demo version of PAL main0 800263BC (initHudState.c): sets one more HUD word to 1, gauge style chosen by D2f81 < 9.
typedef struct { char a; char pad[0x1f]; void *p[6]; char pad2[0x5c - 0x38]; char r[3][4]; } GT;
extern GT HUD_STATE;
extern int D1454;
extern char D1412, D1413;
extern short D145c, D145a, D1462, D1460, D1464;
extern int D1428;
extern void *D142c;
extern int S2d8;
extern unsigned char GAME_PLAYER_HEALTH;
extern unsigned char D2f81;
extern char A2048[], A2068[], A20ec[], A2100[], A2150[], A22a0[], A24f8[], A2678[];

extern short D145e;
void initHudState(void)
{
    int i;
    GT *g = &HUD_STATE;
    g->a = 1;
    D1454 = -1;
    D145c = 600;
    D1412 = 0;
    D1413 = 0;
    D145e = 1;
    D145a = 0;
    D1428 = S2d8;
    D1462 = GAME_PLAYER_HEALTH;
    D1460 = GAME_PLAYER_HEALTH;
    D1464 = GAME_PLAYER_HEALTH;
    if (D2f81 < 9)
        D142c = A2048;
    else
        D142c = A2068;
    g->p[0] = A20ec;
    g->p[1] = A2100;
    g->p[2] = A2150;
    g->p[3] = A22a0;
    g->p[4] = A24f8;
    g->p[5] = A2678;
    i = 0;
    do {
        g->r[0][i] = 0;
        g->r[1][i] = 0;
        g->r[2][i] = 0;
        i++;
    } while (i < 3);
}
#else
typedef struct { char a; char pad[0x1f]; void *p[6]; char pad2[0x5c - 0x38]; char r[3][4]; } GT;
extern GT HUD_STATE;
extern int D1454;
extern char D1412, D1413;
extern short D145c, D145a, D1462, D1460, D1464;
extern int D1428;
extern void *D142c;
extern int S2d8;
extern unsigned char GAME_PLAYER_HEALTH;
extern unsigned char D2f81;
extern char A2048[], A2068[], A20ec[], A2100[], A2150[], A22a0[], A24f8[], A2678[];

void initHudState(void)
{
    int i;
    GT *g = &HUD_STATE;
    g->a = 1;
    D1454 = -1;
    D1412 = 0;
    D1413 = 0;
    D145c = 600;
    D145a = 0;
    D1428 = S2d8;
    D1462 = GAME_PLAYER_HEALTH;
    D1460 = GAME_PLAYER_HEALTH;
    D1464 = GAME_PLAYER_HEALTH;
    if (D2f81 == 0)
        D142c = A2048;
    else
        D142c = A2068;
    g->p[0] = A20ec;
    g->p[1] = A2100;
    g->p[2] = A2150;
    g->p[3] = A22a0;
    g->p[4] = A24f8;
    g->p[5] = A2678;
    i = 0;
    do {
        g->r[0][i] = 0;
        g->r[1][i] = 0;
        g->r[2][i] = 0;
        i++;
    } while (i < 3);
}
#endif
