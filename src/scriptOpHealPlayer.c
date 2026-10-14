// FUNC 8003aedc 148 MAIN0
// MATCHING 8003aedc 148
#include "gamestate.h"
#include "scriptctx.h"
#include "player.h"

void scriptOpHealPlayer(void)
{
    ScriptCtx *g = SCRIPT_CTX;
    unsigned char m = GAME.playerHealthDisplayed;
    short v = ((short*)&PLAYER.obj.w98)[0];
    int a = g->arg[0];
    if (v < m) {
        ((short*)&PLAYER.obj.w98)[0] = v + a;
        if (((short*)&PLAYER.obj.w98)[0] > m) ((short*)&PLAYER.obj.w98)[0] = m;
        ((short*)&PLAYER.obj.w9a)[0] = ((short*)&PLAYER.obj.w98)[0];
        ((unsigned char*)&GAME.playerHealth)[0] = ((short*)&PLAYER.obj.w98)[0];
        g->arg[0] = 0;
    } else {
        g->arg[0] = 1;
    }
    g->pc++;
}
