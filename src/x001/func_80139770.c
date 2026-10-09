// FUNC 80139770 420 X001
// MATCHING 80139770 420
#include "TOBJ.H"

extern unsigned char D_8009C93F[], D_8009C942[];
extern unsigned char D_800A603C[], D_800A603D[], D_800A603E[];
extern void *D_8013FACC[], *D_8013FAB8[];
extern TObj *FUN_8002dcc8(int, int, Fix16 *);
extern void FUN_8001fe6c(TObj *);
extern int FUN_8001fec0(TObj *);

void func_80139770(TObj *o)
{
    TObj *p;

    switch (o->state) {
    case 0:
        o->b68 = 0;
        o->state++;
        break;
    case 1:
        FUN_8001fec0(o);
        if (o->b68 == 0)
            break;
        o->state++;
        D_8009C93F[0] = 1;
        D_8009C942[0] = 1;
        D_800A603C[0] = 5;
        D_800A603D[0] = 0;
        D_800A603E[0] = 0;
        if (o->w98 == 0)
            o->d90 = (int)FUN_8002dcc8(2, 0x7e, &o->a);
        else
            o->d90 = (int)FUN_8002dcc8(2, 0x7f, &o->a);
        o->wac = 0xf;
        o->animFrame = o->b68 & 1;
        o->anim = D_8013FACC[0];
        FUN_8001fe6c(o);
        break;
    case 2:
        FUN_8001fec0(o);
        p = (TObj *)o->d90;
        if (p->b04 != 2)
            break;
        p->b04 = 3;
        D_8009C93F[0] = 0;
        D_8009C942[0] = 0;
        D_800A603C[0] = 1;
        D_800A603D[0] = 0;
        D_800A603E[0] = 0;
        o->wac = 10;
        o->animFrame = o->w98;
        o->anim = D_8013FAB8[0];
        FUN_8001fe6c(o);
        o->state = 0;
        break;
    }
}
