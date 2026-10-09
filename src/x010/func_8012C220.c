// FUNC 8012c220 908 X010
// MATCHING 8012c220 908
#include "TOBJ.H"

extern unsigned char D_8009C93F[], D_8009C942[], D_8009D07A;
extern unsigned char D_800A603C[], D_800A603D[], D_800A603E[];
extern void *D_801322F4[], *D_801322E4[];
extern void ObjSetFacingToPlayer(TObj *);
extern void AnimLoadDuration(TObj *);
extern int AnimAdvance(TObj *);
extern TObj *FUN_8002dcc8(int, int, void *);
extern void FUN_8005a9a4(int, int);

void func_8012C220(TObj *o)
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
        o->wac = 5;
        o->anim = D_801322F4[0];
        AnimLoadDuration(o);
        o->d90 = (int)FUN_8002dcc8(2, 0x26, &o->a);
    case 1:
        AnimAdvance(o);
        if (((TObj *)o->d90)->b04 == 2) {
            ((TObj *)o->d90)->b04 = 3;
            o->timer = 0x20;
            o->animFrame = 0;
            o->state++;
        }
        break;
    case 2:
        switch (o->substep) {
        case 0:
            AnimAdvance(o);
            o->a.p.whole++;
            o->y.raw += -0x8000;
            if (--o->timer == -1) {
                o->timer = 0x20;
                goto next;
            }
            break;
        case 1:
            AnimAdvance(o);
            if (--o->timer == -1) {
                o->timer = 0x20;
                o->wac = 5;
                o->substep++;
                o->animFrame = 1 - o->animFrame;
                o->anim = D_801322F4[0];
                AnimLoadDuration(o);
            }
            break;
        case 2:
            AnimAdvance(o);
            o->a.p.whole--;
            o->y.raw += 0xa000;
            if (--o->timer == -1) {
            next:
                o->wac = 1;
                o->substep++;
                o->anim = D_801322E4[0];
                AnimLoadDuration(o);
            }
            break;
        case 3:
            if (AnimAdvance(o)) {
                o->wac = 5;
                o->anim = D_801322F4[0];
                AnimLoadDuration(o);
                o->substep = 0;
                o->state++;
            }
            break;
        }
        break;
    case 3:
        o->state++;
        o->d90 = (int)FUN_8002dcc8(2, 0x27, &o->a);
        break;
    case 4:
        AnimAdvance(o);
        if (((TObj *)o->d90)->b04 == 2) {
            ((TObj *)o->d90)->b04 = 3;
            o->state++;
        }
        break;
    case 5:
        o->state++;
        o->d90 = (int)FUN_8002dcc8(2, 0x28, &o->a);
        break;
    case 6:
        AnimAdvance(o);
        if (((TObj *)o->d90)->b04 == 2) {
            ((TObj *)o->d90)->b04 = 3;
            D_8009C93F[0] = 0;
            D_8009C942[0] = 0;
            D_800A603C[0] = 1;
            D_800A603D[0] = 0;
            D_800A603E[0] = 0;
            o->state++;
            FUN_8005a9a4(0x3e, 1);
        }
        break;
    case 7:
        o->step = 0;
        o->state = 0;
        D_8009D07A = 3;
        break;
    }
}
