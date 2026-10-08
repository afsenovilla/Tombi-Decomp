// FUNC 80109e50 792 X001
// MATCHING 80109e50 792
#include "TOBJ.H"
#define B(o, k) (*(unsigned char *)((char *)(o) + (k)))
#define S16(o, k) (*(short *)((char *)(o) + (k)))
extern void FUN_800eeb5c(TObj *, int);
extern void FUN_8001e5f4(int, int);
extern void FUN_8001fec0(TObj *);
extern int FUN_8003facc(TObj *);
extern void FUN_8001e560(int, int);
extern int FUN_8004232c(short, short, short);
extern TObj *DAT_8009c330;
extern unsigned short DAT_1f8001f8;

void FUN_80109e50(TObj *o)
{
    short v;
    short a;
    unsigned short hx;

    switch (o->state) {
    case 0:
        B(o, 0xa0) = 0;
        FUN_800eeb5c(o, 0x1b);
        o->wb6 = 0;
        S16(DAT_8009c330, 2) = 0;
        B(o, 0xa2) = 3;
        B(o, 0xaa) = 1;
        o->wb0 = 0;
        o->wb2 = 0;
        o->velH = 0;
        o->velV = 0;
        o->d8c = 0;
        o->timer = 0xf;
        FUN_8001e5f4(4, 0x7f);
        o->state++;
    case 1:
        FUN_8001fec0(o);
        if (--o->timer == 0) {
            o->timer = 8;
            FUN_800eeb5c(o, 0x17);
            o->state++;
        }
        break;
    case 2:
        if (FUN_8003facc(o) == 0) o->y.raw -= 0x18000;
        FUN_8001fec0(o);
        if ((DAT_1f8001f8 & 0xf) == 0) FUN_8001e560(0x1d, 0);
        v = o->y.p.whole - 8;
        hx = o->h->p.whole + 8;
        a = (short)(o->d->p.whole / 90) * 90;
        if ((FUN_8004232c(o->h->p.whole - 8, v, a) & 2) || (FUN_8004232c(hx, v, a) & 2)) {
            o->d->p.whole = (short)(o->d->p.whole / 90) * 90;
            o->b04 = 1;
            o->step = 0x16;
            o->state = 0;
        }
        break;
    case 3:
        FUN_8001fec0(o);
        o->y.raw += o->velY << 8;
        o->velY += 0x10;
        v = o->y.p.whole - 8;
        hx = o->h->p.whole + 8;
        a = (short)(o->d->p.whole / 90) * 90;
        if ((FUN_8004232c(o->h->p.whole - 8, v, a) & 2) || (FUN_8004232c(hx, v, a) & 2)) {
            o->d->p.whole = (short)(o->d->p.whole / 90) * 90;
            o->b04 = 1;
            o->step = 0x16;
            o->state = 0;
        }
        break;
    }
}
