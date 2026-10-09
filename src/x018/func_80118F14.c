// FUNC 80118f14 712 X018
// MATCHING 80118f14 712
#include "TOBJ.H"
typedef struct { short v[6]; } V6;
typedef struct { void **anims; int a; int b; } AT;
extern AT D_8011A564[];
extern unsigned char D_8009D2B0;
extern short D_800A60EA[];
extern unsigned char D_8009C942[], D_8009C93F[];
extern short D_1F8001C6;
extern unsigned char D_800A603C[], D_800A603D[], D_800A603E[];
extern unsigned char D_8009CE28[];
extern void FUN_8001fe94(TObj *, int);
extern int FUN_8001fec0(TObj *);
extern int FUN_8002dcc8(int, int, V6 *);
extern void FUN_8005a8a8(int, int, int);

#define WD2(o) (*(unsigned short *)((char *)(o) + 0xd2))

static __inline__ void setanim(TObj *o, short n)
{
    if (WD2(o) != n) {
        o->anim = D_8011A564[o->subtype].anims[n];
        FUN_8001fe94(o, 0);
        WD2(o) = n;
    }
}

void func_80118F14(TObj *o)
{
    V6 v;
    TObj *p;

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
        o->d90 = FUN_8002dcc8(3, 0xb, &v);
        o->state++;
        break;
    case 1:
        FUN_8001fec0(o);
        p = (TObj *)o->d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        o->timer = 4;
        if (D_8009CE28[0] == 0) {
            FUN_8005a8a8(0x84, 0, 0);
            o->timer = 200;
        }
        o->state = 8;
        setanim(o, 1);
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
        D_800A603C[0] = 1;
        D_800A603D[0] = 0;
        D_800A603E[0] = 0;
        o->b68 = 0;
        o->step = 0;
        o->state = 0;
        break;
    }
}
