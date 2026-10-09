// FUNC 8012bf24 440 X010
// MATCHING 8012bf24 440
#include "TOBJ.H"

extern TObj D_800A6038;
extern unsigned char D_8009C93F[], D_8009C942[];
extern unsigned char D_8009D07A;
extern void *D_801322F4[];
extern void ObjSetFacingToPlayer(TObj *);
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern int FUN_8002dcc8(int, int, void *);
extern void FUN_8005a8a8(int, int, int);

void func_8012BF24(TObj *o)
{
    TObj *q;

    switch (o->state) {
    case 0:
        o->state++;
        ObjSetFacingToPlayer(o);
        D_800A6038.b04 = 5;
        D_8009C93F[0] = 1;
        D_8009C942[0] = 1;
        D_800A6038.step = 0;
        D_800A6038.state = 0;
        o->wac = 5;
        o->anim = D_801322F4[0];
        AnimLoadDuration(o);
        o->d90 = FUN_8002dcc8(2, 8, &o->a);
        break;
    case 3:
        o->state++;
        o->d90 = FUN_8002dcc8(2, 9, &o->a);
        break;
    case 1:
    case 4:
        AnimAdvance(o);
        {
            TObj *p = (TObj *)o->d90;
            if (p->b04 == 2) {
                p->b04 = 3;
                o->state++;
            }
        }
        break;
    case 2:
    case 5:
        o->state++;
        break;
    case 6:
        o->state++;
        o->d90 = FUN_8002dcc8(2, 10, &o->a);
        break;
    case 7:
        AnimAdvance(o);
        q = (TObj *)o->d90;
        if (q->b04 == 2) {
            q->b04 = 3;
            o->state++;
            D_8009C93F[0] = 0;
            D_8009C942[0] = 0;
            D_800A6038.b04 = 1;
            D_800A6038.step = 0;
            D_800A6038.state = 0;
            FUN_8005a8a8(0x3e, 0, 1);
        }
        break;
    case 8:
        o->step = 0;
        o->state = 0;
        D_8009D07A = 2;
        break;
    }
}
