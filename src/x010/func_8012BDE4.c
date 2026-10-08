// FUNC 8012bde4 320 X010
// MATCHING 8012bde4 320
#include "TOBJ.H"

extern TObj D_800A6038;
extern unsigned char D_8009C93F[], D_8009C942[];
extern void *D_801322F4[];
extern void ObjSetFacingToPlayer(TObj *);
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern int FUN_8002dcc8(int, int, void *);

void func_8012BDE4(TObj *o)
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
        o->d90 = FUN_8002dcc8(2, 7, &o->a);
        break;
    case 1:
        AnimAdvance(o);
        q = (TObj *)o->d90;
        if (q->b04 == 2) {
            q->b04 = 3;
            D_8009C93F[0] = 0;
            D_8009C942[0] = 0;
            D_800A6038.b04 = 1;
            D_800A6038.step = 0;
            D_800A6038.state = 0;
            o->state++;
        }
        break;
    case 2:
        o->step = 0;
        o->state = 0;
        break;
    }
}
