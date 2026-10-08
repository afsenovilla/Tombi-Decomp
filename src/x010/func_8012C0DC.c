// FUNC 8012c0dc 324 X010
// MATCHING 8012c0dc 324
#include "TOBJ.H"

extern unsigned char D_800A603C[], D_800A603D[], D_800A603E[], D_8009C93F[], D_8009C942[];
extern void *D_801322F4[];
extern void ObjSetFacingToPlayer(TObj *);
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern TObj *FUN_8002dcc8(int, int, void *);

void func_8012C0DC(TObj *o)
{
    TObj *p;

    switch (o->state) {
    case 0:
        o->state++;
        ObjSetFacingToPlayer(o);
        D_800A603C[0] = 5;
        D_8009C93F[0] = 1;
        D_8009C942[0] = 1;
        D_800A603D[0] = 0;
        D_800A603E[0] = 0;
        o->wac = 5;
        o->anim = D_801322F4[0];
        AnimLoadDuration(o);
        o->d90 = (int)FUN_8002dcc8(2, 0xa, &o->a);
        break;
    case 1:
        AnimAdvance(o);
        p = (TObj *)o->d90;
        if (p->b04 == 2) {
            p->b04 = 3;
            o->state++;
            D_8009C93F[0] = 0;
            D_8009C942[0] = 0;
            D_800A603C[0] = 1;
            D_800A603D[0] = 0;
            D_800A603E[0] = 0;
        }
        break;
    case 2:
        o->step = 0;
        o->state = 0;
        break;
    }
}
