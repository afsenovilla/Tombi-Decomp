// FUNC 8003aedc 148 MAIN0
// MATCHING 8003aedc 148
#include "gamestate.h"
typedef struct { char p0[0x8a]; unsigned short c; char p1[0x1190-0x8c]; int v; } G;
extern G *SCRIPT_CTX;
extern short D_800A60D0[];
extern short D_800A60D2[];

void func_8003AEDC(void)
{
    G *g = SCRIPT_CTX;
    unsigned char m = GAME.playerHealthDisplayed;
    short v = D_800A60D0[0];
    int a = g->v;
    if (v < m) {
        D_800A60D0[0] = v + a;
        if (D_800A60D0[0] > m) D_800A60D0[0] = m;
        D_800A60D2[0] = D_800A60D0[0];
        ((unsigned char*)&GAME.playerHealth)[0] = D_800A60D0[0];
        g->v = 0;
    } else {
        g->v = 1;
    }
    g->c++;
}
