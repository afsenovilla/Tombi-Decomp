// FUNC 800f328c 476 I0NFO2
// MATCHING 800f328c 476
#include "TOBJ.H"
#include "raw7.h"
extern void func_8001FE94(TObj *o, int n);
extern void func_8001FEC0(TObj *o);
extern void **D_800FEEFC[][3];

static inline void setAnim(TObj *o, int i, short n)
{
    o->anim = D_800FEEFC[o->subtype][0][o->animFrame + 4];
    func_8001FE94(o, n);
}

void func_800F328C(TObj *o)
{
    switch (o->state) {
    case 0:
        o->animFrame |= 1;
        S16(o, 0x7E) = (o->animFrame & 2) ? 0x80 : -0x80;
        setAnim(o, 4, 0);
        o->state++;
    case 1:
        func_8001FEC0(o);
        if (o->b69 - 1 != o->animFrame) {
            o->y.raw += S16(o, 0x7E) << 8;
        }
        o->b69 = 0;
        if ((unsigned short)(o->y.p.whole - S16(o, 0x76)) > S16(o, 0x7A) - S16(o, 0x76)) {
            if (o->h->p.whole < S16(o, 0x74)) o->h->p.whole = S16(o, 0x74);
            if (S16(o, 0x78) < o->h->p.whole) o->h->p.whole = S16(o, 0x78);
            if (o->y.p.whole < S16(o, 0x76)) o->y.p.whole = S16(o, 0x76);
            if (S16(o, 0x7A) < o->y.p.whole) o->y.p.whole = S16(o, 0x7A);
            o->animFrame ^= 2;
            S16(o, 0x7E) = -S16(o, 0x7E);
            setAnim(o, 4, 0);
        }
        break;
    }
}
