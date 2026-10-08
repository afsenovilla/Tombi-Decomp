// FUNC 801308d8 352 X000
// MATCHING 801308d8 352
#include "TOBJ.H"
extern void FUN_8001fe6c(TObj *o);
extern void FUN_8001fec0(TObj *o);
extern unsigned FUN_8001f9e0(void);
extern void *PTR_8013ad48[];
extern unsigned short TBL_80139240[];
extern Fix16 *DAT_800a607c;
extern Fix16 *DAT_800a6078;
extern unsigned short DAT_800a604e;

void FUN_801308d8(TObj *o)
{
    switch (o->substep) {
    case 0:
        o->timer = 0x8a;
        o->w22 = 1;
        o->wb4 = 0;
        o->substep++;
    case 1:
        if (--o->w22 == 0) {
            o->wac = 6;
            o->anim = PTR_8013ad48[0];
            FUN_8001fe6c(o);
            o->w22 = 0x2e;
        }
        if (--o->timer == 0) {
            o->state = TBL_80139240[FUN_8001f9e0() & 0xf];
            o->substep = 0;
            if (o->d->p.whole == DAT_800a607c->p.whole) {
                int dy = DAT_800a604e - (unsigned short)o->y.p.whole + 0x30;
                int dx = (unsigned short)DAT_800a6078->p.whole - (unsigned short)o->h->p.whole + 0x80;
                if ((unsigned short)dx < 0x100 && (unsigned short)dy < 0xf8) {
                    o->state = 4;
                    o->substep = 0;
                }
            }
        }
    }
    FUN_8001fec0(o);
}
