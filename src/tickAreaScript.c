// FUNC 80039580 1092 MAIN0
// MATCHING 80039580 1092
#include "gv6.h"
#include "gamestate.h"
#include "player.h"
#include "scriptctx.h"
extern ScriptCtx AREA_SCRIPT_CTX;
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
    SCRIPT_CTX = &AREA_SCRIPT_CTX;
    p = SCRIPT_CTX;
    SCRIPT_CODE = AREA_SCRIPT_CTX.p84;
    if (AREA_SCRIPT_CTX.state == 2 && ++D_800A0A10 >= D_800A0A14) {
        AREA_SCRIPT_CTX.state = 1;
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

void tickAreaScript(void)
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
