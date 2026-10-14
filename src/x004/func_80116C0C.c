// FUNC 80116c0c 304 X004
// MATCHING 80116c0c 304
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"

extern short D_1F80016A, D_1F800172;
extern unsigned short D_1F8001FC;
extern unsigned char D_8009D2B0;
extern int tryStartItemInteraction(TObj *, int, int);

static __inline__ short Blocked(void)
{
    if ((*(unsigned char*)&PLAYER.obj.wac) >= 2) return 1;
    if (!PLAYER.obj.b69) return 1;
    if (GVARS.b40 | ((*(unsigned char*)&PLAYER.obj.waa) | (PLAYER.obj.b9c | PLAYER.obj.b9e))) return 1;
    if (D_1F8001FC & 0x10) return 0;
    return 1;
}

void func_80116C0C(TObj *o)
{
    if (D_1F80016A < 0x78 && D_1F800172 == 0x546) {
        if (tryStartItemInteraction(o, 1, 0)) {
            D_8009D2B0 = 0;
            GVARS.b3f = 1;
            GVARS.b42 = 1;
        }
        return;
    }
    if (Blocked()) return;
    if (D_1F800172 == 0x4ec && (unsigned short)D_1F80016A - 0x408 < 0x60u) {
        if (tryStartItemInteraction(o, 1, 2)) {
            D_8009D2B0 = 0;
            GVARS.b3f = 1;
            GVARS.b42 = 1;
        }
    }
}
