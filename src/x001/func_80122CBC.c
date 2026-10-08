// FUNC 80122cbc 664 X001
// MATCHING 80122cbc 664
#include "TOBJ.H"

extern TObj *D_8009F0EC;
extern int rcos(int);
extern int rsin(int);

static __inline__ int orbit(TObj *o, int a)
{
    o->wb8 = (rcos(a) * (D_8009F0EC->box0 - 12)) >> 12;
    o->wba = -(rsin(a) * (D_8009F0EC->box0 - 12)) >> 12;
    return a;
}

int func_80122CBC(TObj *o)
{
    TObj *g = D_8009F0EC;
    int a = g->d8c;
    short b = (a + 0x800) & 0xfff;
    int r = 0;

    switch (g->w7a) {
    case 0:
        if (o->animFrame & 1) {
            a &= 0xfff;
            orbit(o, a);
            if (b < 0x600) {
                o->b69 = 1;
                r = 1;
            }
        } else {
            if (orbit(o, b) > 0xa00) {
                o->b69 = 1;
                r = 1;
            }
        }
        break;
    case 1:
        a &= 0xfff;
        orbit(o, a);
        break;
    case 2:
        a += 0x400;
        a &= 0xfff;
        orbit(o, a);
        if (b > 0xe00) {
            o->b69 = 1;
            r = 1;
        }
        break;
    }
    D_8009F0EC->b69 = 1;
    return r;
}
