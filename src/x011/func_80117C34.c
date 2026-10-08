// FUNC 80117c34 708 X011
// MATCHING 80117c34 708
#include "TOBJ.H"
typedef struct { short v[6]; } V6;
typedef struct { void **anims; int a; int b; } AT;
extern AT D_80119C48[];
extern unsigned char D_8009D2B0;
extern short D_800A60EA[];
extern unsigned char D_8009C942[];
extern short D_1F8001C6;
extern unsigned char D_800A603C[], D_800A603D[], D_800A603E[];
extern void AnimLoadDuration(TObj *);
extern int FUN_8002dcc8(int, int, V6 *);
extern void FUN_8005a8a8(int, int, int);

void func_80117C34(TObj *o)
{
    V6 v;
    TObj *p;

    switch (o->state) {
    case 0:
        if (o->b68 == 0) break;
        v = *(V6 *)&o->a;
        D_8009C942[0] = 1;
        D_8009D2B0 = 2;
        D_800A603C[0] = 5;
        D_800A603D[0] = 0;
        D_800A603E[0] = 0;
        o->wbc = o->animFrame;
        o->animFrame = o->b68 & 1;
        o->anim = D_80119C48[o->subtype].anims[2];
        AnimLoadDuration(o);
        o->d90 = FUN_8002dcc8(2, 0, &v);
        o->state++;
        break;
    case 1: {
        TObj *q = (TObj *)o->d90;
        if (q->b04 != 2) break;
        q->b04 = 3;
        v = *(V6 *)&o->a;
        o->d90 = FUN_8002dcc8(2, 1, &v);
        o->state++;
        break;
    }
    case 2:
        p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        FUN_8005a8a8(0x41, 0, 0);
        o->timer = 0x168;
        o->state++;
        break;
    case 3:
        if (--o->timer != 0) break;
        v = *(V6 *)&o->a;
        o->d90 = FUN_8002dcc8(2, 2, &v);
        o->state++;
        break;
    case 4: {
        TObj *q = (TObj *)o->d90;
        if (q->b04 != 2) break;
        q->b04 = 3;
        o->anim = D_80119C48[o->subtype].anims[0];
        AnimLoadDuration(o);
        o->state++;
        break;
    }
    case 5:
        D_8009C942[0] = 0;
        D_800A60EA[0] = 0;
        o->animFrame = o->wbc;
        D_1F8001C6 = 0;
        D_800A603C[0] = 1;
        D_800A603D[0] = 0;
        D_800A603E[0] = 0;
        o->b68 = 0;
        o->step = 0;
        o->state = 0;
        break;
    }
}
