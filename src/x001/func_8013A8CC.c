// FUNC 8013a8cc 492 X001
// MATCHING 8013a8cc 492
#include "TOBJ.H"
extern TObj D_800A6038;
extern TObj *D_800A60C8;
extern unsigned char D_8009C93A, D_8009C93F, D_8009C93E, D_8009C942, D_8009CEDE;
extern unsigned char D_800A60F8, D_800A603C, D_800A603D, D_800A603E;
extern TObj *FUN_8002dc50(int, int, int, int);
extern void FUN_800eeae4(TObj *, int, int);
extern void FUN_8003fd78(TObj *, int, int);

void func_8013A8CC(TObj *o)
{
    TObj *e = *(TObj **)&o->category;
    TObj *p;

    switch (o->step) {
    case 0:
        D_8009C942 = 1;
        D_8009C93F = 1;
        D_800A6038.animFrame = 0;
        D_800A6038.b04 = 5;
        D_800A6038.step = 100;
        D_800A6038.state = 0;
        FUN_800eeae4(&D_800A6038, 2, 0);
        o->step++;
    case 1:
        D_800A6038.h->raw += 0x10000;
        D_800A6038.y.raw += 0x50000;
        FUN_8003fd78(&D_800A6038, 0, 0);
        if (D_800A6038.h->p.whole >= 0x279) {
            e->animFrame = 0;
            D_800A60C8 = FUN_8002dc50(2, 0x19, 0x80, 0x6c);
            FUN_800eeae4(&D_800A6038, 0, 0);
            o->step++;
        }
        break;
    case 2:
        p = D_800A60C8;
        if (p->b04 == 2) {
            p->b04 = 3;
            D_8009C93A = 1;
            D_8009C93F = 0;
            D_8009C93E = 0;
            D_8009C942 = 0;
            D_8009CEDE = 1;
            D_800A60F8 = 0;
            D_800A603C = 1;
            D_800A603D = 0;
            D_800A603E = 0;
            e->subtype = 1;
            o->step = 0;
            o->state = 0;
        }
        break;
    }
}
