// FUNC 801179ec 664 X017
// MATCHING 801179ec 664
#include "TOBJ.H"

typedef struct V { char c[12]; } V;
typedef struct { void **p; int pad[2]; } E12;
extern E12 D_80119990[];
extern unsigned char D_8009C942[], D_8009D2B0[];
extern unsigned char D_800A603C[], D_800A603D[], D_800A603E[];
extern unsigned char D_8009CFE1, D_8009CFF7;
extern short D_800A60EA[];
extern short D_1F8001C6;
extern TObj *FUN_8002dcc8(int, int, V *);
extern void AnimJump(TObj *, int);
extern void AnimAdvance(TObj *);
extern void func_8004D620(int, int);

void func_801179EC(TObj *o)
{
    TObj *p;
    V v;

    switch (o->state) {
    case 0:
        if (o->b68 == 0)
            break;
        v = *(V *)&o->a;
        D_8009D2B0[0] = 2;
        D_8009C942[0] = 1;
        D_800A603C[0] = 5;
        D_800A603D[0] = 0;
        D_800A603E[0] = 0;
        o->wbc = o->animFrame;
        o->animFrame = o->b68 & 1;
        o->anim = D_80119990[o->subtype].p[2];
        AnimJump(o, 0);
        *(int *)((char *)o + 0x90) = (int)FUN_8002dcc8(2, 0x30, &v);
        if (D_8009CFE1 == 0) {
            D_8009CFE1 = 1;
            D_8009CFF7++;
        }
        func_8004D620(D_8009CFF7 + 0x32, 2);
        o->state++;
        break;
    case 1:
        AnimAdvance(o);
        p = (TObj *)o->d90;
        if (p->b04 != 2)
            break;
        p->b04 = 3;
        o->anim = D_80119990[o->subtype].p[0];
        AnimJump(o, 0);
        o->state = 2;
        break;
    case 2:
        AnimAdvance(o);
        if (--o->timer != 0) {
            o->state = 9;
        }
        break;
    case 9:
        D_8009C942[0] = 0;
        D_800A60EA[0] = 0;
        D_1F8001C6 = 0;
        o->animFrame = o->wbc;
        o->anim = D_80119990[o->subtype].p[0];
        AnimJump(o, 0);
        D_800A603C[0] = 1;
        D_800A603D[0] = 0;
        D_800A603E[0] = 0;
        o->b68 = 0;
        o->step = 0;
        o->state = 0;
        break;
    }
}
