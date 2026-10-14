// FUNC 80039580 1092 MAIN0
// MATCHING 80039580 1092
#include "gv6.h"
#include "gamestate.h"
#include "player.h"
#include "scriptctx.h"
extern ScriptCtx D_8009F840;
extern unsigned char *SCRIPT_CODE;
extern unsigned int D_800A0A10, D_800A0A14;
extern int D_8009D4F0;
extern int func_80038CD0(unsigned char);
extern int func_8003AFB4(unsigned char);

static __inline__ unsigned char run(void)
{
    ScriptCtx *p;
    int r;
    unsigned char x;
    SCRIPT_CTX = &D_8009F840;
    p = SCRIPT_CTX;
    SCRIPT_CODE = D_8009F840.p84;
    if (D_8009F840.state == 2 && ++D_800A0A10 >= D_800A0A14) {
        D_8009F840.state = 1;
    }
    x = p->state;
    if (x != 1) return x;
    do {
        ScriptCtx *q = SCRIPT_CTX;
        unsigned char *b = SCRIPT_CODE;
        unsigned char c = b[q->pc];
        if (c < 0x80) {
            r = func_80038CD0(c);
        } else {
            r = func_8003AFB4(c);
        }
    } while (r != 0);
    return p->state;
}

void func_80039580(void)
{
    if (GV6.i9c == 0) return;
    if ((*(unsigned short*)&PLAYER.obj.b04) == 0x505) return;
    switch (GAME.selectedArea) {
    case 0:
        switch (GAME.selectedSection) {
        case 0 ... 2:
            D_8009D4F0 = run();
        }
        break;
    case 1:
        switch (GAME.selectedSection) {
        case 0 ... 4:
            D_8009D4F0 = run();
        }
        break;
    case 2:
        switch (GAME.selectedSection) {
        case 0 ... 2:
            D_8009D4F0 = run();
        }
        break;
    case 0x13:
        switch (GAME.selectedSection) {
        case 0 ... 1:
            D_8009D4F0 = run();
        }
        break;
    }
}
