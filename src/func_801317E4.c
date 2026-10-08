// FUNC 801317e4 492 X000
// MATCHING 801317e4 492
#include "TOBJ.H"
extern void *D_8013AD64[];
extern Fix16 *D_800A607C;
extern Fix16 *D_800A6078;
extern unsigned short D_800A604E;
extern unsigned short D_80139240[];
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern int Rand(void);

static __inline__ int check(TObj *o)
{
    short r;
    unsigned short d;
    int x;
    if (o->d->p.whole != D_800A607C->p.whole) {
        x = 0;
    } else {
        r = 0;
        if ((unsigned short)(D_800A6078->p.whole - o->h->p.whole + 0x40) < 0x80 && (Rand() & 0xf) < 12) {
            r = 1;
        } else if ((unsigned short)(D_800A6078->p.whole - o->h->p.whole + 0x80) < 0x100 && (Rand() & 0xf) < 6) {
            r = 1;
        }
        x = 0;
        d = D_800A604E - o->y.p.whole + 0x30;
        if (r == 1)
            x = d < 0xf8;
        else
            x = 0;
    }
    return x;
}

void func_801317E4(TObj *o)
{
    switch (o->substep) {
    case 0:
        o->timer = 0xa8;
        o->w22 = 1;
        o->wb4 = 0;
        o->substep++;
    case 1:
        if (--o->w22 == 0) {
            o->wac = 13;
            o->anim = D_8013AD64[0];
            AnimLoadDuration(o);
            o->w22 = 0x54;
        }
        if (--o->timer == 0) {
            if (check(o)) {
                o->step = 1;
                o->state = 0;
                o->substep = 0;
            } else {
                o->state = D_80139240[Rand() & 0xf];
                o->substep = 0;
            }
        }
        break;
    }
    AnimAdvance(o);
}
