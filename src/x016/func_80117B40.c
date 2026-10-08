// FUNC 80117b40 500 X016
// MATCHING 80117b40 500
#include "TOBJ.H"
typedef struct { short v[6]; } V6;
typedef struct { void **anims; int a; int b; } AT;
extern AT D_80118D1C[];
extern unsigned char D_8009D2B0;
extern short D_800A60EA[];
extern unsigned char D_8009C942[], D_8009C93F[];
extern short D_1F8001C6;
extern unsigned char D_800A603C[], D_800A603D[], D_800A603E[];
extern void AnimLoadDuration(TObj *);
extern void AnimAdvance(TObj *);
extern int FUN_8002dcc8(int, int, V6 *);

void func_80117B40(TObj *o)
{
    V6 v;
    TObj *q;

    switch (o->state) {
    case 0:
        if (o->b68 == 0) break;
        v = *(V6 *)&o->a;
        D_800A603C[0] = 5;
        D_8009C942[0] = 1;
        D_8009C93F[0] = 1;
        D_800A603D[0] = 0;
        D_800A603E[0] = 0;
        D_8009D2B0 = 2;
        o->wbc = o->animFrame;
        o->anim = D_80118D1C[o->subtype].anims[2];
        AnimLoadDuration(o);
        o->animFrame = o->b68 & 1;
        o->d90 = FUN_8002dcc8(2, 0xc, &v);
        o->state = 1;
        break;
    case 1:
        AnimAdvance(o);
        q = (TObj *)o->d90;
        if (q->b04 != 2) break;
        q->b04 = 3;
        o->anim = D_80118D1C[o->subtype].anims[0];
        AnimLoadDuration(o);
        o->state = 9;
        break;
    case 9:
        D_8009C942[0] = 0;
        D_8009C93F[0] = 0;
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
