// FUNC 801189f8 488 X011
// MATCHING 801189f8 488
#include "TOBJ.H"

typedef struct V { char c[12]; } V;
typedef struct { void **p; int pad[2]; } E12;
extern E12 D_80119C48[];
extern unsigned char D_8009C942[], D_8009D2B0[];
extern unsigned char D_800A603C[], D_800A603D[], D_800A603E[];
extern short D_800A60EA[];
extern short D_1F8001C6;
extern TObj *FUN_8002dcc8(int, int, V *);
extern void AnimLoadDuration(TObj *);

void func_801189F8(TObj *o)
{
    TObj *p;
    V v;

    switch (o->state) {
    case 0:
        if (o->b68 == 0)
            break;
        D_8009C942[0] = 1;
        D_8009D2B0[0] = 2;
        D_800A603C[0] = 5;
        D_800A603D[0] = 0;
        D_800A603E[0] = 0;
        o->wbc = o->animFrame;
        o->animFrame = o->b68 & 1;
        v = *(V *)&o->a;
        o->anim = D_80119C48[o->subtype].p[2];
        AnimLoadDuration(o);
        o->d90 = (int)FUN_8002dcc8(2, 0x11, &v);
        o->state++;
        break;
    case 1:
        p = (TObj *)o->d90;
        if (p->b04 != 2)
            break;
        p->b04 = 3;
        o->anim = D_80119C48[o->subtype].p[0];
        AnimLoadDuration(o);
        o->state++;
        break;
    case 2:
        D_8009C942[0] = 0;
        D_800A60EA[0] = 0;
        D_1F8001C6 = 0;
        o->animFrame = o->wbc;
        D_800A603C[0] = 1;
        D_800A603D[0] = 0;
        D_800A603E[0] = 0;
        o->b68 = 0;
        o->step = 0;
        o->state = 0;
        break;
    }
}
