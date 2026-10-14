// FUNC 8003acd8 124 MAIN0
// MATCHING 8003acd8 124
#include "scriptctx.h"
extern void printInfoMessage(int, int);
extern void playSFX(int);
extern void increaseMaxHealth(void);
extern void addItemToInventory(int, int, int);

void scriptOpGiveItem(void)
{
    ScriptCtx *p = SCRIPT_CTX;
    int a = p->arg[0];
    int b = p->arg[1];
    int c = p->arg[2];
    if (a < 0) {
        if (c != 0) {
            printInfoMessage(0x15, 3);
            playSFX(10);
        }
        increaseMaxHealth();
    } else {
        addItemToInventory(a, b, c);
    }
    p->pc += 1;
}
