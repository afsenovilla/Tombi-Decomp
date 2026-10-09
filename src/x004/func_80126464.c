// FUNC 80126464 548 X004
// MATCHING 80126464 548
#include "TOBJ.H"

extern void *D_801345BC[], *D_801345D0[];
extern unsigned short D_80131128[];
extern Fix16 *D_800A6078, *D_800A607C;
extern unsigned short D_800A604E;
extern void AnimLoadDuration(TObj *);
extern int Rand(void);

void func_80126464(TObj *o)
{
    int r;
    short t;
    short f;
    unsigned short dy;

    switch (o->substep) {
    case 0:
        if (*(unsigned short *)&o->wb4)
            t = 0xa0;
        else
            t = 0xc8;
        o->timer = t;
        o->w22 = 1;
        o->wb4 = 0;
        o->substep++;
    case 1:
        if (--o->w22 == 0) {
            if ((unsigned short)o->wb4 == 0) {
                o->wac = 2;
                o->anim = D_801345BC[0];
                AnimLoadDuration(o);
                o->w22 = 0x32;
            } else {
                o->wac = 7;
                o->anim = D_801345D0[0];
                AnimLoadDuration(o);
                o->w22 = 0x28;
            }
        }
        if (--o->timer) break;
        if (o->d->p.whole != D_800A607C->p.whole) r = 0; else {
            f = 0;
            if (((unsigned short)(D_800A6078->p.whole - o->h->p.whole + 0x40) < 0x80 && (Rand() & 0xf) < 0xc)
                || ((unsigned short)(D_800A6078->p.whole - o->h->p.whole + 0x80) < 0x100 && (Rand() & 0xf) < 6))
                f = 1;
            dy = D_800A604E - o->y.p.whole + 0x30;
            r = f == 1 && dy < 0xf8;
        }
        if (r) {
            o->step = 1;
            o->state = 0;
            o->substep = 0;
        } else {
            o->state = D_80131128[Rand() & 0xf];
            o->substep = 0;
        }
        break;
    }
}
