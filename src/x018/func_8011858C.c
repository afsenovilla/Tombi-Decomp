// FUNC 8011858c 1784 X018
// MATCHING 8011858c 1784
#include "TOBJ.H"
typedef struct { short v[6]; } V6;
typedef struct { void **anims; int a; int b; } AT;
typedef struct { TObj o; char pc0[0xd2 - 0xc0]; unsigned short wd2; } TX;
extern AT D_8011A564[];
extern unsigned char D_8009D2B0;
extern short D_800A60EA[];
extern unsigned char D_8009C942[], D_8009C93F[];
extern short D_1F8001C6;
extern unsigned char D_800A603C[], D_800A603D[], D_800A603E[];
extern void AnimJump(TObj *, int);
extern int AnimAdvance(TObj *);
extern int FUN_8002dcc8(int, int, V6 *);
extern void addItemToInventory(int, int, int);
extern void FUN_8005a9a4(int, int);

static __inline__ void setanim(TObj *o, short n)
{
    if (((TX *)o)->wd2 != n) {
        o->anim = D_8011A564[o->subtype].anims[n];
        AnimJump(o, 0);
        ((TX *)o)->wd2 = n;
    }
}

void func_8011858C(TObj *o)
{
    V6 v;
    TObj *q;

    switch (o->state) {
    case 0:
        if (o->b68 == 0) break;
        v = *(V6 *)&o->a;
        D_8009C942[0] = 1;
        D_8009C93F[0] = 1;
        D_8009D2B0 = 2;
        D_800A603C[0] = 5;
        D_800A603D[0] = 0;
        D_800A603E[0] = 0;
        o->wbc = o->animFrame;
        o->animFrame = o->b68 & 1;
        setanim(o, 4);
        v = *(V6 *)&o->a;
        o->d90 = FUN_8002dcc8(3, 6, &v);
        o->state++;
        break;
    case 1:
        AnimAdvance(o);
        q = (TObj *)o->d90;
        if (q->b04 != 2) break;
        q->b04 = 3;
        setanim(o, 1);
        addItemToInventory(0x38, 1, 1);
        FUN_8005a9a4(0xb6, 0);
        o->timer = 200;
        o->state = 2;
        break;
    case 2:
        if (--o->timer > 0) break;
        v = *(V6 *)&o->a;
        o->d90 = FUN_8002dcc8(3, 8, &v);
        setanim(o, 4);
        o->state++;
        break;
    case 3: {
        TObj *r;
        AnimAdvance(o);
        r = (TObj *)o->d90;
        if (r->b04 != 2) break;
        r->b04 = 3; }
        setanim(o, 2);
        o->timer = 0x3c;
        o->state++;
        break;
    case 4:
        AnimAdvance(o);
        if (--o->timer > 0) break;
        o->timer = 0x1e;
        o->animFrame ^= 1;
        setanim(o, 1);
        o->state++;
        break;
    case 5:
        if (--o->timer > 0) break;
        o->animFrame ^= 1;
        v = *(V6 *)&o->a;
        o->d90 = FUN_8002dcc8(3, 9, &v);
        setanim(o, 4);
        o->state++;
        break;
    case 6:
        AnimAdvance(o);
        q = (TObj *)o->d90;
        if (q->b04 != 2) break;
        q->b04 = 3;
        v = *(V6 *)&o->a;
        o->d90 = FUN_8002dcc8(3, 7, &v);
        o->state++;
        break;
    case 7:
        AnimAdvance(o);
        q = (TObj *)o->d90;
        if (q->b04 != 2) break;
        q->b04 = 3;
        setanim(o, 8);
        FUN_8005a9a4(0x74, 0);
        o->state++;
        break;
    case 8:
        if (AnimAdvance(o) == 0) break;
        setanim(o, 7);
        o->state++;
        break;
    case 9:
        if (AnimAdvance(o) == 0) break;
        setanim(o, 1);
        addItemToInventory(0x22, 1, 1);
        o->timer = 200;
        o->state = 0x62;
        break;
    case 0x62:
        if (--o->timer > 0) break;
        o->state = 0x63;
        break;
    case 0x63:
        D_8009C93F[0] = 0;
        D_8009C942[0] = 0;
        D_800A60EA[0] = 0;
        D_1F8001C6 = 0;
        setanim(o, 1);
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
