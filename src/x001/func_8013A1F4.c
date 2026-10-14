// FUNC 8013a1f4 228 X001
// MATCHING 8013a1f4 228
#include "player.h"
#include "TOBJ.H"

extern unsigned char D_8009CEAB;
extern signed char D_8009D2B0;
extern unsigned short D_1F8001FC;
extern unsigned short D_1F8003C4;
extern unsigned char D_8009C942;
extern unsigned char D_800A60F8;

int func_8013A1F4(TObj *o)
{
    int r = 0;

    if (D_8009CEAB != 0 && D_8009CEAB < 4 && PLAYER.obj.b69 != 0
        && (unsigned short)((*(Fix16 **)&PLAYER.obj.h)->p.whole - 0xaad) < 0x40 && PLAYER.obj.y.p.whole >= -0x10f) {
        if (o->animTimer != 0) {
            r = 1;
        } else if (D_8009D2B0 == 1 && (D_1F8001FC & D_1F8003C4)) {
            r = 1;
            D_8009C942 = 1;
            D_800A60F8 = 1;
            o->animTimer = 1;
        }
    }
    return r;
}
