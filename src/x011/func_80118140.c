// FUNC 80118140 488 X011
// MATCHING 80118140 488
#include "TOBJ.H"
typedef struct { char c[12]; } B12;
typedef struct { void **p; int x, y; } AT3;

extern unsigned char D_8009C942A[], D_8009D2B0;
extern unsigned char D_800A603C[], D_800A603D[], D_800A603E[];
extern short D_800A60EA[], D_1F8001C6;
extern AT3 D_80119C48[];
extern TObj *FUN_8002dcc8(int, int, B12 *);
extern void FUN_8001fe6c(TObj *);

void func_80118140(TObj *o)
{
    B12 v;
    TObj *p;

    switch (o->state) {
    case 0:
        if (o->b68 == 0)
            break;
        D_8009C942A[0] = 1;
        D_8009D2B0 = 2;
        D_800A603C[0] = 5;
        D_800A603D[0] = 0;
        D_800A603E[0] = 0;
        o->wbc = o->animFrame;
        o->animFrame = o->b68 & 1;
        v = *(B12 *)&o->a;
        o->anim = D_80119C48[o->subtype].p[2];
        FUN_8001fe6c(o);
        o->d90 = (int)FUN_8002dcc8(2, 0xc, &v);
        o->state++;
        break;
    case 1:
        p = (TObj *)o->d90;
        if (p->b04 != 2)
            break;
        p->b04 = 3;
        o->anim = D_80119C48[o->subtype].p[0];
        FUN_8001fe6c(o);
        o->state++;
        break;
    case 2:
        D_8009C942A[0] = 0;
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
