// FUNC 80128110 448 X001
// MATCHING 80128110 448
#include "TOBJ.H"

extern unsigned short D_1F80016A[], D_1F80016E;
extern unsigned char D_8013C7E0[], D_8013C7F0[];
extern int Rand(void);

static __inline__ int outside(TObj *o, short *c)
{
    if ((unsigned short)(o->y.p.whole - c[1] + 0xc8) > 0x190) return 2;
    return (unsigned short)(o->h->p.whole - c[0] + 0xf0) > 0x1e0;
}

void func_80128110(TObj *o)
{
    short *c = &o->wb4;
    int r = outside(o, c);
    Fix16 *h;

    if (r) {
        o->w22 = 1;
        h = o->h;
        o->animFrame = h->p.whole > c[0];
        o->step = 1;
        if (r == 2) {
            if (o->y.p.whole > c[1]) o->step = 3;
            else if (c[4] == 1) o->step = 3;
            else o->w22 = 2;
        }
    } else {
        o->w22 = 0;
        o->step = 0;
        h = o->h;
        if ((unsigned short)(D_1F80016A[0] - h->p.whole + 0xf0) < 0x1e1
            && (unsigned short)(D_1F80016E - o->y.p.whole + 0xa0) < 0x141) {
            if (o->ba7) o->step = D_8013C7E0[Rand() & 0xf];
            else o->step = D_8013C7F0[Rand() & 0xf];
            if (o->step == 0) o->step++;
            o->animFrame = (short)D_1F80016A[0] < o->h->p.whole;
        }
    }
}
