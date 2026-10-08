// FUNC 801206f0 284 X003
// MATCHING 801206f0 284
#include "TOBJ.H"
extern short D_1F80027E;
extern unsigned short D_1F800282, D_1F800284;
extern short FUN_80040278(TObj *, int, int);

int func_801206F0(TObj *o)
{
    short v;

    if (o->b69 == 1) {
        o->d8c = 0;
        o->wb2 = 0;
        o->b69 = 0;
        o->wae = -1;
        o->b9c = 0;
        return 1;
    }
    if (FUN_80040278(o, o->h->p.whole, (short)(o->y.p.whole + 0x10)) == 0)
        return 0;
    o->b69 = 0;
    v = D_1F80027E;
    if (v < 0)
        v = -v;
    if (v > 8)
        v = 8;
    if (D_1F80027E < 0)
        v = -v;
    o->wb2 = v;
    o->d8c = -v & 0xff;
    o->wb6 = (-v << 2) & 0xff;
    o->wae = D_1F800284;
    o->b9c = 0;
    if ((short)((D_1F800282 >> 5) & 0xf) >= 0xe)
        o->w98 = 0;
    return 1;
}
