// FUNC 80128114 496 X004
// MATCHING 80128114 496
#include "TOBJ.H"
typedef struct { short v[6]; } V6;
typedef struct { void **anims; int a; int b; } AT;
extern AT D_8013117C[];
extern unsigned char D_8009C942[], D_8009C93F[], D_8009C93E[];
extern unsigned char D_800A603C[], D_800A603D[], D_800A603E[];
extern short D_800A60EA[];
extern unsigned char D_8009CE3D;
extern void AnimJump(TObj *, int);
extern int FUN_8002dcc8(int, int, V6 *);

void func_80128114(TObj *o)
{
    V6 v;
    TObj *p;

    switch (o->state) {
    case 0:
        D_800A603C[0] = 5;
        D_800A603D[0] = 0;
        D_800A603E[0] = 0;
        D_8009C93F[0] = 1;
        D_8009C942[0] = 1;
        o->state++;
        break;
    case 1:
        v = *(V6 *)&o->a;
        if (D_8009CE3D == 0xff)
            o->d90 = FUN_8002dcc8(4, 9, &v);
        else if (D_8009CE3D != 0)
            o->d90 = FUN_8002dcc8(4, 3, &v);
        else
            o->d90 = FUN_8002dcc8(4, 0, &v);
        o->anim = D_8013117C[o->subtype].anims[2];
        AnimJump(o, 0);
        o->state++;
        break;
    case 2:
        p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        o->anim = D_8013117C[o->subtype].anims[0];
        AnimJump(o, 0);
        o->state++;
        break;
    case 3:
        D_800A603C[0] = 1;
        D_8009C93F[0] = 0;
        D_8009C942[0] = 0;
        D_8009C93E[0] = 0;
        D_800A60EA[0] = 0;
        D_800A603D[0] = 0;
        D_800A603E[0] = 0;
        o->animFrame = 1;
        o->b68 = 0;
        o->step = 0;
        o->state = 0;
        break;
    }
}
