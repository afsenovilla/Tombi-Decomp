// FUNC 8003926c 204 MAIN0
// MATCHING 8003926c 204
// Ported from psx_tomba (scriptexec.c, runScriptContext); MIT licence of the original project.
#define SKIP_ASM
#include "common.h"
#include "game.h"
extern u8 D_8009C119;
extern u8 D_8009C120;
extern u8 D_8009C121;
extern u8 D_8009C245;

u_char runScriptContext(unkstruct_8009E458* ctx)
{
    unkstruct_8009E458* p;
    u8* base;
    int ret;

    base = *(u8**)((u8*)ctx + 0x84);
    D_8009E458 = ctx;
    D_8009C974 = base;
    p = D_8009E458;
    if (p->state == 2) {
        *(u_int*)((u8*)p + 0x11D0) += 1;
        if (*(u_int*)((u8*)p + 0x11D0) >= *(u_int*)((u8*)p + 0x11D4)) {
            p->state = 1;
        }
    }
    if (p->state != 1) {
        return p->state;
    }
    do {
        unkstruct_8009E458* q = D_8009E458;
        u8* script = D_8009C974;
        u8 op = script[q->pc];

        if (op < 0x80) {
            ret = execCoreOpcode(op);
        } else {
            ret = execGameOpcode(op);
        }
    } while (ret != 0);
    return p->state;
}
