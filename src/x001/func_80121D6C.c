// FUNC 80121d6c 728 X001
// MATCHING 80121d6c 728
#include "TOBJ.H"
#include "raw7.h"

extern TObj *D_8009C330;
extern unsigned char D_8009C942[], D_8009C93F[];
extern unsigned short D_1F8001F8;
extern void FUN_800eeb5c(TObj *, int);
extern int FUN_8003facc(TObj *);
extern void FUN_8001e560(int, int);
extern int FUN_8004232c(short, short, short);
extern int AnimAdvance(TObj *);

void func_80121D6C(TObj *o)
{
    short y, z;
    short x;
    short w;

    switch (o->state) {
    case 0:
        U8(o, 0xa0) = 0;
        FUN_800eeb5c(o, 0x1b);
        o->wb6 = 0;
        S16(D_8009C330, 2) = 0;
        U8(o, 0xa2) = 3;
        o->wb0 = 0;
        o->wb2 = 0;
        o->velH = 0;
        o->velV = 0;
        o->d8c = 0;
        U8(o, 0xaa) = 1;
        o->timer = 0xf;
        D_8009C942[0] = 1;
        D_8009C93F[0] = 1;
        o->state++;
    case 1:
        AnimAdvance(o);
        if (--o->timer == 0) {
            o->timer = 8;
            FUN_800eeb5c(o, 0x17);
            o->state++;
        }
        break;
    case 2:
        if (!FUN_8003facc(o)) o->y.p.whole--;
        AnimAdvance(o);
        if (!(D_1F8001F8 & 0xf)) FUN_8001e560(0x1d, 0);
        y = o->y.p.whole - 8;
        w = o->h->p.whole + 8;
        z = o->d->p.whole;
        if ((FUN_8004232c(o->h->p.whole - 8, y, z) & 2) || (FUN_8004232c(w, y, z) & 2)) {
            o->b04 = 1;
            o->step = 0x16;
            o->state = 0;
        }
        break;
    case 3:
        AnimAdvance(o);
        o->y.raw += o->velY << 8;
        o->velY += 0x10;
        y = o->y.p.whole - 8;
        x = o->h->p.whole;
        w = x + 8;
        z = (short)(o->d->p.whole / 90) * 90;
        if ((FUN_8004232c(x - 8, y, z) & 2) || (FUN_8004232c(w, y, z) & 2)) {
            D_8009C942[0] = 0;
            o->b04 = 1;
            o->step = 0x16;
            o->state = 0;
        }
        break;
    }
}
