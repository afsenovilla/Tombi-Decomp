// FUNC 80117fac 848 X018
// MATCHING 80117fac 848
#include "TOBJ.H"

typedef struct {
    TObj o;
    unsigned char pad[0x12];
    unsigned short wd2;
} P;

typedef struct { short s[6]; } V;
typedef struct { void **tbl; int x; int y; } E;

extern E D_8011A564[];
extern unsigned char D_8009C942[], D_8009C93F[], D_8009D2B0;
extern unsigned char D_800A603C, D_800A603D, D_800A603E;
extern unsigned char D_8009D132, D_8009D0B7;
extern short D_800A60EA[], D_1F8001C6;
extern void FUN_8001fe94(P *, int);
extern TObj *FUN_8002dcc8(int, int, V *);
extern int AnimAdvance(P *);
extern void FUN_80026e0c(int, int);
extern void FUN_8005a9a4(int, int);

static __inline__ void SetAnim(P *o, short n)
{
    if (o->wd2 != n) {
        o->o.anim = D_8011A564[o->o.subtype].tbl[n];
        FUN_8001fe94(o, 0);
        o->wd2 = n;
    }
}

void func_80117FAC(P *o)
{
    V v;
    TObj *p;
    int k2;

    switch (o->o.state) {
    case 0:
        if (!o->o.b68) break;
        k2 = 2;
        v = *(V *)&o->o.a;
        D_8009C942[0] = 1;
        D_8009C93F[0] = 1;
        D_8009D2B0 = k2;
        D_800A603C = 5;
        D_800A603D = 0;
        D_800A603E = 0;
        o->o.wbc = o->o.animFrame;
        o->o.animFrame = o->o.b68 & 1;
        SetAnim(o, 4);
        if (D_8009D132 && D_8009D0B7) {
            v = *(V *)&o->o.a;
            o->o.d90 = (int)FUN_8002dcc8(3, 4, &v);
            o->o.state = k2;
        } else {
            v = *(V *)&o->o.a;
            o->o.d90 = (int)FUN_8002dcc8(3, 3, &v);
            o->o.state++;
        }
        break;
    case 1:
        AnimAdvance(o);
        {
        TObj *p = (TObj *)o->o.d90;
        if (p->b04 == 2) {
            p->b04 = 3;
            o->o.state = 9;
        }
        }
        break;
    case 2:
        AnimAdvance(o);
        p = (TObj *)o->o.d90;
        if (p->b04 == 2) {
            p->b04 = 3;
            o->o.timer = 0xc8;
            FUN_80026e0c(0x8e, 1);
            FUN_80026e0c(0x13, 1);
            FUN_8005a9a4(0x72, 0);
            o->o.state = 8;
        }
        break;
    case 8:
        if (--o->o.timer > 0) break;
        o->o.state = 9;
        break;
    case 9:
        D_8009C93F[0] = 0;
        D_8009C942[0] = 0;
        D_800A60EA[0] = 0;
        o->o.animFrame = o->o.wbc;
        D_1F8001C6 = 0;
        SetAnim(o, 1);
        D_800A603C = 1;
        D_800A603D = 0;
        D_800A603E = 0;
        o->o.b68 = 0;
        o->o.step = 0;
        o->o.state = 0;
        break;
    }
}
