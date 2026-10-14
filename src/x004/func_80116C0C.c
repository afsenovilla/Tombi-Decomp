// FUNC 80116c0c 304 X004
// MATCHING 80116c0c 304
#include "spad.h"
#include "gv2.h"
#include "gvars.h"
#include "player.h"
#include "TOBJ.H"

extern int tryStartItemInteraction(TObj *, int, int);

static __inline__ short Blocked(void)
{
    if ((*(unsigned char*)&PLAYER.obj.wac) >= 2) return 1;
    if (!PLAYER.obj.b69) return 1;
    if (GVARS.b40 | ((*(unsigned char*)&PLAYER.obj.waa) | (PLAYER.obj.b9c | PLAYER.obj.b9e))) return 1;
    if (SPAD.s1fc & 0x10) return 0;
    return 1;
}

void func_80116C0C(TObj *o)
{
    if ((*(short*)((char*)&SPAD.i168 + 2)) < 0x78 && (*(short*)((char*)&SPAD.i170 + 2)) == 0x546) {
        if (tryStartItemInteraction(o, 1, 0)) {
            GV2.bb0 = 0;
            GVARS.b3f = 1;
            GVARS.b42 = 1;
        }
        return;
    }
    if (Blocked()) return;
    if ((*(short*)((char*)&SPAD.i170 + 2)) == 0x4ec && (unsigned short)(*(short*)((char*)&SPAD.i168 + 2)) - 0x408 < 0x60u) {
        if (tryStartItemInteraction(o, 1, 2)) {
            GV2.bb0 = 0;
            GVARS.b3f = 1;
            GVARS.b42 = 1;
        }
    }
}
