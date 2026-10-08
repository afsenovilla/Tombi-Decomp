// FUNC 8010dd8c 504 X014
// MATCHING 8010dd8c 504
/* volatile store of velY makes the later read reload it (game: lh + move copy) */
#include "TOBJ.H"
#include "raw7.h"
extern TObj *DAT_8009c330;
extern int DAT_8009d2e8;
extern void PlayerSetAnimIfChanged(TObj *, int);
extern short FUN_8001fe3c(int, int);
extern short FUN_8003fd78(TObj *, int, int);
extern void FUN_800eea7c(TObj *, int, int);

void FUN_8010dd8c(TObj *o)
{
    short r, w;
    short v;
    short u;
    int d;

    switch (o->state) {
    case 0:
        o->velY = 0x300;
        o->b9c = 2;
        U8(o, 0xac) = 1;
        o->b69 = 0;
        o->wb2 = 0x200;
        U8(o, 0xca) = 0;
        PlayerSetAnimIfChanged(o, 0x1d);
        o->timer = 0x78;
        o->state++;
    case 1:
        break;
    default:
        return;
    }
    d = o->d8c;
    o->d8c = (o->animFrame & 1) ? d + 0x10 : d - 0x10;
    r = FUN_8001fe3c(o->wb6, o->wb2);
    o->velH = r;
    o->h->raw += r << 8;
    o->y.raw += o->velY << 8;
    w = o->wb2 - 0x10;
    o->wb2 = w;
    if (w < 0)
        o->wb2 = 0;
    *(volatile short *)&o->velY = o->velY - 0x10;
    o->timer--;
    if (U8(o, 0xac) == 2) {
        DAT_8009d2e8 = S32(o, 0xe4);
        U8(DAT_8009c330, 8) = 0;
        o->ba7 = 0;
        o->ba5 = 0;
        o->step = 0x48;
        o->state = 0;
        o->substep = 0;
        U8(o, 0xab) &= 0x7f;
        return;
    }
    v = o->velY;
    u = v;
    if (v >= 0) {
        if (o->b69) {
            o->velY = u - 0x10;
            o->b69 = 0;
            return;
        }
        if (FUN_8003fd78(o, 0, 0)) {
            o->b69 = 0;
            o->velY -= 0x10;
            return;
        }
        if (o->timer > 0)
            return;
    }
    FUN_800eea7c(o, 0x44, 0);
    o->b69 = 0;
    o->b9c = 0;
    U8(o, 0xac) = 0;
    o->step = 0x3e;
    o->state = 0;
}
