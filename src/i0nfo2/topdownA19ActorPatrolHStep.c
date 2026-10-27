// FUNC 800f58f4 544 I0NFO2
// MATCHING 800f58f4 544
#include "TOBJ.H"
#include "raw7.h"
extern void func_8001FE94(TObj *o, int n);
extern void func_8001FEC0(TObj *o);
extern void **TOPDOWN_A19_ACTOR_ANIMS[][3];

static inline void setAnim(TObj *o, int i, short n)
{
    short f = o->animFrame + i;
    if (f != U16(o, 0xD2)) {
        o->anim = TOPDOWN_A19_ACTOR_ANIMS[o->subtype][0][f];
        func_8001FE94(o, n);
        S16(o, 0xD2) = f;
    }
}

void topdownA19ActorPatrolHStep(TObj *o)
{
    switch (o->state) {
    case 0:
        o->animFrame &= 0xE;
        S16(o, 0x7C) = (o->animFrame & 2) ? -0x80 : 0x80;
        setAnim(o, 4, 0);
        o->state++;
    case 1:
        func_8001FEC0(o);
        if (o->b69 - 1 != o->animFrame) {
            o->h->raw += S16(o, 0x7C) << 8;
        }
        o->b69 = 0;
        if ((unsigned short)(o->h->p.whole - S16(o, 0x74)) > S16(o, 0x78) - S16(o, 0x74)) {
            if (o->h->p.whole < S16(o, 0x74)) o->h->p.whole = S16(o, 0x74);
            if (S16(o, 0x78) < o->h->p.whole) o->h->p.whole = S16(o, 0x78);
            if (o->y.p.whole < S16(o, 0x76)) o->y.p.whole = S16(o, 0x76);
            if (S16(o, 0x7A) < o->y.p.whole) o->y.p.whole = S16(o, 0x7A);
            o->animFrame ^= 2;
            S16(o, 0x7C) = -S16(o, 0x7C);
            setAnim(o, 4, 0);
        }
        break;
    }
}
