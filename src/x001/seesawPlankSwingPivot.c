// FUNC 80122cbc 664 X001
// MATCHING 80122cbc 664
#include "TOBJ.H"

extern TObj *GRABBED_OBJECT;
extern int rcos(int);
extern int rsin(int);

static __inline__ int orbit(TObj *o, int a)
{
    o->wb8 = (rcos(a) * (GRABBED_OBJECT->box0 - 12)) >> 12;
    o->wba = -(rsin(a) * (GRABBED_OBJECT->box0 - 12)) >> 12;
    return a;
}

int seesawPlankSwingPivot(TObj *o)
{
    TObj *g = GRABBED_OBJECT;
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
    GRABBED_OBJECT->b69 = 1;
    return r;
}
