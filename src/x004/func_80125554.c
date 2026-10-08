// FUNC 80125554 340 X004
// MATCHING 80125554 340
#include "TOBJ.H"
extern void *D_801345B4[0];
extern unsigned short D_80131128[];
extern Fix16 *D_800A6078, *D_800A607C;
extern unsigned short D_800A604E;
extern void FUN_8001fe6c(TObj *);
extern int FUN_8001f9e0(void);

void func_80125554(TObj *o)
{
    unsigned short dy;

    switch (o->substep) {
    case 0:
        o->timer = 0x8a;
        o->w22 = 1;
        o->wb4 = 0;
        o->substep++;
    case 1:
        if (--o->w22 == 0) {
            o->wac = 0;
            o->anim = D_801345B4[0];
            FUN_8001fe6c(o);
            o->w22 = 0x2e;
        }
        if (--o->timer == 0) {
            o->state = D_80131128[FUN_8001f9e0() & 0xf];
            o->substep = 0;
            if (o->d->p.whole == D_800A607C->p.whole) {
                dy = D_800A604E - o->y.p.whole;
                dy += 0x30;
                if ((unsigned short)(D_800A6078->p.whole - o->h->p.whole + 0x80) < 0x100
                    && dy < 0xf8) {
                    o->state = 4;
                    o->substep = 0;
                }
            }
        }
        break;
    }
}
