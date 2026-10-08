// FUNC 801307a0 312 X000
#include "TOBJ.H"
extern void FUN_8001fe6c(TObj *o);
extern void FUN_8001fec0(TObj *o);
extern unsigned FUN_8001f9e0(void);
extern void *PTR_8013ad30;
extern unsigned short TBL_80139240[];
extern Fix16 *DAT_800a607c;
extern Fix16 *DAT_800a6078;
extern unsigned short DAT_800a604e;

void FUN_801307a0(TObj *o)
{
    switch (o->substep) {
    case 0:
        o->timer = 0x70;
        o->wb4 = 0;
        o->wac = 0;
        o->anim = PTR_8013ad30;
        FUN_8001fe6c(o);
        o->substep++;
    case 1:
        if (--o->timer == 0) {
            o->state = TBL_80139240[FUN_8001f9e0() & 0xf];
            o->substep = 0;
            if (o->d->p.whole == DAT_800a607c->p.whole) {
                if ((unsigned short)(DAT_800a6078->p.whole - o->h->p.whole + 0x80) < 0x100) {
                    if ((unsigned short)(DAT_800a604e - o->y.p.whole + 0x30) < 0xf8) {
                        o->state = 4;
                        o->substep = 0;
                    }
                }
            }
        }
    }
    FUN_8001fec0(o);
}
