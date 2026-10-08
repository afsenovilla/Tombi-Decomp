// FUNC 8013b0f4 352 X001
// MATCHING 8013b0f4 352
#include "TOBJ.H"
extern TObj D_800A6038;
extern unsigned short D_800A6066;
extern TObj *D_800A60C8;
extern unsigned char D_8009C93F, D_8009C93E, D_8009C942, D_8009CE59, D_8009CEDE;
extern unsigned char D_800A60F8, D_800A603C, D_800A603D, D_800A603E;
extern char D_80010748[];
extern void AnimLoadDuration(TObj *);
extern TObj *FUN_8002dc50(int, int, int, int);
extern void FUN_800eeae4(TObj *, int, int);

void func_8013B0F4(TObj *o)
{
    TObj *e = *(TObj **)&o->category;
    TObj *p;

    switch (o->step) {
    case 0:
        D_8009C93F = 1;
        D_8009C93E = 1;
        D_8009C942 = 1;
        D_800A6038.anim = D_80010748;
        AnimLoadDuration(&D_800A6038);
        e->animFrame = 0;
        D_800A6066 = 0;
        D_800A60C8 = FUN_8002dc50(2, 0x19, 0x80, 0x6c);
        FUN_800eeae4(&D_800A6038, 0, 0);
        o->step++;
        break;
    case 1:
        p = D_800A60C8;
        if (p->b04 == 2) {
            p->b04 = 3;
            D_8009C93F = 0;
            D_8009C93E = 0;
            D_8009C942 = 0;
            D_8009CE59 = 1;
            D_8009CEDE = 1;
            D_800A60F8 = 0;
            D_800A603C = 1;
            D_800A603D = 0;
            D_800A603E = 0;
            e->subtype = 1;
            e->step = 1;
            e->state = 0;
            e->animFrame = 0;
            o->step = 0;
            o->state = 0;
        }
        break;
    }
}
