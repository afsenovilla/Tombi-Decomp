// FUNC 801182fc 656 X018
// MATCHING 801182fc 656
#include "TOBJ.H"
typedef struct { short v[6]; } V6;
typedef struct { void **anims; int a; int b; } AT;
typedef struct {
    TObj t;
    char pad[0x12];
    unsigned short wd2;
} TO2;
extern AT D_8011A564[];
extern unsigned char D_8009C942[];
extern unsigned char D_8009C93F[];
extern unsigned char D_8009D2B0;
extern short D_800A60EA[];
extern short D_1F8001C6;
extern unsigned char D_800A603C[], D_800A603D[], D_800A603E[];
extern void AnimJump(TObj *, int);
extern int AnimAdvance(TObj *);
extern int FUN_8002dcc8(int, int, V6 *);

static __inline__ void setAnim(TO2 *o, unsigned short n)
{
    if (o->wd2 != n) {
        o->t.anim = D_8011A564[o->t.subtype].anims[n];
        AnimJump(&o->t, 0);
        o->wd2 = n;
    }
}

void func_801182FC(TO2 *o)
{
    V6 v;
    TObj *p;

    switch (o->t.state) {
    case 0:
        if (o->t.b68 == 0) break;
        v = *(V6 *)&o->t.a;
        D_8009C942[0] = 1;
        D_8009C93F[0] = 1;
        D_8009D2B0 = 2;
        D_800A603C[0] = 5;
        D_800A603D[0] = 0;
        D_800A603E[0] = 0;
        o->t.wbc = o->t.animFrame;
        o->t.animFrame = o->t.b68 & 1;
        setAnim(o, 4);
        v = *(V6 *)&o->t.a;
        o->t.d90 = FUN_8002dcc8(3, 5, &v);
        o->t.state++;
        break;
    case 1:
        AnimAdvance(&o->t);
        p = (TObj *)o->t.d90;
        if (p->b04 != 2) break;
        p->b04 = 3;
        o->t.state = 9;
        break;
    case 8:
        if (--o->t.timer > 0) break;
        o->t.state = 9;
        break;
    case 9:
        D_8009C93F[0] = 0;
        D_8009C942[0] = 0;
        D_800A60EA[0] = 0;
        D_1F8001C6 = 0;
        setAnim(o, 1);
        o->t.animFrame = o->t.wbc;
        D_800A603C[0] = 1;
        D_800A603D[0] = 0;
        D_800A603E[0] = 0;
        o->t.b68 = 0;
        o->t.step = 0;
        o->t.state = 0;
        break;
    }
}
