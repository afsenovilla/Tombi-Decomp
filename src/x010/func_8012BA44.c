// FUNC 8012ba44 928 X010
// MATCHING 8012ba44 928
#include "TOBJ.H"
extern unsigned char D_8009C942[];
extern unsigned char D_8009C93F[];
extern unsigned char D_800A603C[], D_800A603D[], D_800A603E[];
extern unsigned char D_8009D07A;
extern void *D_801322EC[];
extern void *D_801322E4[];
extern void *D_801322F4[];
extern void *D_801322E8[];
extern int AnimAdvance(TObj *);
extern void AnimLoadDuration(TObj *);
extern void ObjSetFacingToPlayer(TObj *);
extern int FUN_8002dcc8(int, int, Fix16 *);
extern void FUN_8005a8a8(int, int, int);

void func_8012BA44(TObj *o)
{

    switch (o->state) {
    case 0:
        o->state++;
        ObjSetFacingToPlayer(o);
        D_800A603C[0] = 5;
        D_8009C93F[0] = 1;
        D_8009C942[0] = 1;
        D_800A603D[0] = 0;
        D_800A603E[0] = 0;
        o->wac = 3;
        o->anim = D_801322EC[0];
        AnimLoadDuration(o);
        o->d90 = FUN_8002dcc8(2, 4, &o->a);
    case 1:
        AnimAdvance(o);
        {
        TObj *p = (TObj *)o->d90;
        if (p->b04 == 2) {
            p->b04 = 3;
            o->timer = 0x20;
            o->animFrame = 0;
            o->state++;
        }
        }
        break;
    case 2:
        switch (o->substep) {
        case 0:
            AnimAdvance(o);
            o->a.p.whole++;
            o->y.raw += -0x8000;
            if (--o->timer == -1) {
                o->wac = 1;
                o->substep++;
                o->anim = D_801322E4[0];
                AnimLoadDuration(o);
            }
            break;
        case 1:
            if (AnimAdvance(o)) {
                o->substep = 0;
                o->state++;
            }
            break;
        }
        break;
    case 3:
        o->state++;
        o->d90 = FUN_8002dcc8(2, 5, &o->a);
        break;
    case 4:
        AnimAdvance(o);
        {
        TObj *p = (TObj *)o->d90;
        if (p->b04 == 2) {
            p->b04 = 3;
            o->timer = 0x20;
            o->animFrame = 1;
            o->wac = 5;
            o->state++;
            o->anim = D_801322F4[0];
            AnimLoadDuration(o);
        }
        }
        break;
    case 5:
        switch (o->substep) {
        case 0:
            AnimAdvance(o);
            o->a.p.whole--;
            o->y.raw += 0xa000;
            if (--o->timer == -1)
                o->substep++;
            break;
        case 1:
            o->substep = 0;
            o->wac = 2;
            o->state++;
            o->anim = D_801322E8[0];
            AnimLoadDuration(o);
            break;
        }
        break;
    case 6:
        o->state++;
        o->d90 = FUN_8002dcc8(2, 6, &o->a);
        break;
    case 7:
        AnimAdvance(o);
        {
        TObj *p = (TObj *)o->d90;
        if (p->b04 == 2) {
            p->b04 = 3;
            o->state++;
        }
        }
        break;
    case 8:
        o->state++;
        o->d90 = FUN_8002dcc8(2, 7, &o->a);
        break;
    case 9:
        AnimAdvance(o);
        {
        TObj *p = (TObj *)o->d90;
        if (p->b04 == 2) {
            p->b04 = 3;
            D_8009C93F[0] = 0;
            D_8009C942[0] = 0;
            D_800A603C[0] = 1;
            D_800A603D[0] = 0;
            D_800A603E[0] = 0;
            o->state++;
            FUN_8005a8a8(0x3f, 0, 1);
        }
        }
        break;
    case 10:
        o->step = 0;
        o->state = 0;
        D_8009D07A = 1;
        break;
    }
}
