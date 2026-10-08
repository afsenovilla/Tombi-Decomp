// FUNC 80125420 308 X004
// MATCHING 80125420 308
#include "TOBJ.H"

extern void *D_801345DC[];
extern unsigned short D_80131128[];
extern Fix16 *D_800A6078, *D_800A607C;
extern unsigned short D_800A604E;
extern void AnimLoadDuration(TObj *);
extern int Rand(void);

void func_80125420(TObj *o)
{
    unsigned short dy;

    switch (o->substep) {
    case 0:
        o->timer = 0x70;
        o->wb4 = 0;
        o->wac = 0xa;
        o->anim = D_801345DC[0];
        AnimLoadDuration(o);
        o->substep++;
    case 1:
        if (--o->timer) break;
        o->state = D_80131128[Rand() & 0xf];
        o->substep = 0;
        if (o->d->p.whole == D_800A607C->p.whole) {
            dy = D_800A604E - o->y.p.whole + 0x30;
            if ((unsigned short)(D_800A6078->p.whole - o->h->p.whole + 0x80) < 0x100 && dy < 0xf8) {
                o->state = 4;
                o->substep = 0;
            }
        }
        break;
    }
}
