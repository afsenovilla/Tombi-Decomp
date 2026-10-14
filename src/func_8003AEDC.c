// FUNC 8003aedc 148 MAIN0
// MATCHING 8003aedc 148
#include "gamestate.h"
#include "scriptctx.h"
extern short D_800A60D0[];
extern short D_800A60D2[];

void func_8003AEDC(void)
{
    ScriptCtx *g = SCRIPT_CTX;
    unsigned char m = GAME.playerHealthDisplayed;
    short v = D_800A60D0[0];
    int a = g->arg[0];
    if (v < m) {
        D_800A60D0[0] = v + a;
        if (D_800A60D0[0] > m) D_800A60D0[0] = m;
        D_800A60D2[0] = D_800A60D0[0];
        ((unsigned char*)&GAME.playerHealth)[0] = D_800A60D0[0];
        g->arg[0] = 0;
    } else {
        g->arg[0] = 1;
    }
    g->pc++;
}
