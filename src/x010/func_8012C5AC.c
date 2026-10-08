// FUNC 8012c5ac 320 X010
// MATCHING 8012c5ac 320
#include "TOBJ.H"

extern unsigned char D_800A603Cx[], D_800A603Dx[], D_800A603Ex[];
extern unsigned char D_8009C93Fx[], D_8009C942x[];
#define D_800A603C D_800A603Cx[0]
#define D_800A603D D_800A603Dx[0]
#define D_800A603E D_800A603Ex[0]
#define D_8009C93F D_8009C93Fx[0]
#define D_8009C942 D_8009C942x[0]
extern void *D_801322F4[];
extern void ObjSetFacingToPlayer(TObj *);
extern void AnimLoadDuration(TObj *);
extern void AnimAdvance(TObj *);
extern int FUN_8002dcc8(int, int, void *);

void func_8012C5AC(TObj *o)
{
    TObj *p;

    switch (o->state) {
    case 0:
        o->state++;
        ObjSetFacingToPlayer(o);
        D_800A603C = 5;
        D_8009C93F = 1;
        D_8009C942 = 1;
        D_800A603D = 0;
        D_800A603E = 0;
        o->wac = 5;
        o->anim = D_801322F4[0];
        AnimLoadDuration(o);
        o->d90 = FUN_8002dcc8(2, 0x28, &o->a);
        break;
    case 1:
        AnimAdvance(o);
        p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        D_8009C93F = 0;
        D_8009C942 = 0;
        D_800A603C = 1;
        D_800A603D = 0;
        D_800A603E = 0;
        o->state++;
        break;
    case 2:
        o->step = 0;
        o->state = 0;
        break;
    }
}
