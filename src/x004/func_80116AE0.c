// FUNC 80116ae0 300 X004
// MATCHING 80116ae0 300
#include "gv2.h"
#include "gvars.h"
#include "TOBJ.H"
#include "raw7.h"

extern TObj PLAYER;
extern short D_1F80016A, D_1F800172;
extern unsigned short D_1F8001FC;
extern int tryStartItemInteraction(TObj *, int, int);

static __inline__ short busy(void)
{
    if (U8(&PLAYER, 0xac) >= 2) return 1;
    if (PLAYER.b69 == 0) return 1;
    if (PLAYER.b9c | PLAYER.b9e | U8(&PLAYER, 0xaa) | GVARS.b40) return 1;
    if (D_1F8001FC & 0x10) return 0;
    return 1;
}

void func_80116AE0(TObj *o)
{
    if (D_1F80016A >= 0x6d0 && D_1F800172 == 0x168) {
        if (tryStartItemInteraction(o, 1, 7)) {
            GV2.bb0 = 0;
            GVARS.b3f = 1;
            GVARS.b42 = 1;
        }
        return;
    }
    if (busy()) return;
    if (D_1F80016A < 0x28a && D_1F800172 == 0x21c) {
        if (tryStartItemInteraction(o, 1, 8)) {
            GV2.bb0 = 0;
            GVARS.b3f = 1;
            GVARS.b42 = 1;
        }
    }
}
