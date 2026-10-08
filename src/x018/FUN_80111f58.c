// FUNC 80111f58 684 X018
// MATCHING 80111f58 684
#include "TOBJ.H"
extern int FUN_800202b4(TObj *);
extern void FUN_80111dc8(TObj *);
extern short DAT_1f80016a;
extern short DAT_1f80016ab[];
extern short DAT_8007a1f0[];
extern short DAT_8007a5f0[];

void FUN_80111f58(TObj *o)
{
    TObj *p = (TObj *)o->d90;
    unsigned char d;
    int w;
    short v;

    o->h->p.whole += p->h->p.whole - o->wb4;
    o->y.p.whole += p->y.p.whole - o->wb6;
    o->wb4 = p->h->p.whole;
    o->wb6 = p->y.p.whole;
    if (FUN_800202b4(o) == 0) return;
    switch (o->step) {
    case 0:
        if (o->visible) o->step++;
        break;
    case 1:
        o->h->p.whole = p->h->p.whole + 0x18;
        o->y.p.whole = p->y.p.whole - 0x10;
        if (DAT_1f80016a > o->h->p.whole) o->step++;
        break;
    case 2:
        o->h->p.whole = p->h->p.whole + 0x18;
        o->y.p.whole = p->y.p.whole - 0x10;
        if (DAT_1f80016a < o->h->p.whole + 0x18 && p->d8c > 0x150) break;
        o->velY = 0x100;
        o->b69 = 0;
        o->velX = 0x80;
        o->step++;
        o->w78 = ((o->d38 + 0x800) & 0xfff) >> 4;
        if (o->w78 > 0x80) o->w78 = 0x80;
        w = (unsigned short)o->w78;
        d = o->d8c - 0x80 - w;
        if (d != 0) {
            if (d < 0x80) o->d8c--;
            else o->d8c++;
            o->d8c = *(unsigned char *)&o->d8c;
        }
        v = -o->velX;
        o->velV = v * DAT_8007a1f0[o->w78] >> 12;
        o->velH = v * DAT_8007a5f0[o->w78] >> 12;
    case 3:
        FUN_80111dc8(o);
        if (o->y.p.whole >= -0xae) {
            o->timer = 10;
            o->active = 2;
            o->b04 = 2;
            o->step = 7;
        }
        break;
    }
}
