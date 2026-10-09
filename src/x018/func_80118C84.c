// FUNC 80118c84 656 X018
// MATCHING 80118c84 656
#include "TOBJ.H"

typedef struct V { char c[12]; } V;
typedef struct { void **p; int pad[2]; } E12;
typedef struct { TObj o; char pc0[0x12]; unsigned short wd2; } TX;
#define X(o) ((TX *)(o))
extern E12 D_8011A564[];
extern unsigned char D_8009C942[], D_8009C93F[], D_8009D2B0[];
extern unsigned char D_800A603C[], D_800A603D[], D_800A603E[];
extern short D_800A60EA[];
extern short D_1F8001C6;
extern TObj *FUN_8002dcc8(int, int, V *);
extern void FUN_8001fe94(TObj *, int);
extern void AnimAdvance(TObj *);

static __inline__ void setanim(TObj *o, short n)
{
    if (X(o)->wd2 != n) {
        o->anim = D_8011A564[o->subtype].p[n];
        FUN_8001fe94(o, 0);
        X(o)->wd2 = n;
    }
}

void func_80118C84(TObj *o)
{
    TObj *p;
    V v;

    switch (o->state) {
    case 0:
        if (o->b68 == 0)
            break;
        v = *(V *)&o->a;
        D_8009C942[0] = 1;
        D_8009C93F[0] = 1;
        D_8009D2B0[0] = 2;
        D_800A603C[0] = 5;
        D_800A603D[0] = 0;
        D_800A603E[0] = 0;
        o->wbc = o->animFrame;
        o->animFrame = o->b68 & 1;
        setanim(o, 4);
        v = *(V *)&o->a;
        o->d90 = (int)FUN_8002dcc8(3, 0xa, &v);
        o->state++;
        break;
    case 1:
        AnimAdvance(o);
        p = (TObj *)o->d90;
        if (p->b04 != 2)
            break;
        p->b04 = 3;
        setanim(o, 1);
        o->state = 9;
        break;
    case 8:
        if (--o->timer <= 0) {
            o->state = 9;
        }
        break;
    case 9:
        D_8009C93F[0] = 0;
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
