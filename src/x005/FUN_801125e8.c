// FUNC 801125e8 636 X005
// MATCHING 801125e8 636
#include "TOBJ.H"
extern char DAT_80115d28[];
extern void **DAT_80115a08[];
extern void **PTR_PTR_80115948[];
extern int DAT_8009c960[];
extern unsigned short DAT_8009c960h[];
extern int FUN_800202b4(TObj *o);
extern int FUN_80040278(TObj *o, int x, int y);
extern void FUN_8003ecb0(char *d, Fix16 *pos, int a, int b, TObj *o);
extern void FUN_8001fe6c(TObj *o);
extern void FUN_80020490(TObj *o);
extern void FUN_800eb8f0(TObj *o, int x, int y, int z);

void FUN_801125e8(TObj *o)
{
    void **p;
    FUN_800202b4(o);
    switch (o->step) {
    case 0:
        o->velV += 0x20;
        if (o->velV > 0x400)
            o->velV = 0x400;
        o->y.raw += o->velV << 8;
        FUN_80040278(o, o->h->p.whole, (short)(o->y.p.whole + 0x10));
        break;
    case 1:
        FUN_8003ecb0(DAT_80115d28, &o->a, -0x40, -0x400, o);
        FUN_8003ecb0(DAT_80115d28 + 6, &o->a, 0x40, -0x400, o);
        o->step = 3;
        o->timer = 10;
        o->wac = 1;
        if (DAT_8009c960[0] == 0x30009)
            o->anim = DAT_80115a08[o->b0c & 0x7f][1];
        else
            o->anim = PTR_PTR_80115948[DAT_8009c960h[0] * 4 + (o->b0c & 0x7f)][1];
        FUN_8001fe6c(o);
        break;
    case 2:
        o->active = 1;
        o->b04 = 1;
        o->step = 0;
        break;
    case 3:
        if (--o->timer == -1) {
            o->active = 3;
            o->velV = 0;
            o->step++;
        }
        break;
    case 4:
        o->velV += 0x20;
        if (o->velV > 0x400)
            o->velV = 0x400;
        o->y.raw += o->velV << 8;
        FUN_80040278(o, o->h->p.whole, (short)(o->y.p.whole + 0x10));
        FUN_80020490(o);
        break;
    case 7:
        FUN_800eb8f0(o, o->a.p.whole, o->y.p.whole, o->b.p.whole);
        o->b04++;
        break;
    }
}
