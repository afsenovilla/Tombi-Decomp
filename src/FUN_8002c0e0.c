// FUNC 8002c0e0 344 MAIN0
// MATCHING 8002c0e0 344
#include "TOBJ.H"
extern void FUN_80020490(TObj *);
extern short MulCos(int, int);
extern short MulNegSinScaled(int, int);
extern int AnimAdvance(TObj *);

void FUN_8002c0e0(TObj *o)
{
    unsigned char *p;
    int f;
    unsigned short u;
    int v;
    unsigned char st;
    if (o->visible == 0)
        FUN_80020490(o);
    st = o->step;
    if (st != 0) {
        if (st != 1)
            return;
    } else
        o->step = st + 1;
    p = (unsigned char *)o->d90;
    switch ((short)(o->animFrame = *(unsigned short *)(p + 0x2e) & 7)) {
    case 0: case 2:
        o->d88 = 0x40;
        o->d8c = 0xe0;
        break;
    case 1: case 3:
        o->d88 = 0x40;
        o->d8c = 0x20;
        break;
    case 4:
        o->d88 = 0x60;
        o->d8c = 0;
        break;
    case 5:
        o->d88 = 0x20;
        o->d8c = 0;
        break;
    case 6:
        o->d88 = 0x80;
        o->d8c = 0x20;
        break;
    case 7:
        o->d88 = 0;
        o->d8c = 0xe0;
        break;
    }
    o->a.p.whole = *(short *)(p + 0x12) + MulCos((short)o->d88, 8);
    v = o->d88;
    o->y.p.whole = *(short *)(p + 0x16) + MulNegSinScaled((short)v, 8);
    o->b.p.whole = *(short *)(p + 0x1a);
    if (AnimAdvance(o)) {
        o->b04 = 2;
        o->step = 0;
    }
}
