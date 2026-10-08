// FUNC 801177d0 1128 X018
// MATCHING 801177d0 1128
#include "TOBJ.H"
typedef struct { short v[6]; } V6;
typedef struct { void **anims; int a; int b; } AT;
typedef struct { TObj o; char pc0[0xd2 - 0xc0]; unsigned short wd2; } TX;
extern AT D_8011A564[];
extern unsigned char D_8009D2B0;
extern unsigned char D_8009CE16[];
extern short D_800A60EA[];
extern unsigned char D_8009C942[], D_8009C93F[];
extern short D_1F8001C6;
extern unsigned char D_800A603C[], D_800A603D[], D_800A603E[];
extern void AnimJump(TObj *, int);
extern void AnimAdvance(TObj *);
extern int FUN_8002dcc8(int, int, V6 *);
extern void FUN_8005a8a8(int, int, int);
extern void FUN_8005a9a4(int, int);

static __inline__ void setanim(TObj *o, short n)
{
    if (((TX *)o)->wd2 != n) {
        o->anim = D_8011A564[o->subtype].anims[n];
        AnimJump(o, 0);
        ((TX *)o)->wd2 = n;
    }
}

void func_801177D0(TObj *o)
{
    V6 v;
    TObj *q;

    switch (o->state) {
    case 0:
        if (o->b68 == 0) break;
        v = *(V6 *)&o->a;
        D_8009C942[0] = 1;
        D_8009D2B0 = 2;
        D_8009C93F[0] = 1;
        D_800A603C[0] = 5;
        D_800A603D[0] = 0;
        D_800A603E[0] = 0;
        o->wbc = o->animFrame;
        o->animFrame = o->b68 & 1;
        setanim(o, 4);
        v = *(V6 *)&o->a;
        o->d90 = FUN_8002dcc8(3, 0, &v);
        o->state++;
        break;
    case 1:
        AnimAdvance(o);
        q = (TObj *)o->d90;
        if (q->b04 != 2) break;
        q->b04 = 3;
        setanim(o, 1);
        D_800A60EA[0] = 0;
        o->timer = 4;
        if (D_8009CE16[-0xb] != 0xff) {
            FUN_8005a9a4(0x67, 0);
            o->timer = 200;
        }
        o->state = 2;
        break;
    case 2:
        AnimAdvance(o);
        if (--o->timer > 0) break;
        setanim(o, 4);
        v = *(V6 *)&o->a;
        o->d90 = FUN_8002dcc8(3, 0xe, &v);
        o->state = 3;
        break;
    case 3:
        AnimAdvance(o);
        q = (TObj *)o->d90;
        if (q->b04 != 2) break;
        q->b04 = 3;
        setanim(o, 1);
        D_800A60EA[0] = 0;
        o->timer = 4;
        if (D_8009CE16[6] == 0) {
            FUN_8005a8a8(0x78, 0, 0);
            o->timer = 200;
        }
        o->state = 8;
        break;
    case 8:
        if (--o->timer > 0) break;
        o->state = 9;
        break;
    case 9:
        D_8009C93F[0] = 0;
        D_8009C942[0] = 0;
        D_800A60EA[0] = 0;
        o->animFrame = o->wbc;
        D_1F8001C6 = 0;
        setanim(o, 1);
        D_800A603C[0] = 1;
        D_800A603D[0] = 0;
        D_800A603E[0] = 0;
        o->b68 = 0;
        o->step = 0;
        o->state = 0;
        break;
    }
}
