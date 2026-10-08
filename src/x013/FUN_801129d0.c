// FUNC 801129d0 528 X013
// MATCHING 801129d0 528
#include "TOBJ.H"
extern unsigned short DAT_8009c960[];
extern unsigned short DAT_8009c960b;
extern int DAT_8009c960_w[];
extern unsigned char DAT_8009d0aa;
extern unsigned char DAT_8009d0c8;
extern unsigned char DAT_8009c93b;
extern void **DAT_80115a08[];
extern void **DAT_80115948[];
extern void FUN_8001fe6c(TObj *);
extern short FUN_80040278(TObj *, short, short);
extern void FUN_800202b4(TObj *);

#define SETANIM(o) \
    o->wac = 1; \
    if (DAT_8009c960_w[0] == 0x30009) o->anim = DAT_80115a08[o->b0c & 0x7f][1]; \
    else o->anim = DAT_80115948[DAT_8009c960b * 4 + (o->b0c & 0x7f)][1]; \
    FUN_8001fe6c(o);

void FUN_801129d0(TObj *o)
{
    switch (o->step) {
    case 0:
        o->active = 4;
        o->y.p.whole += 4;
        if (DAT_8009c960[0] == 0 && DAT_8009d0aa != 0) {
            SETANIM(o)
        }
        if ((DAT_8009c960[0] == 4 || DAT_8009c960[0] == 0xc) && DAT_8009d0c8 != 0) {
            SETANIM(o)
        }
        if (FUN_80040278(o, o->h->p.whole, o->y.p.whole + 0x10)) o->step++;
    case 1:
        if (DAT_8009c93b) {
            o->b04 = 2;
            o->step = 0;
        }
        FUN_800202b4(o);
        break;
    }
}
